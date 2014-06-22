#ifdef __WAND__
target[name[bufferinfo.h] type[include]]
#endif

#ifndef SIM2D_BUFFERINFO_H
#define SIM2D_BUFFERINFO_H

#include "valuetype.h"
#include <cstring>
#include <cstdint>

namespace Sim2d
	{
	class BufferinfoPair;
	void swap(BufferinfoPair& a,BufferinfoPair& b);

	struct Bufferinfo
		{
		
		public:
			Bufferinfo(ValueType* const* buffer,uint32_t width,uint32_t height)
				:m_buffer(buffer),m_width(width),m_height(height)
				{}
		
			template<class T>
			const T* const* bufferGet() const
				{return (const T* const*)m_buffer;}
			
			template<class T>
			T* const* bufferGet() 
				{return (T* const*)m_buffer;}
		
			template<class T>
			uint32_t widthGet() const
				{return m_width/ (sizeof(T)/sizeof(ValueType));}
			
			uint32_t heightGet() const
				{return m_height;}
			
		private:
			ValueType* const* m_buffer;
			uint32_t m_width;
			uint32_t m_height;
			
			friend void swap(BufferinfoPair& a,BufferinfoPair& b);			
		};
	}

#endif
