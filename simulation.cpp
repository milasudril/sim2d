#ifdef __WAND__
target[name[simulation.o] type[object]]
#endif

#include "simulation.h"
#include "simulationsetupblob.h"
#include "datablockprocessor.h"
#include "model.h"
#include "modelstate.h"
#include "ui_simulationview.h"

#include <chunkio/reader.h>
#include <chunkio/writer.h>
#include <herbs/exceptionmissing/exceptionmissing.h>
#include <herbs/thread/thread.h>

namespace
	{
	class DatablockInit
		{
		public:
			DatablockInit(Herbs::LogWriter& log
				,const Sim2d::Model& model
				,const uintmax_t& framecounter
				,Sim2d::RandBootstrap& rand_seeder
				,Herbs::PairSame<Vector::MatrixStorage<Sim2d::ValueType>>& buffers):
				 m_log(log),m_model(model),m_framecounter(framecounter)
				,m_rand_seeder(rand_seeder)
				,m_buffers(buffers)
				{}
		
			void operator()(Sim2d::DatablockProcessor* obj,size_t k,size_t N) const
				{
				double bh_avg=double(m_buffers.first.nRowsGet())/N;
				uint32_t offset=uint32_t(k*bh_avg);
				uint32_t height_block=uint32_t((k+1)*bh_avg) - offset;
				Sim2d::BufferinfoPair buffers[2]=
					{
					 {m_buffers.first,m_buffers.second,height_block,offset}
					,{m_buffers.second,m_buffers.first,height_block,offset}
					};
				new(obj)Sim2d::DatablockProcessor(m_log,m_model,m_framecounter
					,m_rand_seeder(),buffers,offset);
				}
				
		private:
			Herbs::LogWriter& m_log;
			const Sim2d::Model& m_model;
			const uintmax_t& m_framecounter;
			Sim2d::RandBootstrap& m_rand_seeder; 
			Herbs::PairSame<Vector::MatrixStorage<Sim2d::ValueType>>& m_buffers;
		};
	}

size_t Sim2d::Simulation::stateGlobalSizeGet() const
	{
	if(m_stop)
		{
		size_t ret=world_state->stateGlobalSizeGet();
		auto ptr=processors.begin();
		while(ptr!=processors.end())
			{
			ret+=ptr->stateGlobalSizeGet();
			++ptr;
			}
		return ret;
		}
	return 0;
	}
	
void Sim2d::Simulation::stateGlobalStore(ChunkIO::Writer& writer) const
	{
	if(m_stop)
		{
		world_state->stateGlobalStore(writer);
		auto ptr=processors.begin();
		while(ptr!=processors.end())
			{
			ptr->stateGlobalStore(writer);
			++ptr;
			}
		}
	}
	
void Sim2d::Simulation::stateGlobalLoad(ChunkIO::Reader& reader)
	{
	if(m_stop)
		{
		world_state->stateGlobalLoad(reader);
		auto ptr=processors.begin();
		while(ptr!=processors.end())
			{
			ptr->stateGlobalLoad(reader);
			++ptr;
			}
		}
	}

void Sim2d::Simulation::stateCellStore(ChunkIO::Writer& writer) const
	{
	writer.dataWrite(buffers[framecounter%2].rowGet(0),stateCellSizeGet());
	}

void Sim2d::Simulation::stateCellLoad(ChunkIO::Reader& reader)
	{
	if(m_stop)
		{
		reader.dataRead(buffers[0].rowGet(0),stateCellSizeGet());
		buffers[1]=buffers[0];
		if(m_view!=nullptr)
			{pixelsUpload();}
		}
	}
	
Sim2d::Simulation::Simulation(Herbs::LogWriter& log
	,const SimulationSetupBlob& setup,const Model& model,UiSimulationView* view):
	 Herbs::Runnable(log)
	,buffers
		{
		 Vector::MatrixStorage<ValueType>
			{setup.domain_height,setup.domain_width*model.nVarsGet()}
		,Vector::MatrixStorage<ValueType>
			{setup.domain_height,setup.domain_width*model.nVarsGet()}
		}
	,framecounter(setup.framecounter)
	,n_frames_max(setup.num_iter)
	,rand_seeder(setup.seed)
	,processors(setup.n_threads,DatablockInit
		{
		 log
		,model
		,framecounter
		,rand_seeder
		,buffers
		})
	,world_bipair
		{
		 {buffers.first,buffers.second,(uint32_t)buffers.first.nRowsGet(),0}
		,{buffers.second,buffers.first,(uint32_t)buffers.second.nRowsGet(),0}
		}
	,m_model(model),m_view(view),buffer_out(1,1),ui_refreshrate(setup.ui_refreshrate)
	,varindex_sel(0)
	,m_stop(1)
	{
	if(m_view!=nullptr)
		{
		pixelsUpload();
		}
	world_state=model.stateCreate(rand_seeder());
	}
	
Sim2d::Simulation::~Simulation()
	{
	stop();
	world_state->destroy();
	}
	
