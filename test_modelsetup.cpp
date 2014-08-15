#ifdef __WAND__
target[name[test_modelsetup.o] type[object]]
#endif

#include "test_modelsetup.h"
#include "datareader.h"
#include "datawriter.h"
#include <config/param_valueraw_info.h>

const char_t* Sim2d::Test_ModelSetup::titleGet() const
	{
	return STR("Model parameters");
	}
	
Herbs::ListPacked Sim2d::Test_ModelSetup::paraminfoGet()
	{
	return
		{
		Config::ParamValuerawInfo<float>
			{
			 STR("Diffusion ratio")
			,Config::Paraminfo::Type::FLOAT
			,1
			,blob.diff_ratio
			,Config::ParamValueInfo<float>::UpdateMethod::NORMAL
			}
		,Config::ParamValuerawInfo<float>
			{
			 STR("Feed rate")
			,Config::Paraminfo::Type::FLOAT
			,2
			,blob.feed_rate
			,Config::ParamValueInfo<float>::UpdateMethod::NORMAL
			}
		,Config::ParamValuerawInfo<float>
			{
			 STR("Decay rate")
			,Config::Paraminfo::Type::FLOAT
			,2
			,blob.decay_rate
			,Config::ParamValueInfo<float>::UpdateMethod::NORMAL
			}
		};
	}

void Sim2d::Test_ModelSetup::destroy()
	{delete this;}

	
bool Sim2d::Test_ModelSetup::load(DataReader& source)
	{
	return source.dataRead(&blob, chunkSizeGet())==chunkSizeGet();
	}

bool Sim2d::Test_ModelSetup::store(DataWriter& dest) const
	{
	return dest.dataWrite(&blob, chunkSizeGet())==chunkSizeGet();
	}

