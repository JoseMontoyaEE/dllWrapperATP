#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <math.h>
#include <complex.h>
#include <string.h>
#include "PH2ICC_Library.h"
#include "IEEE_Cigre_DLLInterface.h"


// ----------------------------------------------------------------------
// Global constants
// ----------------------------------------------------------------------
#define NINPUTS 2
#define NOUTPUTS 1
#define NPARAMETERS 25
#define NSTATES 19
#define NVARS 0


// ---------------------------------------------------------------------------------------------------------------------
// Structures defining inputs, outputs, parameters and model information
// ---------------------------------------------------------------------------------------------------------------------
typedef struct _ModelInputs {
    real64_T VS1;
    real64_T VS2;
} ModelInputs;


typedef struct _ModelOutputs {
    real64_T VPSS;
} ModelOutputs;


typedef struct _ModelParameters {
    real64_T TW1;
    real64_T TW2;
    real64_T T6;
    real64_T TW3;
    real64_T TW4;
    real64_T T7;
    real64_T KS2;
    real64_T KS3;
    real64_T T8;
    real64_T T9;
    real64_T KS1;
    real64_T T1;
    real64_T T2;
    real64_T T3;
    real64_T T4;
    real64_T T10;
    real64_T T11;
    real64_T VS1MAX;
    real64_T VS1MIN;
    real64_T VS2MAX;
    real64_T VS2MIN;
    real64_T VSTMAX;
    real64_T VSTMIN;
    int32_T M;
    int32_T N;
} ModelParameters;


IEEE_Cigre_DLLInterface_Signal InputSignals[] = {
    [0] = {
        .Name = "VS1",
        .Description = "Input signal 1",
        .Unit = "pu",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .Width = 1
    },
    [1] = {
        .Name = "VS2",
        .Description = "Input signal 2",
        .Unit = "pu",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .Width = 1
    }
};


IEEE_Cigre_DLLInterface_Signal OutputSignals[] = {  
    [0] = {
        .Name = "VPSS",
        .Description = "PSS output signal",
        .Unit = "pu",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .Width = 1
    }  
};


