#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "IEEE_Cigre_DLLInterface.h"


typedef int32_T ( *PrintInfo )( void ); 
typedef IEEE_Cigre_DLLInterface_Model_Info* ( *GetInfo )( void );
static GetInfo getInfo;


void variablesNames( const char *label, int32_T size, IEEE_Cigre_DLLInterface_Model_Info *modelInfo, const char **namesVectors ) {

  int i;
  for ( i= 0; i < size; i++ ) {

    if ( strcmp( label, "Inputs" ) == 0 ) {            
      namesVectors[i]= modelInfo -> InputPortsInfo[i].Name;  
    } else if ( strcmp( label, "Outputs" ) == 0 ) {
      namesVectors[i]= modelInfo -> OutputPortsInfo[i].Name;   
    } else if ( strcmp( label, "Parameters" ) == 0 ) {
      namesVectors[i]= modelInfo -> ParametersInfo[i].Name;           
    }

  }
}


void appendSection( char **buffer, const char *sectionTitle, const char **names, int size ) {

  // Append section title
  if ( sectionTitle ) { 
    strcat( *buffer, sectionTitle );
    strcat( *buffer, "\n  " );
  }

  // Append all names separated by commas
  int i;
  for ( i= 0; i < size; i++ ) {

    if ( i == 0 ) strcat( *buffer, "  " );
    strcat( *buffer, names[i] );
    if ( i != size - 1 ) {
      strcat( *buffer, ", " );
    }
  }
  strcat( *buffer, "\n" );
}


void inputSection( char **blueprint, int sizeInputs, const char **namesInputs, int sizeOutputs, const char **namesOutputs ) {
  
  strcat( *blueprint, "  INPUT\n" );

  char *outInit= malloc( 1024 );
  outInit[0]= '\0';
  char out_init[512];
  
  int i;
  for ( i= 0; i < sizeInputs; i++ ) {

    if ( i == 0 ) strcat( outInit, "    " );
    strcat( outInit, namesInputs[i] );
    if ( i != sizeInputs - 1 ) {
      strcat( outInit, ", " );
    }
  }
  
  for ( i= 0; i < sizeOutputs; i++ ) {

    if ( i == 0 ) strcat( outInit, "\n    " );
    sprintf( out_init, "%s_0", namesOutputs[i] );
    if ( i != sizeOutputs - 1 ) strcat( out_init, ", " );
    strcat( outInit, out_init );

  }

  strcat( *blueprint, outInit );  
  strcat( *blueprint, "\n\n" );
  
  free( outInit );
}


void initSection( char **blueprint, 
                  int sizeInputs, 
                  const char **namesInputs, 
                  int sizeOutputs, 
                  const char **namesOutputs, 
                  int sizeParams, 
                  const char **namesParams, 
                  char *inputsFromATP, 
                  char *paramsFromATP, 
                  char *outputsInit ) {
 
  strcat( *blueprint, "  INIT\n" );
    
  char *outInit= malloc( 2024 );
  outInit[0]= '\0';
  char _init[512];
  _init[0]= '\0';

  
  // Initializing the outpus variables with the output's init values
  int i;
  for ( i= 0; i < sizeOutputs; i++ ) {
    sprintf( _init, "    %s:= %s_0 \n", namesOutputs[i], namesOutputs[i] );
    strcat( outInit, _init );
  }

  // Assigning the inputs values to the 'inputsdFromATP' variable
  char inputs_init[512];
  inputs_init[0]= '\0';
  
  for ( i= 0; i < sizeInputs; i++ ) {
    sprintf( _init, "%s", namesInputs[i] );
    strcat( inputs_init, _init );
    if ( i != sizeInputs - 1 ) {
      strcat( inputs_init, ", " );
    }
  }
  
  sprintf( _init, ( sizeInputs > 1 ? "    %s:= [ %s ]\n": "    %s:= %s\n" ), inputsFromATP, inputs_init );
  strcat( outInit, _init );


  // Assigning the parameters values to the 'paramsFromATP' variable
  char params_init[512];
  params_init[0]= '\0';

  for ( i= 0; i < sizeParams; i++ ) {
    sprintf( _init, "%s", namesParams[i] );
    strcat( params_init, _init );
    if ( i != sizeParams - 1 ) {
      strcat( params_init, ", " );
    }
  }

  sprintf( _init, ( sizeParams > 1 ? "    %s:= [ %s ]\n": "    %s:= %s\n" ), paramsFromATP, params_init );
  strcat( outInit, _init );


  // Assigning the outputs values to the 'outputsInit' variable
  char outputs_init[512];
  outputs_init[0]= '\0';

  for ( i= 0; i < sizeOutputs; i++ ) {
    sprintf( _init, "%s_0", namesOutputs[i] );
    strcat( outputs_init, _init );
    if ( i != sizeOutputs - 1 ) {
      strcat( outputs_init, ", " );
    }
  }

  sprintf( _init, ( sizeOutputs > 1 ? "    %s:= [ %s ]\n": "    %s:= %s\n" ), outputsInit, outputs_init );
  strcat( outInit, _init );
  
  strcat( *blueprint, outInit );  
  strcat( *blueprint, "\n" );
  strcat( *blueprint, "  ENDINIT\n\n" );
  
  free( outInit );
}


