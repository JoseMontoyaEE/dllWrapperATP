#include <stdio.h>    // needed for printLIS_
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <windows.h>
#include "IEEE_Cigre_DLLInterface.h"


#define MAXL 256
static HMODULE hDLL = NULL;
static IEEE_Cigre_DLLInterface_Instance *ptr_toModel= NULL;

static double timeStep= 0;
static double timeStepDLL= 0;
static double nextTimeStepDLL= 0;

static int sizeInputs= 0;
static int sizeOutputs= 0;
static int sizeParams= 0; 

static int *outputsTypes= NULL;
static size_t *outputsOffsets= NULL;                                                                                                             // Save the offsets values based on the data type of outputs struct
// static void *Parameters= NULL;

typedef int32_T ( *PrintInfo )( void ); 
typedef IEEE_Cigre_DLLInterface_Model_Info* ( *GetInfo )( void );
typedef int32_T ( *CheckParameters )( IEEE_Cigre_DLLInterface_Instance* instance ); 
typedef int32_T ( *ModelInitialize )( IEEE_Cigre_DLLInterface_Instance* instance ); 
typedef int32_T ( *ModelOutputs )( IEEE_Cigre_DLLInterface_Instance* instance ); 

void outsix_( char *, int * );


// Print in  '.LIS' file
void printLIS_( const char *fmt, ... ) {
  char buffer[512];                                                                                       
  va_list args;

  // Start processing variable arguments
  va_start( args, fmt );
  vsprintf( buffer, fmt, args );                                                                          
  va_end( args );

  int len= strlen( buffer );                                                                              
  outsix_( buffer, &len );                                                                                

  // Example:
  // printLIS_( "xvar_ar[0]= %.2f, %.2f, %.2f.", xvar_ar[0], xvar_ar[1], xvar_ar[2] );
}

// Read the external dll model
void readDllName( char* FileName ) {

  // Removing the final newline character if present
  // FileName[ strcspn( FileName, "\n" ) ]= '\0';

  if ( hDLL == NULL ) {
    hDLL= LoadLibrary( FileName );
    if ( hDLL == NULL ) {
      printLIS_( "Cannot find \"%s\"\n", FileName );
      exit( EXIT_FAILURE );
    }
  }
}

// Compute the amount of memory to reserve depending on the data type and number of 'Inputs', 'Outputs', and 'Parameters' the DLL model needs
void getAlignmentSizeAndOffset( int type, int *alignment, size_t *currentOffset, size_t *offsets, int i ) {

  switch ( type ) {
    case 6: *alignment= sizeof( int32_T ); break;
    case 9: *alignment= sizeof( real64_T ); break;
    // default: *alignment= 1; break;
  }

  size_t remainder= *currentOffset % *alignment;
  if ( remainder != 0 ) *currentOffset += ( *alignment - remainder );                                                                     // If ( remainder != 0 ),  currentOffset remains the same

  offsets[i]= *currentOffset;

  *currentOffset += *alignment;

}

