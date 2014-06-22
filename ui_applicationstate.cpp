#ifdef __WAND__
target[name[ui_applicationstate.o] type[object]]
#endif

#include "ui_applicationstate.h"
#include "ui_fonts.h"
#include "ui_mainwin.h"
#include <gui/gui.h>
#include <gui/button_push.h>
#include <gui/combobox.h>
#include <gui/label.h>
#include <herbs/path/path.h>
#include <herbs/exceptionmissing/exceptionmissing.h>
	
Sim2d::UiApplicationState::UiApplicationState(Gui::Gui& gui_obj,uint32_t style_0
	,uint32_t style_1,Window* parent,UiMainwin& ui_controller):
	WindowCustom(gui_obj,style_0,style_1,parent)
	,m_ui_controller(&ui_controller)
	{
	for(uint32_t k=0;k<buttonIndexGet(CommandId::N_BUTTONS);++k)
		{
		if(k==buttonIndexGet(CommandId::SIM_START))
			{
			buttons[k]=Gui::ButtonPush::create(gui_obj,0
				,StyleVisible|StyleChild,this);
			}
		else
			{
			buttons[k]=Gui::Button::create(gui_obj,0
				,StyleVisible|StyleChild,this);
			}
		buttons[k]->idSet(k+COMMANDID_FIRST);
		}
	buttons[buttonIndexGet(CommandId::MODEL_LOAD)]->titleSet(STR("ƒ(x, y, t)"));
	buttons[buttonIndexGet(CommandId::STATE_OPEN)]->titleSet(STR("✇"));
	buttons[buttonIndexGet(CommandId::STATE_SAVE)]->titleSet(STR("✍"));
	buttons[buttonIndexGet(CommandId::SIM_SETUP)]->titleSet(STR("❏Sim"));
	buttons[buttonIndexGet(CommandId::SIM_CREATE)]->titleSet(STR("✳"));
	buttons[buttonIndexGet(CommandId::SIM_START)]->titleSet(STR("▶"));
	buttons[buttonIndexGet(CommandId::SIM_STOP)]->titleSet(STR("▪"));
	buttons[buttonIndexGet(CommandId::MOD_SETUP)]->titleSet(STR("❏Mod"));
	
	varindex_label=Gui::Label::create(gui_obj,0,StyleVisible|StyleChild,this);
	varindex_label->titleSet(STR("Visad variabel: "));
	varindex_selector=Gui::Combobox::create(gui_obj,0,StyleVisible|StyleChild
		,this);
	varindex_selector->idSet((uint32_t)CommandId::VARINDEX_SEL);
	m_ui_controller=&ui_controller;
	
	data_import=Gui::Button::create(gui_obj,0,StyleVisible|StyleChild,this);
	data_import->titleSet(STR(" ⇓"));
	data_import->idSet((uint32_t)CommandId::DATA_IMPORT);
	
	data_export=Gui::Button::create(gui_obj,0,StyleVisible|StyleChild,this);
	data_export->titleSet(STR(" ⇑"));
	data_export->idSet((uint32_t)CommandId::DATA_EXPORT);
	
	sizeMinimize();
	}

void Sim2d::UiApplicationState::simulationRunningSet(bool state)
	{
	((Gui::ButtonPush*)buttons[buttonIndexGet(CommandId::SIM_START)] )
				->checkNoBlock(state);
	}

void Sim2d::UiApplicationState::modelVarnamesFill
	(const char_t* const * varnames)
	{
	varindex_selector->itemsClear();
	while(*varnames!=nullptr)
		{
		varindex_selector->itemAdd(*varnames);
		++varnames;
		}
	varindex_selector->itemAdd(STR("Ingen"));
	varindex_selector->itemSelect(0);
	}

void Sim2d::UiApplicationState::fontsSet(const UiFonts& fonts)
	{
	if(fonts.content_large!=nullptr)
		{
		for(uint32_t k=0;k<buttonIndexGet(CommandId::N_BUTTONS);++k)
			{
			if(buttons[k]->titleGet().length()>1)
				{buttons[k]->fontChangeRequest(*fonts.content);}
			else
				{buttons[k]->fontChangeRequest(*fonts.content_large);}
			}
		data_import->fontChangeRequest(*fonts.content_large);
		data_export->fontChangeRequest(*fonts.content_large);
		}
	
	if(fonts.label!=nullptr)
		{varindex_label->fontChangeRequest(*fonts.label);}
	
	if(fonts.content!=nullptr)
		{
		varindex_selector->fontChangeRequest(*fonts.content);
		}

	sizeMinimize();
	}

