#ifdef __WAND__
target[name[test_modelstate.h] type[include]]
dependency[test_modelstate.o]
#endif

#ifndef SIM2D_TESTMODELSTATE_H
#define SIM2D_TESTMODELSTATE_H

#include "modelstate.h"
#include <random>

namespace Sim2d
	{
	class Test_ModelSetup;
	
	class Test_ModelState:public ModelState
		{
		public:
			Test_ModelState(const Test_ModelSetup& params,uint32_t seed);
		
			void process(uintmax_t framecounter
				,const Bufferinfo& buffer_src,Bufferinfo& buffer_dest
				,unsigned int offset);
			void destroy();
			
			bool stateGlobalLoad(ChunkIO::Reader& source);
			bool stateGlobalStore(ChunkIO::Writer& dest) const;
			size_t stateGlobalSizeGet() const
				{return 0;}
			
		private:
			const Test_ModelSetup& m_params;
		};
	}

#endif
