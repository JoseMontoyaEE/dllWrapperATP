# Translated IEEE_Cigre_DLLInterface.h header

import ctypes
from IEEE_Cigre_DLLInterface_types import *

# --- Signal Struct ---
class IEEE_Cigre_DLLInterface_Signal( ctypes.Structure ):
  _fields_= [
    ( "Name", ctypes.c_char_p) ,
    ( "Description", ctypes.c_char_p ),
    ( "Unit", ctypes.c_char_p ),
    ( "DataType", ctypes.c_int ),
    ( "Width", int32_T )
  ]

# --- Unions ---
class DefaultValueU( ctypes.Union ):
  _fields_= [
    ( "Char_Val", char_T ),
    ( "Char_Ptr", ctypes.c_char_p ),
    ( "Int8_Val", int8_T ),
    ( "Uint8_Val", uint8_T ),
    ( "Int16_Val", int16_T ),
    ( "Uint16_Val", uint16_T ),
    ( "Int32_Val", int32_T ),
    ( "Uint32_Val", uint32_T ),
    ( "Real32_Val", real32_T ),
    ( "Real64_Val", real64_T )
  ]

class MinMaxValueU( DefaultValueU ):
  pass

# --- Parameter Struct ---
class IEEE_Cigre_DLLInterface_Parameter( ctypes.Structure ):
  _fields_=[
    ( "Name", ctypes.c_char_p ),
    ( "GroupName", ctypes.c_char_p ),
    ( "Description", ctypes.c_char_p ),
    ( "Unit", ctypes.c_char_p ),
    ( "DataType", ctypes.c_int ),
    ( "FixedValue", int32_T ),
    ( "DefaultValue", DefaultValueU ),
    ( "MinValue", MinMaxValueU ),
    ( "MaxValue", MinMaxValueU )
  ]

# --- Model Info Struct ---
class IEEE_Cigre_DLLInterface_Model_Info( ctypes.Structure ):
  _fields_=[
    ( "DLLInterfaceVersion", uint8_T * 4 ),
    ( "ModelName", ctypes.c_char_p ),
    ( "ModelVersion", ctypes.c_char_p ),
    ( "ModelDescription", ctypes.c_char_p ),
    ( "GeneralInformation", ctypes.c_char_p ),
    ( "ModelCreated", ctypes.c_char_p ),
    ( "ModelCreator", ctypes.c_char_p ),
    ( "ModelLastModifiedDate", ctypes.c_char_p ),
    ( "ModelLastModifiedBy", ctypes.c_char_p ),
    ( "ModelModifiedComment", ctypes.c_char_p ),
    ( "ModelModifiedHistory", ctypes.c_char_p ),
    ( "FixedStepBaseSampleTime", real64_T ),
    ( "EMT_RMS_Mode", uint8_T ),
    ( "NumInputPorts", int32_T ),
    ( "InputPortsInfo", ctypes.POINTER( IEEE_Cigre_DLLInterface_Signal ) ),
    ( "NumOutputPorts", int32_T ),
    ( "OutputPortsInfo", ctypes.POINTER( IEEE_Cigre_DLLInterface_Signal ) ),
    ( "NumParameters", int32_T ),
    ( "ParametersInfo", ctypes.POINTER( IEEE_Cigre_DLLInterface_Parameter ) ),
    ( "NumIntStates", int32_T ),
    ( "NumFloatStates", int32_T ),
    ( "NumDoubleStates", int32_T )
  ]

# --- Instance Struct ---
class IEEE_Cigre_DLLInterface_Instance( ctypes.Structure ):
  _fields_= [
    ( "ExternalInputs", ctypes.c_void_p ),
    ( "ExternalOutputs", ctypes.c_void_p ),
    ( "Parameters", ctypes.c_void_p ),
    ( "Time", real64_T ),
    ( "SimTool_EMT_RMS_Mode", uint8_T ),
    ( "LastErrorMessage", ctypes.c_char_p ),
    ( "LastGeneralMessage", ctypes.c_char_p ),
    ( "IntStates", ctypes.POINTER( int32_T ) ),
    ( "FloatStates", ctypes.POINTER( real32_T ) ),
    ( "DoubleStates", ctypes.POINTER( real64_T ) )
  ]