IEEE_Cigre_DLLInterface_Parameter Parameters[] = {
    [0] = {
        .Name = "TW1",
        .GroupName = "",
        .Description = "1st washout 1th time constant (>0)",
        .Unit = "[s]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 2.0,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [1] = {
        .Name = "TW2",
        .GroupName = "",
        .Description = "1st washout 2th time constant",
        .Unit = "[s]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 0.0,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [2] = {
        .Name = "T6",
        .GroupName = "",
        .Description = "1st signal transducer time constant",
        .Unit = "[s]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 0.01,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [3] = {
        .Name = "TW3",
        .GroupName = "",
        .Description = "2nd washout 1th time constant",
        .Unit = "[s]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 2.0,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [4] = {
        .Name = "TW4",
        .GroupName = "",
        .Description = "2nd washout 2th time constant",
        .Unit = "[s]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 0.0,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [5] = {
        .Name = "T7",
        .GroupName = "",
        .Description = "2nd signal transducer time constant",
        .Unit = "[s]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 2.0,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [6] = {
        .Name = "KS2",
        .GroupName = "",
        .Description = "2nd signal transducer factor (typically T7/2H)",
        .Unit = "[pu]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 0.2,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [7] = {
        .Name = "KS3",
        .GroupName = "",
        .Description = "Washouts coupling factor",
        .Unit = "[pu]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 1.0,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [8] = {
        .Name = "T8",
        .GroupName = "",
        .Description = "Ramp tracking filter deriv. time constant (must be M * T9)",
        .Unit = "[s]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 0.5,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [9] = {
        .Name = "T9",
        .GroupName = "",
        .Description = "Ramp tracking filter delay time constant (>0)",
        .Unit = "[s]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 0.1,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [10] = {
        .Name = "KS1",
        .GroupName = "",
        .Description = "PSS gain",
        .Unit = "[pu]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 20.0,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [11] = {
        .Name = "T1",
        .GroupName = "",
        .Description = "1st lead-lag derivative time constant",
        .Unit = "[s]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 0.2,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [12] = {
        .Name = "T2",
        .GroupName = "",
        .Description = "1st lead-lag delay time constant",
        .Unit = "[s]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 0.01,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [13] = {
        .Name = "T3",
        .GroupName = "",
        .Description = "2nd lead-lag derivative time constant",
        .Unit = "[s]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 0.2,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [14] = {
        .Name = "T4",
        .GroupName = "",
        .Description = "2nd lead-lag delay time constant",
        .Unit = "[s]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 0.01,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [15] = {
        .Name = "T10",
        .GroupName = "",
        .Description = "3rd lead-lag derivative time constant",
        .Unit = "[s]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 0.2,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [16] = {
        .Name = "T11",
        .GroupName = "",
        .Description = "3rd lead-lag delay time constant",
        .Unit = "[s]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 0.2,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [17] = {
        .Name = "VS1MAX",
        .GroupName = "",
        .Description = "Input signal 1 maximum limit",
        .Unit = "[pu]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 1.0,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [18] = {
        .Name = "VS1MIN",
        .GroupName = "",
        .Description = "Input signal 1 minimum limit",
        .Unit = "[pu]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = -1.0,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [19] = {
        .Name = "VS2MAX",
        .GroupName = "",
        .Description = "Input signal 2 maximum limit",
        .Unit = "[pu]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 1.0,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [20] = {
        .Name = "VS2MIN",
        .GroupName = "",
        .Description = "Input signal 2 minimum limit",
        .Unit = "[pu]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = -1.0,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [21] = {
        .Name = "VSTMAX",
        .GroupName = "",
        .Description = "Controller maximum output",
        .Unit = "[pu]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 0.05,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [22] = {
        .Name = "VSTMIN",
        .GroupName = "",
        .Description = "Controller minimum output",
        .Unit = "[pu]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = -0.05,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [23] = {
        .Name = "M",
        .GroupName = "",
        .Description = "Ramp tracking filter factor ",
        .Unit = "",
        .DataType = IEEE_Cigre_DLLInterface_DataType_int32_T,
        .FixedValue = 0,
        .DefaultValue.Int32_Val = 5,
        .MinValue.Int32_Val = -9999,
        .MaxValue.Int32_Val = 9999
    },
    [24] = {
        .Name = "N",
        .GroupName = "",
        .Description = "Ramp tracking filter multiplier",
        .Unit = "",
        .DataType = IEEE_Cigre_DLLInterface_DataType_int32_T,
        .FixedValue = 0,
        .DefaultValue.Int32_Val = 1,
        .MinValue.Int32_Val = -9999,
        .MaxValue.Int32_Val = 9999
    }
};


IEEE_Cigre_DLLInterface_Model_Info Model_Info = {
    .DLLInterfaceVersion = { 1, 1, 0, 0 },
    .ModelName = "PSS2B",                                        
    .ModelVersion = "1.0.0",                                  
    .ModelDescription = "PSS2B - IEEE 421.5 2005 PSS2B IEEE Dual-Input Stabilizer Model",
    .GeneralInformation = "Model ",
    .ModelCreated = "July 31, 2025",                               
    .ModelCreator = "IA",                               
    .ModelLastModifiedDate = "-",
    .ModelLastModifiedBy = "-",
    .ModelModifiedComment = "-",
    .ModelModifiedHistory = "Version 1.0.0: Initial release",
    .FixedStepBaseSampleTime = 0.001,
    .EMT_RMS_Mode = 1,
    .NumInputPorts = NINPUTS,
    .InputPortsInfo = InputSignals,
    .NumOutputPorts = NOUTPUTS,
    .OutputPortsInfo = OutputSignals,
    .NumParameters = NPARAMETERS,
    .ParametersInfo = Parameters,
    .NumIntStates = 0,
    .NumFloatStates = 0,
    .NumDoubleStates = (NVARS + NSTATES * 2)
};


