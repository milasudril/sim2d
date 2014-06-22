#ifdef __WAND__
target[name[modelinfo_export.h] type[include]]
#endif

#ifndef SIM2D_STATEMODELEXPORT_H
#define SIM2D_STATEMODELEXPROT_H

#include <herbs/module/dllentry.h>

namespace Sim2d
	{
	class ModelInfo;
	}

extern "C"
	{
	Sim2d::ModelInfo* DLLSYMBOL_EXPORT(Sim2d_ModelInfo_create)();
	}

#endif

