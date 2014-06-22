#ifdef __WAND__
target[name[ui_simulationview.o] type[object]]
#endif

#include "ui_simulationview.h"
#include "ui_fonts.h"
#include <gui/blitter.h>
#include <gui/label.h>
#include <gui/gui.h>
#include <herbs/thread/thread.h>
#include <herbs/mutexblocklw/mutexblocklw.h>
#include <herbs/runnable/runnable.h>
#include <herbs/tictoc/tictoc.h>
#include <herbs/floatformat/floatformat.h>
#include <herbs/intformat/intformat.h>

#include <windows.h>

class Sim2d::UiSimulationView::PollThreadCallback:public Herbs::Runnable
	{
	public:
		PollThreadCallback(Herbs::LogWriter& writer
			,Gui::Window& rate_view,Gui::Window& counter_view):
			Herbs::Runnable(writer)
			,m_rate_view(rate_view),m_counter_view(counter_view)
			{}

		int run()
			{
			uintmax_t v_0=0;
			while(!m_stop)
				{
				double time;
				uintmax_t diff;
					{
					Herbs::TicToc timer(time);
					Sleep(1000);
					intmax_t x;
						{
						Herbs::MutexBlockLW::Guard guard(counter_mutex);
						x=m_counter;
						}
					diff=x-v_0;
					v_0=x;
					if(m_stop)
						{break;}
					m_counter_view.titleSet(Herbs::IntFormat<uintmax_t>(x));
					m_counter_view.sizeMinimize();
					}
				m_rate_view.titleSet(Herbs::FloatFormat<4>(diff/time));
				}
			m_stop=0;
			return 0;
			}
		
		void stop()
			{m_stop=1;}
			
		void counterUpdate(uintmax_t counter)
			{
			Herbs::MutexBlockLW::Guard guard(counter_mutex);
			m_counter=counter;
			}
	
	private:
		Herbs::MutexBlockLW counter_mutex;
		Gui::Window& m_rate_view;
		Gui::Window& m_counter_view;
		uintmax_t m_counter;
		bool m_stop;
	};

Sim2d::UiSimulationView::UiSimulationView(Gui::Gui& gui_obj,uint32_t style_0
	,uint32_t style_1,Window* parent):
	Gui::WindowCustom(gui_obj,style_0,style_1,parent)
	,poll_thread(nullptr)
	{
	blitter=Gui::Blitter::create(gui_obj,StyleEx_clientedge
		,StyleVisible|StyleChild,this);
		
	rate_label=Gui::Label::create(gui_obj,0,StyleVisible|StyleChild,this);
	rate_label->titleSet(STR("Renderingshastighet/fps: "));
	rate_view=Gui::Label::create(gui_obj,0,StyleVisible|StyleChild,this);
	rate_view->titleSet(STR("0"));
	
	counter_label=Gui::Label::create(gui_obj,0,StyleVisible|StyleChild,this);
	counter_label->titleSet(STR("Antal tidssteg: "));
	counter_view=Gui::Label::create(gui_obj,0,StyleVisible|StyleChild,this);
	counter_view->titleSet(STR("0"));
	
	poll_callback=new PollThreadCallback(gui_obj.sysout()
		,*rate_view,*counter_view);
	poll_thread=new Herbs::Thread(*poll_callback);
	}

Sim2d::UiSimulationView::~UiSimulationView()
	{
	poll_callback->stop();
	delete poll_thread;
	delete poll_callback;
	}
	
void Sim2d::UiSimulationView::fontsSet(const UiFonts& fonts)
	{
	if(fonts.label!=nullptr)
		{
		rate_label->fontChangeRequest(*fonts.label);
		counter_label->fontChangeRequest(*fonts.label);
		}
		
	if(fonts.content!=nullptr)
		{
		rate_view->fontChangeRequest(*fonts.content);
		counter_view->fontChangeRequest(*fonts.content);
		}
	}

void Sim2d::UiSimulationView::counterUpdate(uintmax_t counter_new)
	{
	poll_callback->counterUpdate(counter_new);
	}

size_t Sim2d::UiSimulationView::onEvent(uint32_t event_type,size_t param_0
	,size_t param_1)
	{
	switch(event_type)
		{
		case MessageSize:
			{
			rate_label->sizeMinimize();
			auto size_res=rate_label->sizeWindowGet();
			rate_label->moveRelative({-1,1},{0,1});
			rate_view->sizeMinimize();
			rate_view->sizeAbsolute(128,size_res.y);
			auto pos=rate_label->positionAbsoluteGet();
			rate_view->moveAbsolute({-1,1},Vector::cast<float>(size_res+pos));
			
			counter_label->sizeMinimize();
			size_res=counter_label->sizeWindowGet();
			counter_label->moveRelative({-1,1},{0.5,1});
			counter_view->sizeMinimize();
			counter_view->sizeAbsolute(128,size_res.y);
			pos=counter_label->positionAbsoluteGet();
			counter_view->moveAbsolute({-1,1},Vector::cast<float>(size_res+pos));
			
			size_res=sizeClientGet() - Vector::Vector2d<int>(0,size_res.y);
			Herbs::MutexBlockLW::Guard guard(size_block);
			blitter->sizeAbsolute(size_res.x,size_res.y);
			blitter->moveAbsolute({-1,-1},{0,0});
			}
			break;
		}
	
	return WindowCustom::onEvent(event_type,param_0,param_1);
	}
	
Vector::Vector2d<int> Sim2d::UiSimulationView::viewportSizeGet() const
	{
	Vector::Vector2d<int> ret;
		{
		Herbs::MutexBlockLW::Guard guard(size_block);
		ret=blitter->sizeClientGet();
		}
	return ret;
	}

void Sim2d::UiSimulationView::pixelsSet(const Vector::MatrixStorage<ValueType>& pixels)
	{
	blitter->pixelsSet(pixels);
	}
