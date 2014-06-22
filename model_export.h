#ifdef __WAND__
target[name[model_export.h] type[include]]
#endif

#ifndef SIM2D_MODELEXPORT_H
#define SIM2D_MODELEXPROT_H

namespace Sim2d
	{
	class Model;
	}

#include <herbs/module/dllentry.h>

extern "C"
	{
	Sim2d::Model* DLLSYMBOL_EXPORT(Sim2d_Model_instanceCreate)();
	}

#endif

