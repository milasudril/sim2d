#ifdef __WAND__
target[name[applicationstate.h] type[include]]
dependency[applicationstate.o]
#endif

#ifndef SIM2D_APPLICATIONSTATE_H
#define SIM2D_APPLICATIONSTATE_H

#include "simulationsetup.h"

namespace Herbs
	{
	class Thread;
	class Path;
	class LogWriter;
	
/*	template<class T>
	class Stringbase;
	typedef Stringbase<char_t> String;*/
	}
	
namespace Gui
	{
	class Blitter;
	}

namespace ChunkIO
	{
	class Reader;
	class Writer;
	}

namespace Sim2d
	{
	class ModelLoader;
	class Model;
	class ModelSetup;
	class Simulation;
	class UiSimulationView;
	class UiApplicationState;
	class DataImporter;
	class DataExporter;
		
	class ApplicationState
		{
		public:
			ApplicationState():importer(nullptr),exporter(nullptr)
				,m_loader(nullptr),m_model(nullptr),m_simulation(nullptr)
				,m_watch(nullptr),m_watchthread(nullptr)
				{}
				
			~ApplicationState();
			
			void load(const Herbs::Path& filename
				,Herbs::LogWriter& logwriter,Sim2d::UiSimulationView* view
				,Sim2d::UiApplicationState* ui_appstate);
			void store(const Herbs::Path& filename);
			
			void modelLoad(const Herbs::Path& model_new);
			bool modelLoaded() const
				{return m_model!=nullptr;}
			const Herbs::Path& modelFilenameGet() const;
			const char_t* const* modelVarnamesGet() const;
			const char_t* modelNameGet() const;
			ModelSetup& modelSetupGet();
			
			void simulationCreate(Herbs::LogWriter& logwriter
				,UiSimulationView* view
				,UiApplicationState* ui_appstate);
			bool simulationCreated() const
				{return m_simulation!=nullptr;}
			void simulationVarindexSet(unsigned int index);
			void simulationSetupUpdate(const SimulationSetup& obj);
			const SimulationSetup& simulationSetupGet() const
				{return simsetup;}
			void simulationDestroy();	
			void simulationStart();
			void simulationStop();
			bool simulationRunning() const
				{return m_watchthread!=nullptr;}
			
			bool dataimporterCreated() const
				{return importer!=nullptr;}
			void dataimportPathSet(const Herbs::Path& path);
			void dataimportPixelsPush();
			const Herbs::Path& dataimportPathGet() const;
			
			bool dataexporterCreated() const
				{return exporter!=nullptr;}
			void dataexportPathSet(const Herbs::Path& path);
			void dataexportPixelsPull() const;
			const Herbs::Path& dataexportPathGet() const;
	
		private:
			void modelUnload();
			class WatchThreadCallback;
			
			SimulationSetup simsetup;
			DataImporter* importer;
			DataExporter* exporter;
			ModelLoader* m_loader;
			Model* m_model;
			Simulation* m_simulation;
			WatchThreadCallback* m_watch;
			Herbs::Thread* m_watchthread;	
		};
	}

#endif
