#ifdef __WAND__
target[name[simulationsetup.o] type[object]]
#endif

#include "simulationsetup.h"
#include <config/param_valueraw_info.h>
#include <config/param_group_info.h>
#include <chunkio/writer.h>
#include <chunkio/reader.h>
#include <herbs/exception/exception.h>
#include <herbs/thread/thread.h>
#include <herbs/logmessage/logmessage.h>
#include <herbs/logwriter/logwriter.h>
#include <herbs/stringformat/stringformat.h>
#include <herbs/intformat/intformat.h>


Sim2d::SimulationSetup::SimulationSetup()
	:blob{0,-1,256,256,0,Herbs::Thread::hardwareCount(),1024}
	{}
	
namespace
	{
	class DomainsizeException:public Herbs::Exception
		{
		public:
			DomainsizeException(uint32_t w,uint32_t h,uint32_t max):
				m_w(w),m_h(h),m_max(max)
				{}
				
			void print(Herbs::LogWriter& writer) const
				{
				auto message=Herbs::format(STR("Måtten %0×%1 resulterar "
					"i en ogiltig områdesstorlek. Högsta tillåtna storlek är "
					"%2 celler.")
					,{
					 Herbs::IntFormat<uint32_t>(m_w)
					,Herbs::IntFormat<uint32_t>(m_h)
					,Herbs::IntFormat<uint32_t>(m_max)
					});
				
				writer.write({message.begin(),Herbs::LogMessage::Type::ERROR});
				}
				
		private:
			uint32_t m_w;
			uint32_t m_h;
			uint32_t m_max;
		};
	
	class ThreadcountException:public Herbs::Exception
		{
		public:
			ThreadcountException(uint32_t n,uint32_t max):
				m_n(n),m_max(max)
				{}
				
			void print(Herbs::LogWriter& writer) const
				{
				auto message=Herbs::format(STR("Det givna antalet trådar (%0) är "
					"ogiltigt. Maximalt antal trådar är %1.")
					,{
					 Herbs::IntFormat<uint32_t>(m_n)
					,Herbs::IntFormat<uint32_t>(m_max)
					});
				
				writer.write({message.begin(),Herbs::LogMessage::Type::ERROR});
				}
				
		private:
			uint32_t m_n;
			uint32_t m_max;
		};
	}

void Sim2d::SimulationSetup::validate() const
	{
	uint64_t size_domain=(uint64_t)blob.domain_width*(uint64_t)blob.domain_height;

	if(size_domain<1 || size_domain>0x40000000 )
		{throw DomainsizeException(blob.domain_width,blob.domain_height,0x40000000);}
	
	uint32_t n_threads_max=std::min(Herbs::Thread::hardwareCount()
		,blob.domain_height);
	
	if(blob.n_threads<1 || blob.n_threads>n_threads_max)
		{throw ThreadcountException(blob.n_threads,n_threads_max);}
	}
	
const char_t* Sim2d::SimulationSetup::titleGet() const
	{
	return STR("Simuleringsinställningar");
	}

void Sim2d::SimulationSetup::store(ChunkIO::Writer& dest)
	{
	dest.dataWrite(&blob,chunkSizeGet());
	}
	
void Sim2d::SimulationSetup::load(ChunkIO::Reader& source)
	{
	source.dataRead(&blob,chunkSizeGet());
	}
	
Herbs::ListPacked Sim2d::SimulationSetup::paraminfoGet()
	{
	return
		{
		Config::ParamGroupInfo{STR("Tidsomfång"),1,1}
		,Config::ParamValuerawInfo<int64_t>
			{
			STR("Starttid")
			,Config::Paraminfo::Type::INT64
			,2
			,blob.framecounter
			,Config::ParamValueInfo<int64_t>::UpdateMethod::NORMAL
			}
		,Config::ParamValuerawInfo<int64_t>
			{
			STR("Antal iterationer (-1 ↦ ∞)")
			,Config::Paraminfo::Type::INT64
			,3
			,blob.num_iter
			,Config::ParamValueInfo<int64_t>::UpdateMethod::NORMAL
			}
		,Config::ParamGroupInfo{STR("Storlek på område/celler"),2,1}
		,Config::ParamValuerawInfo<uint32_t>
			{
			STR("Bredd")
			,Config::Paraminfo::Type::UINT32
			,4
			,blob.domain_width
			,Config::ParamValueInfo<uint32_t>::UpdateMethod::NORMAL
			}
		,Config::ParamValuerawInfo<uint32_t>
			{
			STR("Höjd")
			,Config::Paraminfo::Type::UINT32
			,5
			,blob.domain_height
			,Config::ParamValueInfo<uint32_t>::UpdateMethod::NORMAL
			}
		,Config::ParamGroupInfo{STR("Slumpinställningar"),6,1}
		,Config::ParamValuerawInfo<uint32_t>
			{
			STR("Frö")
			,Config::Paraminfo::Type::UINT32
			,7
			,blob.seed
			,Config::ParamValueInfo<uint32_t>::UpdateMethod::NORMAL
			}
		,Config::ParamGroupInfo{STR("Prestanda"),8,1}
		,Config::ParamValuerawInfo<uint32_t>
			{
			STR("Antal beräkningstrådar")
			,Config::Paraminfo::Type::UINT32
			,9
			,blob.n_threads
			,Config::ParamValueInfo<uint32_t>::UpdateMethod::NORMAL
			}
		,Config::ParamValuerawInfo<uint32_t>
			{
			STR("Antal tidssteg mellan vyuppdatering")
			,Config::Paraminfo::Type::UINT32
			,10
			,blob.ui_refreshrate
			,Config::ParamValueInfo<uint32_t>::UpdateMethod::NORMAL
			}
		};
	}
