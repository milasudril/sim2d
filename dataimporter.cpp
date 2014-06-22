#ifdef __WAND__
target[name[dataimporter.o] type[object]]
#endif

#include "dataimporter.h"
#include <vector/matrixstorage.h>
#include <imagio/reader.h>
#include <imagio/metadata.h>
#include <imagio/pixel.h>
#include <herbs/filein/filein.h>

void Sim2d::DataImporter::pixelsPull(Vector::MatrixStorage<ValueType>& matrix)
	{
	Herbs::FileIn source(m_filename);
	ImagIO::Reader reader(source);
		
	Vector::MatrixStorage<ImagIO::Pixel> temp(1,1);
	ImagIO::Metadata metadata;
		
	reader.dataRead(temp,metadata);
	
	matrix.resize(temp.nRowsGet(),temp.nColsGet());
	
	for(size_t k=0;k<matrix.nRowsGet();++k)
		{
		for(size_t l=0;l<matrix.nColsGet();++l)
			{
			matrix(k,l)=temp(k,l).red;
			}
		}
	}
