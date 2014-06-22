#ifdef __WAND__
target[name[noise_modelinfo.o] type[object]]
#endif

#include "noise_modelinfo.h"
#include "noise_modelsetup.h"
#include "noise_modelstate.h"
#include "bufferinfo.h"

Sim2d::ModelSetup* Sim2d::Noise_ModelInfo::setupCreate() const
	{
	return new Noise_ModelSetup;
	}

Sim2d::ModelState* Sim2d::Noise_ModelInfo::stateCreate(const ModelSetup& setup
	,uint32_t seed) const
	{
	return new Noise_ModelState((const Noise_ModelSetup&)(setup),seed);
	}

void Sim2d::Noise_ModelInfo::destroy()
	{}

const char_t* Sim2d::Noise_ModelInfo::nameGet() const
	{
	return STR("White noise");
	}
	
static const char_t* varnames[]={STR("u"),nullptr};

const char_t* const* Sim2d::Noise_ModelInfo::varnamesGet() const
	{
	return varnames;
	}

void Sim2d::Noise_ModelInfo::pixelsPull(const Bufferinfo& src,Bufferinfo& dest
	,unsigned int varindex) const
	{
	float s_x=float(src.widthGet<ValueType>())/dest.widthGet<ValueType>();
	float s_y=float(src.heightGet())/dest.heightGet();

	auto buffer_dest=dest.bufferGet<ValueType>();
	auto buffer_src=src.bufferGet<ValueType>();
	
	for(uint32_t k=0;k<dest.heightGet();++k)
		{
		for(uint32_t l=0;l<dest.widthGet<ValueType>();++l)
			{
			size_t s_k=size_t(s_y*k);
			size_t s_l=size_t(s_x*l);
			
			buffer_dest[k][l]=buffer_src[s_k][s_l];
			}
		}	
	}

void Sim2d::Noise_ModelInfo::pixelsPush(const Bufferinfo& src,Bufferinfo& dest
	,unsigned int varindex) const
	{
	float s_x=float(src.widthGet<ValueType>())/dest.widthGet<ValueType>();
	float s_y=float(src.heightGet())/dest.heightGet();

	auto buffer_dest=dest.bufferGet<ValueType>();
	auto buffer_src=src.bufferGet<ValueType>();
	
	for(uint32_t k=0;k<dest.heightGet();++k)
		{
		for(uint32_t l=0;l<dest.widthGet<ValueType>();++l)
			{
			size_t s_k=size_t(s_y*k);
			size_t s_l=size_t(s_x*l);
			
			buffer_dest[k][l]=buffer_src[s_k][s_l];
			}
		}	
	}