Vector::Vector2d<int> Sim2d::UiApplicationState::sizeUpdate()
	{
	Vector::Vector2d<int> ret={0,0};

//	Find largest button
	Vector::Vector2d<int> size_max={0,0};
	for(uint32_t k=0;k<buttonIndexGet(CommandId::N_BUTTONS);++k)
		{
		buttons[k]->sizeMinimize();
		auto size_new=buttons[k]->sizeWindowGet();
		size_max=
			{std::max(size_new.x,size_max.x),std::max(size_new.y,size_max.y)};
		}

//	Set the size of all buttons to the size of the largest and move them
	for(uint32_t k=0;k<buttonIndexGet(CommandId::N_BUTTONS);++k)
		{
		buttons[k]->sizeAbsolute(size_max.x,size_max.y);
		buttons[k]->moveAbsolute({-1,-1},{2+float(k*(size_max.x+2) ),2});
		}
	
	ret=
		{
		int(2+buttonIndexGet(CommandId::N_BUTTONS)*(size_max.x+2) + 32)
		,int(6+size_max.y)
		};
		
	varindex_label->sizeMinimize();
	auto size_res=varindex_label->sizeWindowGet();
	varindex_label->moveAbsolute({-1,-1},{2,float(ret.y)});
	Vector::Vector2d<int> newline={2+size_res.x,ret.y};
	ret={ret.x , ret.y+size_res.y};	
	
	varindex_selector->sizeMinimize();
	varindex_selector->sizeAbsolute(128,0);
	size_res=varindex_selector->sizeWindowGet();
	varindex_selector->moveAbsolute({-1,-1},{float(newline.x),float(newline.y)});
	newline.x+=size_res.x+2;
	ret={ret.x ,std::max(newline.y+size_res.y,ret.y)+2};
	
	data_import->sizeMinimize();
	size_res=data_import->sizeWindowGet();
	data_import->moveAbsolute({-1,-1},{float(newline.x),float(newline.y)});
	
	
	data_export->sizeMinimize();
	size_res=data_export->sizeWindowGet();
	newline.x+=size_res.x+2;
	data_export->moveAbsolute({-1,-1},{float(newline.x),float(newline.y)});
	
	ret={ret.x ,std::max(newline.y+size_res.y,ret.y)+2};
	return ret;
	}

size_t Sim2d::UiApplicationState::onEvent(uint32_t message_id
	,size_t param_0,size_t param_1)
	{
	switch(message_id)
		{
		case MessageSize:
			{
			auto size_min=sizeUpdate();
			auto size_this=sizeWindowGet();
			sizeAbsolute(std::max(size_this.x,size_min.x)
				,std::max(size_this.y,size_min.y));
			}
			break;
		}
	return WindowCustom::onEvent(message_id,param_0,param_1);
	}

void Sim2d::UiApplicationState::varindexSel(uint32_t event_status)
	{
	if(event_status==Gui::Combobox::NotifySelchange)
		{
		m_ui_controller->simulationVarindexSelect
			(varindex_selector->itemSelectedGet());
		}
	}

void Sim2d::UiApplicationState::commandThis(uint32_t event_status
	,CommandId command_id)
	{
	switch(command_id)
		{
		case CommandId::MODEL_LOAD:
			m_ui_controller->modelLoad();
			break;
		case CommandId::STATE_OPEN:
			m_ui_controller->stateOpen();
			break;
		case CommandId::STATE_SAVE:
			m_ui_controller->stateSave();
			break; 
		case CommandId::SIM_SETUP:
			m_ui_controller->simulationSetupdlgShow();
			break;
		case CommandId::SIM_CREATE:
			m_ui_controller->simulationCreate();
			m_ui_controller->simulationVarindexSelect
				(varindex_selector->itemSelectedGet());
			break;
		case CommandId::SIM_START:
			m_ui_controller->simulationStart();
			break;
		case CommandId::SIM_STOP:
			m_ui_controller->simulationStop();
			break;
		case CommandId::MOD_SETUP:
			m_ui_controller->modelSetupdlgShow();
			break;
		case CommandId::VARINDEX_SEL:
			varindexSel(event_status);
			break;
		case CommandId::DATA_IMPORT:
			m_ui_controller->dataImport();
			break;
		case CommandId::DATA_EXPORT:
			m_ui_controller->dataExport();
			break;
		default:
			throw Herbs::ExceptionMissing(___FILE__,__LINE__);
		}
	}

size_t Sim2d::UiApplicationState::onCommand(uint32_t event_status
	,uint32_t command_id,Window& source)
	{
	commandThis(event_status,(CommandId)command_id);
	return 0;
	}

