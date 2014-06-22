#ifdef __WAND__
target[name[test_pixeldata.h] type[include]]
#endif

#ifndef SIM2D_TEST_PIXELDATA_H
#define SIM2D_TEST_PIXELDATA_H

#include "valuetype.h"

namespace Sim2d
	{
	struct Test_PixelData
		{
		Sim2d::ValueType u;
		Sim2d::ValueType v;
		};
	}
	
#endif
