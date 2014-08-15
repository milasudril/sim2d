#ifdef __WAND__
target[name[modelstate.h] type[include]]
#endif

#ifndef SIM2D_MODELSTATE_H
#define SIM2D_MODELSTATE_H

#include <cstdint>


namespace Sim2d
	{
	class Bufferinfo;
	class DataReader;
	class DataWriter;
	
	class ModelState
		{
		public:
			virtual void process(uintmax_t framecounter
				,const Bufferinfo& buffer_src,Bufferinfo& buffer_dest
				,unsigned int offset)
				{}
				
			virtual void process(uintmax_t framecounter
				,const Bufferinfo& buffer_src,Bufferinfo& buffer_dest)
				{}
				
			virtual void destroy()=0;
			virtual ~ModelState(){}
			
			virtual size_t stateGlobalSizeGet() const=0;
			virtual bool stateGlobalLoad(DataReader& source)=0;
			virtual bool stateGlobalStore(DataWriter& dest) const=0;
		};
	}

#endif
