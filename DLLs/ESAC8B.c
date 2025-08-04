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
#define NINPUTS 6
#define NOUTPUTS 1
#define NPARAMETERS 21
#define NSTATES 5
#define NVARS 1


// ---------------------------------------------------------------------------------------------------------------------
// Structures defining inputs, outputs, parameters and model information
// ---------------------------------------------------------------------------------------------------------------------
typedef struct _ModelInputs {
    real64_T VREF;
    real64_T ECOMP;
    real64_T IFD;
    real64_T VOEL;
    real64_T VUEL;
    real64_T VPSS;
} ModelInputs;


typedef struct _ModelOutputs {
    real64_T EFD;
} ModelOutputs;


typedef struct _ModelParameters {
    real64_T TR;
    real64_T KPR;
    real64_T KIR;
    real64_T KDR;
    real64_T TDR;
    real64_T VPIDMAX;
    real64_T VPIDMIN;
    real64_T KA;
    real64_T TA;
    real64_T VRMAX;
    real64_T VRMIN;
    real64_T KC;
    real64_T KD;
    real64_T KE;
    real64_T TE;
    real64_T VFEMAX;
    real64_T VEMIN;
    real64_T E1;
    real64_T SE1;
    real64_T E2;
    real64_T SE2;
} ModelParameters;


IEEE_Cigre_DLLInterface_Signal InputSignals[] = {
    [0] = {
        .Name = "VREF",
        .Description = "Reference voltage",
        .Unit = "pu",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .Width = 1
    },
    [1] = {
        .Name = "ECOMP",
        .Description = "Compensated voltage",
        .Unit = "pu",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .Width = 1
    },
    [2] = {
        .Name = "IFD",
        .Description = "Field current",
        .Unit = "pu",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .Width = 1
    },
    [3] = {
        .Name = "VOEL",
        .Description = "Over excitation limit",
        .Unit = "pu",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .Width = 1
    },
    [4] = {
        .Name = "VUEL",
        .Description = "Under excitation limit",
        .Unit = "pu",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .Width = 1
    },
    [5] = {
        .Name = "VPSS",
        .Description = "Stabilizer signal",
        .Unit = "pu",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .Width = 1
    }
};


IEEE_Cigre_DLLInterface_Signal OutputSignals[] = {  
    [0] = {
        .Name = "EFD",
        .Description = "Output Field Voltage",
        .Unit = "pu",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .Width = 1
    }  
};


