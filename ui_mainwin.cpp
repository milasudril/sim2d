#ifdef __WAND__
target[name[ui_mainwin.o] type[object]]
#endif

#include "ui_mainwin.h"
#include "ui_applicationstate.h"
#include "model.h"
#include "simulation.h"
#include "ui_simulationview.h"
#include "applicationstate.h"
#include "ui_fonts.h"
#include "modelsetup.h"
#include <gui/gui.h>
#include <gui/dialog.h>
#include <configdlg/dialog.h>
#include <configdlg/dialogbuilder.h>
#include <herbs/exceptionmissing/exceptionmissing.h>
#include <herbs/stringformat/stringformat.h>

Sim2d::UiMainwin::UiMainwin(Gui::Gui& gui_obj,uint32_t style_0,uint32_t style_1
	,Window* parent,ApplicationState& app_state):
	Gui::WindowCustom(gui_obj,style_0,style_1,parent)
	{
	m_state_ctrl=UiApplicationState::create(gui_obj,0
		,StyleVisible|StyleChild,this,*this);

	m_simview=UiSimulationView::create(gui_obj,0
		,StyleChild|StyleVisible,this);
	simsetup_dlg=nullptr;
	model_paramset=nullptr;
	modelsetup_dlg=nullptr;
	applicationStateSet(app_state);
	}
	
void Sim2d::UiMainwin::fontsSet(const UiFonts& fonts)
	{
	m_state_ctrl->fontsSet(fonts);
	m_simview->fontsSet(fonts);
		
	m_fonts=&fonts;
	}
	
Sim2d::UiMainwin::~UiMainwin()
	{
	m_app_state->simulationDestroy();
	if(modelsetup_dlg!=nullptr)
		{modelsetup_dlg->destroy();}
	if(model_paramset!=nullptr)
		{delete model_paramset;}
	}
	
void Sim2d::UiMainwin::appstateUpdated()
	{
	m_state_ctrl->modelVarnamesFill(m_app_state->modelVarnamesGet());
	if(model_paramset!=nullptr)
		{
		if(modelsetup_dlg!=nullptr)
			{
			modelsetup_dlg->destroy();
			modelsetup_dlg=nullptr;
			}
		delete model_paramset;
		}
	titleSet(Herbs::format(STR("Sim2d: %0")
		,{m_app_state->modelNameGet()}).begin());
	model_paramset=new Config::Paramset(m_app_state->modelSetupGet());
	}
	
void Sim2d::UiMainwin::applicationStateSet(ApplicationState& app_state_new)
	{
	m_app_state=&app_state_new;
	
	if(m_app_state->modelLoaded())
		{
		appstateUpdated();
		}
	else
		{
		const char_t* varnames[]={nullptr};
		m_state_ctrl->modelVarnamesFill(varnames);
		titleSet(STR("Sim2d: Ingen modell laddad"));
		}
	
	m_simsetup.dataGet()=m_app_state->simulationSetupGet();
	}
	
void Sim2d::UiMainwin::dataImport()
	{
	Herbs::Path src_old;
	if(m_app_state->dataimporterCreated())
		{src_old=m_app_state->dataimportPathGet();}
	if(Gui::Dialog::prompt(this,STR("Importera variabel"),src_old
		,Gui::Dialog::FILENAMEPROMPT_READ))
		{
		m_app_state->dataimportPathSet(src_old);
		if(m_app_state->simulationCreated())
			{m_app_state->dataimportPixelsPush();}
		}
	}

void Sim2d::UiMainwin::dataExport()
	{
	Herbs::Path dest_old;
	if(m_app_state->dataexporterCreated())
		{dest_old=m_app_state->dataexportPathGet();}
	if(Gui::Dialog::prompt(this,STR("Exportera variabel"),dest_old	
		,Gui::Dialog::FILENAMEPROMPT_WRITE))
		{
		m_app_state->dataexportPathSet(dest_old);
		if(m_app_state->simulationCreated())
			{m_app_state->dataexportPixelsPull();}
		}
	
	}
	
void Sim2d::UiMainwin::modelLoad()
	{
	Herbs::Path load_path;
	if(m_app_state->modelLoaded())
		{
		load_path=m_app_state->modelFilenameGet();
		if(m_app_state->simulationRunning())
			{simulationStop();}
		}
	if(Gui::Dialog::prompt(this,STR("Ladda modell"),load_path
		,Gui::Dialog::FILENAMEPROMPT_READ))
		{
		m_app_state->modelLoad(load_path);
		m_app_state->simulationCreate(m_gui.sysout(),m_simview,m_state_ctrl);
		appstateUpdated();
		}
	}
	
void Sim2d::UiMainwin::stateSave()
	{
	if(m_app_state->simulationCreated() && !m_app_state->simulationRunning())
		{
		Herbs::Path filename;
		if(Gui::Dialog::prompt(this,STR("Spara tillstånd"),filename
			,Gui::Dialog::FILENAMEPROMPT_WRITE))
			{m_app_state->store(filename);}
		}
	}
	
