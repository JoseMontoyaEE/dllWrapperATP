#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <math.h>
#include <complex.h>
#include <string.h>
#include "EE_PH_Library.h"
#include "IEEE_Cigre_DLLInterface.h"


// ----------------------------------------------------------------------
// Global constants
// ----------------------------------------------------------------------
#define CBUFSIZE 2000
#define LTOL  1e-06
#define NINPUTS 7
#define NOUTPUTS 1
#define NPARAMETERS 8
#define NSTATES 2
#define NVARS 1


// ---------------------------------------------------------------------------------------------------------------------
// Structures defining inputs, outputs, parameters and model information
// ---------------------------------------------------------------------------------------------------------------------
typedef struct _ModelInputs {
    real64_T VRef;
    real64_T Ec;
    real64_T Vs;
    real64_T IFD;
    real64_T VT;
    real64_T VUEL;
    real64_T VOEL;
} ModelInputs;


typedef struct _ModelOutputs {
    real64_T EFD;
} ModelOutputs;


typedef struct _ModelParameters {
    real64_T TAdTB;
    real64_T TB;
    real64_T K;
    real64_T TE;
    real64_T EMin;
    real64_T EMax;
    int32_T CSwitch;
    real64_T RCdRFD;
} ModelParameters;


IEEE_Cigre_DLLInterface_Signal InputSignals[] = {
    [0] = {
        .Name = "VRef",
        .Description = "Reference voltage",
        .Unit = "pu",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .Width = 1
    },
    [1] = {
        .Name = "Ec",
        .Description = "Measured voltage",
        .Unit = "pu",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .Width = 1
    },
    [2] = {
        .Name = "Vs",
        .Description = "Stabilizer signal",
        .Unit = "pu",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .Width = 1
    },
    [3] = {
        .Name = "IFD",
        .Description = "Field Current",
        .Unit = "pu",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .Width = 1
    },
    [4] = {
        .Name = "VT",
        .Description = "Terminal voltage",
        .Unit = "pu",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .Width = 1
    },
    [5] = {
        .Name = "VUEL",
        .Description = "Under excitation limit",
        .Unit = "pu",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .Width = 1
    },
    [6] = {
        .Name = "VOEL",
        .Description = "Over excitation limit",
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
        .Name = "TAdTB",
        .GroupName = "",
        .Description = "Smoothing Time Constant",
        .Unit = "s",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 0.1,
        .MinValue.Real64_Val = 0.001,
        .MaxValue.Real64_Val = 100.0
    },
    [1] = {
        .Name = "TB",
        .GroupName = "",
        .Description = "Smoothing Time Constant",
        .Unit = "s",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 10.0,
        .MinValue.Real64_Val = 0.001,
        .MaxValue.Real64_Val = 100.0
    },
    [2] = {
        .Name = "K",
        .GroupName = "",
        .Description = "Gain",
        .Unit = "pu",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 100.0,
        .MinValue.Real64_Val = 0.001,
        .MaxValue.Real64_Val = 1000.0
    },
    [3] = {
        .Name = "TE",
        .GroupName = "",
        .Description = "Time Constant",
        .Unit = "s",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 0.05,
        .MinValue.Real64_Val = 0.001,
        .MaxValue.Real64_Val = 100.0
    },
    [4] = {
        .Name = "EMin",
        .GroupName = "",
        .Description = "Min Field Voltage",
        .Unit = "pu",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = -5.0,
        .MinValue.Real64_Val = -100.0,
        .MaxValue.Real64_Val = -1.0
    },
    [5] = {
        .Name = "EMax",
        .GroupName = "",
        .Description = "Max Field Voltage",
        .Unit = "pu",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 5.0,
        .MinValue.Real64_Val = 1.0,
        .MaxValue.Real64_Val = 100.0
    },
    [6] = {
        .Name = "CSwitch",
        .GroupName = "",
        .Description = "Power source: 0=VT, 1=1.0",
        .Unit = "[0/1]",
        .DataType = IEEE_Cigre_DLLInterface_DataType_int32_T,
        .FixedValue = 1,
        .DefaultValue.Int32_Val = 1,
        .MinValue.Int32_Val = 0,
        .MaxValue.Int32_Val = 1
    },
    [7] = {
        .Name = "RCdRFD",
        .GroupName = "",
        .Description = "Field resistance ratio",
        .Unit = "-",
        .DataType = IEEE_Cigre_DLLInterface_DataType_real64_T,
        .FixedValue = 0,
        .DefaultValue.Real64_Val = 10.0,
        .MinValue.Real64_Val = 0.001,
        .MaxValue.Real64_Val = 100.0
    }
};


