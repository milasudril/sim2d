#ifdef __WAND__
target[name[sim2d-noise.dll] type[lib_dynamic] platform[;Windows]]
target[name[sim2d-noise.so] type[lib_dynamic] platform[;GNU/Linux]]
#endif

#include "modelinfo_export.h"
#include "noise_modelinfo.h"

static Sim2d::Noise_ModelInfo model_info;

Sim2d::ModelInfo* DLLSYMBOL_EXPORT(Sim2d_ModelInfo_create)()
	{
	return &model_info;
	}
