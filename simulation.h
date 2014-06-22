#ifdef __WAND__
target[name[simulation.h] type[include]]
dependency[simulation.o]
#endif

#ifndef SIM2D_SIMULATION_H
#define SIM2D_SIMULATION_H

#include "valuetype.h"
#include "rand_bootstrap.h"
#include "bufferinfo_pair.h"

#include <herbs/runnable/runnable.h>
#include <vector/matrixstorage.h>
#include <herbs/array_fixedsize/array_fixedsize.h>
#include <herbs/pairsame/pairsame.h>
#include <cstdint>

namespace ChunkIO
	{
	class Reader;
	class Writer;
	}

namespace Sim2d
	{
	class DatablockProcessor;
	class Model;
	class SimulationSetupBlob;
	class UiSimulationView;
	class ModelState;
	
	class Simulation:public Herbs::Runnable
		{
		public:
			Simulation(Herbs::LogWriter& log,const SimulationSetupBlob& setup
				,const Model& model,UiSimulationView* view);

			void setupUpdate(const SimulationSetupBlob& setup);
			
			int run();
	
			void stop()
				{m_stop=1;}
				
			uintmax_t framecounterGet()
				{return framecounter;}
				
			void varindexSet(unsigned int varindex);
			void dataSet(const Vector::MatrixStorage<ValueType>& data);
			void dataGet(Vector::MatrixStorage<ValueType>& data);
			void pixelsUpload();
			
			size_t stateGlobalSizeGet() const;
			void stateGlobalStore(ChunkIO::Writer& writer) const;
			void stateGlobalLoad(ChunkIO::Reader& reader);
			
			size_t stateCellSizeGet() const
				{
				return sizeof(ValueType)
					*buffers.first.nRowsGet()
					*buffers.second.nColsGet();
				}
			void stateCellStore(ChunkIO::Writer& writer) const;
			void stateCellLoad(ChunkIO::Reader& reader);

			~Simulation();
		
		private:
			Herbs::PairSame<Vector::MatrixStorage<ValueType> > buffers;
			uintmax_t framecounter;
			uintmax_t n_frames_max;
			RandBootstrap rand_seeder;
			
			Herbs::ArrayFixedsize<DatablockProcessor> processors;
			
			BufferinfoPair world_bipair[2];
			ModelState* world_state;
				
			const Model& m_model;
			UiSimulationView* m_view;
			Vector::MatrixStorage<Sim2d::ValueType> buffer_out;
			unsigned int ui_refreshrate;
			unsigned int varindex_sel;
			volatile unsigned int m_stop;
		};
	}

#endif
