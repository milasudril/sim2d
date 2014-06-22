#ifdef __WAND__
target[name[ui_applicationstate.h] type[include]]
dependency[ui_applicationstate.o]
#endif

#ifndef SIM2D_UIAPPLICAIONSTATE_H
#define SIM2D_UIAPPLICAIONSTATE_H

#include <gui/window-custom.h>

namespace Gui
	{
	class Button;
	class Font;
	class Label;
	class Combobox;
	}

namespace Sim2d
	{
	class UiFonts;
	class UiMainwin;
	class SimulationSetup;
		
	class UiApplicationState:public Gui::WindowCustom
		{
		public:		
			static UiApplicationState* create(Gui::Gui& gui_obj,uint32_t style_0
				,uint32_t style_1,Window* parent,UiMainwin& ui_controller)
				{
				return new UiApplicationState(gui_obj,style_0,style_1,parent
					,ui_controller);
				}

			void fontsSet(const UiFonts& fonts);
			void simulationRunningSet(bool state);

			void modelVarnamesFill(const char_t* const * varnames);

		protected:
			UiApplicationState(Gui::Gui& gui_obj,uint32_t style_0
				,uint32_t style_1,Window* parent,UiMainwin& ui_controller);
		
		private:			
			size_t onEvent(uint32_t message_id,size_t param_0,size_t param_1);
			
			static constexpr uint32_t COMMANDID_FIRST=0x10;
			enum class CommandId:uint32_t
				{
				 MODEL_LOAD=COMMANDID_FIRST
				,STATE_OPEN
				,STATE_SAVE
				,SIM_SETUP
				,SIM_CREATE
				,SIM_START
				,SIM_STOP
				,MOD_SETUP
				,N_BUTTONS
				,VARINDEX_SEL
				,DATA_IMPORT
				,DATA_EXPORT
				};
				
			static constexpr uint32_t buttonIndexGet(CommandId id)
				{return uint32_t(id) - COMMANDID_FIRST;}
			
			Gui::Button* buttons[uint32_t(CommandId::N_BUTTONS) 
				- COMMANDID_FIRST];
			
			UiMainwin* m_ui_controller;			
				
			Gui::Label* varindex_label;
			Gui::Combobox* varindex_selector;
			Gui::Button* data_import;
			Gui::Button* data_export;

			size_t onCommand(uint32_t event_status,uint32_t command_id
				,Window& source);
			
			Vector::Vector2d<int> sizeUpdate();
			
			void commandSimsetup(uint32_t command_id);
			void varindexSel(uint32_t event_status);
			void commandThis(uint32_t event_status,CommandId command_id);
		};
	}

#endif
