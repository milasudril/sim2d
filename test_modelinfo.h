#ifdef __WAND__
target[name[test_modelinfo.h] type[include]]
dependency[test_modelinfo.o]
#endif

#ifndef SIM2D_TESTMODELINFO_H
#define SIM2D_TESTMODELINFO_H

#include "modelinfo.h"

namespace Sim2d
	{
	class Test_ModelInfo:public ModelInfo
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
