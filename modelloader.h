#ifdef __WAND__
target[name[modelloader.h] type[include]]
dependency[modelloader.o]
#endif

#ifndef SIM2D_MODELLOADER_H
#define SIM2D_MODELLOADER_H

#include <memory>

namespace ChunkIO
	{
	class Reader;
	class Writer;
	}

namespace Herbs
	{
	class Module;
	class Path;
	}

namespace Sim2d
	{
	class ModelInfo;
	class ModelLoader
		{
		public:
			ModelLoader(ChunkIO::Reader& reader);
			ModelLoader(const Herbs::Path& path);
			
			~ModelLoader();
			
			void store(ChunkIO::Writer& writer) const;
			
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
