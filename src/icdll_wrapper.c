#include <stdio.h>    // needed for printToLIS
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <windows.h>
#include "IEEE_Cigre_DLLInterface.h"

/*

    IEEE/CIGRE DLL INTERFACE MODEL WRAPPER

*/

typedef IEEE_Cigre_DLLInterface_Model_Info* ( *GetInfo )( void );
typedef int32_T ( *PrintInfo )( void );
typedef int32_T ( *ModelFirstCall )( IEEE_Cigre_DLLInterface_Instance* instance );
typedef int32_T ( *CheckParameters )( IEEE_Cigre_DLLInterface_Instance* instance ); 
typedef int32_T ( *ModelInitialize )( IEEE_Cigre_DLLInterface_Instance* instance ); 
typedef int32_T ( *ModelOutputs )( IEEE_Cigre_DLLInterface_Instance* instance ); 
typedef int32_T ( *ModelTerminate )( IEEE_Cigre_DLLInterface_Instance* instance );

#define NAME_SIZE 260

typedef struct {
  CheckParameters checkParameters;
  ModelFirstCall firstCall;
  ModelInitialize initialize;
  ModelOutputs outputs;
  ModelTerminate terminate;
  GetInfo getInfo;
  PrintInfo printInfo;
} IEEE_Cigre_DLL_methods;

typedef struct {
  char name[NAME_SIZE+1];
  int32_T used;
  IEEE_Cigre_DLLInterface_Instance *pointer ;
  HMODULE hDLL;
  IEEE_Cigre_DLL_methods model;
  // 
  real64_T dllTimeStep; // instance time step
  real64_T stopTime; // instance simulation stop time
  real64_T nextTime; // instance next call time
  real64_T TRelease; // instance initialization release time
  // size of arrays
  int32_T sizeInputs;
  int32_T sizeOutputs;
  int32_T sizeParameters;
  // number of states
  int32_T numIntStates;
  int32_T numFloatStates;
  int32_T numDoubleStates;
  // type information and offsets
  int32_T *inputsTypes;
  size_t  *inputsOffsets;
  int32_T *outputsTypes;
  size_t  *outputsOffsets;
} Arena;

// memory arena for model instances
#define ARENA_SIZE 100
static Arena arena[ARENA_SIZE];

static int arena_initialized = 0; // to initialize arena only once
static int arena_verbose = 0; // send verbose messages. 0: no messages, 1: normal messages, 2: debug

void initialize_arena() {

  int i;
  for ( i = 0; i < ARENA_SIZE; ++i ) {
    arena[i].used = 0;
    arena[i].name[0] = '\0';
    arena[i].pointer = NULL;
    arena[i].hDLL = NULL;
    arena[i].model.checkParameters = NULL;
    arena[i].model.firstCall = NULL;
    arena[i].model.initialize = NULL;
    arena[i].model.outputs = NULL;
    arena[i].model.terminate = NULL;
    arena[i].model.getInfo = NULL;
    arena[i].model.printInfo = NULL;
    arena[i].dllTimeStep = 0;
    arena[i].stopTime = 0;
    arena[i].nextTime = 0;
    arena[i].TRelease = 0;
    arena[i].sizeInputs = 0;
    arena[i].sizeOutputs = 0;
    arena[i].sizeParameters = 0;
    arena[i].numIntStates = 0;
    arena[i].numFloatStates = 0;
    arena[i].numDoubleStates = 0;
    arena[i].inputsTypes = NULL;
    arena[i].inputsOffsets = NULL;
    arena[i].outputsTypes = NULL;
    arena[i].outputsOffsets = NULL;
  }

}

// atp time step is common for all instances
static real64_T atpTimeStep = 0;

/*
    Definitions to access ATP functions OUTSIX and STOPTP
*/

extern void outsix_( char *, int32_T * );
extern void stoptp_( char *, int * );

/*
    Print to '.LIS' file, using variable arguments
    Example:
       printToLIS("some value = %f", double_value);
*/
void printToLIS( const char *fmt, ... ) {

  char buffer[512];
  int32_T len;
  va_list args;

  va_start( args, fmt );
  vsprintf( buffer, fmt, args );
  va_end( args );
  len = strlen( buffer );
  outsix_( buffer, &len );

}