// ---------------------------------------------------------------------------------------------------------------------
// States documentation
// ---------------------------------------------------------------------------------------------------------------------
//      state[0] =  / [PSS2B/Wsh1$0] 
//      state[1] =  / [PSS2B/Wsh2$0] 
//      state[2] =  / [PSS2B/Wsh3$0] 
//      state[3] =  / [PSS2B/Wsh4$0] 
//      state[4] =  / [PSS2B/PT1_K52_T7$0] 
//      state[5] =  / [PSS2B/PT1_T6$0] 
//      state[6] =  / [PSS2B/PDT1$0] 
//      state[7] =  / [PSS2B/PDT2$0] 
//      state[8] =  / [PSS2B/PDT3$0] 
//      state[9] =  / [PSS2B/RampTracking/PDT1$0] 
//      state[10] =  / [PSS2B/RampTracking/PT1_1$0] 
//      state[11] =  / [PSS2B/RampTracking/PT1_2$0] 
//      state[12] =  / [PSS2B/RampTracking/PT1_3$0] 
//      state[13] =  / [PSS2B/RampTracking/PT1_4$0] 
//      state[14] =  / [PSS2B/RampTracking/PDT1_0$0] 
//      state[15] =  / [PSS2B/RampTracking/PT1_5$0] 
//      state[16] =  / [PSS2B/RampTracking/PT1_6$0] 
//      state[17] =  / [PSS2B/RampTracking/PT1_7$0] 
//      state[18] =  / [PSS2B/RampTracking/PT1_8$0] 


// ---------------------------------------------------------------------------------------------------------------------
// Vars documentation
// ---------------------------------------------------------------------------------------------------------------------
//      No vars


// ---------------------------------------------------------------------------------------------------------------------
// Global variables
// ---------------------------------------------------------------------------------------------------------------------
char ErrorMessage[CBUFSIZE];
char WarningMessage[CBUFSIZE];
bool initialization;
bool warning_flag;
bool error_flag;
int woffset;
int eoffset;
ModelInputs* inputs;
ModelOutputs* outputs;
ModelParameters* parameters;
real64_T delt;
real64_T time;
real64_T* var;
real64_T* state;
real64_T* store;
real64_T dstate[NSTATES];


// ---------------------------------------------------------------------------------------------------------------------
// Auxiliary functions
// ---------------------------------------------------------------------------------------------------------------------

void set_globals(IEEE_Cigre_DLLInterface_Instance* instance, bool init) {
    ErrorMessage[0] = '\0';
    WarningMessage[0] = '\0';
    initialization = init;
    warning_flag = false;
    error_flag = false;
    woffset = 0;
    eoffset = 0;
    inputs = (ModelInputs*)instance->ExternalInputs;
    outputs = (ModelOutputs*)instance->ExternalOutputs;
    parameters = (ModelParameters*)instance->Parameters;    
    delt = Model_Info.FixedStepBaseSampleTime;   
    time = instance->Time;
    var = instance->DoubleStates;
    state = var + NVARS;
    store = state + NSTATES;
}


void integrate_mod_euler(void) {
    for (int i = 0; i < NSTATES; i++) {
        store[i] = store[i] + delt * dstate[i];
        state[i] = store[i] + delt * dstate[i] / 2.0;
    }
}


void initialize_mod_euler(void) {
    for (int i = 0; i < NSTATES; i++) {
       dstate[i] = 0.0;
       store[i] = state[i];
    }
}


int bytes_written(int result_of_sprintf) {
    return (result_of_sprintf > 0) ? result_of_sprintf : 0;
}


size_t cbuff_size(int bytes_written) {
   return ((CBUFSIZE-bytes_written) > 0) ? (size_t)(CBUFSIZE-bytes_written) : 0; 
} 


void warning(const char *format, ...) {
    va_list args;
    va_start(args, format);
    woffset += bytes_written(vsnprintf(WarningMessage+woffset, cbuff_size(woffset), format, args));
    warning_flag = true;
    va_end(args);
}


void error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    eoffset += bytes_written(vsnprintf(ErrorMessage+eoffset, cbuff_size(eoffset), format, args));
    error_flag = true;
    va_end(args);
}


void ph3_ovride_int(int k0, real64_T value) {       
    state[k0] = value;
    store[k0] = value;
}



