#ifdef __WAND__
target[name[bufferinfo_pair.o] type[object]]
#endif

#include "bufferinfo_pair.h"
#include <vector/matrixstorage.h>

Sim2d::BufferinfoPair::BufferinfoPair(
	 Vector::MatrixStorage<ValueType>& matrix_first
	,Vector::MatrixStorage<ValueType>& matrix_second
	,uint32_t height_block,uint32_t offset):
	first
		{
		 matrix_first.rowsGet()
		,uint32_t(matrix_first.nColsGet())
		,uint32_t(matrix_first.nRowsGet())
		}
	,second
		{
		 matrix_second.rowsGet()+offset
		,uint32_t(matrix_second.nColsGet())
		,uint32_t(height_block)
		}
	{}
