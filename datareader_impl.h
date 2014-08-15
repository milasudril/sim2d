#ifdef __WAND__
target[name[datareader_impl.h] type[include]]
#endif

#include "datareader.h"
#include <bigchunk/reader.h>

#ifndef SIM2D_DATAREADERIMPL_H
#define SIM2D_DATAREADERIMPL_H

namespace Sim2d
	{
	class DataReaderImpl:public DataReader,private Bigchunk::Reader
		{
		public:
			DataReaderImpl(Herbs::StreamIn& source):Bigchunk::Reader(source){}
		
			bool headerRead(Herbs::Stringbase<char>& name)
				{return Bigchunk::Reader::headerRead(name);}
				
			size_t dataRead(void* buffer,size_t buffer_size)
				{return Bigchunk::Reader::dataRead(buffer,buffer_size);}
				
			bool skip()
				{return Bigchunk::Reader::skip();}
		};
	}
	
#endif
