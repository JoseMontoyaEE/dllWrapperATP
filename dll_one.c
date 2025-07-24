#include <stdio.h>    // needed for printLIS_
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <windows.h>
#include "IEEE_Cigre_DLLInterface.h"


static HMODULE hDLL = NULL;
static IEEE_Cigre_DLLInterface_Instance *ptr_toModel= NULL;
static FILE *pFile= NULL;

static real64_T timeStep= 0;
static real64_T timeStepDLL= 0;
static real64_T nextTimeStepDLL= 0;
static real64_T TRelease= 0;

static int32_T sizeInputs= 0;
static int32_T sizeOutputs= 0;
static int32_T sizeParams= 0; 

static int32_T *inputsTypes= NULL;  
static size_t *inputsOffsets= NULL;
static int32_T *outputsTypes= NULL;
static size_t *outputsOffsets= NULL; 

typedef int32_T ( *PrintInfo )( void ); 
typedef IEEE_Cigre_DLLInterface_Model_Info* ( *GetInfo )( void );
typedef int32_T ( *ModelFirstCall )( IEEE_Cigre_DLLInterface_Instance* instance );
typedef int32_T ( *CheckParameters )( IEEE_Cigre_DLLInterface_Instance* instance ); 
typedef int32_T ( *ModelInitialize )( IEEE_Cigre_DLLInterface_Instance* instance ); 
typedef int32_T ( *ModelOutputs )( IEEE_Cigre_DLLInterface_Instance* instance ); 
static ModelOutputs modelOutputs;
static ModelInitialize modelInitialize;



void outsix_( char *, int32_T * );

// Print in  '.LIS' file
void printLIS_( const char *fmt, ... ) {
  char buffer[512];                                                                                       
  va_list args;

  // Start processing variable arguments
  va_start( args, fmt );
  vsprintf( buffer, fmt, args );                                                                          
  va_end( args );

  int32_T len= strlen( buffer );                                                                              
  outsix_( buffer, &len );                                                                                

  // Example:
  // printLIS_( "xvar_ar[0]= %.2f, %.2f, %.2f.", xvar_ar[0], xvar_ar[1], xvar_ar[2] );
}

// Read the external dll model
void readDllName( char* FileName ) {

  if ( hDLL == NULL ) {
    hDLL= LoadLibrary( FileName );
    if ( hDLL == NULL ) {
      printLIS_( "Cannot find \"%s\"\n", FileName );
      // exit( EXIT_FAILURE );
    }
  }
}

// Compute the amount of memory to reserve depending on the data type and number of 'Inputs', 'Outputs', and 'Parameters' the DLL model needs
void getAlignmentSizeAndOffset( int32_T type, int32_T *alignment, size_t *currentOffset, size_t *offsets, int32_T i ) {

  switch ( type ) {
    case IEEE_Cigre_DLLInterface_DataType_char_T: *alignment= sizeof( char_T ); break;
    case IEEE_Cigre_DLLInterface_DataType_int8_T: *alignment= sizeof( int8_T ); break;
    case IEEE_Cigre_DLLInterface_DataType_uint8_T: *alignment= sizeof( uint8_T ); break;
    case IEEE_Cigre_DLLInterface_DataType_int16_T: *alignment= sizeof( int16_T ); break;
    case IEEE_Cigre_DLLInterface_DataType_uint16_T: *alignment= sizeof( uint16_T ); break;
    case IEEE_Cigre_DLLInterface_DataType_int32_T: *alignment= sizeof( int32_T ); break;
    case IEEE_Cigre_DLLInterface_DataType_uint32_T: *alignment= sizeof( uint32_T ); break;
    case IEEE_Cigre_DLLInterface_DataType_real32_T: *alignment= sizeof( real32_T ); break;
    case IEEE_Cigre_DLLInterface_DataType_real64_T: *alignment= sizeof( real64_T ); break;
    // case IEEE_Cigre_DLLInterface_DataType_c_string_T: *alignment= sizeof( '\0' ); break;
  }

  size_t remainder= *currentOffset % *alignment;
  if ( remainder != 0 ) *currentOffset += ( *alignment - remainder );                                                                     // If ( remainder != 0 ),  currentOffset remains the same

  offsets[i]= *currentOffset;

  *currentOffset += *alignment;

}

