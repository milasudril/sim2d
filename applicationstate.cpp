#ifdef __WAND__
target[name[applicationstate.o] type[object]]
#endif

#include "applicationstate.h"
#include "simulation.h"
#include "modelloader.h"
#include "modelinfo.h"
#include "model.h"
#include "dataimporter.h"
#include "dataexporter.h"
#include "ui_applicationstate.h"
#include "modelsetup.h"
#include <chunkio/bigchunk_reader.h>
#include <chunkio/bigchunk_writer.h>
#include <herbs/thread/thread.h>
#include <herbs/runnable/runnable.h>
#include <herbs/filein/filein.h>
#include <herbs/fileout/fileout.h>
#include <herbs/exceptionmissing/exceptionmissing.h>

#include <memory>

void Sim2d::ApplicationState::load(const Herbs::Path& filename
	,Herbs::LogWriter& logwriter,Sim2d::UiSimulationView* view
	,Sim2d::UiApplicationState* ui_appstate)
	{
	Herbs::FileIn file_in(filename);
	ChunkIO::BigchunkReader reader(file_in);
	Herbs::Stringbase<char> header_name(8);
	size_t status=0;
	while(reader.headerRead(header_name))
		{
		if(header_name=="SIM2DMET")
			{
			status|=1;
			break;
			}
		reader.skip();
		}
		
	if(!(status&1))
		{throw Herbs::ExceptionMissing(___FILE__,__LINE__);}
		
	SimulationSetup simsetup_new;
	std::unique_ptr<ModelLoader> loader_new;
	while(reader.headerRead(header_name))
		{
		if(header_name=="END" || header_name=="STATEGLO" 
			|| header_name=="STATECEL")
			{break;	}
		else	
		if(header_name=="SIMSETUP")
			{
			simsetup_new.load(reader);
			status|=2;
			}
		else
		if(header_name=="MODINFO")
			{
			loader_new.reset( new ModelLoader(reader) );
			status|=4;
			}
		reader.skip();
		}
	if(status!=0x7)
		{throw Herbs::ExceptionMissing(___FILE__,__LINE__);}
	
		{
		simulationSetupUpdate(simsetup_new);
		auto model_new=new Model(loader_new->modelInfoGet());
		modelUnload();
		m_loader=loader_new.get();
		loader_new.release();
		m_model=model_new;
		
		simulationCreate(logwriter,view,ui_appstate);
		}
	
	do
		{
		fflush(stdout);
		if(header_name=="END")
			{break;}
		else
		if(header_name=="STATEGLO")
			{m_simulation->stateGlobalLoad(reader);}
		else
		if(header_name=="STATECEL")
			{m_simulation->stateCellLoad(reader);}
		
		reader.skip();
		}
	while(reader.headerRead(header_name));
	}

void Sim2d::ApplicationState::store(const Herbs::Path& filename)
	{
	Herbs::FileOut file_out(filename);
	ChunkIO::BigchunkWriter writer(file_out);
	writer.chunkIDWrite("SIM2DMET");
	writer.chunkSizeWrite(0);
	
	writer.chunkIDWrite("SIMSETUP");
	writer.chunkSizeWrite(simsetup.chunkSizeGet());
	simsetup.store(writer);
	
	writer.chunkIDWrite("MODINFO");
	writer.chunkSizeWrite(m_loader->chunkSizeGet());
	m_loader->store(writer);
	
	writer.chunkIDWrite("MODPARAM");
	writer.chunkSizeWrite(m_model->setupGet().chunkSizeGet());
	m_model->setupGet().store(writer);
	
	writer.chunkIDWrite("STATEGLO");
	writer.chunkSizeWrite(m_simulation->stateGlobalSizeGet());
	m_simulation->stateGlobalStore(writer);
	
	writer.chunkIDWrite("STATECEL");
	writer.chunkSizeWrite(m_simulation->stateCellSizeGet());
	m_simulation->stateCellStore(writer);
	
	writer.chunkIDWrite("END\0\0\0\0");
	writer.chunkSizeWrite(0);
	}

void Sim2d::ApplicationState::modelLoad(const Herbs::Path& model_new_path)
	{
	std::unique_ptr<ModelLoader> loader_new( new ModelLoader(model_new_path) );
	auto model_new=new Model(loader_new->modelInfoGet());
	modelUnload();
	m_loader=loader_new.get();
	loader_new.release();
	m_model=model_new;
	}