// Clean instance
void cleanInstance( int32_T id ) {

  Arena* inst;
  inst= &arena[ id ];
  if ( arena_verbose > 1 ) printToLIS( "Cleaning instance #%d - file: %s", id, inst->name );
  free( inst->inputsTypes );
  free( inst->inputsOffsets );
  free( inst->outputsTypes );
  free( inst->outputsOffsets );
  free( inst->pointer->ExternalInputs );
  free( inst->pointer->ExternalOutputs );
  free( inst->pointer->Parameters );
  free( inst->pointer );
  FreeLibrary( inst->hDLL );

}

// Stop the simulation in a 'well-ordered manner'
void stopSim( const char *fmt, ... ) {
  
  Arena* inst;
  int i;
  if ( arena_verbose > 1 ) printToLIS( "ICDLL/CLEANUP" );
  for ( i= 0; i < ARENA_SIZE; ++i ) {
    inst = &arena[i];
    if ( inst->used ) {
      if ( inst->hDLL ){
        cleanInstance( i );
      }
    }
  }

  if ( arena_verbose > 1 ) printToLIS( "ICDLL/CLEANUP ENDED OK" );

  char buffer[512];
  va_list args;

  va_start( args, fmt );
  vsprintf( buffer, fmt, args );
  va_end( args );

  printToLIS( "//////////////////////////////////////////" );
  printToLIS( "ERROR: \n%s", buffer );
  printToLIS( "//////////////////////////////////////////" );
  
  char msg[]= "Stopping ATP simulation due to error";
  int len= strlen( msg );
  stoptp_( msg, &len );

}

/*
  Load the icdll_list.txt file,
*/
void load_icdll_file() {

  FILE* dll_list_fp = fopen( "icdll_list.txt", "r" );
  if ( !dll_list_fp ) {
    stopSim( "Could not open 'icdll_list.txt' file" );
    return;
  }

  size_t count = 0;
  char line[512];
  int id;
  while (fgets(line, sizeof line, dll_list_fp)) {
    char pathbuf[NAME_SIZE+1];

    char* p = line; // p will point to the start of a meaningful line
    while(*p == ' ' || *p == '\t') ++p; // trim leading spaces

    // ignore comments (# ...) and empty lines
    if (*p == '#' || *p == '\n' || *p == '\r' || *p == '\0') {
      ++count;
      continue;
    }

    if (*p == '$') {
      sscanf(p, "$verbose=%d", &arena_verbose);
      ++count;
      continue;
    }
    
    if (sscanf(p, "%d;%260s", &id, pathbuf) != 2){
      fclose(dll_list_fp);
      stopSim( "Parse error in line %zu: %s", count + 1, p );
      return;
    }
    
    if (id >= ARENA_SIZE) {
      fclose(dll_list_fp);
      stopSim( "Parse error in line %zu: %s ; instance id = %d is greater than max(50)", count + 1, p, id);
      return;
    }

    if (arena[id].used == 1){
      fclose(dll_list_fp);
      stopSim( "Parse error in line %zu: %s ; instance id = %d already in use", count + 1, p, id );
      return;
    }

    arena[id].used = 1;
    strncpy(arena[id].name, pathbuf, NAME_SIZE + 1);
    arena[id].name[NAME_SIZE] = '\0'; // safety

    ++count;
  }

  fclose(dll_list_fp);

  // now, print the loaded arena information
  if (arena_verbose > 0) {
    printToLIS( "List of IEEE/Cigre DLLs read:" );
    int i;
    for (i = 0; i <= ARENA_SIZE; ++i){
      if (arena[i].used) printToLIS( "%d : %s", i, arena[i].name );
    }
  }
}

