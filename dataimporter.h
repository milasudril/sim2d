#ifdef __WAND__
target[name[dataimporter.h] type[include]]
dependency[dataimporter.o]
#endif

#ifndef SIM2D_DATAIMPORTER_H
#define SIM2D_DATAIMPORTER_H

#include "valuetype.h"
#include <herbs/path/path.h>

namespace Vector
	{
	template<class T>
	class MatrixStorage;
	}

namespace Sim2d
	{
	class DataImporter
		{
		public:
			explicit DataImporter(const Herbs::Path& path):m_filename(path){}
			void pixelsPull(Vector::MatrixStorage<ValueType>& matrix);
			const Herbs::Path& filenameGet() const
				{return m_filename;}
		private:
			Herbs::Path m_filename;
		};
	}

#endif
