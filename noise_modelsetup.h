#ifdef __WAND__
target[name[noise_modelsetup.h] type[include]]
dependency[noise_modelsetup.o]
#endif

#ifndef SIM2D_NOISEMODELSETUP_H
#define SIM2D_NOISEMODELSETUP_H

#include "modelsetup.h"

namespace Sim2d
	{
	class Noise_ModelSetup:public ModelSetup
		{
		public:
			Noise_ModelSetup(): dummy(0)
				{}
			
			float dummy;
			
			const char_t* titleGet() const;
			Herbs::ListPacked paraminfoGet();
			void destroy();
			
			bool load(DataReader& source);
			bool store(DataWriter& dest) const;
			size_t chunkSizeGet() const
				{return 0;}
		};
	}

#endif
