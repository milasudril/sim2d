#ifdef __WAND__
target[name[ui_simulationview.h] type[include]]
dependency[ui_simulationview.o]
#endif

#ifndef SIM2D_UI_SIMULATIONVIEW_H
#define SIM2D_UI_SIMULATIONVIEW_H

#include "valuetype.h"
#include <herbs/mutexblocklw/mutexblocklw.h>
#include <gui/window-custom.h>

namespace Herbs
	{
	class Thread;
	}

namespace Gui
	{
	class Blitter;
	class Label;
	}

namespace Vector
	{
	template<class T>
	class MatrixStorage;
	}

namespace Sim2d
	{
	class UiFonts;
	
	class UiSimulationView:public Gui::WindowCustom
		{
		public:
			static UiSimulationView* create(Gui::Gui& gui_obj,uint32_t style_0
				,uint32_t style_1,Window* parent)
				{
				return new UiSimulationView(gui_obj,style_0,style_1,parent);
				}
				
			void fontsSet(const UiFonts& fonts);
			
			void counterPollStart();
			void counterPollStop();
			
			void counterUpdate(uintmax_t counter_new);
			Vector::Vector2d<int> viewportSizeGet() const;
			void pixelsSet(const Vector::MatrixStorage<ValueType>& pixels);
		
			~UiSimulationView();
		
		protected:
			UiSimulationView(Gui::Gui& gui_obj,uint32_t style_0,uint32_t style_1
				,Window* parent);
				
			virtual size_t onEvent(uint32_t event_type,size_t param_0,size_t param_1);
			
		private:
			Gui::Blitter* blitter;
			Gui::Label* rate_label;
			Gui::Label* rate_view;
			
			Gui::Label* counter_label;
			Gui::Label* counter_view;
			
			class PollThreadCallback;
			class PollThreadCallback* poll_callback;
			Herbs::Thread* poll_thread;
			mutable Herbs::MutexBlockLW size_block;
		};
	}
	
#endif