// Assign the data type to a vector based on a structure coming from the dll model
void changeDataType( double *valuesFromATP, int *types, size_t *offsets, int size, double *valuesToModel ) { 

  int32_T i;

  // Write the values into 'valuesToModel':
  for ( i= 0; i < size; i++ ) {

    switch ( types[i] ) {

      case IEEE_Cigre_DLLInterface_DataType_char_T: {
        char_T val= ( char_T )( valuesFromATP[i] );                                                                                             
        *( ( char_T* )( ( uint8_T* ) valuesToModel + offsets[i] ) )= val;
        break;
      }

      case IEEE_Cigre_DLLInterface_DataType_int8_T: {
        int8_T val= ( int8_T )( valuesFromATP[i] );                                                                                             
        *( ( int8_T* )( ( uint8_T* ) valuesToModel + offsets[i] ) )= val;
        break;
      }
      
      case IEEE_Cigre_DLLInterface_DataType_uint8_T: {
        uint8_T val= ( uint8_T )( valuesFromATP[i] );                                                                                             
        *( ( uint8_T* )( ( uint8_T* ) valuesToModel + offsets[i] ) )= val;
        break;
      }

      case IEEE_Cigre_DLLInterface_DataType_int16_T: {
        int16_T val= ( int16_T )( valuesFromATP[i] );                                                                                             
        *( ( int16_T* )( ( uint8_T* ) valuesToModel + offsets[i] ) )= val;
        break;
      }

      case IEEE_Cigre_DLLInterface_DataType_uint16_T: {
        uint16_T val= ( uint16_T )( valuesFromATP[i] );                                                                                             
        *( ( uint16_T* )( ( uint8_T* ) valuesToModel + offsets[i] ) )= val;
        break;
      }

      case IEEE_Cigre_DLLInterface_DataType_int32_T: {
        int32_T val= ( int32_T )( valuesFromATP[i] );                                                                                             
        *( ( int32_T* )( ( uint8_T* ) valuesToModel + offsets[i] ) )= val;
        break;
      }

      case IEEE_Cigre_DLLInterface_DataType_uint32_T: {
        uint32_T val= ( uint32_T )( valuesFromATP[i] );                                                                                             
        *( ( uint32_T* )( ( uint8_T* ) valuesToModel + offsets[i] ) )= val;
        break;
      }

      case IEEE_Cigre_DLLInterface_DataType_real32_T: {
        real32_T val= ( real32_T )( valuesFromATP[i] );                                                                                             
        *( ( real32_T* )( ( uint8_T* ) valuesToModel + offsets[i] ) )= val;
        break;
      }

      case IEEE_Cigre_DLLInterface_DataType_real64_T: {
        real64_T val= ( real64_T )( valuesFromATP[i] );
        *( ( real64_T* )( ( uint8_T* ) valuesToModel + offsets[i] ) )= val;                                                                         // memcpy( ( uint8_t* )buffer + offset, &val, sizeof( real64_T ) );
        break;
      }

    }

  }  

}

