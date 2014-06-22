#ifdef __WAND__
target[name[test_modelinfo.o] type[object]]
#endif

#include "test_modelinfo.h"
#include "test_modelsetup.h"
#include "test_modelstate.h"
#include "test_pixeldata.h"
#include "bufferinfo.h"

Sim2d::ModelSetup* Sim2d::Test_ModelInfo::setupCreate() const
	{
	return new Test_ModelSetup;
	}

Sim2d::ModelState* Sim2d::Test_ModelInfo::stateCreate(const ModelSetup& setup
	,uint32_t seed) const
	{
	return new Test_ModelState((const Test_ModelSetup&)(setup),seed);
	}

void Sim2d::Test_ModelInfo::destroy()
	{}

const char_t* Sim2d::Test_ModelInfo::nameGet() const
	{
	return STR("Gray-Scott Pattern formation");
	}
	
static const char_t* varnames[]={STR("u"),STR("v"),nullptr};

const char_t* const* Sim2d::Test_ModelInfo::varnamesGet() const
	{
	return varnames;
	}

void Sim2d::Test_ModelInfo::pixelsPull(const Bufferinfo& src,Bufferinfo& dest
	,unsigned int varindex) const
	{
	float s_x=float(src.widthGet<Test_PixelData>())/dest.widthGet<ValueType>();
	float s_y=float(src.heightGet())/dest.heightGet();

	auto buffer_dest=dest.bufferGet<ValueType>();
	auto buffer_src=src.bufferGet<ValueType>();
	
	for(uint32_t k=0;k<dest.heightGet();++k)
		{
		for(uint32_t l=0;l<dest.widthGet<ValueType>();++l)
			{
			size_t s_k=size_t(s_y*k);
			size_t s_l=size_t(s_x*l);
			
			buffer_dest[k][l]=buffer_src[s_k][2*s_l+varindex];
			}
		}	
	}

void Sim2d::Test_ModelInfo::pixelsPush(const Bufferinfo& src,Bufferinfo& dest
	,unsigned int varindex) const
	{
	float s_x=float(src.widthGet<ValueType>())/dest.widthGet<Test_PixelData>();
	float s_y=float(src.heightGet())/dest.heightGet();

	auto buffer_dest=dest.bufferGet<ValueType>();
	auto buffer_src=src.bufferGet<ValueType>();
	
	for(uint32_t k=0;k<dest.heightGet();++k)
		{
		for(uint32_t l=0;l<dest.widthGet<Test_PixelData>();++l)
			{
			size_t s_k=size_t(s_y*k);
			size_t s_l=size_t(s_x*l);
			
			buffer_dest[k][2*l + varindex]=buffer_src[s_k][s_l];
			}
		}	
	}