// ---------------------------------------------------------------------------------------------------------------------
// Subroutines that can be called by the main power system program
// ---------------------------------------------------------------------------------------------------------------------
__declspec(dllexport) int32_T __cdecl Model_CheckParameters(IEEE_Cigre_DLLInterface_Instance* instance) {
    set_globals(instance, false);

    // -----------------------------------------------------------------------------------------------------------------
    // Parameters checking      
    if (!((parameters->T9) > 2*delt)) {
        warning("Warning: T9 (%g) must be > %g ==> T9 > 2*delt \n", (parameters->T9), 2*delt);
    }
    
    if (!((parameters->T9) > 2*delt)) {
        warning("Warning: T9 (%g) must be > %g ==> T9 > 2*delt \n", (parameters->T9), 2*delt);
    }
    
    if (!((parameters->T9) > 2*delt)) {
        warning("Warning: T9 (%g) must be > %g ==> T9 > 2*delt \n", (parameters->T9), 2*delt);
    }
    
    if (!((parameters->T9) > 2*delt)) {
        warning("Warning: T9 (%g) must be > %g ==> T9 > 2*delt \n", (parameters->T9), 2*delt);
    }
    
    if (!((parameters->T9) > 2*delt)) {
        warning("Warning: T9 (%g) must be > %g ==> T9 > 2*delt \n", (parameters->T9), 2*delt);
    }
    
    if (!((parameters->T9) > 2*delt)) {
        warning("Warning: T9 (%g) must be > %g ==> T9 > 2*delt \n", (parameters->T9), 2*delt);
    }
    
    if (!((parameters->T9) > 2*delt)) {
        warning("Warning: T9 (%g) must be > %g ==> T9 > 2*delt \n", (parameters->T9), 2*delt);
    }
    
    if (!((parameters->T9) > 2*delt)) {
        warning("Warning: T9 (%g) must be > %g ==> T9 > 2*delt \n", (parameters->T9), 2*delt);
    }
    
    if (!((parameters->VS1MAX) > (parameters->VS1MIN))) {
        warning("Warning: VS1MAX (%g) must be > %g ==> VS1MAX > VS1MIN \n", (parameters->VS1MAX), (parameters->VS1MIN));
    }
    
    if (!((parameters->VS2MAX) > (parameters->VS2MIN))) {
        warning("Warning: VS2MAX (%g) must be > %g ==> VS2MAX > VS2MIN \n", (parameters->VS2MAX), (parameters->VS2MIN));
    }
    
    if (!((parameters->VSTMAX) > (parameters->VSTMIN))) {
        warning("Warning: VSTMAX (%g) must be > %g ==> VSTMAX > VSTMIN \n", (parameters->VSTMAX), (parameters->VSTMIN));
    }
     

    // -----------------------------------------------------------------------------------------------------------------
    // Output     
    if (error_flag) {
        instance->LastErrorMessage = ErrorMessage;
        return IEEE_Cigre_DLLInterface_Return_Error;
    } else if (warning_flag) {
        instance->LastGeneralMessage = WarningMessage;
        return IEEE_Cigre_DLLInterface_Return_Message;
    }
    return IEEE_Cigre_DLLInterface_Return_OK;
}


__declspec(dllexport) int32_T __cdecl Model_Initialize(IEEE_Cigre_DLLInterface_Instance* instance) {
    set_globals(instance, true);

    // -----------------------------------------------------------------------------------------------------------------
    // Model initialization
    state[0] = 1.0 * (inputs->VS1);
    state[1] = 0.0;
    state[2] = 1.0 * (inputs->VS2);
    state[3] = 0.0;
    state[4] = 0.0;
    state[5] = 0.0;
    state[6] = 0.0;
    state[7] = 0.0;
    state[8] = 0.0;
    state[9] = 0.0;
    state[10] = 0.0;
    state[11] = 0.0;
    state[12] = 0.0;
    state[13] = 0.0;
    state[14] = 0.0;
    state[15] = 0.0;
    state[16] = 0.0;
    state[17] = 0.0;
    state[18] = 0.0;
    

    // -----------------------------------------------------------------------------------------------------------------
    // Numerical integration method initialization
    initialize_mod_euler();

    // -----------------------------------------------------------------------------------------------------------------
    // Output 
    if (error_flag) {
        instance->LastErrorMessage = ErrorMessage;
        return IEEE_Cigre_DLLInterface_Return_Error;
    } else if (warning_flag) {
        instance->LastGeneralMessage = WarningMessage;
        return IEEE_Cigre_DLLInterface_Return_Message;
    }
    return IEEE_Cigre_DLLInterface_Return_OK;
}


