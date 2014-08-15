#ifdef __WAND__
target[name[modelsetup.h] type[include]]
#endif

#ifndef SIM2D_MODELSETUP_H
#define SIM2D_MODELSETUP_H

#include <config/paramdescriptor.h>

namespace Sim2d
	{
	class DataReader;
	class DataWriter;
	class ModelSetup:public Config::Paramdescriptor
		{
		public:
			virtual void destroy()=0;
			virtual ~ModelSetup(){}
			virtual size_t chunkSizeGet() const=0;
			virtual bool load(DataReader& source)=0;
			virtual bool store(DataWriter& dest) const=0;
		};
	}

#endif
