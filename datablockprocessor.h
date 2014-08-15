#ifdef __WAND__
target[name[datablockprocessor.h] type[include]]
dependency[datablockprocessor.o]
#endif

#ifndef SIM2D_DATABLOCKPROCESSOR_H
#define SIM2D_DATABLOCKPROCESSOR_H

#include "valuetype.h"
#include "bufferinfo_pair.h"
#include <herbs/eventauto/eventauto.h>
#include <herbs/runnable/runnable.h>
#include <cstdint>

namespace Sim2d
	{
	class ModelState;
	class Model;
	class DataReader;
	class DataWriter;
		
	class DatablockProcessor : public Herbs::Runnable
		{
		public:
			DatablockProcessor(Herbs::LogWriter& lw,const Model& model
				,const uintmax_t& framecounter
				,uint32_t seed
				,BufferinfoPair* bi_pairs
				,unsigned int offset);
				
			int run();
			
			void wait()
				{ready.wait();}
			
			void frameNext()
				{start.set();}
			
			void stop()
				{m_stop=1;}
				
			size_t stateGlobalSizeGet() const;
			void stateGlobalStore(DataWriter& writer) const;
			void stateGlobalLoad(DataReader& reader);
				
			~DatablockProcessor();

		private:
			Herbs::EventAuto ready;
			Herbs::EventAuto start;
			BufferinfoPair m_buffers[2];
			ModelState* m_model;
			const uintmax_t& m_framecounter;
			unsigned int m_offset;
			volatile unsigned int m_stop;
		};
	}

#endif

