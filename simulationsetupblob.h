#ifdef __WAND__
target[name[simulationsetupblob.h] type[include]]
#endif

#ifndef SIM2D_SIMULATIONSETUPBLOB_H
#define SIM2D_SIMULATIONSETUPBLOB_H

#include <cstdint>

namespace Sim2d
	{
	struct SimulationSetupBlob
		{
		int64_t framecounter;
		int64_t num_iter;
		uint32_t domain_width;
		uint32_t domain_height;
		uint32_t seed;
		uint32_t n_threads;
		uint32_t ui_refreshrate;
		uint32_t reserved[3];
		};
	}

#endif
