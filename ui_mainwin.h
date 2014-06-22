#ifdef __WAND__
target[name[ui_mainwin.h] type[include]]
dependency[ui_mainwin.o]
#endif

#ifndef SIM2D_UIMAINWIN_H
#define SIM2D_UIMAINWIN_H

#include "simulationsetup.h"
#include <config/paramset_data.h>
#include <gui/window-custom.h>

namespace ConfigDlg
	{
	class Dialog;
	}

namespace Sim2d
	{
	class ApplicationState;	
	class UiApplicationState;
	class UiSimulationView;
	class UiFonts;
	
	class UiMainwin:public Gui::WindowCustom
		{
		public:
			static UiMainwin* create(Gui::Gui& gui_obj,uint32_t style_0
				,uint32_t style_1,Window* parent,ApplicationState& app_state)
				{
				return new UiMainwin(gui_obj,style_0,style_1,parent,app_state);
				}
				
			void fontsSet(const UiFonts& fonts);
			
			void modelLoad();
			
			void stateOpen();
			void stateSave();
			
			void dataImport();
			void dataExport();
			
			void simulationSetupUpdate();
			void simulationSetupdlgShow();
			void simulationCreate();
			void simulationStart();
			void simulationStop();
			
			void simulationVarindexSelect(unsigned int varindex);
			
			void modelSetupdlgShow();
			
			void applicationStateSet(ApplicationState& app_state_new);
			
			~UiMainwin();
		
		protected:
			UiMainwin(Gui::Gui& gui_obj,uint32_t style_0,uint32_t style_1
				,Window* parent,ApplicationState& app_state);
			
			virtual size_t onEvent(uint32_t message_id,size_t param_0,size_t param_1);

		private:
			Config::ParamsetData<SimulationSetup> m_simsetup;
			ConfigDlg::Dialog* simsetup_dlg;
			Config::Paramset* model_paramset;
			ConfigDlg::Dialog* modelsetup_dlg;
			ApplicationState* m_app_state;
			UiApplicationState* m_state_ctrl;
			UiSimulationView* m_simview;
			const UiFonts* m_fonts;
			
			void commandModelsetup(uint32_t command_id);
			void commandSimsetup(uint32_t command_id);
			size_t onCommand(uint32_t event_status,uint32_t command_id
				,Window& source);
			
			void appstateUpdated();
		};
	}

#endif