// Create 'Input', 'Output' and 'Parameters' vectors with the data type that the DLL model needs
void *processModelVector( const char *label, int size, int *types, size_t *offsets, IEEE_Cigre_DLLInterface_Model_Info *modelInfo, double *valuesFromATP ) {

  const char **names= malloc( size * sizeof( char * ) );

  if ( !names || !types || !offsets ) {
    fprintf( stderr, "Memory allocation failed in processModelVector for %s\n", label );
    exit( EXIT_FAILURE );
  }

  int i;
  for ( i= 0; i < size; i++ ) {

    if ( strcmp( label, "Inputs" ) == 0 ) {            
      names[i]= ( const char * ) modelInfo -> InputPortsInfo[i].Name;                                                           
      types[i]= ( int ) modelInfo -> InputPortsInfo[i].DataType;
    } else if ( strcmp( label, "Outputs" ) == 0 ) {
      names[i]= ( const char * ) modelInfo -> OutputPortsInfo[i].Name;                                                           
      types[i]= ( int ) modelInfo -> OutputPortsInfo[i].DataType;
    } else if ( strcmp( label, "Parameters" ) == 0 ) {
      names[i]= ( const char * ) modelInfo -> ParametersInfo[i].Name;                                                            
      types[i]= ( int ) modelInfo -> ParametersInfo[i].DataType;                
    }

    // printLIS_( "%s[%d]= %s, Type= %s\n", label, i, names[i], types[i] == 6 ? "int" : "double" );
  }

  size_t totalSize= 0;
  int alignment= 0;

  for ( i= 0; i < size; i++ ) {
    getAlignmentSizeAndOffset( types[i], &alignment, &totalSize, offsets, i );
  }

  void *valuesToModel= malloc( totalSize );

  if ( !valuesToModel ) {
    fprintf( stderr, "Memory allocation failed in processModelVector for 'valuesToModel' in %s\n", label );
    exit( EXIT_FAILURE );
  }

  // Write the values into 'valuesToModel':
  for ( i= 0; i < size; i++ ) {

    if ( types[i] == 6 ) {
      int32_T val= strcmp( label, "Inputs" ) == 0 ? ( int32_T )( valuesFromATP[ i + 1 ] ) : ( int32_T )( valuesFromATP[i] );                    // Initial validation with the aim of 'jump' the first value from ATP because that value is the simulation time
      *( ( int32_T* )( ( uint8_t* ) valuesToModel + offsets[i] ) )= val;                                                                        // memcpy( ( uint8_t* )buffer + offset, &val, sizeof( int32_T ) );
    } else if ( types[i] == 9 ) {
      real64_T val= strcmp( label, "Inputs" ) == 0 ? ( real64_T )( valuesFromATP[ i + 1 ] ) : ( real64_T )( valuesFromATP[i] );
      *( ( real64_T* )( ( uint8_t* ) valuesToModel + offsets[i] ) )= val;                                                                       // memcpy( ( uint8_t* )buffer + offset, &val, sizeof( real64_T ) );
    }

  }    


  // Print the values
  for ( i= 0; i < size; i++ ) {
    if ( types[i] == 6 ) {
      int value= *( int * )( ( uint8_t * )( valuesToModel ) + offsets[i] );
      printLIS_( "%s_M[%d] : %s = %d (int)\n", label, i, names[i], value );
    } else if ( types[i] == 9 ) {
      double value= *( double * )( ( uint8_t * )( valuesToModel ) + offsets[i] );
      printLIS_( "%s_M[%d] : %s = %.4f (double)\n", label, i, names[i], value );
    }
  }


  // Clean up internal allocations of 'names'
  free( names );

  return valuesToModel;
}

// Return back the values to ATP in 'double' data type
void writeValuesToATP( void *valuesFromModel, int *types, size_t *offsets, int size, double *valuesToATP ) { 

  int i;
  for ( i= 0; i < size; i++ ) {

    double valueAsDouble;
    
    if ( types[i] == 6 )  {  
      int32_T val= *( int32_T * )( ( uint8_t * ) valuesFromModel + offsets[i] );
      valueAsDouble= ( double ) val;
    } else if ( types[i] == 9 ) { 
      real64_T val= *( real64_T * )( ( uint8_t * ) valuesFromModel + offsets[i] );
      valueAsDouble= ( double ) val;
    } else {
      valueAsDouble= 0.007;
    }

    valuesToATP[i]= valueAsDouble;
    // valuesToATP[i]= *( double *)( ( uint8_t * ) valuesFromModel + offsets[i] );
  }

}



