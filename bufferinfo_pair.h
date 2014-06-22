#ifdef __WAND__
target[name[bufferinfo_pair.h] type[include] ]
dependency[bufferinfo_pair.o]
#endif

#ifndef SIM2D_BUFFERINFOPAIR_H
#define SIM2D_BUFFERINFOPAIR_H

#include "bufferinfo.h"
#include <utility>

namespace Vector
	{
	template<class T>
	class MatrixStorage;
	}

namespace Sim2d
	{
	struct BufferinfoPair
		{
		BufferinfoPair(Vector::MatrixStorage<ValueType>& matrix_first
			,Vector::MatrixStorage<ValueType>& matrix_second
			,uint32_t height_block,uint32_t offset);
		
		Bufferinfo first;
		Bufferinfo second;
		};
		
	inline void swap(BufferinfoPair& a,BufferinfoPair& b)
		{
		::std::swap(a.first.m_buffer,b.first.m_buffer);
		::std::swap(a.second.m_buffer,b.second.m_buffer);
		}
	}

#endif
