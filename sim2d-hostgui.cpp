#ifdef __WAND__
target[name[sim2d-hostgui.exe] type[application] platform[;Windows]]
target[name[sim2d-hostgui] type[application] ]
#endif

#include "host_gui.h"
#include <herbs/maindefault/maindefault.h>
#include <utility>

Herbs::Application* Herbs::Application::instanceCreate(Herbs::LogWriter& log
	,Herbs::Directory&& config)
	{
	return new Sim2d::HostGui(log,std::move(config));
	}