// Create 'Input', 'Output' and 'Parameters' vectors with the data type that the DLL model needs
void* processModelVector( const char *label, int32_T size, int32_T *types, size_t *offsets, IEEE_Cigre_DLLInterface_Model_Info *modelInfo, real64_T *valuesFromATP ) {

  const char **names= malloc( size * sizeof( char * ) );

  if ( !names ) {
    fprintf( stderr, "Memory allocation failed in processModelVector for %s\n", label );
    // exit( EXIT_FAILURE );
  }

  int32_T i;
  for ( i= 0; i < size; i++ ) {

    if ( strcmp( label, "Inputs" ) == 0 ) {            
      names[i]= ( const char * ) modelInfo -> InputPortsInfo[i].Name;                                                           
      types[i]= ( int32_T ) modelInfo -> InputPortsInfo[i].DataType;
    } else if ( strcmp( label, "Outputs" ) == 0 ) {
      names[i]= ( const char * ) modelInfo -> OutputPortsInfo[i].Name;                                                           
      types[i]= ( int32_T ) modelInfo -> OutputPortsInfo[i].DataType;
    } else if ( strcmp( label, "Parameters" ) == 0 ) {
      names[i]= ( const char * ) modelInfo -> ParametersInfo[i].Name;                                                            
      types[i]= ( int32_T ) modelInfo -> ParametersInfo[i].DataType;                
    }

    // printLIS_( "%s[%d]= %s, Type= %s\n", label, i, names[i], types[i] == 6 ? "int32_T" : "real64_T" );
  }

  size_t totalSize= 0;
  int32_T alignment= 0;

  for ( i= 0; i < size; i++ ) {
    getAlignmentSizeAndOffset( types[i], &alignment, &totalSize, offsets, i );
  }

  void *valuesToModel= malloc( totalSize );

  if ( !valuesToModel ) {
    fprintf( stderr, "Memory allocation failed in processModelVector for 'valuesToModel' in %s\n", label );
    // exit( EXIT_FAILURE );
  }

  // Write the values into 'valuesToModel':
  changeDataType( valuesFromATP, types, offsets, size, valuesToModel );


  // Print the values
  for ( i= 0; i < size; i++ ) {
    if ( types[i] == IEEE_Cigre_DLLInterface_DataType_int32_T ) {
      int32_T value= *( int32_T * )( ( uint8_t * )( valuesToModel ) + offsets[i] );
      printLIS_( "%s_M[%d] : %s = %d ( int32_T )\n", label, i, names[i], value );
    } else if ( types[i] == IEEE_Cigre_DLLInterface_DataType_real64_T ) {
      real64_T value= *( real64_T * )( ( uint8_t * )( valuesToModel ) + offsets[i] );
      printLIS_( "%s_M[%d] : %s = %.4f ( real64_T )\n", label, i, names[i], value );
    }
  }


  // Clean up internal allocations of 'names'
  free( names );

  return valuesToModel;
}

