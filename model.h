#ifdef __WAND__
target[name[model.h] type[include]]
dependency[model.o]
#endif

#ifndef SIM2D_MODEL_H
#define SIM2D_MODEL_H

#include "modelinfofactory.h"
#include "modelinfo.h"

namespace Sim2d
	{
	class Bufferinfo;
	
	class Model
		{
		public:
			Model(ModelInfo& model);
			
			~Model();
			
			unsigned int nVarsGet() const
				{return n_vars;}
			
			const char_t* const* varnamesGet() const
				{return model_loaded.varnamesGet();}
				
			const char_t* nameGet() const
				{return model_loaded.nameGet();}
			
			ModelState* stateCreate(uint32_t seed) const
				{return model_loaded.stateCreate(*model_setup,seed);}
				
			ModelSetup& setupGet()
				{return *model_setup;}
			
			void pixelsPull(const Bufferinfo& src,Bufferinfo& dest
				,unsigned int varindex) const
				{model_loaded.pixelsPull(src,dest,varindex);}
				
			void pixelsPush(const Bufferinfo& src,Bufferinfo& dest
				,unsigned int varindex) const
				{model_loaded.pixelsPush(src,dest,varindex);}
			
		private:
			ModelInfo& model_loaded;
			ModelSetup* model_setup;
			size_t n_vars;
		};
	}
	
#endif
