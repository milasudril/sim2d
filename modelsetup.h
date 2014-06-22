#ifdef __WAND__
target[name[modelsetup.h] type[include]]
#endif

#ifndef SIM2D_MODELSETUP_H
#define SIM2D_MODELSETUP_H

#include <config/paramdescriptor.h>

namespace ChunkIO
	{
	class Reader;
	class Writer;
	}

namespace Sim2d
	{	
	class ModelSetup:public Config::Paramdescriptor
		{
		public:
			virtual void destroy()=0;
			virtual ~ModelSetup(){}
			virtual size_t chunkSizeGet() const=0;
			virtual bool load(ChunkIO::Reader& source)=0;
			virtual bool store(ChunkIO::Writer& dest) const=0;
		};
	}

#endif