void dll_one_i__( double xdata_ar[], double xin_ar[], double xout_ar[], double xvar_ar[] ) {

  printLIS_( "Initializing model 'dll_one_i'" );
  

  int i;
  double t= xin_ar[0];


  // Read the external DLL model
  readDllName( "scm_32" );       // realCodeExample     scm_32       SCRX_Photon


  // GetInfo function

  GetInfo getInfo= ( GetInfo )GetProcAddress( hDLL, "Model_GetInfo" );
  if ( getInfo == NULL ) {
    printLIS_( "Cannot locate Model_GetInfo function in dll\n" );
    exit( EXIT_FAILURE );
  }

  IEEE_Cigre_DLLInterface_Model_Info *modelInfo= getInfo();
  printLIS_( "Model Inputs: \n Name= %s\n", modelInfo -> ModelName );
  
  sizeInputs=  modelInfo -> NumInputPorts;
  sizeOutputs= modelInfo -> NumOutputPorts;
  sizeParams=  modelInfo -> NumParameters;
  int sizeNumIntStates= modelInfo -> NumIntStates;
  int sizeNumFloatStates= modelInfo -> NumFloatStates;
  int sizeNumDoubleStates= modelInfo -> NumDoubleStates;

  timeStep= xdata_ar[ sizeParams + sizeOutputs ];
  timeStepDLL= modelInfo -> FixedStepBaseSampleTime;  
  nextTimeStepDLL= 2 * timeStepDLL;
  printLIS_( "TimeStep= %f - TimeStepDLL= %f\n", timeStep, timeStepDLL );


  printLIS_( "N Inputs= %d\n", sizeInputs );
  printLIS_( "N Outputs= %d\n", sizeOutputs );
  printLIS_( "N Parameters= %d\n", sizeParams );
  printLIS_( "N IntStates= %d\n", sizeNumIntStates );
  printLIS_( "N FloatStates= %d\n", sizeNumFloatStates );
  printLIS_( "N DoubleStates= %d\n", sizeNumDoubleStates ); 
  


  printLIS_( "_____________________________________________________________\n\n" );


  // IEEE_Cigre_DLLInterface_Signal - Inputs
  printLIS_( "Model Inputs: \n" );  
  
  int *inputsTypes= malloc( sizeInputs * sizeof( int ) );  
  size_t *inputsOffsets= malloc( sizeInputs * sizeof( size_t ) );
  void *InputSignals= processModelVector( "Inputs", sizeInputs, inputsTypes, inputsOffsets, modelInfo, xin_ar );



  printLIS_( "_____________________________________________________________\n\n" );


  // IEEE_Cigre_DLLInterface_Parameter
  printLIS_( "Model Parameters: \n" );

  int *paramsTypes= malloc( sizeParams * sizeof( int ) );  
  size_t *paramsOffsets= malloc( sizeParams * sizeof( size_t ) );
  void *Parameters= processModelVector( "Parameters", sizeParams, paramsTypes, paramsOffsets, modelInfo, xdata_ar );

  
  
  printLIS_( "_____________________________________________________________\n\n" );
  

  // IEEE_Cigre_DLLInterface_Signal - Outputs
  printLIS_( "Model Outputs: \n" );  
    

  outputsTypes= malloc( sizeOutputs * sizeof( int ) );  
  outputsOffsets= malloc( sizeOutputs * sizeof( size_t ) );

    // Initializing outputs array from ATP
  for ( i= sizeParams; i < ( sizeParams + sizeOutputs ); ++i ) {
    xout_ar[ i - sizeParams ]= xdata_ar[i];
  }

  void *OutputSignals= processModelVector( "Outputs", sizeOutputs, outputsTypes, outputsOffsets, modelInfo, xout_ar );



  printLIS_( "_____________________________________________________________\n\n" );



  int32_T *intStates= malloc( sizeNumIntStates * sizeof( int32_T ) );
  for ( i= 0; i < sizeNumIntStates; ++i ) {
    intStates[i]= 0;
  }

  real32_T *floatStates= malloc( sizeNumFloatStates * sizeof( real32_T ) );
  for ( i= 0; i < sizeNumFloatStates; ++i ) {
    floatStates[i]= 0;
  }

  real64_T *doubleStates= malloc( sizeNumDoubleStates * sizeof( real64_T ) );
  for ( i= 0; i < sizeNumDoubleStates; ++i ) {
    doubleStates[i]= 11;
  }


  
  printLIS_( "_________________________________________________________________________________________________________________________________\n\n" );



    
  ptr_toModel= ( IEEE_Cigre_DLLInterface_Instance * ) malloc( sizeof( IEEE_Cigre_DLLInterface_Instance ) );

  if ( ptr_toModel == NULL ) {
    printLIS_( "Memory allocation failed for 'toModel'\n" );
    exit( EXIT_FAILURE );
  }

  ptr_toModel->ExternalInputs= InputSignals;                    // InputSignals
  ptr_toModel->ExternalOutputs= OutputSignals;                  // OutputSignals
  ptr_toModel->Parameters= Parameters;                          // Parameters
  ptr_toModel->Time= t;
  // ptr_toModel->SimTool_EMT_RMS_Mode= 1;
  ptr_toModel->LastErrorMessage= "LastErrorMessage";       
  ptr_toModel->LastGeneralMessage= "LastGeneralMessage";     
  ptr_toModel->IntStates= intStates;              
  ptr_toModel->FloatStates= floatStates;            
  ptr_toModel->DoubleStates= doubleStates;

  
  printLIS_( "_________________________________________________________________________________________________________________________________\n\n" );



  CheckParameters checkParameters= ( CheckParameters ) GetProcAddress( hDLL, "Model_CheckParameters" );
  if ( checkParameters == NULL ) {
    printLIS_( "Cannot locate Model_CheckParameters function in dll\n" );
    exit( EXIT_FAILURE );
  }

  int32_T checkParams= checkParameters( ptr_toModel );
  printLIS_( "CheckParams: %i\n", checkParams );



  printLIS_( "_________________________________________________________________________________________________________________________________\n\n" );



  ModelInitialize modelInitialize= ( ModelInitialize ) GetProcAddress( hDLL, "Model_Initialize" );
  if ( modelInitialize == NULL ) {
    printLIS_( "Cannot locate Model_Initialize function in dll\n" );
    exit( EXIT_FAILURE );
  }  

  int32_T modelInit= modelInitialize( ptr_toModel );
  printLIS_( "ModelInit: %i\n", modelInit );        






  // free( InputSignals );
  // free( OutputSignals );
  // free( Parameters );

  // free( ptr_toModel );0.005628954



  // Close the dll
  // FreeLibrary(hDLL);
      
    
}



