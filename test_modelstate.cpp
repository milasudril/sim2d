#ifdef __WAND__
target[name[test_modelstate.o] type[object]]
#endif

#include "test_modelstate.h"
#include "test_pixeldata.h"
#include "test_modelsetup.h"
#include "bufferinfo.h"

Sim2d::Test_ModelState::Test_ModelState(const Test_ModelSetup& params
	,uint32_t seed):
	m_params(params)
	{}

void Sim2d::Test_ModelState::process(uintmax_t framecounter
	,const Bufferinfo& buffer_src,Bufferinfo& buffer_dest
	,unsigned int offset)
	{		
	auto ptr_dest=buffer_dest.bufferGet<Test_PixelData>();
	auto ptr_src=buffer_src.bufferGet<Test_PixelData>();
	
	auto W=buffer_dest.widthGet<Test_PixelData>();
	auto h=buffer_dest.heightGet();
	auto H=buffer_src.heightGet();
	
	auto F=m_params.blob.feed_rate;
	auto ka=m_params.blob.decay_rate;
	auto d=m_params.blob.diff_ratio;
	auto k_start=offset==0?1:0;

	for(uint32_t k=k_start; k<h && k+offset<H-1; ++k)
		{
		for(uint32_t l=1;l<W-1;++l)
			{
			auto dyy_u=ptr_src[k+offset+1][l].u - 2*ptr_src[k+offset][l].u
				+ ptr_src[k+offset-1][l].u;
			auto dxx_u=ptr_src[k+offset][l+1].u - 2*ptr_src[k+offset][l].u
				+ ptr_src[k+offset][l-1].u;
				
			auto dyy_v=ptr_src[k+offset+1][l].v - 2*ptr_src[k+offset][l].v
				+ ptr_src[k+offset-1][l].v;
			auto dxx_v=ptr_src[k+offset][l+1].v - 2*ptr_src[k+offset][l].v
				+ ptr_src[k+offset][l-1].v;
				
			auto l_u=dxx_u+dyy_u;
			auto l_v=dxx_v+dyy_v;
			
			auto v_u=ptr_src[k+offset][l].u;
			auto v_v=ptr_src[k+offset][l].v;
			
			ptr_dest[k][l].u=v_u + ( d*l_u - v_u*v_v*v_v + F*(1-v_u) )/32.0f;
			ptr_dest[k][l].v=v_v + ( l_v + v_u*v_v*v_v - (F+ka)*v_v )/32.0f;
			
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

void Sim2d::Test_ModelState::destroy()
	{delete this;}
	
bool Sim2d::Test_ModelState::stateGlobalLoad(DataReader& source)
	{return 1;}

bool Sim2d::Test_ModelState::stateGlobalStore(DataWriter& dest) const
	{return 1;}
