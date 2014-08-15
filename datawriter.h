#ifdef __WAND__
target[name[datawriter.h] type[include]]
#endif

#ifndef SIM2D_DATAWRITER_H
#define SIM2D_DATAWRITER_H

#include <cstdint>
#include <cstddef>

namespace Sim2d
	{	
	class DataWriter
		{
		public:
			virtual bool chunkIDWrite(const char* header_name)=0;
			virtual bool chunkSizeWrite(uint64_t size)=0;
			virtual size_t dataWrite(const void* buffer,size_t buffer_size)=0;
		};
	};

#endif
