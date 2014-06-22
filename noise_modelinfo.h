#ifdef __WAND__
target[name[noise_modelinfo.h] type[include]]
dependency[noise_modelinfo.o]
#endif

#ifndef SIM2D_NOISEMODELINFO_H
#define SIM2D_NOISEMODELINFO_H

#include "modelinfo.h"

namespace Sim2d
	{
	class Noise_ModelInfo:public ModelInfo
		{
		public:
			const char_t* nameGet() const;
			const char_t* const* varnamesGet() const;
			ModelSetup* setupCreate() const;
			ModelState* stateCreate(const ModelSetup& setup,uint32_t seed) const;
			void pixelsPull(const Bufferinfo& src,Bufferinfo& dest
				,unsigned int varindex) const;
			void pixelsPush(const Bufferinfo& src,Bufferinfo& dest
				,unsigned int varindex) const;
			void destroy();
		};
	}

#endif