__declspec(dllexport) int32_T __cdecl Model_Outputs(IEEE_Cigre_DLLInterface_Instance* instance) {
    set_globals(instance, false);

    // -----------------------------------------------------------------------------------------------------------------
    // Model variables
    real64_T pss2b_limiter, pss2b_limiter_0, pss2b_limiter_1, pss2b_pdt1, pss2b_pdt2, pss2b_pdt3, pss2b_pt1_k52_t7, pss2b_pt1_t6, pss2b_ramptracking_bypass, pss2b_ramptracking_pdt1, pss2b_ramptracking_pdt1_0, pss2b_ramptracking_pt1_1, pss2b_ramptracking_pt1_2, pss2b_ramptracking_pt1_3, pss2b_ramptracking_pt1_4, pss2b_ramptracking_pt1_5, pss2b_ramptracking_pt1_6, pss2b_ramptracking_pt1_7, pss2b_ramptracking_pt1_8, pss2b_ramptracking_select_m1, pss2b_ramptracking_select_m2, pss2b_ramptracking_select_n, pss2b_sum, pss2b_sum_0, pss2b_wsh1, pss2b_wsh2, pss2b_wsh3, pss2b_wsh4;

    // -----------------------------------------------------------------------------------------------------------------
    // Auxiliary variables
    real64_T x;


    // -----------------------------------------------------------------------------------------------------------------
    // Algebraic equations
    pss2b_ramptracking_pt1_1 = state[10];
    
    pss2b_ramptracking_pt1_2 = state[11];
    
    pss2b_ramptracking_pt1_3 = state[12];
    
    pss2b_ramptracking_pt1_4 = state[13];
    
    pss2b_ramptracking_pt1_8 = state[18];
    
    pss2b_ramptracking_pt1_7 = state[17];
    
    pss2b_ramptracking_pt1_6 = state[16];
    
    pss2b_ramptracking_pt1_5 = state[15];
    
    pss2b_limiter_0 = fmin(fmax((inputs->VS1), (parameters->VS1MIN)), (parameters->VS1MAX));
    
    // ------------------ pss2b_wsh1 --------------------
    if ((parameters->TW1) > (2*delt)) {
       pss2b_wsh1 = (pss2b_limiter_0 * 1.0 - state[0]);
    } else {
       pss2b_wsh1 = pss2b_limiter_0 * 1.0; // behave like a Gain
       ph3_ovride_int(0, pss2b_wsh1);
    }
    
    
    // ------------------ pss2b_wsh2 --------------------
    if ((parameters->TW2) > (2*delt)) {
       pss2b_wsh2 = (pss2b_wsh1 * 1.0 - state[1]);
    } else {
       pss2b_wsh2 = pss2b_wsh1 * 1.0; // behave like a Gain
       ph3_ovride_int(1, pss2b_wsh2);
    }
    
    
    // ------------------ pss2b_pt1_t6 --------------------
    if ((parameters->T6) > (2*delt)) {
       pss2b_pt1_t6 = state[5];
    } else {
       pss2b_pt1_t6 = pss2b_wsh2 * 1.0;
       ph3_ovride_int(5, pss2b_pt1_t6);
    }
    
    pss2b_limiter_1 = fmin(fmax((inputs->VS2), (parameters->VS2MIN)), (parameters->VS2MAX));
    
    // ------------------ pss2b_wsh3 --------------------
    if ((parameters->TW3) > (2*delt)) {
       pss2b_wsh3 = (pss2b_limiter_1 * 1.0 - state[2]);
    } else {
       pss2b_wsh3 = pss2b_limiter_1 * 1.0; // behave like a Gain
       ph3_ovride_int(2, pss2b_wsh3);
    }
    
    
    // ------------------ pss2b_wsh4 --------------------
    if ((parameters->TW4) > (2*delt)) {
       pss2b_wsh4 = (pss2b_wsh3 * 1.0 - state[3]);
    } else {
       pss2b_wsh4 = pss2b_wsh3 * 1.0; // behave like a Gain
       ph3_ovride_int(3, pss2b_wsh4);
    }
    
    
    // ------------------ pss2b_pt1_k52_t7 --------------------
    if ((parameters->T7) > (2*delt)) {
       pss2b_pt1_k52_t7 = state[4];
    } else {
       pss2b_pt1_k52_t7 = pss2b_wsh4 * (parameters->KS2);
       ph3_ovride_int(4, pss2b_pt1_k52_t7);
    }
    
    pss2b_sum = + pss2b_pt1_t6 + ((parameters->KS3) * pss2b_pt1_k52_t7);
    
    // ------------------ pss2b_ramptracking_pdt1 --------------------
    if ((parameters->T9) > (2*delt)) {
       pss2b_ramptracking_pdt1 = (pss2b_sum * 1.0 - state[9]) / (parameters->T9) * (parameters->T8) + state[9];
    } else {
       pss2b_ramptracking_pdt1 = pss2b_sum * 1.0;
       ph3_ovride_int(9, pss2b_ramptracking_pdt1);
    }
    
    
    // ------------------ pss2b_ramptracking_select_m1 --------------------
    switch ((parameters->M)) {
       case (1):
          x = pss2b_ramptracking_pdt1;
          break;
       case (2):
          x = pss2b_ramptracking_pt1_1;
          break;
       case (3):
          x = pss2b_ramptracking_pt1_2;
          break;
       case (4):
          x = pss2b_ramptracking_pt1_3;
          break;
       case (5):
          x = pss2b_ramptracking_pt1_4;
          break;
       default:
          x = 0.0;
    }
    pss2b_ramptracking_select_m1 = x;
    
    
    // ------------------ pss2b_ramptracking_pdt1_0 --------------------
    if ((parameters->T9) > (2*delt)) {
       pss2b_ramptracking_pdt1_0 = (pss2b_ramptracking_select_m1 * 1.0 - state[14]) / (parameters->T9) * (parameters->T8) + state[14];
    } else {
       pss2b_ramptracking_pdt1_0 = pss2b_ramptracking_select_m1 * 1.0;
       ph3_ovride_int(14, pss2b_ramptracking_pdt1_0);
    }
    
    
    // ------------------ pss2b_ramptracking_select_m2 --------------------
    switch ((parameters->M)) {
       case (1):
          x = pss2b_ramptracking_pdt1_0;
          break;
       case (2):
          x = pss2b_ramptracking_pt1_8;
          break;
       case (3):
          x = pss2b_ramptracking_pt1_7;
          break;
       case (4):
          x = pss2b_ramptracking_pt1_6;
          break;
       case (5):
          x = pss2b_ramptracking_pt1_5;
          break;
       default:
          x = 0.0;
    }
    pss2b_ramptracking_select_m2 = x;
    
    
    // ------------------ pss2b_ramptracking_select_n --------------------
    switch ((parameters->N)) {
       case (1):
          x = pss2b_ramptracking_select_m1;
          break;
       case (2):
          x = pss2b_ramptracking_select_m2;
          break;
       default:
          x = 0.0;
    }
    pss2b_ramptracking_select_n = x;
    
    
    // ------------------ pss2b_ramptracking_bypass --------------------
    x = INT_TO_DOUBLE((parameters->M));
    if (x > 0.5) {
       x = pss2b_ramptracking_select_n;
    } else {
       x = pss2b_sum;
    }
    pss2b_ramptracking_bypass = x;
    
    pss2b_sum_0 = + pss2b_ramptracking_bypass - pss2b_pt1_k52_t7;
    
    // ------------------ pss2b_pdt1 --------------------
    if ((parameters->T2) > (2*delt)) {
       pss2b_pdt1 = (((parameters->KS1) * pss2b_sum_0) * 1.0 - state[6]) / (parameters->T2) * (parameters->T1) + state[6];
    } else {
       pss2b_pdt1 = ((parameters->KS1) * pss2b_sum_0) * 1.0;
       ph3_ovride_int(6, pss2b_pdt1);
    }
    
    
    // ------------------ pss2b_pdt2 --------------------
    if ((parameters->T4) > (2*delt)) {
       pss2b_pdt2 = (pss2b_pdt1 * 1.0 - state[7]) / (parameters->T4) * (parameters->T3) + state[7];
    } else {
       pss2b_pdt2 = pss2b_pdt1 * 1.0;
       ph3_ovride_int(7, pss2b_pdt2);
    }
    
    
    // ------------------ pss2b_pdt3 --------------------
    if ((parameters->T11) > (2*delt)) {
       pss2b_pdt3 = (pss2b_pdt2 * 1.0 - state[8]) / (parameters->T11) * (parameters->T10) + state[8];
    } else {
       pss2b_pdt3 = pss2b_pdt2 * 1.0;
       ph3_ovride_int(8, pss2b_pdt3);
    }
    
    pss2b_limiter = fmin(fmax(pss2b_pdt3, (parameters->VSTMIN)), (parameters->VSTMAX));
    
    
    // -----------------------------------------------------------------------------------------------------------------
    // Model outputs
    (outputs->VPSS) = pss2b_limiter;
    


    // -----------------------------------------------------------------------------------------------------------------
    // State derivatives
    
    // ------------------ pss2b_ramptracking_pt1_1 --------------------
    dstate[10] = (pss2b_ramptracking_pdt1 * 1.0 - state[10]) / (parameters->T9);
    
    
    // ------------------ pss2b_ramptracking_pt1_2 --------------------
    dstate[11] = (pss2b_ramptracking_pt1_1 * 1.0 - state[11]) / (parameters->T9);
    
    
    // ------------------ pss2b_ramptracking_pt1_3 --------------------
    dstate[12] = (pss2b_ramptracking_pt1_2 * 1.0 - state[12]) / (parameters->T9);
    
    
    // ------------------ pss2b_ramptracking_pt1_4 --------------------
    dstate[13] = (pss2b_ramptracking_pt1_3 * 1.0 - state[13]) / (parameters->T9);
    
    
    // ------------------ pss2b_ramptracking_pt1_8 --------------------
    dstate[18] = (pss2b_ramptracking_pdt1_0 * 1.0 - state[18]) / (parameters->T9);
    
    
    // ------------------ pss2b_ramptracking_pt1_7 --------------------
    dstate[17] = (pss2b_ramptracking_pt1_8 * 1.0 - state[17]) / (parameters->T9);
    
    
    // ------------------ pss2b_ramptracking_pt1_6 --------------------
    dstate[16] = (pss2b_ramptracking_pt1_7 * 1.0 - state[16]) / (parameters->T9);
    
    
    // ------------------ pss2b_ramptracking_pt1_5 --------------------
    dstate[15] = (pss2b_ramptracking_pt1_6 * 1.0 - state[15]) / (parameters->T9);
    
    
    // ------------------ pss2b_wsh1 --------------------
    if ((parameters->TW1) > (2*delt)) {
       dstate[0] = (pss2b_limiter_0 * 1.0 - state[0]) / (parameters->TW1);
    } else {
       dstate[0] = 0.0;
    }
    
    
    // ------------------ pss2b_wsh2 --------------------
    if ((parameters->TW2) > (2*delt)) {
       dstate[1] = (pss2b_wsh1 * 1.0 - state[1]) / (parameters->TW2);
    } else {
       dstate[1] = 0.0;
    }
    
    
    // ------------------ pss2b_pt1_t6 --------------------
    if ((parameters->T6) > (2*delt)) {
       dstate[5] = (pss2b_wsh2 * 1.0 - state[5]) / (parameters->T6);
    } else {
       dstate[5] = 0.0;
    }
    
    
    // ------------------ pss2b_wsh3 --------------------
    if ((parameters->TW3) > (2*delt)) {
       dstate[2] = (pss2b_limiter_1 * 1.0 - state[2]) / (parameters->TW3);
    } else {
       dstate[2] = 0.0;
    }
    
    
    // ------------------ pss2b_wsh4 --------------------
    if ((parameters->TW4) > (2*delt)) {
       dstate[3] = (pss2b_wsh3 * 1.0 - state[3]) / (parameters->TW4);
    } else {
       dstate[3] = 0.0;
    }
    
    
    // ------------------ pss2b_pt1_k52_t7 --------------------
    if ((parameters->T7) > (2*delt)) {
       dstate[4] = (pss2b_wsh4 * (parameters->KS2) - state[4]) / (parameters->T7);
    } else {
       dstate[4] = 0.0;
    }
    
    
    // ------------------ pss2b_ramptracking_pdt1 --------------------
    if ((parameters->T9) > (2*delt)) {
       dstate[9] = (pss2b_sum * 1.0 - state[9]) / (parameters->T9);
    } else {
       dstate[9] = 0.0;
    }
    
    
    // ------------------ pss2b_ramptracking_pdt1_0 --------------------
    if ((parameters->T9) > (2*delt)) {
       dstate[14] = (pss2b_ramptracking_select_m1 * 1.0 - state[14]) / (parameters->T9);
    } else {
       dstate[14] = 0.0;
    }
    
    
    // ------------------ pss2b_pdt1 --------------------
    if ((parameters->T2) > (2*delt)) {
       dstate[6] = (((parameters->KS1) * pss2b_sum_0) * 1.0 - state[6]) / (parameters->T2);
    } else {
       dstate[6] = 0.0;
    }
    
    
    // ------------------ pss2b_pdt2 --------------------
    if ((parameters->T4) > (2*delt)) {
       dstate[7] = (pss2b_pdt1 * 1.0 - state[7]) / (parameters->T4);
    } else {
       dstate[7] = 0.0;
    }
    
    
    // ------------------ pss2b_pdt3 --------------------
    if ((parameters->T11) > (2*delt)) {
       dstate[8] = (pss2b_pdt2 * 1.0 - state[8]) / (parameters->T11);
    } else {
       dstate[8] = 0.0;
    }
    
            


    // -----------------------------------------------------------------------------------------------------------------
    // Numerical integration
    integrate_mod_euler();
    
    
    // -----------------------------------------------------------------------------------------------------------------
    // Output 
    if (error_flag) {
        instance->LastErrorMessage = ErrorMessage;
        return IEEE_Cigre_DLLInterface_Return_Error;
    } else if (warning_flag) {
        instance->LastGeneralMessage = WarningMessage;
        return IEEE_Cigre_DLLInterface_Return_Message;
    }
    return IEEE_Cigre_DLLInterface_Return_OK;
}


