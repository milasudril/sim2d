#ifdef __WAND__
target[name[dataexporter.o] type[object]]
#endif

#include "dataexporter.h"
#include <vector/matrixstorage.h>
#include <imagio/writer.h>
#include <imagio/metadata.h>
#include <imagio/pixel.h>
#include <herbs/fileout/fileout.h>

void Sim2d::DataExporter::pixelsPush(const Vector::MatrixStorage<ValueType>& matrix)
	{
	Vector::MatrixStorage<ImagIO::Pixel> temp(matrix.nRowsGet()
		,matrix.nColsGet());

	for(size_t k=0;k<temp.nRowsGet();++k)
		{
		for(size_t l=0;l<temp.nColsGet();++l)
			{
			temp(k,l).red=matrix(k,l);
			temp(k,l).green=matrix(k,l);
			temp(k,l).blue=matrix(k,l);
			temp(k,l).alpha=1.0f;
			}
		}
	Herbs::FileOut dest(m_filename);
	ImagIO::Writer reader(dest);
	ImagIO::Metadata metadata;
	reader.dataWrite(temp,metadata);
	}