// Compute the amount of memory to reserve depending on the data type and number of 'Inputs', 'Outputs', and 'Parameters' the DLL model needs
void getAlignmentSizeAndOffset( int32_T type, int32_T *alignment, size_t *currentOffset, size_t *offsets, int32_T i ) {

  switch ( type ) {
    case IEEE_Cigre_DLLInterface_DataType_char_T:   *alignment= sizeof( char_T ); break;
    case IEEE_Cigre_DLLInterface_DataType_int8_T:   *alignment= sizeof( int8_T ); break;
    case IEEE_Cigre_DLLInterface_DataType_uint8_T:  *alignment= sizeof( uint8_T ); break;
    case IEEE_Cigre_DLLInterface_DataType_int16_T:  *alignment= sizeof( int16_T ); break;
    case IEEE_Cigre_DLLInterface_DataType_uint16_T: *alignment= sizeof( uint16_T ); break;
    case IEEE_Cigre_DLLInterface_DataType_int32_T:  *alignment= sizeof( int32_T ); break;
    case IEEE_Cigre_DLLInterface_DataType_uint32_T: *alignment= sizeof( uint32_T ); break;
    case IEEE_Cigre_DLLInterface_DataType_real32_T: *alignment= sizeof( real32_T ); break;
    case IEEE_Cigre_DLLInterface_DataType_real64_T: *alignment= sizeof( real64_T ); break;
    // case IEEE_Cigre_DLLInterface_DataType_c_string_T: *alignment= sizeof( '\0' ); break;
  }

  size_t remainder= *currentOffset % *alignment;
  if ( remainder != 0 ) *currentOffset += ( *alignment - remainder ); // If ( remainder != 0 ),  currentOffset remains the same

  offsets[i]= *currentOffset;

  *currentOffset += *alignment;

}

// Assign the data type to a vector based on a structure coming from the dll model
void atp_to_model( double *valuesFromATP, int atp_offset, int *types, size_t *offsets, int size, double *valuesToModel ) { 

  int32_T i;
  uint8_T* value_pos;

  // Write the values into 'valuesToModel':
  for (i = 0; i < size; ++i ) {
    
    value_pos = ( uint8_T* ) valuesToModel + offsets[i];
    
    double from_atp = valuesFromATP[atp_offset + i];
    
    switch ( types[i] ) {
      case IEEE_Cigre_DLLInterface_DataType_char_T:   *( (char_T*)   value_pos ) = ( char_T )  ( from_atp ); break;
      case IEEE_Cigre_DLLInterface_DataType_int8_T:   *( (int8_T*)   value_pos ) = ( int8_T )  ( from_atp ); break;
      case IEEE_Cigre_DLLInterface_DataType_uint8_T:  *( (uint8_T*)  value_pos ) = ( uint8_T ) ( from_atp ); break;
      case IEEE_Cigre_DLLInterface_DataType_int16_T:  *( (int16_T*)  value_pos ) = ( int16_T ) ( from_atp ); break;
      case IEEE_Cigre_DLLInterface_DataType_uint16_T: *( (uint16_T*) value_pos ) = ( uint16_T )( from_atp ); break;
      case IEEE_Cigre_DLLInterface_DataType_int32_T:  *( (int32_T*)  value_pos ) = ( int32_T ) ( from_atp ); break;
      case IEEE_Cigre_DLLInterface_DataType_uint32_T: *( (uint32_T*) value_pos ) = ( uint32_T )( from_atp ); break;
      case IEEE_Cigre_DLLInterface_DataType_real32_T: *( (real32_T*) value_pos ) = ( real32_T )( from_atp ); break;
      case IEEE_Cigre_DLLInterface_DataType_real64_T: *( (real64_T*) value_pos ) = ( real64_T )( from_atp ); break;
      default:
        stopSim("mmmm some error in atp_to_model for position %d", i);
        break;
    }
  }  
}

