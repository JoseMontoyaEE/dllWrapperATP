# Translated IEEE_Cigre_DLLInterface_types.h header

import ctypes
from enum import IntEnum

# --- Type Aliases ---
char_T   = ctypes.c_char
int8_T   = ctypes.c_char        # C: char used for int8_T
uint8_T  = ctypes.c_ubyte
int16_T  = ctypes.c_short
uint16_T = ctypes.c_ushort
int32_T  = ctypes.c_int
uint32_T = ctypes.c_uint
real32_T = ctypes.c_float
real64_T = ctypes.c_double

# --- DataType Enum ---
class IEEE_Cigre_DLLInterface_DataType( IntEnum ):
  char_T     = 1
  int8_T     = 2
  uint8_T    = 3
  int16_T    = 4
  uint16_T   = 5
  int32_T    = 6
  uint32_T   = 7
  real32_T   = 8
  real64_T   = 9
  c_string_T = 10

# --- Return Value Enum ---
class IEEE_Cigre_DLLInterface_Return_Value( IntEnum ):
  OK      = 0
  Message = 1
  Error   = 2