void Sim2d::UiMainwin::stateOpen()
	{
	if(!m_app_state->simulationRunning())
		{
		Herbs::Path filename;
		if(Gui::Dialog::prompt(this,STR("Öppna tillstånd"),filename
			,Gui::Dialog::FILENAMEPROMPT_READ))
			{
			m_app_state->load(filename,m_gui.sysout(),m_simview,m_state_ctrl);
			appstateUpdated();
			}
		}
	}
	
void Sim2d::UiMainwin::simulationStart()
	{
	if(m_app_state->simulationCreated() && !m_app_state->simulationRunning())
		{
		m_app_state->simulationStart();
		m_state_ctrl->simulationRunningSet(1);
		}
	}

void Sim2d::UiMainwin::simulationStop()
	{
	m_app_state->simulationStop();
//	m_state_ctrl->simulationRunningSet(0);
	}
	
void Sim2d::UiMainwin::simulationCreate()
	{
	if(m_app_state->modelLoaded())
		{
		simulationStop();
		m_app_state->simulationCreate(m_gui.sysout(),m_simview,m_state_ctrl);
		}
	}

void Sim2d::UiMainwin::simulationVarindexSelect(unsigned int i)
	{
	if(m_app_state->simulationCreated())
		{
		m_app_state->simulationVarindexSet(i);
		}
	}
	
void Sim2d::UiMainwin::modelSetupdlgShow()
	{
	if(model_paramset!=nullptr)
		{
		if(modelsetup_dlg==nullptr)
			{
			ConfigDlg::DialogBuilder builder(hostGet(),this);
			model_paramset->uiCreate(builder);
			modelsetup_dlg=builder.rootGet();
			modelsetup_dlg->fontsSet(m_fonts->header,m_fonts->label
				,m_fonts->content);
			modelsetup_dlg->sizeAbsolute(256,512);
			modelsetup_dlg->moveRelative({0,0},{0.5,0.5});
			modelsetup_dlg->show(::Gui::Window::DisplayNormal);
			}
		else
			{modelsetup_dlg->focusSet();}	
		}
	}

void Sim2d::UiMainwin::simulationSetupUpdate()
	{
	m_simsetup.dataGet().validate();
	simulationStop();
	m_app_state->simulationDestroy();
	m_app_state->simulationSetupUpdate(m_simsetup.dataGet());
	if(m_app_state->modelLoaded())
		{m_app_state->simulationCreate(m_gui.sysout(),m_simview,m_state_ctrl);}
	}


void Sim2d::UiMainwin::commandModelsetup(uint32_t command_id)
	{
	modelsetup_dlg->destroy();
	modelsetup_dlg=nullptr;
	}
	
	
void Sim2d::UiMainwin::commandSimsetup(uint32_t command_id)
	{
	switch(command_id)
		{
		case Gui::Dialog::MESSAGESTATUS_OK:
			simulationSetupUpdate();
		case Gui::Dialog::MESSAGESTATUS_CANCEL:
			simsetup_dlg->destroy();
			simsetup_dlg=nullptr;
			break;
		case ConfigDlg::Dialog::COMMAND_APPLY:
			simulationSetupUpdate();
			break;
		}
	}

size_t Sim2d::UiMainwin::onCommand(uint32_t event_status
	,uint32_t command_id,Window& source)
	{
	if(&source==simsetup_dlg)
		{
		commandSimsetup(command_id);
		}
	if(&source==modelsetup_dlg)
		{
		commandModelsetup(command_id);
		}
	return 0;
	}

void Sim2d::UiMainwin::simulationSetupdlgShow()
	{
	if(simsetup_dlg==nullptr)
		{
		m_simsetup.dataGet()=m_app_state->simulationSetupGet();
		ConfigDlg::DialogBuilder builder(hostGet(),this);
		m_simsetup.uiCreate(builder);
		simsetup_dlg=builder.rootGet();
		simsetup_dlg->buttonsAdd();
		simsetup_dlg->fontsSet(m_fonts->header,m_fonts->label,m_fonts->content);
		simsetup_dlg->sizeAbsolute(256,512);
		simsetup_dlg->moveRelative({0,0},{0.5,0.5});
		simsetup_dlg->show(::Gui::Window::DisplayNormal);
		}
	else
		{simsetup_dlg->focusSet();}	
	}

size_t Sim2d::UiMainwin::onEvent(uint32_t message_id,size_t param_0,size_t param_1)
	{
	switch(message_id)
		{
		case MessageSize:
			{
			auto size_ctrl=m_state_ctrl->sizeWindowGet();
			auto size_rem=sizeClientGet() - Vector::Vector2d<int>(0,size_ctrl.y);
			m_simview->sizeAbsolute(size_rem.x,size_rem.y);
			m_simview->moveAbsolute({-1,-1},{0,float(size_ctrl.y)});
			}
			break;
		}
	
	return WindowCustom::onEvent(message_id,param_0,param_1);
	}