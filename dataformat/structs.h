
/**Sim2d Simulation State FileThe Sim2d application uses a simulation state file is used to store the current state of a simulation so the simulation can continue at a later moment. A simulation state file stores information about the model chosen, together with the current parameters. It also stores the simulation state matrix in a lossless manner.Simulation state files are primarly intended to use with this implementation of the Sim2d application. This means that the format may change in the future. To avoid compatibility problems, it is best to not use this format for other purpose than interfacing with Sim2d.
*/



/**
*/
struct FileHeader
	{
	
	/**The size of the entire file.
	*/
	uint64_t size;
	
	/**The magic number of the file. In ASCII, this value reads SIM2D\xa\xff\0
	*/
	uint64_t magic_number;
	
	/**Reserved for future use
	*/
	uint64_t reserved;
	
	};



/**
*/
struct SimulationData
	{
	
	/**The size of this chunk. This value equals 64.
	*/
	uint64_t chunk_size;
	
	/**The id of this chunk. In ASCII, this value reads SIMUDATA
	*/
	uint64_t chunk_id;
	
	/**The total number of iterations to be done
	*/
	uint64_t n_iter_tot;
	
	/**The  number of iterations completed
	*/
	uint64_t framecounter;
	
	/**The number of cells in x direction
	*/
	uint32_t domain_width;
	
	/**The number of cells in y direction
	*/
	uint32_t domain_height;
	
	/**The seed used to initiate any random number generators
	*/
	uint32_t seed;
	
	/**The number of computation threads used for the simulation
	*/
	uint32_t n_threads;
	
	/**The display refresh rate
	*/
	uint32_t ui_refreshrate;
	
	/**Reserved for future use
	*/
	uint32_t[3] reserved;
	
	};



/**This chunk contains information about the model used for the simulation. The model code itself is not stored inside the simulation state file, but refered to by its name. To make it easier to restart a simulation, the name of the file contiaining the model code is also stored in the model file. If the name claimed by the module refered to by the given filename does not match the model name, the latter name shall be used to identify the model.
*/
struct ModelInformation
	{
	
	/**The size of this chunk. This value equals 24+2*(model_source_length + model_name_length) .
	*/
	uint64_t chunk_size;
	
	/**The id of this chunk. In ASCII, this value reads MODINFO\0.
	*/
	uint64_t chunk_id;
	
	/**The number of elements in the name of the file conaining the model code.
	*/
	uint32_t model_source_length;
	
	/**The name of the file containing the model code. The string is encoded in UTF-16. 
	*/
	uint16_t[] model_source;
	
	/**The length of the model name
	*/
	uint32_t model_name_length;
	
	/**The name of the model. The string is encoded in UTF-16. 
	*/
	uint16_t[] model_name;
	
	};



/**Model parameters are values that control the behaviour of the system described by the model. Since different models takes different parameters, the content of this chunk is defined by the model code.
*/
struct ModelParameters
	{
	
	/**The size of this chunk. This value equals 16+N, where N is defined by the current model.
	*/
	uint64_t chunk_size;
	
	/**The id of this chunk. In ASCII, this value reads MODPARAM.
	*/
	uint64_t chunk_id;
	
	/**The model parameters
	*/
	uint8_t[] params;
	
	};



/**The global state collects all state variables that are only dependent on time. Since different models contains different state variables, the content of this chunk is defined by the model code.
*/
struct GlobalState
	{
	
	/**The size of this chunk. This value equals 16+N, where N is defined by the current model.
	*/
	uint64_t chunk_size;
	
	/**The id of this chunk. In ASCII, this value reads GLOSTATE.
	*/
	uint64_t chunk_id;
	
	/**The state variables
	*/
	uint8_t[] state_vars;
	
	};



/**The block state collects all state variables that are only dependent on time, but is maintained by individual computation threads. Since different models contains different state variables, the content of this chunk is defined by the model code.
*/
struct BlockState
	{
	
	/**The size of this chunk. This value equals 16+simulation_info.n_threads*N, where N is defined by the current model.
	*/
	uint64_t chunk_size;
	
	/**The id of this chunk. In ASCII, this value reads BLOSTATE.
	*/
	uint64_t chunk_id;
	
	/**The state variables
	*/
	uint8_t[] state_vars;
	
	};



/**This chunk contains the current values for the spatially dependent state variables.
*/
struct CellState
	{
	
	/**The size of this chunk. This value equals 16+4*N*simulation_info.domain_width*simulation_info.domain_height, where N is the number of spatially dependent state variables in the model.
	*/
	uint64_t chunk_size;
	
	/**The id of this chunk. In ASCII, this value reads CELSTATE.
	*/
	uint64_t chunk_id;
	
	/**The state variables
	*/
	ValueType[] state_vars;
	
	};



/**To be able to verify data, the file contains a CRC-32 checksum. This checksum is computed over all bytes in the file up to but not including the checksum field.
*/
struct Checksum
	{
	
	/**The size of this chunk. This value equals 24.
	*/
	uint64_t chunk_size;
	
	/**The id of this chunk. In ASCII this value reads CHECKSUM.
	*/
	uint64_t chunk_id;
	
	/**The CRC-32 checksum computed over all bytes in the file up to this field.
	*/
	uint32_t checksum;
	
	/**Must be zero
	*/
	uint32_t padding;
	
	};

