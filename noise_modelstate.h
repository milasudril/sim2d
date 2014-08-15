#ifdef __WAND__
target[name[noise_modelstate.h] type[include]]
dependency[noise_modelstate.o]
#endif

#ifndef SIM2D_NOISEMODELSTATE_H
#define SIM2D_NOISEMODELSTATE_H

#include "modelstate.h"

#include <random>

namespace Sim2d
	{
	class Noise_ModelSetup;
	
	class Noise_ModelState:public ModelState
		{
		public:
			Noise_ModelState(const Noise_ModelSetup& params,uint32_t seed);
		
			void process(uintmax_t framecounter
				,const Bufferinfo& buffer_src,Bufferinfo& buffer_dest
				,unsigned int offset);
			void destroy();
			
			bool stateGlobalLoad(DataReader& source);
			bool stateGlobalStore(DataWriter& dest) const;
			size_t stateGlobalSizeGet() const
				{return sizeof(randgen);}
			
		private:
			const Noise_ModelSetup& m_params;
			std::mt19937 randgen;
		};
	}

#endif
