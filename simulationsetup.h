#ifdef __WAND__
target[name[simulationsetup.h] type[include]]
dependency[simulationsetup.o]
#endif

#ifndef SIM2D_SIMULATIONSETUP_H
#define SIM2D_SIMULATIONSETUP_H

#include "simulationsetupblob.h"

#include <config/paramdescriptor.h>
namespace ChunkIO
	{
	class Writer;
	class Reader;
	}

namespace Sim2d
	{
	struct SimulationSetup:public Config::Paramdescriptor
		{
		SimulationSetup();
		
		void validate() const;
		const char_t* titleGet() const;
		Herbs::ListPacked paraminfoGet();
		
		void store(ChunkIO::Writer& dest);
		void load(ChunkIO::Reader& source);
		
		size_t chunkSizeGet() const
			{return sizeof(blob);}
		
		
		SimulationSetupBlob blob;
		};


	};

#endif