__declspec(dllexport) const IEEE_Cigre_DLLInterface_Model_Info* __cdecl Model_GetInfo() {
    return &Model_Info;
}


__declspec(dllexport) int32_T __cdecl Model_PrintInfo() {

    printf("Cigre/IEEE DLL Standard\n");
    printf("Model name:             %s\n", Model_Info.ModelName);
    printf("Model version:          %s\n", Model_Info.ModelVersion);
    printf("Model description:      %s\n", Model_Info.ModelDescription);
    printf("Model general info:     %s\n", Model_Info.GeneralInformation);
    printf("Model created on:       %s\n", Model_Info.ModelCreated);
    printf("Model created by:       %s\n", Model_Info.ModelCreator);
    printf("Model last modified:    %s\n", Model_Info.ModelLastModifiedDate);
    printf("Model last modified by: %s\n", Model_Info.ModelLastModifiedBy);
    printf("Model modified comment: %s\n", Model_Info.ModelModifiedComment);
    printf("Model modified history: %s\n", Model_Info.ModelModifiedHistory);
    printf("Time Step Sampling Time (sec): %0.5g\n", Model_Info.FixedStepBaseSampleTime);
    switch (Model_Info.EMT_RMS_Mode) {
        case 1:
            printf("EMT/RMS mode:           EMT\n");
            break;
        case 2:
            printf("EMT/RMS mode:           RMS\n");
            break;
        case 3:
            printf("EMT/RMS mode:           EMT and RMS\n");
            break;
        default:
            printf("EMT/RMS mode:           <not available>\n");
    }
    printf("Number of inputs:       %d\n", Model_Info.NumInputPorts);
    printf("Input description:\n");
    for (int k = 0; k < Model_Info.NumInputPorts; k++) {
        printf("  %s\n", Model_Info.InputPortsInfo[k].Name);
    }
    printf("Number of outputs:      %d\n", Model_Info.NumOutputPorts);
    printf("Output description:\n");
    for (int k = 0; k < Model_Info.NumOutputPorts; k++) {
        printf("  %s\n", Model_Info.OutputPortsInfo[k].Name);
    }

    printf("Number of parameters:   %d\n", Model_Info.NumParameters);
    printf("Parameter description:\n");
    for (int k = 0; k < Model_Info.NumParameters; k++) {
        printf("  %s\n", Model_Info.ParametersInfo[k].Name);
    }

    printf("Number of int    state variables:   %d\n", Model_Info.NumIntStates);
    printf("Number of float  state variables:   %d\n", Model_Info.NumFloatStates);
    printf("Number of double state variables:   %d\n", Model_Info.NumDoubleStates);
    printf("\n");
    fflush(stdout);

    return IEEE_Cigre_DLLInterface_Return_OK;
}


__declspec(dllexport) int32_T __cdecl Model_FirstCall(IEEE_Cigre_DLLInterface_Instance* instance){

    return IEEE_Cigre_DLLInterface_Return_OK;
}


__declspec(dllexport) int32_T __cdecl Model_Terminate(IEEE_Cigre_DLLInterface_Instance* instance) {

    return IEEE_Cigre_DLLInterface_Return_OK;
}


__declspec(dllexport) int32_T __cdecl Model_Iterate(IEEE_Cigre_DLLInterface_Instance* instance){

    return IEEE_Cigre_DLLInterface_Return_OK;
}

