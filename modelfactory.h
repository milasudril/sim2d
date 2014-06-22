#ifdef __WAND__
target[name[modelfactory.h]  type[include]]
#endif

#ifndef SIM2D_MODELFACTORY_H
#define SIM2D_MODELFACTORY_H

namespace Sim2d
	{
	class Model;
	
	typedef Model (*ModelFactory)();
	}

#endif