// Return back the values to ATP in 'double' data type
void writeValuesToATP( void *valuesFromModel, int *types, size_t *offsets, int size, double *valuesToATP ) { 

  int32_T i;
  for ( i= 0; i < size; i++ ) {

    double valueAsDouble;

    switch ( types[i] ) {

      case IEEE_Cigre_DLLInterface_DataType_char_T: {
        char_T val= *( char_T * )( ( uint8_T * ) valuesFromModel + offsets[i] );                                                                                            // Initial validation with the aim of 'jump' the first value from ATP because that value is the simulation time
        valueAsDouble= ( double ) val;
        break;
      }

      case IEEE_Cigre_DLLInterface_DataType_int8_T: {
        int8_T val= *( int8_T * )( ( uint8_t * ) valuesFromModel + offsets[i] );                                                                                            // Initial validation with the aim of 'jump' the first value from ATP because that value is the simulation time
        valueAsDouble= ( double ) val;
        break;
      }
      
      case IEEE_Cigre_DLLInterface_DataType_uint8_T: {
        uint8_T val= *( uint8_T * )( ( uint8_t * ) valuesFromModel + offsets[i] );                                                                                            // Initial validation with the aim of 'jump' the first value from ATP because that value is the simulation time
        valueAsDouble= ( double ) val;
        break;
      }

      case IEEE_Cigre_DLLInterface_DataType_int16_T: {
        int16_T val= *( int16_T * )( ( uint8_t * ) valuesFromModel + offsets[i] );                                                                                            // Initial validation with the aim of 'jump' the first value from ATP because that value is the simulation time
        valueAsDouble= ( double ) val;
        break;
      }

      case IEEE_Cigre_DLLInterface_DataType_uint16_T: {
        uint16_T val= *( uint16_T * )( ( uint8_t * ) valuesFromModel + offsets[i] );                                                                                            // Initial validation with the aim of 'jump' the first value from ATP because that value is the simulation time
        valueAsDouble= ( double ) val;
        break;
      }

      case IEEE_Cigre_DLLInterface_DataType_int32_T: {
        int32_T val= *( int32_T * )( ( uint8_t * ) valuesFromModel + offsets[i] );                                                                                         // Initial validation with the aim of 'jump' the first value from ATP because that value is the simulation time
        valueAsDouble= ( double ) val;
        break;
      }
      
      case IEEE_Cigre_DLLInterface_DataType_uint32_T: {
        uint32_T val= *( uint32_T * )( ( uint8_t * ) valuesFromModel + offsets[i] );                                                                                            // Initial validation with the aim of 'jump' the first value from ATP because that value is the simulation time
        valueAsDouble= ( double ) val;
        break;
      }

      case IEEE_Cigre_DLLInterface_DataType_real32_T: {
        real32_T val= *( real32_T * )( ( uint8_t * ) valuesFromModel + offsets[i] );                                                                                            // Initial validation with the aim of 'jump' the first value from ATP because that value is the simulation time
        valueAsDouble= ( double ) val;
        break;
      }

      case IEEE_Cigre_DLLInterface_DataType_real64_T: {
        real64_T val= *( real64_T * )( ( uint8_t * ) valuesFromModel + offsets[i] );    
        valueAsDouble= ( double ) val;
        break;
      }

    }

    valuesToATP[i]= valueAsDouble;
    
  }

}


void showMessage( int32_T fcn ) {

  if ( fcn == 1 ) {
    printLIS_( "GeneralMessage= %s\n", ptr_toModel -> LastGeneralMessage );
  } else if ( fcn == 2 ) {
    printLIS_( "ErrorMessage= %s\n", ptr_toModel -> LastErrorMessage );
    return;
  }

}