IEEE_Cigre_DLLInterface_Parameter Parameters[] = {
    [0] = {
        .Name = "TR",
        .GroupName = "",
        .Description = "Voltage regulator input filter time constant",
        .Unit = "[s]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 0.02,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [1] = {
        .Name = "KPR",
        .GroupName = "",
        .Description = "Voltage regulator proportional gain",
        .Unit = "[pu]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 35.0,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [2] = {
        .Name = "KIR",
        .GroupName = "",
        .Description = "Voltage regulator integral gain",
        .Unit = "[pu]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 15.0,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [3] = {
        .Name = "KDR",
        .GroupName = "",
        .Description = "Voltage regulator derivative gain",
        .Unit = "[pu]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 2.0,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [4] = {
        .Name = "TDR",
        .GroupName = "",
        .Description = "Lag time constant for derivative channel of PID controller",
        .Unit = "[s]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 0.02,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [5] = {
        .Name = "VPIDMAX",
        .GroupName = "",
        .Description = "PID maximum limit",
        .Unit = "[pu]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 10.0,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [6] = {
        .Name = "VPIDMIN",
        .GroupName = "",
        .Description = "PID minimum limit",
        .Unit = "[pu]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = -10.0,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [7] = {
        .Name = "KA",
        .GroupName = "",
        .Description = "Rectifier bridge gain",
        .Unit = "[pu]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 1.0,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [8] = {
        .Name = "TA",
        .GroupName = "",
        .Description = "Rectifier bridge time constant",
        .Unit = "[s]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 0.02,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [9] = {
        .Name = "VRMAX",
        .GroupName = "",
        .Description = "Maximum regulator output",
        .Unit = "[pu]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 10.0,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [10] = {
        .Name = "VRMIN",
        .GroupName = "",
        .Description = "Minimum regulator output",
        .Unit = "[pu]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = -10.0,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [11] = {
        .Name = "KC",
        .GroupName = "",
        .Description = "Rectifier loading factor proportional to commutating reactance",
        .Unit = "[pu]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 0.3,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [12] = {
        .Name = "KD",
        .GroupName = "",
        .Description = "Demagnetizing factor, function of exciter alternator reactances",
        .Unit = "[pu]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 0.4,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [13] = {
        .Name = "KE",
        .GroupName = "",
        .Description = "Exciter field proportional constant",
        .Unit = "[pu]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 1.0,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [14] = {
        .Name = "TE",
        .GroupName = "",
        .Description = "Exciter field time constant",
        .Unit = "[pu]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 0.3,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [15] = {
        .Name = "VFEMAX",
        .GroupName = "",
        .Description = "Maximum exciter field current",
        .Unit = "[pu]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 10.0,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [16] = {
        .Name = "VEMIN",
        .GroupName = "",
        .Description = "Minimum exciter field current",
        .Unit = "[pu]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = -10.0,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [17] = {
        .Name = "E1",
        .GroupName = "",
        .Description = "Exciter output voltage for saturation factor SE(E1)",
        .Unit = "-",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 1.0,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [18] = {
        .Name = "SE1",
        .GroupName = "",
        .Description = "Exciter saturation factor at exciter output voltage E1",
        .Unit = "[pu]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 0.06,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [19] = {
        .Name = "E2",
        .GroupName = "",
        .Description = "Exciter output voltage for saturation factor SE(E2)",
        .Unit = "-",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 10.0,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    },
    [20] = {
        .Name = "SE2",
        .GroupName = "",
        .Description = "Exciter saturation factor at exciter output voltage E2",
        .Unit = "[pu]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 0.6,
        .MinValue.Real64_Val = -9999,
        .MaxValue.Real64_Val = 9999
    }
};


IEEE_Cigre_DLLInterface_Model_Info Model_Info = {
    .DLLInterfaceVersion = { 1, 1, 0, 0 },
    .ModelName = "ESAC8B",                                        
    .ModelVersion = "1.0.0",                                  
    .ModelDescription = "IEEE 421.5 2005 AC8B Excitation System",
    .GeneralInformation = "Model created by Photon-ICC - Electrical Studies Corp",
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
//      state[0] =  / [ESAC8B/Intaw$0] 
//      state[1] =  / [ESAC8B/PT1aw$0] 
//      state[2] =  / [ESAC8B/PID$0] 
//      state[3] =  / [ESAC8B/PID$1] 
//      state[4] =  / [ESAC8B/PT1$0] 


// ---------------------------------------------------------------------------------------------------------------------
// Vars documentation
// ---------------------------------------------------------------------------------------------------------------------
//      var[0] =  / [ESAC8B/VOFFSET_VAR&0] 


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


void ph3_limit_int(int k0, real64_T xmin, real64_T xmax) {
    if (state[k0] >= xmax) {
        state[k0] = xmax;
        store[k0] = xmax;
        if (dstate[k0] >= 0.0) {
            dstate[k0] = 0.0;
        }
    } else if (state[k0] <= xmin) {
        state[k0] = xmin;
        store[k0] = xmin;
        if (dstate[k0] <= 0.0) {
            dstate[k0] = 0.0;
        }
    }
}


real64_T ph3_fex(real64_T i, real64_T v, real64_T kc) {
    real64_T z = 1.0;
    
    if (v > 0) { 
        z = kc * i/v; 
    }
    
    if (z >= 0.75 && z <= 1.0) {
        return 1.732 * (1-z);
    } else if (z > 0.433 && z < 0.75) {
        return sqrt(0.75 - z*z);
    } else if (z <= 0.433 && z > 0.0) {
        return 1.0 - 0.577*z;
    } else if (z <= 0.0) {
        return 1.0;
    } else {
        return 0.0;
    }    
}


real64_T ph3_invfex(real64_T efd, real64_T kc) {
    real64_T x;
    
    x = kc / (1+0.577*kc);
    
    if (x == 0.0) {   // kc = 0 -> fex(.) = 1.0 -> no commutation
        return efd;
    }    
    if (x > 0.0 && x < 0.433) {
        return kc*efd/x;
    } 
    
    x = kc*sqrt(0.75/(1 + kc*kc));            
    
    if (x >= 0.433 && x < 0.75) {
        return kc*efd/x;
    }

    x = 1.732*kc/(1+1.732*kc);
    
    if (x >= 0.75 && x <= 1.0) {
        return kc*efd/x;
    } 
    
    return 0.0;  // this must be an error
}


void ph3_ovride_int(int k0, real64_T value) {       
    state[k0] = value;
    store[k0] = value;
}


real64_T ph3_expsat(real64_T e, real64_T e1, real64_T se1, real64_T e2, real64_T se2, bool asgain) {
    real64_T z = 99999.9;
    
    if ((se1 * e1) > 0.0) {
        z = (e2 - e1 * sqrt( (se2*e2)/(se1*e1) )) / (1.0 - sqrt( (se2*e2)/(se1*e1) ));
    }
    
    if (e > z) {
        if (asgain) {
            return (se1*e1)/((e1-z)*(e1-z)) * ((e-z)*(e-z));
        } else {
            return ((se1*e1)/((e1-z)*(e1-z)) * ((e-z)*(e-z))) / e;
        }
    } else {
        return 0.0;
    }
}


void ph3_limit_pi(int k0, real64_T prop, real64_T xmin, real64_T xmax) {
    real64_T x;
    
    x = state[k0] + prop;  // PIaw output
    if (x >= xmax) {
        state[k0] = fmin(state[k0], xmax);
        store[k0] = state[k0];
        if (dstate[k0] >= 0.0) {
            dstate[k0] = 0.0;
        }
    } else if (x <= xmin) {
        state[k0] = fmax(state[k0], xmin);
        store[k0] = state[k0];
        if (dstate[k0] <= 0.0) {
            dstate[k0] = 0.0;
        }
    }
}



// ---------------------------------------------------------------------------------------------------------------------
// Subroutines that can be called by the main power system program
// ---------------------------------------------------------------------------------------------------------------------
__declspec(dllexport) int32_T __cdecl Model_CheckParameters(IEEE_Cigre_DLLInterface_Instance* instance) {
    set_globals(instance, false);

    // -----------------------------------------------------------------------------------------------------------------
    // Parameters checking      
    if (!((parameters->VFEMAX) > (parameters->VEMIN))) {
        warning("Warning: VFEMAX (%g) must be > %g ==> VFEMAX > VEMIN \n", (parameters->VFEMAX), (parameters->VEMIN));
    }
    
    if (!((parameters->KC) >= 0.0)) {
        warning("Warning: KC (%g) must be >= %g ==> KC >= 0 \n", (parameters->KC), 0.0);
    }
    
    if (!((parameters->E1) >= 0.0)) {
        warning("Warning: E1 (%g) must be >= %g ==> E1 >= 0 \n", (parameters->E1), 0.0);
    }
    
    if (!((parameters->E2) >= (parameters->E1))) {
        warning("Warning: E2 (%g) must be >= %g ==> E2 >= E1 \n", (parameters->E2), (parameters->E1));
    }
    
    if (!((parameters->SE1) >= 0.0)) {
        warning("Warning: SE1 (%g) must be >= %g ==> SE1 >= 0 \n", (parameters->SE1), 0.0);
    }
    
    if (!((parameters->SE2) >= (parameters->SE1))) {
        warning("Warning: SE2 (%g) must be >= %g ==> SE2 >= SE1 \n", (parameters->SE2), (parameters->SE1));
    }
    
    if (!((parameters->TDR) > 0.0)) {
        warning("Warning: TDR (%g) must be > %g ==> TDR > 0 \n", (parameters->TDR), 0.0);
    }
    
    if (!((parameters->VPIDMAX) > (parameters->VPIDMIN))) {
        warning("Warning: VPIDMAX (%g) must be > %g ==> VPIDMAX > VPIDMIN] \n", (parameters->VPIDMAX), (parameters->VPIDMIN));
    }
    
    if (!((parameters->VRMAX) > (parameters->VRMIN))) {
        warning("Warning: VRMAX (%g) must be > %g ==> VRMAX > VRMIN \n", (parameters->VRMAX), (parameters->VRMIN));
    }
    
    if (!((parameters->TE) != 0.0)) {
        warning("Warning: TE (%g) must be != %g ==> TE != 0 \n", (parameters->TE), 0.0);
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
    double efd, vref, ecomp, ifd, ve;
    
    efd = (outputs->EFD);
    vref = (inputs->VREF);
    ecomp = (inputs->ECOMP);
    ifd = (inputs->IFD);
    
    
    // rotating exciter output voltage
    if ((parameters->KC) == 0.0) {
       ve = efd;
    } else {
       ve = ph3_invfex(efd, (parameters->KC));
    }
    
    // internal states
    state[0] = ve;
    state[1] = (parameters->KE)*ve + ph3_expsat(ve, (parameters->E1), (parameters->SE1), (parameters->E2), (parameters->SE2), true) + (parameters->KD)*ifd;
    state[2] = state[1] / (parameters->KA);
    state[3] = 0.0;
    state[4] = ecomp;
    
    
    // internal memory offset (needed to add to input voltage summation loop (constant))
    var[0] = ecomp - vref;
    
    
    // error messages
    if (state[0] > (parameters->VFEMAX)) {
       warning("Warning: initial state of ve (%g) is greater than VFEMAX (%g) .\n", state[0], (parameters->VFEMAX));
    } else if (state[0] < (parameters->VEMIN)) {
       warning("Warning: initial state of ve (%g) is lower than VEMIN (%g) .\n", state[0], (parameters->VEMIN));
    }
    
    if (state[1] > (parameters->VRMAX)) {
       warning("Warning: initial regulator output (%g) is greater than VRMAX (%g) .\n", state[1], (parameters->VRMAX));
    } else if (state[1] < (parameters->VRMIN)) {
       warning("Warning: initial regulator output (%g) is lower than VRMIN (%g) .\n", state[1], (parameters->VRMIN));
    }
    
    if (state[2] > (parameters->VPIDMAX)) {
       warning("Warning: initial PID output (%g) is greater than VPIDMAX (%g) .\n", state[2], (parameters->VPIDMAX));
    } else if (state[2] < (parameters->VPIDMIN)) {
       warning("Warning: initial PID output (%g) is lower than VPIDMIN (%g) .\n", state[2], (parameters->VPIDMIN));
    }

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
    real64_T esac8b_expsat, esac8b_fex, esac8b_intaw, esac8b_pid, esac8b_product, esac8b_pt1, esac8b_pt1aw, esac8b_sum, esac8b_sum_0, esac8b_sum_1, esac8b_sum_2, esac8b_sum_3;

    // -----------------------------------------------------------------------------------------------------------------
    // Auxiliary variables
    


    // -----------------------------------------------------------------------------------------------------------------
    // Algebraic equations
    esac8b_intaw = state[0];
    esac8b_fex = ph3_fex((inputs->IFD), esac8b_intaw, (parameters->KC));
    esac8b_product = 1.0 * esac8b_intaw * esac8b_fex;
    
    // ------------------ esac8b_pt1 --------------------
    if ((parameters->TR) > (2*delt)) {
       esac8b_pt1 = state[4];
    } else {
       esac8b_pt1 = (inputs->ECOMP) * 1.0;
       ph3_ovride_int(4, esac8b_pt1);
    }
    
    esac8b_expsat = ph3_expsat(esac8b_intaw, (parameters->E1), (parameters->SE1), (parameters->E2), (parameters->SE2), true);
    esac8b_sum_2 = + esac8b_expsat + ((parameters->KD) * (inputs->IFD)) + ((parameters->KE) * esac8b_intaw);
    esac8b_sum_3 = - esac8b_pt1 + var[0];
    esac8b_sum = + esac8b_sum_3 + (inputs->VUEL) + (inputs->VREF);
    esac8b_sum_0 = + esac8b_sum + (inputs->VOEL) + (inputs->VPSS);
    
    // ------------------ esac8b_pid --------------------
    esac8b_pid = (parameters->KPR) * esac8b_sum_0 + state[2] + ((parameters->KDR) * esac8b_sum_0 - state[3]) / (parameters->TDR);
    esac8b_pid = fmin(fmax(esac8b_pid, (parameters->VPIDMIN)), (parameters->VPIDMAX));
    
    
    // ------------------ esac8b_pt1aw --------------------
    if ((parameters->TA) > (2*delt)) {
       esac8b_pt1aw = state[1];
    } else { // time constant == 0 -> behave like a Gain with limits
       esac8b_pt1aw = fmin(fmax(esac8b_pid * (parameters->KA), (parameters->VRMIN)), (parameters->VRMAX));
       ph3_ovride_int(1, esac8b_pt1aw);
    }
    
    esac8b_sum_1 = + esac8b_pt1aw - esac8b_sum_2;
    
    
    // -----------------------------------------------------------------------------------------------------------------
    // Model outputs
    (outputs->EFD) = esac8b_product;
    


    // -----------------------------------------------------------------------------------------------------------------
    // State derivatives
    
    // ------------------ esac8b_intaw --------------------
    dstate[0] = 1/(parameters->TE) * esac8b_sum_1;
    ph3_limit_int(0, (parameters->VEMIN), (parameters->VFEMAX));
    
    
    // ------------------ esac8b_pt1 --------------------
    if ((parameters->TR) > (2*delt)) {
       dstate[4] = ((inputs->ECOMP) * 1.0 - state[4]) / (parameters->TR);
    } else {
       dstate[4] = 0.0;
    }
    
    
    // ------------------ esac8b_pid --------------------
    dstate[2] = esac8b_sum_0 * (parameters->KIR);
    dstate[3] = ((parameters->KDR) * esac8b_sum_0 - state[3]) / (parameters->TDR);
    // limit (parameters->VPIDMIN) <= state[2] + (parameters->KPR) * esac8b_sum_0 + ((parameters->KDR) * esac8b_sum_0 - state[3]) / (parameters->TDR) <= (parameters->VPIDMAX)
    ph3_limit_pi(2, (parameters->KPR) * esac8b_sum_0 + ((parameters->KDR) * esac8b_sum_0 - state[3]) / (parameters->TDR), (parameters->VPIDMIN), (parameters->VPIDMAX));
    
    
    // ------------------ esac8b_pt1aw --------------------
    if ((parameters->TA) > (2*delt)) {
       dstate[1] = (esac8b_pid * (parameters->KA) - state[1]) / (parameters->TA);
       ph3_limit_int(1, (parameters->VRMIN), (parameters->VRMAX));
    } else { // time constant == 0.0
       dstate[1] = 0.0;
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

