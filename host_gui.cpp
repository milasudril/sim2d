#ifdef __WAND__
target[name[host_gui.o] type[object]]
#endif

#include "host_gui.h"
#include "ui_applicationstate.h"
#include "ui_mainwin.h"
#include "ui_fonts.h"

Sim2d::HostGui::HostGui(Herbs::LogWriter& log,Herbs::Directory&& config):
	Gui::Gui(my_sysout),
	 font_header(::Gui::Font::Element::CAPTION)
	,font_label(::Gui::Font::Element::CAPTION_SMALL)
	,font_content(::Gui::Font::Element::WIDGET)
	,font_content_large(::Gui::Font::Element::WIDGET,2)
	{
	::Gui::WindowCustom::init();
	
	mainwin=UiMainwin::create(*this,0,0,nullptr,app_state);
	my_sysout.ownerSet(mainwin);
	fonts={&font_header,&font_label,&font_content,&font_content_large};
	mainwin->fontsSet(fonts);
	mainwin->popup();
	mainwin->sizeRelative(0.5,0.5);
	mainwin->moveRelative({0,0},{0.5,0.5});
	mainwin->show(::Gui::Window::DisplayNormal);
	
	}

void Sim2d::HostGui::destroy()
	{delete this;}
