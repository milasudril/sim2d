#ifdef __WAND__
target[name[datablockprocessor.o] type[object]]
#endif

#include "datablockprocessor.h"
#include "model.h"
#include "modelstate.h"

int Sim2d::DatablockProcessor::run()
	{
	m_stop=0;
	next_frame:
		{
		start.wait();
		if(m_stop)
			{return STATUS_OK;}
		m_model->process(m_framecounter,m_buffers[0].first,m_buffers[0].second
			,m_offset);
		swap(m_buffers[0],m_buffers[1]);
		ready.set();
		goto next_frame;
		}
	}

Sim2d::DatablockProcessor::DatablockProcessor(Herbs::LogWriter& lw
	,const Model& model,const uintmax_t& framecounter
	,uint32_t seed
	,BufferinfoPair* buffers
	,unsigned int offset):
	 Herbs::Runnable(lw)
	,m_buffers{buffers[0],buffers[1]}
	,m_framecounter(framecounter),m_offset(offset)
	{
	m_model=model.stateCreate(seed);
	}
	
size_t Sim2d::DatablockProcessor::stateGlobalSizeGet() const
	{
	return m_model->stateGlobalSizeGet();
	}
	
void Sim2d::DatablockProcessor::stateGlobalStore(DataWriter& writer) const
	{
	m_model->stateGlobalStore(writer);
	}

void Sim2d::DatablockProcessor::stateGlobalLoad(DataReader& reader)
	{
	m_model->stateGlobalLoad(reader);
	}

Sim2d::DatablockProcessor::~DatablockProcessor()
	{
	m_model->destroy();
	}