// Create 'Input', 'Output' and 'Parameters' vectors with the data type that the DLL model needs
void* processModelVector( const char *label, int32_T size, int32_T *types, size_t *offsets, IEEE_Cigre_DLLInterface_Model_Info *modelInfo, real64_T *valuesFromATP ) {

  const char **names= malloc( size * sizeof( char * ) );

  if ( !names ) {
    stopSim( "Memory allocation failed in processModelVector for 'names'" );
  }

  int32_T i;
  int32_T atp_offset = 0;

  if ( strcmp(label, "Inputs") == 0)
  {
    //        0  1...
    // xin = [t, inputs...]
    atp_offset = 1;
    for (i = 0; i < size; ++i){
      names[i] = ( const char * ) modelInfo -> InputPortsInfo[i].Name;
      types[i] = ( int32_T ) modelInfo -> InputPortsInfo[i].DataType;
    }
  }
  else if ( strcmp(label, "Outputs") == 0)
  {
    atp_offset = 0;
    for (i = 0; i < size; ++i){
      names[i] = ( const char * ) modelInfo -> OutputPortsInfo[i].Name;
      types[i] = ( int32_T ) modelInfo -> OutputPortsInfo[i].DataType;
    }
  }
  else if ( strcmp(label, "Parameters") == 0)
  {
    //          0   1         2         3         4...
    // xdata = [id, timestep, stoptime, TRelease, data...]
    atp_offset = 4;
    for (i = 0; i < size; ++i){
      names[i]= ( const char * ) modelInfo -> ParametersInfo[i].Name;
      types[i]= ( int32_T ) modelInfo -> ParametersInfo[i].DataType;
    }
  }

  size_t totalSize= 0;
  int32_T alignment= 0;

  for ( i= 0; i < size; ++i ) {
    getAlignmentSizeAndOffset( types[i], &alignment, &totalSize, offsets, i );
  }

  void *valuesToModel= malloc( totalSize );

  if ( !valuesToModel ) {
    stopSim( "Memory allocation failed in processModelVector for 'valuesToModel'\n" );
  }

  // Write the values into 'valuesToModel':
  atp_to_model( valuesFromATP, atp_offset, types, offsets, size, valuesToModel );

  // Print the values
  //for ( i= 0; i < size; i++ ) {
  //  if ( types[i] == IEEE_Cigre_DLLInterface_DataType_int32_T ) {
  //    int32_T value= *( int32_T * )( ( uint8_t * )( valuesToModel ) + offsets[i] );
  //    printToLIS( "%s_M[%d] : %s = %d ( int32_T )\n", label, i, names[i], value );
  //  } else if ( types[i] == IEEE_Cigre_DLLInterface_DataType_real64_T ) {
  //    real64_T value= *( real64_T * )( ( uint8_t * )( valuesToModel ) + offsets[i] );
  //    printToLIS( "%s_M[%d] : %s = %.4f ( real64_T )\n", label, i, names[i], value );
  //  }
  //}

  free( names );
  return valuesToModel;

}

// Return back the values to ATP in 'double' data type
void model_to_atp( void *valuesFromModel, int *types, size_t *offsets, int size, double *valuesToATP ) { 

  int32_T i;
  uint8_T* value_pos;

  for ( i= 0; i < size; i++ ) {

    value_pos = ( uint8_T * ) valuesFromModel + offsets[i];

    switch ( types[i] ) {
      case IEEE_Cigre_DLLInterface_DataType_char_T:   valuesToATP[i] = ( double ) (*( char_T * )  ( value_pos )); break;
      case IEEE_Cigre_DLLInterface_DataType_int8_T:   valuesToATP[i] = ( double ) (*( int8_T * )  ( value_pos )); break;      
      case IEEE_Cigre_DLLInterface_DataType_uint8_T:  valuesToATP[i] = ( double ) (*( uint8_T * ) ( value_pos )); break;
      case IEEE_Cigre_DLLInterface_DataType_int16_T:  valuesToATP[i] = ( double ) (*( int16_T * ) ( value_pos )); break;
      case IEEE_Cigre_DLLInterface_DataType_uint16_T: valuesToATP[i] = ( double ) (*( uint16_T * )( value_pos )); break;
      case IEEE_Cigre_DLLInterface_DataType_int32_T:  valuesToATP[i] = ( double ) (*( int32_T * ) ( value_pos )); break;      
      case IEEE_Cigre_DLLInterface_DataType_uint32_T: valuesToATP[i] = ( double ) (*( uint32_T * )( value_pos )); break;
      case IEEE_Cigre_DLLInterface_DataType_real32_T: valuesToATP[i] = ( double ) (*( real32_T * )( value_pos )); break;
      case IEEE_Cigre_DLLInterface_DataType_real64_T: valuesToATP[i] = ( double ) (*( real64_T * )( value_pos )); break;
    }
  }
}