void dll_one_m__( double xdata_ar[], double xin_ar[], double xout_ar[], double xvar_ar[] ) {  

  int i;
  double t=  xin_ar[0];

  if ( t >= nextTimeStepDLL ) {

    // Update the instance at each simulation time
    ptr_toModel->Time= t;

    for ( i= 0; i < sizeInputs; ++i ) {
      ( ( double * ) ptr_toModel->ExternalInputs)[i]= xin_ar[i+1];
      // printLIS_( "toModel->ExternalInputs[%i]= %f\n", i, ( ( double * ) ptr_toModel->ExternalInputs )[i] );
    }

    ModelOutputs modelOutputs= ( ModelOutputs ) GetProcAddress( hDLL, "Model_Outputs" );
    if ( modelOutputs == NULL ) {
      printLIS_( "Cannot locate 'modelOutputs' function in dll\n" );
      exit( EXIT_FAILURE );
    }

    int32_T modelOut= modelOutputs( ptr_toModel );
    // printLIS_( "ModelOutputs: %i\n", modelOut );


    // Return the model's outputs values to ATP
    writeValuesToATP( ptr_toModel->ExternalOutputs, outputsTypes, outputsOffsets, sizeOutputs, xout_ar );

    
    // printLIS_( "NextTimeStepDLL= %f\n", nextTimeStepDLL );
    nextTimeStepDLL += timeStepDLL;
    
    
  }

  // printLIS_( "TimeStep= %f\n", t );
  // printLIS_( "X_out= %f\n", xout_ar[0] );

  // printLIS_( "_________________________________________________________________________________________________________________________________\n\n" );



}