void fgnSection( char **blueprint, const char *modelName, int sizeInputs, int sizeOutputs, int sizeParams, int sizeNumIntStates, int sizeNumFloatStates, int sizeNumDoubleStates ) {
  
  char *fgnSec= malloc( 512 );
  fgnSec[0]= '\0';

  sprintf( fgnSec, "MODEL %s_dll FOREIGN dll_one { ixdata: %i, ixin: %i, ixout: %i, ixvar: %i }\n", modelName, ( sizeParams + 2 ), ( sizeInputs + 2 ), sizeOutputs, ( sizeNumIntStates + sizeNumFloatStates + sizeNumDoubleStates ) );
  appendSection( blueprint, 0, ( const char ** )&fgnSec, 1 );   

  free( fgnSec );
}


void execSection( char **blueprint, 
                  const char *modelName, 
                  int sizeInputs, 
                  const char **namesInputs, 
                  int sizeOutputs, 
                  const char **namesOutputs, 
                  int sizeParams, 
                  const char **namesParams, 
                  char *inputsFromATP, 
                  char *paramsFromATP, 
                  char *outputsInit ) {

  strcat( *blueprint, "  EXEC\n" );

  char *execSec= malloc( 4096 );
  execSec[0]= '\0';
  char exec_sec[512];
  char exec_aux[512];
  exec_sec[0]= '\0';
  
  int i;
  for ( i= 0; i < sizeInputs; i++ ) {
    sprintf( exec_aux, "%s", namesInputs[i] );
    strcat( exec_sec, exec_aux );
    if ( i != sizeInputs - 1 ) {
      strcat( exec_sec, ", " );
    }
  }
  
  sprintf( exec_aux, ( sizeInputs > 1 ? "    %s:= [ %s ]\n": "    %s:= %s\n" ), inputsFromATP, exec_sec );
  strcat( execSec, exec_aux );


  // Assigning the outputs values to the 'outputsInit' variable
  exec_sec[0]= '\0';
  for ( i= 0; i < sizeOutputs; i++ ) {
    sprintf( exec_aux, "%s", namesOutputs[i] );
    strcat( exec_sec, exec_aux );
    if ( i != sizeOutputs - 1 ) {
      strcat( exec_sec, ", " );
    }
  }

  sprintf( exec_aux, ( sizeOutputs > 1 ? "    %s:= [ %s_0 ]\n": "    %s:= %s_0\n" ), outputsInit, exec_sec );
  strcat( execSec, exec_aux );


  // USE
  exec_sec[0]= '\0';
  exec_aux[0]= '\0';
  sprintf( exec_sec, "\n    USE %s_dll AS %s_dll\n\n", modelName, modelName );
  strcat( execSec, exec_sec );

    // DATA
  strcat( execSec, "      DATA\n" );
  sprintf( exec_sec, "        xdata[1..%i]:= [ %s, timestep, TRelease ]\n\n", ( sizeParams > 1 ? ( sizeParams + 2 ) : 3 ), paramsFromATP );
  strcat( execSec, exec_sec );

    // INPUT
  strcat( execSec, "      INPUT\n" );
  sprintf( exec_sec, "        xin[1..%i]:= [ %s, %s, t ]\n\n", ( sizeInputs > 1 ? ( sizeInputs + sizeOutputs + 1 ) : 1 ), inputsFromATP, outputsInit );
  strcat( execSec, exec_sec );

    // OUTPUT
  strcat( execSec, "      OUTPUT\n" );
  if ( sizeOutputs > 1 ) {
    for ( i= 0; i < sizeOutputs; i++ ) {
      sprintf( exec_aux, "        %s:= xout[%i]\n", namesOutputs[i], ( i + 1 ) );
      strcat( execSec, exec_aux );
    }
  } else {
    sprintf( exec_aux, "        %s:= xout\n", namesOutputs[0] );
    strcat( execSec, exec_aux );
  }
 
  strcat( *blueprint, "\n" );
  strcat( execSec, "\n    ENDUSE\n" );
  
  strcat( *blueprint, execSec ); 
  strcat( *blueprint, "\n  ENDEXEC\n\n" );

  free( execSec );
}


