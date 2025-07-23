# -*- coding: utf-8 -*-
from ctypes import *
import sys
import os

from IEEE_Cigre_DLLInterface import *
from IEEE_Cigre_DLLInterface_types import *

# Load the DLL
dll= "scm_32"
dllFile= f"c:/users/josé montoya/documents/documentos_jm/proyecto wrapper dll atp/ejemplo enlazar dll/create_models__32/{ dll }.dll"  # Add .dll extension explicitly
if not os.path.exists( dllFile ):
  print( f'Cannot find "{ dllFile }"' )
  sys.exit(1)

try:
  dll= CDLL( dllFile )
except OSError as e:
  print( f"Failed to load DLL: {e}" )
  sys.exit(1)  

try:
  printInfo= dll.Model_PrintInfo
  printInfo.restype= None
except AttributeError:
  print( "Cannot locate Model_PrintInfo function in dll" )
  sys.exit(1)

try:
  getInfo= dll.Model_GetInfo
  getInfo.restype= POINTER( IEEE_Cigre_DLLInterface_Model_Info )
except AttributeError:
  print( "Cannot locate Model_GetInfo function in dll" )
  sys.exit(1)

# Use the getInfo function
modelInfo_ptr= getInfo()
modelInfo= modelInfo_ptr.contents

modelName= modelInfo.ModelName.decode().replace( ' ', '_' )
print( f"Name= { modelName }" )

szI= modelInfo.NumInputPorts
szO= modelInfo.NumOutputPorts
szP= modelInfo.NumParameters
szIntSt= modelInfo.NumIntStates
szFlSt= modelInfo.NumFloatStates
szDbSt= modelInfo.NumDoubleStates

namesInputs= [ modelInfo.InputPortsInfo[i].Name.decode() for i in range( szI ) ]
namesOutputs= [ modelInfo.OutputPortsInfo[i].Name.decode() for i in range( szO ) ]
namesParams= [ modelInfo.ParametersInfo[i].Name.decode() for i in range( szP ) ]
print( f"NamesInputs= { namesInputs }" )
print( f"NameOutputs= { namesOutputs }" )
print( f"NamesParms= { namesParams }" )

"""
for i in range( modelInfo.NumInputPorts ):
  signal= modelInfo.InputPortsInfo[i]
  dtype= IEEE_Cigre_DLLInterface_DataType( signal.DataType )
  print( f"Input {i}: { signal.Name.decode() } ( type= { dtype.name } )" )
"""

blueprint= f"""
MODELS { modelName }
  DATA
    { ( ", ".join( namesParams ) if szP >= 1 else "" ) }
    TRelease {{ DFLT:0 }}

  INPUT
    { ( ", ".join( namesInputs ) if szI >= 1 else "" ) }
    { ( "_0, ".join( namesOutputs ) if szO >= 1 else "" ) }{ "_0" if szO >= 1 else "" }

  OUTPUT
    { ( ", ".join( namesOutputs ) if szO >= 1 else "" ) }

  VAR
    { ( ", ".join( namesOutputs ) if szO >= 1 else "" ) }
    { "inputsFromATP" if szI >= 1 else "" }{ ( f"[1..{ szI }]" if szI > 1 else "" ) }, { "paramsFromATP" if szP >= 1 else "" }{ ( f"[1..{ szP }]" if szP > 1 else "" ) }, { "outputsInit" if szO >= 1 else "" }{ ( f"[1..{ szO }]" if szO > 1 else "" ) }

  INIT
    { '\n    '.join( f"{ i }:= { i }_0" for i in namesInputs ) }
    { "inputsFromATP" if szI >= 1 else "" }{ ( f"[1..{ szI }]" if szI > 1 else "" ) }:= { ( f"[ { ", ".join( namesInputs ) } ] " if szI > 1 else f"{ namesParams[0] }" ) }
    { "paramsFromATP" if szP >= 1 else "" }{ ( f"[1..{ szP }]" if szP > 1 else "" ) }:= { ( f"[ { ", ".join( namesParams ) } ] " if szP > 1 else f"{ namesParams[0] }" ) }
    { "outputsInit" if szO >= 1 else "" }{ ( f"[1..{ szO }]" if szO > 1 else "" ) }:= { ( f"[ { '_0, '.join( namesOutputs ) }_0 ] " if szO > 1 else f"{ namesOutputs[0] }_0" ) }

  ENDINIT

  MODEL { modelName }_dll FOREIGN dll_one {{ ixdata: { szP + 2 }, ixin: { szI + szO + 1 }, ixout: { szO }, ixvar: { szIntSt + szFlSt + szDbSt } }}

  EXEC

    { "inputsFromATP" if szI >= 1 else "" }{ ( f"[1..{ szI }]" if szI > 1 else "" ) }:= { ( f"[ { ", ".join( namesInputs ) } ] " if szI > 1 else f"{ namesParams[0] }" ) }
    { "outputsInit" if szO >= 1 else "" }{ ( f"[1..{ szO }]" if szO > 1 else "" ) }:= { ( f"[ { '_0, '.join( namesOutputs ) }_0 ] " if szO > 1 else f"{ namesOutputs[0] }_0" ) }

    USE { modelName }_dll AS { modelName }_dll

      DATA
        xdata{ ( f"[1..{ szP + 2 }]" if szP >= 1 else "2" ) }:= [{ ( " paramsFromATP" if szP >= 1 else "" ) }{ ( f"[1..{ szP }]" if szP > 1 else "" ) }{ ( ", " if szP >= 1 else "" ) } timestep, TRelease ]

      INPUT
        xin{ ( f"[1..{ szI + szO + 1 }]" if ( szI >= 1 or szO >= 1) else "2" ) }:= [{ ( " inputsFromATP" if szI >= 1 else "" ) }{ ( f"[1..{ szI }]" if szI > 1 else "" ) }{ ( ", " if szI >= 1 else "" ) }{ ( " outputsInit" if szO >= 1 else "" ) }{ ( f"[1..{ szO }]" if szO > 1 else "" ) }{ ( ", " if szO >= 1 else "" ) } t ]   

      OUTPUT
        { ( '\n        '.join( f"{ i }:= xout[{ k + 1 }]" for k,i in enumerate( namesOutputs ) ) if szO > 1 else ( f"{ namesOutputs[0] }:= xout" if szO == 1 else "" ) ) }

    ENDUSE

  ENDEXEC

ENDMODEL
      
"""

print( blueprint )



  
  
  
  