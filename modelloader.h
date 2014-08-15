#ifdef __WAND__
target[name[modelloader.h] type[include]]
dependency[modelloader.o]
#endif

#ifndef SIM2D_MODELLOADER_H
#define SIM2D_MODELLOADER_H

#include <memory>

namespace Herbs
	{
	class Module;
	class Path;
	}

namespace Sim2d
	{
	class ModelInfo;
	class DataReader;
	class DataWriter;
	
	class ModelLoader
		{
		public:
			ModelLoader(DataReader& reader);
			ModelLoader(const Herbs::Path& path);
			
			~ModelLoader();
			
			void store(DataWriter& writer) const;
			
			const Herbs::Path& filenameGet() const;
			
			size_t chunkSizeGet() const;
			
			ModelInfo& modelInfoGet()
				{return *model_info;}
			
		private:
			std::unique_ptr<Herbs::Module> module;
			ModelInfo* model_info;
			void load(const Herbs::Path& path);
		};
	}

#endif