void* defaultParameters( IEEE_Cigre_DLLInterface_Model_Info *modelInfo, int size, char *dfltPrmSec ) {

  int i;
  char dfltP_aux[128];  
  
  for ( i= 0; i < size; i++ ) {
    const IEEE_Cigre_DLLInterface_Parameter *param = &modelInfo->ParametersInfo[i];

    switch ( param -> DataType ) {

      case IEEE_Cigre_DLLInterface_DataType_char_T:
        sprintf( dfltP_aux, "    %s { DFLT: %c }\n", param -> Name, param -> DefaultValue.Char_Val );
        strcat( dfltPrmSec, dfltP_aux );
        break;

      case IEEE_Cigre_DLLInterface_DataType_int8_T:
        sprintf( dfltP_aux, "    %s { DFLT: %hhd }\n", param -> Name, param -> DefaultValue.Int8_Val );
        strcat( dfltPrmSec, dfltP_aux );
        break;

      case IEEE_Cigre_DLLInterface_DataType_uint8_T:
        sprintf( dfltP_aux, "    %s { DFLT: %hhu }\n", param -> Name, param -> DefaultValue.Uint8_Val );
        strcat( dfltPrmSec, dfltP_aux );
        break;

      case IEEE_Cigre_DLLInterface_DataType_int16_T:
        sprintf( dfltP_aux, "    %s { DFLT: %hd }\n", param -> Name, param -> DefaultValue.Int16_Val );
        strcat( dfltPrmSec, dfltP_aux );
        break;

      case IEEE_Cigre_DLLInterface_DataType_uint16_T:
        sprintf( dfltP_aux, "    %s { DFLT: %hu }\n", param -> Name, param -> DefaultValue.Uint16_Val );
        strcat( dfltPrmSec, dfltP_aux );
        break;

      case IEEE_Cigre_DLLInterface_DataType_int32_T:
        sprintf( dfltP_aux, "    %s { DFLT: %d }\n", param -> Name, param -> DefaultValue.Int32_Val );
        strcat( dfltPrmSec, dfltP_aux );
        break;
   
      case IEEE_Cigre_DLLInterface_DataType_uint32_T:
        sprintf( dfltP_aux, "    %s { DFLT: %u }\n", param -> Name, param -> DefaultValue.Uint32_Val );
        strcat( dfltPrmSec, dfltP_aux );
        break;

      case IEEE_Cigre_DLLInterface_DataType_real32_T:
        sprintf( dfltP_aux, "    %s { DFLT: %.4f }\n", param -> Name, param -> DefaultValue.Real32_Val );
        strcat( dfltPrmSec, dfltP_aux );
        break;

      case IEEE_Cigre_DLLInterface_DataType_real64_T:
        sprintf( dfltP_aux, "    %s { DFLT: %.4lf }\n", param -> Name, param -> DefaultValue.Real64_Val );
        strcat( dfltPrmSec, dfltP_aux ); 
        break;
   
    } 
        
  }

}


