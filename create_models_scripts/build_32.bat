@REM gcc -O2 -shared -o scm_32.dll SCRX9_m.c

@REM gcc reader.c -o reader_32bits_1.exe
gcc create_MODELS.c -o reader_32bits_1.exe
reader_32bits_1.exe

@REM gcc -O2 -shared -o scm.dll SCRX9_m.c