const Herbs::Path& Sim2d::ApplicationState::modelFilenameGet() const
	{return m_loader->filenameGet();}
	
void Sim2d::ApplicationState::dataimportPathSet(const Herbs::Path& path)
	{
	if(dataimporterCreated())
		{delete importer;}
	importer=new DataImporter(path);
	}

const Herbs::Path& Sim2d::ApplicationState::dataimportPathGet() const
	{
	return importer->filenameGet();
	}

void Sim2d::ApplicationState::dataimportPixelsPush()
	{
	Vector::MatrixStorage<ValueType> matrix(1,1);
	importer->pixelsPull(matrix);
	m_simulation->dataSet(matrix);
	}
	
void Sim2d::ApplicationState::dataexportPathSet(const Herbs::Path& path)
	{
	if(dataexporterCreated())
		{delete exporter;}
	exporter=new DataExporter(path);
	}
	
const Herbs::Path& Sim2d::ApplicationState::dataexportPathGet() const
	{
	return exporter->filenameGet();
	}
	
void Sim2d::ApplicationState::dataexportPixelsPull() const
	{
	Vector::MatrixStorage<ValueType> matrix(simsetup.blob.domain_height
		,simsetup.blob.domain_width);
	m_simulation->dataGet(matrix);
	exporter->pixelsPush(matrix);
	}
	
void Sim2d::ApplicationState::modelUnload()
	{
	if(modelLoaded())
		{
		simulationDestroy();
		delete m_model;
		m_model=nullptr;
		delete m_loader;
		m_loader=nullptr;
		}
	}

Sim2d::ModelSetup& Sim2d::ApplicationState::modelSetupGet() 
	{return m_model->setupGet();}

const char_t* const* Sim2d::ApplicationState::modelVarnamesGet() const
	{return m_model->varnamesGet();}
	
const char_t* Sim2d::ApplicationState::modelNameGet() const
	{return m_model->nameGet();}

class Sim2d::ApplicationState::WatchThreadCallback:public Herbs::Runnable
	{
	public:
		WatchThreadCallback(Herbs::LogWriter& writer
			,UiApplicationState* ui_appstate):Herbs::Runnable(writer)
			,m_ui(ui_appstate)
			{}
		
		void assign(Herbs::Thread* thread)
			{m_thread=thread;}
		
		int run()
			{
			delete m_thread;
			if(m_ui!=nullptr)
				{m_ui->simulationRunningSet(0);}
			return STATUS_OK;
			}
		
	private:
		Herbs::Thread* m_thread;
		UiApplicationState* m_ui;
	};
	
void Sim2d::ApplicationState::simulationCreate(Herbs::LogWriter& logwriter
	,UiSimulationView* view,UiApplicationState* ui_appstate)
	{
	auto sim_new=new Simulation(logwriter,simsetup.blob,*m_model,view);
	simulationDestroy();
	m_watch=new WatchThreadCallback(logwriter,ui_appstate);
	m_simulation=sim_new;
	}
	
void Sim2d::ApplicationState::simulationVarindexSet(unsigned int index)
	{
	m_simulation->varindexSet(index);
	}

void Sim2d::ApplicationState::simulationSetupUpdate(const SimulationSetup& obj)
	{
	obj.validate();
	if(simulationCreated())
		{
		simulationStop();
		m_simulation->setupUpdate(obj.blob);
		}
	simsetup=obj;
	}

void Sim2d::ApplicationState::simulationDestroy()
	{
	if(simulationCreated())
		{
		simulationStop();
		delete m_watch;
		delete m_simulation;
		m_simulation=nullptr;
		}
	}
	
void Sim2d::ApplicationState::simulationStart()
	{
	m_watch->assign(new Herbs::Thread(*m_simulation));
	m_watchthread=new Herbs::Thread(*m_watch);
	}

void Sim2d::ApplicationState::simulationStop()
	{
	if(simulationRunning())
		{
		m_simulation->stop();
		delete m_watchthread;
		simsetup.blob.framecounter=m_simulation->framecounterGet();
		m_watchthread=nullptr;
		}
	}

Sim2d::ApplicationState::~ApplicationState()
	{
	if(dataimporterCreated())
		{delete importer;}
	modelUnload();
	}
