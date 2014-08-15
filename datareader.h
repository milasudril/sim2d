#ifdef __WAND__
target[name[datareader.h] type[include]]
#endif

#ifndef SIM2D_DATAREADER_H
#define SIM2D_DATAREADER_H

namespace Herbs
	{
	template<class T>
	class Stringbase;
	}
	
#include <cstddef>

namespace Sim2d
	{
	class DataReader
		{
		public:
			virtual bool headerRead(Herbs::Stringbase<char>& name)=0;
			virtual size_t dataRead(void* buffer,size_t buffer_size)=0;
			virtual bool skip()=0;
		};
	};

#endif