void Sim2d::Simulation::varindexSet(unsigned int varindex)
	{
	varindex_sel=varindex;
	if(m_stop && m_view!=nullptr)
		{pixelsUpload();}
	}
	
namespace
	{
	class ThreadInit
		{
		public:
			ThreadInit(
				Herbs::ArrayFixedsize<Sim2d::DatablockProcessor>& processors):
				m_processors(processors)
				{}
			void operator()(Herbs::Thread* obj,size_t k,size_t N) const
				{
				new(obj)Herbs::Thread(m_processors[k]);
				}
		private:
			Herbs::ArrayFixedsize<Sim2d::DatablockProcessor>& m_processors;
		};
	}
	
int Sim2d::Simulation::run()
	{
	if(framecounter >= n_frames_max)
		{return STATUS_OK;}
	m_stop=0;

	Herbs::ArrayFixedsize<Herbs::Thread> workers
		(processors.length(),ThreadInit{processors});
		
	DatablockProcessor* proc_ptr;
	
	while(!m_stop
		&& (framecounter<n_frames_max || n_frames_max==(uintmax_t)(-1) ))
		{
		proc_ptr=processors.begin();
		while(proc_ptr!=processors.end())
			{
			proc_ptr->frameNext();
			++proc_ptr;
			}
		
		while(proc_ptr!=processors.begin())
			{
			--proc_ptr;
			proc_ptr->wait();
			}
		
		world_state->process(framecounter,world_bipair[0].first
			,world_bipair[0].second);
		swap(world_bipair[0],world_bipair[1]);

		++framecounter;
		if(m_view!=nullptr)
			{
			if(framecounter%16==0)
				{m_view->counterUpdate(framecounter);}
			
			if(framecounter%ui_refreshrate==0)
				{pixelsUpload();}
			}
		}
		
	proc_ptr=processors.begin();
	while(proc_ptr!=processors.end())
		{
		proc_ptr->stop();
		++proc_ptr;
		}
	
	proc_ptr=processors.begin();
	while(proc_ptr!=processors.end())
		{
		proc_ptr->frameNext();
		++proc_ptr;
		}
	
	m_stop=1;

	if(m_view!=nullptr)
		{
		pixelsUpload();
		m_view->counterUpdate(framecounter);
		}
	if(framecounter==n_frames_max)
		{return STATUS_OK;}
	return STATUS_ABORT;
	}

void Sim2d::Simulation::dataSet(const Vector::MatrixStorage<ValueType>& data)
	{
	if(m_stop)
		{
		if(varindex_sel<m_model.nVarsGet())
			{
			
			Bufferinfo inblock=
				{
				(ValueType* const*)data.rowsGet()
				,uint32_t(data.nColsGet())
				,uint32_t(data.nRowsGet())
				};
			for(int k=0;k<2;++k)
				{
				Vector::MatrixStorage<ValueType>& buffer_in=buffers[k];
				Bufferinfo outblock=
					{
					buffer_in.rowsGet()
					,uint32_t(buffer_in.nColsGet())
					,uint32_t(buffer_in.nRowsGet())
					};
				m_model.pixelsPush(inblock,outblock,varindex_sel);
				}
			if(m_view!=nullptr)
				{pixelsUpload();}
			}
		}
	}
	
void Sim2d::Simulation::dataGet(Vector::MatrixStorage<ValueType>& data)
	{
	if(m_stop)
		{
		if(varindex_sel<m_model.nVarsGet())
			{
			Bufferinfo outblock=
				{
				(ValueType* const*)data.rowsGet()
				,uint32_t(data.nColsGet())
				,uint32_t(data.nRowsGet())
				};

			Vector::MatrixStorage<ValueType>& buffer_in=buffers[framecounter%2];
			Bufferinfo inblock=
				{
				buffer_in.rowsGet()
				,uint32_t(buffer_in.nColsGet())
				,uint32_t(buffer_in.nRowsGet())
				};
			m_model.pixelsPull(inblock,outblock,varindex_sel);
			}
		}
	}

void Sim2d::Simulation::pixelsUpload()
	{
	if(varindex_sel<m_model.nVarsGet())
		{
		Vector::MatrixStorage<ValueType>& buffer_in=buffers[framecounter%2];
		auto size_view=m_view->viewportSizeGet();
		buffer_out.resize(size_view.y,size_view.x);
		
		Bufferinfo inblock=
			{
			 buffer_in.rowsGet()
			,uint32_t(buffer_in.nColsGet())
			,uint32_t(buffer_in.nRowsGet())
			};
		
		Bufferinfo outblock=
			{
			 buffer_out.rowsGet()
			,uint32_t(buffer_out.nColsGet())
			,uint32_t(buffer_out.nRowsGet())
			};
	
		m_model.pixelsPull(inblock,outblock,varindex_sel);
		
		m_view->pixelsSet(buffer_out);
		}
	}
	
void Sim2d::Simulation::setupUpdate(const SimulationSetupBlob& setup)
	{
	throw Herbs::ExceptionMissing(___FILE__,__LINE__);
	}