char* modelBlueprint( IEEE_Cigre_DLLInterface_Model_Info *modelInfo ) {
    
  int i;
  const char *modelName= modelInfo -> ModelName;
  char modelNameF[24];  
  strcpy( modelNameF, modelName );  

  for ( i= 0; modelNameF[i] != '\0'; i++ ) {
    if ( modelNameF[i] == ' ' ) {
      modelNameF[i]= '_';
    }
  }
  
  int sizeInputs=  modelInfo -> NumInputPorts;
  int sizeOutputs= modelInfo -> NumOutputPorts;
  int sizeParams=  modelInfo -> NumParameters;
  int sizeNumIntStates= modelInfo -> NumIntStates;
  int sizeNumFloatStates= modelInfo -> NumFloatStates;
  int sizeNumDoubleStates= modelInfo -> NumDoubleStates;

  const char **namesInputs= malloc( sizeInputs * sizeof( const char * ) );
  variablesNames( "Inputs", sizeInputs, modelInfo, namesInputs );

  const char **namesOutputs= malloc( sizeOutputs * sizeof( const char * ) );
  variablesNames( "Outputs", sizeOutputs, modelInfo, namesOutputs );

  const char **namesParams= malloc( sizeParams * sizeof( const char * ) );
  variablesNames( "Parameters", sizeParams, modelInfo, namesParams );



  //_______________________________________________________________________________________________\n\n" );

  


  // Estimate total size required (conservatively)
  int bufferSize= 5120; 
  char *blueprint= malloc( bufferSize );

  if ( !blueprint ) {
    fprintf( stderr, "Memory allocation failed\n" );
    return NULL;
  }

  blueprint[0]= '\0';

  // Append title
  strcat( blueprint, "MODEL " );
  strcat( blueprint, modelName );
  strcat( blueprint, "\n" );

  // DATA
  strcat( blueprint, "  DATA\n" );

  char *dfltPrmSec= malloc( 4096 );
  dfltPrmSec[0]= '\0';

  defaultParameters( modelInfo, sizeParams, dfltPrmSec );
  strcat( blueprint, dfltPrmSec );
  strcat( blueprint, "    TRelease { DFLT: 0 }\n" );
  strcat( blueprint, "\n" );

  // INPUTS
  inputSection( &blueprint, sizeInputs, namesInputs, sizeOutputs, namesOutputs );

  // OUTPUTS
  appendSection( &blueprint, "  OUTPUT", namesOutputs, sizeOutputs );
  strcat( blueprint, "\n" );

  
  // VAR
  appendSection( &blueprint, "  VAR", namesOutputs, sizeOutputs );
  
  char *varLine= malloc( 1024 );
  varLine[0]= '\0';

  char inputsFromATP[512];
  char paramsFromATP[512];
  char outputsInit[512];
  char var_line[512];
  
  strcpy( inputsFromATP, "inputsFromATP" );
  if ( sizeInputs > 1 ) {
    sprintf( var_line, "[1..%i]", sizeInputs );
    strcat( inputsFromATP, var_line );
  }

  strcpy( paramsFromATP, "paramsFromATP" );
  if ( sizeParams > 1 ) {
    sprintf( var_line, "[1..%i]", sizeParams );
    strcat( paramsFromATP, var_line );
  }
  
  strcpy( outputsInit, "outputsInit" );
  if ( sizeOutputs > 1 ) {
    sprintf( var_line, "[1..%i]", sizeOutputs );
    strcat( outputsInit, var_line );
  }

  sprintf( varLine, "  %s, %s, %s\n", inputsFromATP, paramsFromATP, outputsInit );  
  appendSection( &blueprint, 0, ( const char ** )&varLine, 1 );

  // INIT
  initSection( &blueprint, sizeInputs, namesInputs, sizeOutputs, namesOutputs, sizeParams, namesParams, inputsFromATP, paramsFromATP, outputsInit );

  // Load the DLL 
  fgnSection( &blueprint, modelName, sizeInputs, sizeOutputs, sizeParams, sizeNumIntStates, sizeNumFloatStates, sizeNumDoubleStates );

  // EXEC
  execSection( &blueprint, modelName, sizeInputs, namesInputs, sizeOutputs, namesOutputs, sizeParams, namesParams, inputsFromATP, paramsFromATP, outputsInit );

  strcat( blueprint, "ENDMODEL\n" );

  free( dfltPrmSec );  
  free( namesInputs );
  free( namesOutputs );
  free( namesParams );  

  return blueprint;
}


void exportToFile( const char *filename, const char *blueprint ) {
  FILE *file= fopen( filename, "w" ); 
  if ( file == NULL ) {
    perror( "Failed to open file" );
    return;
  }

  fputs( blueprint, file );  
  fclose( file );            
}




int main() {

  char *dllFile= "scm_32";                             // scm_32   realCodeExample  
  int i;

  FILE *pFile= fopen( "dll_list.txt", "r" );
  char buf_dll[128]= { 0 };

  if ( pFile != NULL && fgets( buf_dll, sizeof( buf_dll ), pFile ) != NULL ) {
    // Remove the trailing newline if present
    buf_dll[ strcspn( buf_dll, "\r\n" ) ]= '\0';
  } else {
    printf( "Could not read from file\n" );
    fclose( pFile );
    return 1;
  }

  fclose( pFile );

  printf( "Archivo txt= %s\n", buf_dll );


  // HMODULE hDLL= LoadLibrary( buf_dll );
  HMODULE hDLL= LoadLibrary( dllFile );
  if ( hDLL == NULL ) {
    printf( "Cannot find \"%s\"\n", dllFile );
    exit( EXIT_FAILURE );
  }


  // PrintInfo printInfo= ( PrintInfo ) GetProcAddress( hDLL, "Model_PrintInfo" );
  // if ( printInfo == NULL ) {
  //   printf( "Cannot locate Model_PrintInfo function in dll\n" );
  //   exit( EXIT_FAILURE );
  // }


  // GetInfo function

  getInfo= ( GetInfo ) GetProcAddress( hDLL, "Model_GetInfo" );
  if ( getInfo == NULL ) {
    printf( "Cannot locate Model_GetInfo function in dll\n" );
    exit( EXIT_FAILURE );
  }

  IEEE_Cigre_DLLInterface_Model_Info *modelInfo= getInfo();
  printf( "Model Name= %s\n", ( const char * ) modelInfo -> ModelName );


  printf( "_______________________________________________________________________________________________\n\n" );



  char *blueprint= modelBlueprint( modelInfo );


  exportToFile( "models_output.txt", blueprint );


  if ( blueprint ) {
    printf( "%s", blueprint );
    free( blueprint );
  }

  // Close the dll
  FreeLibrary( hDLL );
  
  return 0;
}
