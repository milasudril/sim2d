#ifdef __WAND__
target[name[modelloader.o] type[object]]
#endif

#include "modelloader.h"
#include "modelinfo.h"
#include "modelinfofactory.h"
#include <chunkio/reader.h>
#include <chunkio/writer.h>
#include <herbs/module/dllentry.h>
#include <herbs/module/module.h>

#include <herbs/exceptionmissing/exceptionmissing.h>

Sim2d::ModelLoader::ModelLoader(const Herbs::Path& path)
	{
	load(path);
	}
	
Sim2d::ModelLoader::ModelLoader(ChunkIO::Reader& reader)
	{
	uint32_t val;
	reader.dataRead(&val,sizeof(val));
	switch(val)
		{
		case 0:
			{
			reader.dataRead(&val,sizeof(val));
			Herbs::String temp;
			temp.lengthValidSet(val); 
			reader.dataRead(temp.begin(),val*sizeof(char_t));
			load(Herbs::Path(temp));
			break;
			}
		default:
			throw Herbs::ExceptionMissing(___FILE__,__LINE__);
		}
	}

void Sim2d::ModelLoader::load(const Herbs::Path& path)
	{
	module.reset( new Herbs::Module(path) );	
	auto factory=module->functionGet<ModelInfoFactory>
		(DLLSYMBOL_IMPORT("Sim2d_ModelInfo_create"));
	model_info=factory();
	}
	
const Herbs::Path& Sim2d::ModelLoader::filenameGet() const
	{
	return module->nameGet();
	}

Sim2d::ModelLoader::~ModelLoader()
	{
	model_info->destroy();
	}

void Sim2d::ModelLoader::store(ChunkIO::Writer& writer) const
	{
	Herbs::String str=filenameGet();
	uint32_t val=0;
	writer.dataWrite(&val,sizeof(val));
	val=str.length();
	writer.dataWrite(&val,sizeof(val));
	writer.dataWrite(str.begin(),sizeof(char_t)*str.length());
	}

size_t Sim2d::ModelLoader::chunkSizeGet() const	
	{
	Herbs::String str=filenameGet();
	return 2*sizeof(uint32_t) + sizeof(char_t)*str.length();
	}