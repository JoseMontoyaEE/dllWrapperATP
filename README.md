# ATP + ICDLL

Interface between IEEE/CIGRE DLL INTERFACE MODEL and ATP software

## Getting started

### makefile.paths

Create a makefile.paths inside `tpmingw` folder, and define `MY_CC` and `MY_FOR`, like:

```
MY_CC=C:\Develop\Mingw\bin\gcc
MY_FOR=C:\Develop\Mingw\bin\g77
```

### setup_env.bat

create a setup_env.bat (or similar name), to setup PATH to MINGW compiler suite, following this example:

```
@echo off
set "TARGET=C:\Develop\MinGW\bin"

echo ";%PATH%;" | findstr /I /C:";%TARGET%;" >NUL

if errorlevel 1 (
    echo [INFO] "%TARGET%" not found on PATH -> appending
    set "PATH=%TARGET%;%PATH%"
    echo [OK] PATH updated
) else (
    echo [SKIP] "%TARGET%" is already on PATH
)
```

### Vardim

Run the vardimn.bat file, to setup dimensions for internal arrays in ATP.

```
.\vardimn.bat listsize.gig
```

### mingw32-make

Compile the sources to create my_tpgig.exe

```
.\setup_env.bat
mingw32-make all
```


## Roadmap

### Version 0.0.1

1) Create a MODELS interface file, from the manufacturer DLL using the `import_tool`.

2) Connect the MODELS interface file in the AtpDraw model. Change the `USE` statement
to adapt to your particular use case.

The MODELS interface file looks like (the current format for the MODELS could change in future specifications)

```
MODEL SCRX9
  DATA 
    Instance {DFLT:1} -- Instance Number from icdll_list.txt
    TAdTB    {DFLT:0.1} -- TA/TB factor
    TB       {DFLT:5.0} -- controller time constant
    K        {DFLT:20.0} -- controller gain
    TE       {DFLT:0.1} -- excitation time constant
    EMin     {DFLT:-5.0} -- minimum excitation output
    EMax     {DFLT:5.0} -- maximum excitation output
    CSwitch  {DFLT:1} -- source selector 1:Ec, 0:Constant
    RCdRFD   {DFLT:10.0} -- RD/RFD factor
    
    TRelease {DFLT:0}

  INPUT
    VRef -- reference voltage
    Ec -- measurement voltage
    Vs {DFLT:0} -- stabilizer input (optional)
    IFD {DFLT:0} -- field current
    VT -- terminal voltage 
    VUEL {DFLT:0} -- UEL input (optional)
    VOEL {DFLT:0} -- OEL input (optional)

    EFD_0 -- initial condition of EFD output

  OUTPUT
    EFD

  VAR
    EFD

  INIT
    EFD := EFD_0
    
  ENDINIT
  
  MODEL SCRX9 FOREIGN icdll { ixdata:8+3, ixin:1+7+1, ixout:1, ixvar:6 }
  
  EXEC
    
    USE SCRX9 AS DEFAULT                
    
      DATA
        xdata[1..8+2] := [ Instance, timestep, TRelease, TAdTB, TB, K, TE, EMin, EMax, CSwitch, RCdRFD ]
        
      INPUT             
        xin[1..1+7+1] := [ t, VRef, Ec, Vs, IFD, VT, VUEL, VOEL, EFD_0 ]
        
      OUTPUT 
        EFD := xout[1]

    ENDUSE
  ENDEXEC
ENDMODEL
```

3) Read a `icdll_list.txt` file to setup IEEE/CIGRE Instances.

The format of the `icdll_list.txt` file is as follows:
```
# this is a comment line
1;C:\some\path\to\file_model.dll
2;C:\some\path\to\other\file_model_1.dll
# ---- comment
3;C:\some\path\to\other\file_model_2.dll
#
# enable verbose messages (0: disabled, 1: normal, 2: debug)
$verbose=1
```

4) Load the selected instance model (1, 2, 3 ...) from the DLL list, and store the instance to be used during `init` and `exec` phases.

## Project status

Project under development.