IEEE_Cigre_DLLInterface_Model_Info Model_Info = {
    .DLLInterfaceVersion = { 1, 1, 0, 0 },
    .ModelName = "SCRX9",                                        
    .ModelVersion = "1.0.0",                                  
    .ModelDescription = "SCRX9 - Bus fed or Solid Fed Static Exciter",
    .GeneralInformation = "Test model for EMTP - Photon benchmarking",
    .ModelCreated = "November 10, 2024",                               
    .ModelCreator = "IA",                               
    .ModelLastModifiedDate = "November 11, 2024",
    .ModelLastModifiedBy = "Clint Eastwood",
    .ModelModifiedComment = "Version 1.0.0 for IEEE/Cigre DLL API",
    .ModelModifiedHistory = "History of Changes: V1.0.0 Initial model for API",
    .FixedStepBaseSampleTime = 0.005,
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

void ph3_set_globals(IEEE_Cigre_DLLInterface_Instance* instance, bool init) {
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


void ph3_integrate_mod_euler() {
    for (int i = 0; i < NSTATES; i++) {
        store[i] = store[i] + delt * dstate[i];
        state[i] = store[i] + delt * dstate[i] / 2.0;
    }
}


void ph3_initialize_mod_euler() {
    for (int i = 0; i < NSTATES; i++) {
       dstate[i] = 0.0;
       store[i] = state[i];
    }
}


int ph3_bytes_written(int result_of_sprintf) {
    return (result_of_sprintf > 0) ? result_of_sprintf : 0;
}


size_t ph3_cbuff_size(int bytes_written) {
   return ((CBUFSIZE-bytes_written) > 0) ? (CBUFSIZE-bytes_written) : 0; 
} 


void ph3_warning(const char *format, ...) {
    va_list args;
    va_start(args, format);
    woffset += ph3_bytes_written(vsnprintf(WarningMessage+woffset, ph3_cbuff_size(woffset), format, args));
    warning_flag = true;
    va_end(args);
}


void ph3_error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    eoffset += ph3_bytes_written(vsnprintf(ErrorMessage+eoffset, ph3_cbuff_size(eoffset), format, args));
    error_flag = true;
    va_end(args);
}


void ph3_ovride_int(int k0, real64_T value) {       
    state[k0] = value;
    store[k0] = value;
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



// ---------------------------------------------------------------------------------------------------------------------
// Subroutines that can be called by the main power system program
// ---------------------------------------------------------------------------------------------------------------------
__declspec(dllexport) int32_T __cdecl Model_CheckParameters(IEEE_Cigre_DLLInterface_Instance* instance) {
    ph3_set_globals(instance, false);

    // -----------------------------------------------------------------------------------------------------------------
    // Parameters checking      
    if (!((parameters->K) < 30.0)) {
        ph3_warning("Warning: K (%g) must be < %g  \n", (parameters->K), 30.0);
    }
    
    if (!((parameters->TAdTB) < (parameters->TB))) {
        ph3_warning("Warning: TAdTB (%g) must be < %g ==> TAdTB < TB \n", (parameters->TAdTB), (parameters->TB));
    }
    
    if (!((8.0 < (parameters->RCdRFD)) && ((parameters->RCdRFD) < 12.0))) {
        ph3_warning("Warning: RCdRFD (%g) must be %g < ... < %g  \n", (parameters->RCdRFD), 8.0, 12.0);
    }
    
    if (!((-7.0 < (parameters->EMin)) && ((parameters->EMin) < -0.5))) {
        ph3_warning("Warning: EMin (%g) must be %g < ... < %g ==> EMin is limited by hardware beetween -7.0 and -0.5 \n", (parameters->EMin), -7.0, -0.5);
    }
    
    if (!((parameters->EMax) > (parameters->EMin))) {
        ph3_warning("Warning: EMax (%g) must be > %g ==> EMax > EMin \n", (parameters->EMax), (parameters->EMin));
    }
    
    if (!((parameters->TE) > 2*delt)) {
        ph3_warning("Warning: TE (%g) must be > %g ==> TE > 2*delt \n", (parameters->TE), 2*delt);
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
    ph3_set_globals(instance, true);

    // -----------------------------------------------------------------------------------------------------------------
    // Model initialization
    double ocontrol, oleadlag, verr, voffset;
    
    // test if initial conditions use negative field logic
    if ((inputs->IFD) < 0.0) {
       ph3_warning("Warning: initial field current: %f is negative.\n", (inputs->IFD));
    }
    
    // check if bus-fed or independent supply
    if ((parameters->CSwitch) == 1) {
       ocontrol = (outputs->EFD);
    } else {
       // some simulators may ramp-up voltages starting from zero, we need to protect for possible divide by zero errors
       if (fabs((inputs->VT)) < LTOL) {    
          ocontrol = (outputs->EFD) / LTOL;    
       }
       else {
          ocontrol = (outputs->EFD) / ((inputs->VT));    
       }
       
    }
    
    // check if EFD initial condition is on a limit
    if (ocontrol < (parameters->EMin)) {
       ph3_warning("Warning: initial field voltage is %f and is < EMin: %f.\n", ocontrol, (parameters->EMin));
       ocontrol = (parameters->EMin);
    }
    if (ocontrol > (parameters->EMax)) {
       ph3_warning("Warning: initial field voltage is %f and is > EMax: %f.\n", ocontrol, (parameters->EMax));
       ocontrol = (parameters->EMax);
    }
    
    // lead-lag output
    oleadlag = ocontrol / (parameters->K);
    
    // internal memory offset (needed to add to input voltage summation loop (constant))
    verr = oleadlag / 1.0;
    voffset = verr + (inputs->Ec) - (inputs->VRef);
    
    
    // initialize vars
    var[0] = voffset; 
    
    // initialize states
    state[1] = ocontrol;
    state[0] = oleadlag;

    // -----------------------------------------------------------------------------------------------------------------
    // Numerical integration method initialization
    ph3_initialize_mod_euler();

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
    ph3_set_globals(instance, false);

    // -----------------------------------------------------------------------------------------------------------------
    // Model variables
    real64_T scrx_efd_sw, scrx_out, scrx_pdt1, scrx_power_sw, scrx_pt1aw, scrx_sum, scrx_sum_2;
    bool scrx_hm1, scrx_hm2, scrx_land;

    // -----------------------------------------------------------------------------------------------------------------
    // Auxiliary variables
    real64_T x;


    // -----------------------------------------------------------------------------------------------------------------
    // Algebraic equations
    scrx_sum = - (inputs->Ec) + (inputs->VRef) + (inputs->VOEL) + (inputs->VUEL) + (inputs->Vs) + var[0];
    
    // ------------------ scrx_pdt1 --------------------
    if ((parameters->TB) > (2*delt)) {
       scrx_pdt1 = (scrx_sum * 1.0 - state[0]) / (parameters->TB) * ((parameters->TAdTB) * (parameters->TB)) + state[0];
    } else {
       scrx_pdt1 = scrx_sum * 1.0;
       ph3_ovride_int(0, scrx_pdt1);
    }
    
    scrx_hm1 = ((-(parameters->RCdRFD)) * (inputs->IFD)) > 0.0;
    
    // ------------------ scrx_power_sw --------------------
    x = INT_TO_DOUBLE((parameters->CSwitch));
    if (x > 0.5) {
       x = 1.0;
    } else {
       x = (inputs->VT);
    }
    scrx_power_sw = x;
    
    scrx_pt1aw = state[1];
    scrx_out = 1.0 * scrx_pt1aw * scrx_power_sw;
    scrx_sum_2 = + ((-(parameters->RCdRFD)) * (inputs->IFD)) - scrx_out;
    scrx_hm2 = scrx_sum_2 > 0.0;
    scrx_land = scrx_hm1 && scrx_hm2;
    
    // ------------------ scrx_efd_sw --------------------
    x = BOOL_TO_DOUBLE(scrx_land);
    if (x > 0.5) {
       x = (-(parameters->RCdRFD)) * (inputs->IFD);
    } else {
       x = scrx_out;
    }
    scrx_efd_sw = x;
    
    
    
    // -----------------------------------------------------------------------------------------------------------------
    // Model outputs
    (outputs->EFD) = scrx_efd_sw;
    


    // -----------------------------------------------------------------------------------------------------------------
    // State derivatives
    
    // ------------------ scrx_pdt1 --------------------
    if ((parameters->TB) > (2*delt)) {
       dstate[0] = (scrx_sum * 1.0 - state[0]) / (parameters->TB);
    } else {
       dstate[0] = 0.0;
    }
    
    
    // ------------------ scrx_pt1aw --------------------
    dstate[1] = (scrx_pdt1 * (parameters->K) - state[1]) / (parameters->TE);
    ph3_limit_int(1, (parameters->EMin), (parameters->EMax));
    
            


    // -----------------------------------------------------------------------------------------------------------------
    // Numerical integration
    ph3_integrate_mod_euler();
    
    
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

