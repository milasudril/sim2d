#ifdef __WAND__
target[name[sim2d-benchmark.exe] type[application] platform[;Windows]]
target[name[sim2d-benchmark] type[application]]
#endif

#include "simulation.h"
#include "simulationsetupblob.h"
#include "modelloader.h"
#include "model.h"
#include <herbs/path/path.h>
#include <herbs/main/main.h>
#include <herbs/exception/exception.h> 
#include <herbs/logwriterdefault/logwriterdefault.h>

int MAIN(int argc,charsys_t* argv[])
	{
	using namespace Sim2d; 
	Herbs::LogWriterDefault log;
	try
		{
		ModelLoader loader(Herbs::Path(STR("sim2d-noise.dll")));
		
		SimulationSetupBlob setup;

	//Vary parameters. Interesting ones are number of threads and domain size
	//(DO NOT TRY TOO SMALL HEIGHT)
		setup.framecounter=0;
		setup.num_iter=1000; 
		setup.domain_width=256;
		setup.domain_height=256;
		setup.n_threads=4;
		Model model(loader.modelInfoGet());
		Simulation sim(log,setup,model,nullptr);
		sim.run();
		}
	catch(const Herbs::Exception& e)
		{e.print(log);}
	return 0;
	}
 