void dll_one_i__( double xdata_ar[], double xin_ar[], double xout_ar[], double xvar_ar[] ) {

  printLIS_( "_________________________________________________________________________________________________________________________________\n\n" );
  printLIS_( "_________________________________________________________________________________________________________________________________\n\n" );

  printLIS_( "Initializing model 'dll_one_i'" );
  
  
  pFile= fopen( "C:/DLL_Files/dll_list.txt", "r" );
  char buf_dll[128]= { 0 };


  if ( pFile != NULL && fgets( buf_dll, sizeof( buf_dll ), pFile ) != NULL ) {
    buf_dll[ strcspn( buf_dll, "\r\n" ) ]= '\0';
  } else {
    printLIS_( "Could not read from file\n" );
    // exit( EXIT_FAILURE );
  }

  fclose( pFile );

  printLIS_( "Archivo txt= %s\n", buf_dll );



  // ___________________________________________________________________

  

  // Read the external DLL model

  readDllName( buf_dll );
  // readDllName( "scm_32" );                                         // realCodeExample     scm_32       scm_Photon



  // ___________________________________________________________________



  // GetInfo function

  GetInfo getInfo= ( GetInfo ) GetProcAddress( hDLL, "Model_GetInfo" );
  if ( getInfo == NULL ) {
    printLIS_( "Cannot locate Model_GetInfo function in dll\n" );
    // exit( EXIT_FAILURE );
  } 

  IEEE_Cigre_DLLInterface_Model_Info *modelInfo= getInfo();
  printLIS_( "Model Inputs: \n Name= %s\n", modelInfo -> ModelName );
  
  sizeInputs=  modelInfo -> NumInputPorts;
  sizeOutputs= modelInfo -> NumOutputPorts;
  sizeParams=  modelInfo -> NumParameters;

  int32_T sizeNumIntStates= modelInfo -> NumIntStates;
  int32_T sizeNumFloatStates= modelInfo -> NumFloatStates;
  int32_T sizeNumDoubleStates= modelInfo -> NumDoubleStates;

  int32_T i;
  real64_T t= ( real64_T ) xin_ar[ sizeInputs + sizeOutputs ];
  timeStep= ( real64_T ) xdata_ar[ sizeParams ];
  timeStepDLL= modelInfo -> FixedStepBaseSampleTime;  
  nextTimeStepDLL= 0;
  TRelease= ( real64_T ) xdata_ar[ sizeParams + 1 ];
  printLIS_( "Time= %f - TimeStep= %f - TimeStepDLL= %f- TRelease= %f\n", t, timeStep, timeStepDLL, TRelease );


  printLIS_( "N Inputs= %d\n", sizeInputs );
  printLIS_( "N Outputs= %d\n", sizeOutputs );
  printLIS_( "N Parameters= %d\n", sizeParams );
  printLIS_( "N IntStates= %d\n", sizeNumIntStates );
  printLIS_( "N FloatStates= %d\n", sizeNumFloatStates );
  printLIS_( "N DoubleStates= %d\n", sizeNumDoubleStates ); 
  


  // ___________________________________________________________________



  // IEEE_Cigre_DLLInterface_Signal - Inputs
  printLIS_( "Model Inputs: \n" );  
  
  inputsTypes= malloc( sizeInputs * sizeof( int32_T ) );  
  inputsOffsets= malloc( sizeInputs * sizeof( size_t ) );
  void *InputSignals= processModelVector( "Inputs", sizeInputs, inputsTypes, inputsOffsets, modelInfo, xin_ar );



  // ___________________________________________________________________



  // IEEE_Cigre_DLLInterface_Parameter
  printLIS_( "Model Parameters: \n" );

  int32_T *paramsTypes= malloc( sizeParams * sizeof( int ) );  
  size_t *paramsOffsets= malloc( sizeParams * sizeof( size_t ) );
  void *Parameters= processModelVector( "Parameters", sizeParams, paramsTypes, paramsOffsets, modelInfo, xdata_ar );

  
  
  // ___________________________________________________________________
  


  // IEEE_Cigre_DLLInterface_Signal - Outputs
  printLIS_( "Model Outputs: \n" );  
    

  outputsTypes= malloc( sizeOutputs * sizeof( int ) );  
  outputsOffsets= malloc( sizeOutputs * sizeof( size_t ) );

    // Initializing outputs array from ATP
  for ( i= sizeInputs; i < ( sizeInputs + sizeOutputs ); ++i ) {
    xout_ar[ i - sizeInputs ]= xin_ar[i];
  }

  void *OutputSignals= processModelVector( "Outputs", sizeOutputs, outputsTypes, outputsOffsets, modelInfo, xout_ar );





  
  // _________________________________________________________________________________________________________________________________



    
  ptr_toModel= ( IEEE_Cigre_DLLInterface_Instance * ) malloc( sizeof( IEEE_Cigre_DLLInterface_Instance ) );

  if ( ptr_toModel == NULL ) {
    printLIS_( "Memory allocation failed for 'toModel'\n" );
    // exit( EXIT_FAILURE );
  }

  ptr_toModel -> ExternalInputs= InputSignals;                    // InputSignals
  ptr_toModel -> ExternalOutputs= OutputSignals;                  // OutputSignals
  ptr_toModel -> Parameters= Parameters;                          // Parameters
  ptr_toModel -> Time= t;
  // ptr_toModel -> SimTool_EMT_RMS_Mode= 1;
  ptr_toModel -> LastErrorMessage= "LastErrorMessage";       
  ptr_toModel -> LastGeneralMessage= "LastGeneralMessage";     

  ptr_toModel -> IntStates= ( sizeNumIntStates > 0 ) ? ( int32_T *) xvar_ar : NULL;   
  ptr_toModel -> FloatStates= ( sizeNumFloatStates > 0 ) ? ( real32_T *) xvar_ar + sizeNumIntStates : NULL;  
  ptr_toModel -> DoubleStates= ( sizeNumDoubleStates > 0 ) ? ( real64_T *) xvar_ar + sizeNumIntStates + sizeNumFloatStates : NULL;
  
  


  
  printLIS_( "_________________________________________________________________________________________________________________________________\n\n" );

  
  
  int32_T firstCall;
  ModelFirstCall modelFirstCall= ( ModelFirstCall ) GetProcAddress( hDLL, "Model_FirstCall" );
  if ( modelFirstCall != NULL ) {
    firstCall= modelFirstCall( ptr_toModel );
    printLIS_( "FirstCall: %i\n", firstCall );
  } else {    
    printLIS_( "FirstCall doesn't exist!\n" );
    return;
  }

  
  // _________________________________________________________________________________________________________________________________


  int32_T checkParams;
  CheckParameters checkParameters= ( CheckParameters ) GetProcAddress( hDLL, "Model_CheckParameters" );
  if ( checkParameters == NULL ) {
    printLIS_( "Cannot locate Model_CheckParameters function in dll\n" );
    // exit( EXIT_FAILURE );
  } else {
    checkParams= checkParameters( ptr_toModel );
    printLIS_( "CheckParams: %i\n", checkParams );
  }
  
  showMessage( checkParams );


  // _________________________________________________________________________________________________________________________________



  modelInitialize= ( ModelInitialize ) GetProcAddress( hDLL, "Model_Initialize" );
  if ( modelInitialize == NULL ) {
    printLIS_( "Cannot locate Model_Initialize function in dll\n" );
    // exit( EXIT_FAILURE );
  }  


  modelOutputs= ( ModelOutputs ) GetProcAddress( hDLL, "Model_Outputs" );
  if ( modelOutputs == NULL ) {
    printLIS_( "Cannot locate 'modelOutputs' function in dll\n" );
    // exit( EXIT_FAILURE );
  }


  // _________________________________________________________________________________________________________________________________



  int32_T modelInit= modelInitialize( ptr_toModel );
  printLIS_( "ModelInit: %i\n", modelInit );        
  showMessage( modelInit );

  
    
}



