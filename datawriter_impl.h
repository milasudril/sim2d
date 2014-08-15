#ifdef __WAND__
target[name[datawriter_impl.h] type[include]]
#endif

#include "datawriter.h"
#include <bigchunk/writer.h>

#ifndef SIM2D_DATAWRITERIMPL_H
#define SIM2D_DATAWRITERIMPL_H

namespace Sim2d
	{
	class DataWriterImpl:public DataWriter,private Bigchunk::Writer
		{
		public:
			DataWriterImpl(Herbs::StreamOut& dest):Bigchunk::Writer(dest){}
		
			bool chunkIDWrite(const char* header_name)
				{return Bigchunk::Writer::chunkIDWrite(header_name);}
			bool chunkSizeWrite(uint64_t size)
				{return Bigchunk::Writer::chunkSizeWrite(size);}
			size_t dataWrite(const void* buffer,size_t buffer_size)
				{return Bigchunk::Writer::chunkSizeWrite(buffer_size);}
		};
	}
	
#endif
