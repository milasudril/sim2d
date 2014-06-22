#ifdef __WAND__
target[name[rand_bootstrap.h] type[include]]
#endif

#ifndef SIM2D_RANDBOOTSTRAP_H
#define SIM2D_RANDBOOTSTRAP_H

#include <cstdint>

namespace Sim2d
	{
	class RandBootstrap
		{
		public:
			RandBootstrap(uint32_t seed): state_current(seed)
				{}
			
			uint32_t operator()()
				{
				state_current = (a*state_current + c)%m;
				return state_current;
				}
			
		private:
			static constexpr uint64_t a=1664525LL;
			static constexpr uint64_t c=1013904223LL;
			static constexpr uint64_t m=0x100000000LL;
			
			uint64_t state_current;
		};
	};
	
#endif