// Shows "LastGeneralMessage" or "LastErrorMessage" if a warning or an error appears
void showErrorIfAny( int32_T id, int32_T error ) {

  switch (error) {
    case 1: printToLIS( "%s", arena[id].pointer->LastGeneralMessage ); break;
    case 2: stopSim( "%s", arena[id].pointer->LastErrorMessage ); break;
  }

}


/*
  Initialization of the FOREIGN MODELS

  xdata = [id, atpTimeStep, stoptime, TRelease, ...]
  xin = [t, inputs..., initial_outputs...]
  xout = [...] :
  xvar = [...] : memory reserved to the state variables inside the model instance

  The first time this function is called, initializes the memory-arena for the IEEE/Cigre interface dll models.

  With the 'id' specified in the first argument of xdata, load the corresponding DLL, see 'icdll_list.txt' file.
*/

void icdll_init__( double xdata[], double xin[], double xout[], double xvar[] ) {

  int32_T error = 0;

  // Read the external DLL models
  if ( arena_initialized == 0 ) {
    printToLIS( "================================================================================" );
    printToLIS( "            Initializing 'ICDLL' (IEEE/CIGRE DLL INTERFACE MODEL)" );
    printToLIS( "________________________________________________________________________________\n" );
    
    initialize_arena();
    
    load_icdll_file();
    
    arena_initialized= 1;

    if (arena_verbose > 0){
      printToLIS( "________________________________________________________________________________" );
    }
  }

  // We use 'id' as the first xdata parameter, to access the arena[id] and then look for other instances info
  // xdata = [id, timestep, TRelease, ...]
  int id= ( int ) xdata[0];

  Arena* inst= NULL; // convenience pointer to the instance in the Arena

  if ( arena[id].used == 1 ) {
    inst = &arena[id];

    if ( inst->hDLL != NULL ) stopSim( "Instance #%d: This instance was already initialized by another MODELS", id );

    inst->hDLL= LoadLibrary( inst->name );
    if ( inst->hDLL == NULL ) stopSim( "Cannot find dll file \"%s\"", inst->name );

  } else {
    stopSim( "Error using an instance #%d not declared in 'icdll_list.txt' file", id );
  }

  
  // -----
  // no need to call Model_PrintInfo ...we obtain all the information from the Model_GetInfo
  //
  //inst->model.printInfo = (PrintInfo) GetProcAddress( inst->hDLL, "Model_PrintInfo" );
  //if (inst->model.printInfo) {
  //  inst->model.printInfo();
  //}
    
  /*
    Map the DLL functions in the arena[id].model structure
  */

  inst->model.getInfo= ( GetInfo ) GetProcAddress( inst->hDLL, "Model_GetInfo" );
  if (inst->model.getInfo == NULL) {
    stopSim( "Cannot locate 'Model_GetInfo' function in dll '%s'", inst->name );
  }

  inst->model.firstCall= ( ModelFirstCall ) GetProcAddress( inst->hDLL, "Model_FirstCall" );


  inst->model.checkParameters= ( CheckParameters ) GetProcAddress( inst->hDLL, "Model_CheckParameters" );
  if ( inst->model.checkParameters == NULL ) {
    stopSim( "Cannot locate 'Model_CheckParameters' function in dll '%s'\n", inst->name );
  }

  inst->model.initialize= ( ModelInitialize ) GetProcAddress( inst->hDLL, "Model_Initialize" );
  if ( inst->model.initialize == NULL ) {
    stopSim( "Cannot locate 'Model_Initialize' function in dll '%s'\n", inst->name );
  }  

  inst->model.outputs= ( ModelOutputs ) GetProcAddress( inst->hDLL, "Model_Outputs" );
  if ( inst->model.outputs == NULL ) {
    stopSim( "Cannot locate 'Model_Outputs' function in dll '%s'\n", inst->name );    
  }
  
  inst->model.terminate= ( ModelTerminate ) GetProcAddress( inst->hDLL, "Model_Terminate" );

 
  // ___________________________________________________________________


  /*
    Access the model information, to create the supporting structures
  */

  IEEE_Cigre_DLLInterface_Model_Info *modelInfo= inst->model.getInfo();
  printToLIS( "Instance #%d\nModel Name= %s", id, modelInfo->ModelName );

  inst->sizeInputs      = modelInfo->NumInputPorts;
  inst->sizeOutputs     = modelInfo->NumOutputPorts;
  inst->sizeParameters  = modelInfo->NumParameters;

  inst->numIntStates    = modelInfo->NumIntStates;
  inst->numFloatStates  = modelInfo->NumFloatStates;
  inst->numDoubleStates = modelInfo->NumDoubleStates;

  int32_T i;
  real64_T t = ( real64_T ) xin[ 0 ];

  atpTimeStep = ( real64_T ) xdata[ 1 ];
  inst->dllTimeStep = modelInfo->FixedStepBaseSampleTime; 
  inst->stopTime = ( real64_T ) xdata[ 2 ];
  inst->nextTime = 0;
  inst->TRelease= ( real64_T ) xdata[ 3 ];
  if ( arena_verbose > 0 ) {
    printToLIS( "AtpTimeStep= %f\nDllTimeStep= %f\nTRelease= %f\nStopTime= %f", atpTimeStep, inst->dllTimeStep, inst->TRelease, inst->stopTime );
    printToLIS( "N Inputs= %d", inst->sizeInputs );
    printToLIS( "N Outputs= %d", inst->sizeOutputs );
    printToLIS( "N Parameters= %d", inst->sizeParameters );
    printToLIS( "N IntStates= %d", inst->numIntStates );
    printToLIS( "N FloatStates= %d", inst->numFloatStates );
    printToLIS( "N DoubleStates= %d", inst->numDoubleStates ); 
  }
  
  // ___________________________________________________________________

  inst->inputsTypes   = malloc( inst->sizeInputs * sizeof( int32_T ) );  
  inst->inputsOffsets = malloc( inst->sizeInputs * sizeof( size_t ) );
  void *InputSignals  = processModelVector( "Inputs", inst->sizeInputs, inst->inputsTypes, inst->inputsOffsets, modelInfo, xin );

  // ___________________________________________________________________
  // no need to store parameters types and offsets, used only once here
  int32_T *paramsTypes = malloc(  inst->sizeParameters * sizeof( int ) );  
  size_t *paramsOffsets = malloc( inst->sizeParameters * sizeof( size_t ) );
  void *Parameters = processModelVector( "Parameters", inst->sizeParameters, paramsTypes, paramsOffsets, modelInfo, xdata );

  // ___________________________________________________________________

  inst->outputsTypes   = malloc( inst->sizeOutputs * sizeof( int ) );  
  inst->outputsOffsets = malloc( inst->sizeOutputs * sizeof( size_t ) );

  // Initializing outputs array from ATP
  // xin[t, inputs..., initial_outputs...]
  //                   1+sizeInputs ...
  int32_T ixin = 1 + inst->sizeInputs + inst->sizeOutputs;
  for ( i = 1 + inst->sizeInputs; i < ixin; ++i ) {
    xout[ i - 1 - inst->sizeInputs ] = xin[i];
  }

  void *OutputSignals = processModelVector( "Outputs", inst->sizeOutputs, inst->outputsTypes, inst->outputsOffsets, modelInfo, xout );
  
  /*
    Create the INSTANCE
  */
  inst->pointer = ( IEEE_Cigre_DLLInterface_Instance * ) malloc( sizeof( IEEE_Cigre_DLLInterface_Instance ) );

  if ( inst->pointer == NULL ) {
    stopSim( "Instance #%d: Memory allocation failed for this instance \n", id );
  }

  inst->pointer->ExternalInputs  = InputSignals;
  inst->pointer->ExternalOutputs = OutputSignals;
  inst->pointer->Parameters = Parameters;
  inst->pointer->Time = t;
  // ptr_toModel -> SimTool_EMT_RMS_Mode= 1;
  inst->pointer->LastErrorMessage = NULL;//"LastErrorMessage";
  inst->pointer->LastGeneralMessage = NULL;//"LastGeneralMessage";

  // map xvar = [IntStates..., FloatStates..., DoubleStates...]
  inst->pointer->IntStates    = ( inst->numIntStates > 0 ) ? ( int32_T *) xvar : NULL;   
  inst->pointer->FloatStates  = ( inst->numFloatStates > 0 ) ? ( real32_T *) xvar + inst->numIntStates : NULL;  
  inst->pointer->DoubleStates = ( inst->numDoubleStates > 0 ) ? ( real64_T *) xvar + inst->numIntStates + inst->numFloatStates : NULL;
  
  
  // ___________________________________________________________________

  
  
  if ( inst->model.firstCall != NULL ) {
    error= inst->model.firstCall( inst->pointer );
    printToLIS( "Instance #%d: In call to 'Model_FirstCall'\n", id );
    showErrorIfAny( id, error );
  } else {    
    printToLIS( "Instance #%d: Cannot locate 'Model_FirstCall' function  in dll '%s'\n", id, inst->name );
  }


  error= inst->model.checkParameters( inst->pointer );
  if ( error != 0 ) {
    printToLIS( "Instance #%d: In call to 'Model_CheckParameters'\n", id );
    showErrorIfAny( id, error );
  }
  
  
  error= inst->model.initialize( inst->pointer );
  if ( error != 0 ) {
    printToLIS( "Instance #%d: In call to 'Model_Initialize'\n", id );
    showErrorIfAny( id, error );
  }
  
  printToLIS( "________________________________________________________________________________" );

  free( paramsTypes );
  free( paramsOffsets );

}



