#ifdef __WAND__
target[name[model.o] type[object]]
#endif

#include "model.h"
#include "modelinfo.h"
#include "modelsetup.h"

Sim2d::Model::Model(ModelInfo& model):model_loaded(model)
	{
	auto ptr_varname=model_loaded.varnamesGet();
	while(*ptr_varname!=nullptr)
		{
		++n_vars;
		++ptr_varname;
		}
	model_setup=model_loaded.setupCreate();
	}

Sim2d::Model::~Model()
	{
	model_setup->destroy();
	}