void dll_one_m__( double xdata_ar[], double xin_ar[], double xout_ar[], double xvar_ar[] ) {  

  int32_T i;
  real64_T t= ( real64_T ) xin_ar[ sizeInputs + sizeOutputs ];                          // Simulation Time
  ptr_toModel->Time= t;

  if ( t >= nextTimeStepDLL ) {

    if ( TRelease > 0 && t <= TRelease) {      

      // Update the instance at each 'nextTimeStepDLL'
      changeDataType( xin_ar, inputsTypes, inputsOffsets, sizeInputs, ptr_toModel -> ExternalInputs );

      int32_T modelInit= modelInitialize( ptr_toModel );
      showMessage( modelInit );

      int32_T modelOut= modelOutputs( ptr_toModel );
      showMessage( modelOut );


    } else {        

      // Update the instance at each 'nextTimeStepDLL'      
      changeDataType( xin_ar, inputsTypes, inputsOffsets, sizeInputs, ptr_toModel -> ExternalInputs );

      int32_T modelOut= modelOutputs( ptr_toModel );
      showMessage( modelOut );

      // Return the model's outputs values to ATP
      writeValuesToATP( ptr_toModel -> ExternalOutputs, outputsTypes, outputsOffsets, sizeOutputs, xout_ar );

    }  
      
    nextTimeStepDLL += timeStepDLL;

  }

}