/*
  Function: ICDLL_EXEC
*/
void icdll_exec__( double xdata[], double xin[], double xout[], double xvar[] ) {  

  int32_T id= ( int ) xdata[0];
  int32_T error= 0;

  real64_T t= ( real64_T ) xin[ 0 ];
  
  Arena* inst= &arena[id];
  
  if ( t >= inst->nextTime ) {
    
    inst->pointer->Time= t;

    // Update the instance at each 'nextTime'
    atp_to_model( xin, 1, inst->inputsTypes, inst->inputsOffsets, inst->sizeInputs, inst->pointer->ExternalInputs );

    if ( inst->TRelease > 0 && t <= inst->TRelease ) {      

      error= inst->model.initialize( inst->pointer );
      showErrorIfAny( id, error );

      error= inst->model.outputs( inst->pointer );
      showErrorIfAny( id, error );

    } else {        

      error= inst->model.outputs( inst->pointer );
      showErrorIfAny( id, error );

      // Return the model's outputs values to ATP
      model_to_atp( inst->pointer->ExternalOutputs, inst->outputsTypes, inst->outputsOffsets, inst->sizeOutputs, xout );

    }

    inst->nextTime += inst->dllTimeStep;

  }

  // real64_T tol= atpTimeStep*0.5;
  if ( t > ( inst->stopTime - atpTimeStep ) + atpTimeStep*0.5 ) {
    printToLIS( "Last timestep: %f - %f", t, ( inst->stopTime - atpTimeStep ) );

    if ( inst->model.terminate != NULL ) {
      error= inst->model.terminate( inst->pointer );
      printToLIS( "In call to 'Model_Terminate' with Instance #%i\n", id );
      showErrorIfAny( id, error );
    } else {    
      printToLIS( "Cannot locate 'Model_Terminate' function in Instance #%i\n", id  );
    }

    error= inst->model.terminate( inst->pointer );
    showErrorIfAny( id, error );
    
    cleanInstance( id );
    
  }

}
