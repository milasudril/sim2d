#ifdef __WAND__
target[name[dataexporter.h] type[include]]
dependency[dataexporter.o]
#endif

#ifndef SIM2D_DATAEXPORTER_H
#define SIM2D_DATAEXPORTER_H

#include "valuetype.h"
#include <herbs/path/path.h>

namespace Vector
	{
	template<class T>
	class MatrixStorage;
	}

namespace Sim2d
	{
	class DataExporter
		{
		public:
			explicit DataExporter(const Herbs::Path& path):m_filename(path){}
			void pixelsPush(const Vector::MatrixStorage<ValueType>& matrix);
			const Herbs::Path& filenameGet() const
				{return m_filename;}
		private:
			Herbs::Path m_filename;
		};
	}

#endif
