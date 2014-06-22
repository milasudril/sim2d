#ifdef __WAND__
target[name[modelinfofactory.h]  type[include]]
#endif

#ifndef SIM2D_MODELINFOFACTORY_H
#define SIM2D_MODELINFOFACTORY_H

namespace Sim2d
	{
	class ModelInfo;
	
	typedef ModelInfo* (*ModelInfoFactory)();
	}

#endif
