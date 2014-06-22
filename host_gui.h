#ifdef __WAND__
target[name[host_gui.h] type[include]]
dependency[host_gui.o]
#endif

#ifndef SIM2D_HOSTGUI_H
#define SIM2D_HOSTGUI_H

#include "applicationstate.h"
#include "ui_fonts.h"
#include <gui/font.h>
#include <gui/gui.h>
#include <gui/logwriter_msgbox.h>

namespace Sim2d
	{
	class UiApplicationState;
	class UiMainwin;

	class HostGui:public Gui::Gui
		{
		public:
			HostGui(Herbs::LogWriter& log,Herbs::Directory&& config);
			void destroy();
		
		private:		
			ApplicationState app_state;
			UiMainwin* mainwin;
			
			::Gui::Font font_header;
			::Gui::Font font_label;
			::Gui::Font font_content;
			::Gui::Font font_content_large;
			
			UiFonts fonts;
			::Gui::LogwriterMsgbox my_sysout;
		};
	}

#endif
