#ifdef __WAND__
target[name[noise_modelstate.o] type[object]]
#endif

#include "noise_modelstate.h"
#include "noise_modelsetup.h"
#include "bufferinfo.h"

#include "datareader.h"
#include "datawriter.h"

Sim2d::Noise_ModelState::Noise_ModelState(const Noise_ModelSetup& params
	,uint32_t seed):
	m_params(params),randgen(seed)
	{}

void Sim2d::Noise_ModelState::process(uintmax_t framecounter
	,const Bufferinfo& buffer_src,Bufferinfo& buffer_dest
	,unsigned int offset)
	{		
	auto ptr_dest=buffer_dest.bufferGet<ValueType>();
	auto ptr_src=buffer_src.bufferGet<ValueType>();
	
	auto W=buffer_dest.widthGet<ValueType>();
	auto h=buffer_dest.heightGet();
	auto H=buffer_src.heightGet();
	
	std::uniform_real_distribution<float> U;

	auto k_start=offset==0?1:0;
		
	for(uint32_t k=k_start; k<h && k+offset<H-1; ++k)
		{
		for(uint32_t l=1;l<W-1;++l)
			{
			ptr_dest[k][l]=U(randgen);
			}
		ptr_dest[k][0]=ptr_dest[k][1];
		ptr_dest[k][W-1]=ptr_dest[k][W-2];
		}
		
	if(offset + h == H)
		{
		for(uint32_t l=0;l<W;++l)
			{ptr_dest[h-1][l]=ptr_dest[h-2][l];}
		}
	if(offset==0)
		{
		for(uint32_t l=0;l<W;++l)
			{ptr_dest[0][l]=ptr_dest[1][l];}
		}
	}

void Sim2d::Noise_ModelState::destroy()
	{delete this;}
	
bool Sim2d::Noise_ModelState::stateGlobalLoad(DataReader& source)
	{
	return source.dataRead(&randgen,stateGlobalSizeGet())
		==stateGlobalSizeGet();
	}

bool Sim2d::Noise_ModelState::stateGlobalStore(DataWriter& dest) const
	{
	return dest.dataWrite(&randgen,stateGlobalSizeGet())
		==stateGlobalSizeGet();
	}
