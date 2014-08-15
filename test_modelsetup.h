#ifdef __WAND__
target[name[test_modelsetup.h] type[include]]
dependency[test_modelsetup.o]
#endif

#ifndef SIM2D_TESTMODELSETUP_H
#define SIM2D_TESTMODELSETUP_H

#include "modelsetup.h"

namespace Sim2d
	{
	class Test_ModelSetup:public ModelSetup
		{
		public:
			Test_ModelSetup(): blob{2,0.038,0.059}
				{}
			
			const char_t* titleGet() const;
			Herbs::ListPacked paraminfoGet();
			void destroy();
			
			bool load(DataReader& source);
			bool store(DataWriter& dest) const;
			size_t chunkSizeGet() const
				{return sizeof(blob);}
			
			struct Blob
				{
				float diff_ratio;
				float feed_rate;
				float decay_rate;
				} blob;
		};
	}

#endif
