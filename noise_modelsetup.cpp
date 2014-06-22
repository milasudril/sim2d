#ifdef __WAND__
target[name[noise_modelsetup.o] type[object]]
#endif

#include "noise_modelsetup.h"
#include <chunkio/reader.h>
#include <chunkio/writer.h>
#include <config/param_valueraw_info.h>

const char_t* Sim2d::Noise_ModelSetup::titleGet() const
	{
	return STR("Model parameters");
	}
	
Herbs::ListPacked Sim2d::Noise_ModelSetup::paraminfoGet()
	{
	return
		{
		Config::ParamValuerawInfo<float>
			{
			 STR("No parameters")
			,Config::Paraminfo::Type::FLOAT
			,1
			,dummy
			,Config::ParamValueInfo<float>::UpdateMethod::NORMAL
			}
		};
	}

void Sim2d::Noise_ModelSetup::destroy()
	{delete this;}

bool Sim2d::Noise_ModelSetup::load(ChunkIO::Reader& source)
	{
	return 1;
	}

bool Sim2d::Noise_ModelSetup::store(ChunkIO::Writer& dest) const
	{
	return 1;
	}
