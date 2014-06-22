#ifdef __WAND__
target[name[modelinfo.h] type[include]]
#endif

#ifndef SIM2D_MODELINFO_H
#define SIM2D_MODELINFO_H

#include <herbs/chartype/chartype.h>
	
namespace Config
	{
	class Paraminfo;
	}

namespace Sim2d
	{
	class ModelState;
	class ModelSetup;
	class Bufferinfo;
	
	/**Objects of a class implementing the ModelInfo interface are used to 
	* describe properties of a loaded model. These properties includes the
	* model name and the name of its state variables.
	*
	* ModelInfo instances also creates ModelState instances, and ModelSetup
	* instances, the former to maintain the current system state, and the
	* latter to store system parameters.
	*
	*/
	class ModelInfo
		{
		public:
			/**This method is called when the instance should be destroyed and
			* deallocated. The implementation shall do the opposite of 
			* Sim2d_Model_instanceCreate.
			*/
			virtual void destroy()=0;
			virtual const char_t* nameGet() const=0;
			virtual const char_t* const* varnamesGet() const=0;
			virtual bool multithreaded() const
				{return 1;}
			virtual ModelSetup* setupCreate() const=0;			
			virtual ModelState* stateCreate(const ModelSetup& setup
				,uint32_t seed) const=0;
			
			/**This method is called when the host needs to pull pixels from
			* the model. For example, it is called when the computed data
			* is exported.
			*
			* \param src contains information about the source buffer.
			* \param dest contains information about the destination buffer.
			* \param varindex gives the current state variable. This index shall
			* be appended to the column index in the source buffer.
			*/
			virtual void pixelsPull(const Bufferinfo& src,Bufferinfo& dest
				,unsigned int varindex) const=0;
				
			/**This method is called when the host needs to push pixels to
			* the model. For example, it is called when initial conditions are
			* set.
			*
			* \param src contains information about the source buffer.
			* \param dest contains information about the destination buffer.
			* \param varindex gives the current state variable. This index shall
			* be appended to the column index in the destination buffer.
			*/
			virtual void pixelsPush(const Bufferinfo& src,Bufferinfo& dest
				,unsigned int varindex) const=0;
			
			virtual ~ModelInfo(){}
		};
	}

#endif
