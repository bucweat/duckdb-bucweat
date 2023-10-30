@echo off
setlocal EnableDelayedExpansion


REM build.bat is provided to ease standalone build 
REM which is helpful for development so that you 
REM don't have to wait for entire DuckDB build to
REM to complete in order run the test app.
REM 
REM Usage: build.bat [Options]
REM
REM Options:    debug - builds debug version
REM             release - builds release version
REM             clean - deletes the local build directory.
REM

REM uncomment the following to provide more verbose output
REM SET __DEBUGECHO=ECHO
IF NOT DEFINED __DEBUGECHO (SET __DEBUGECHO=REM)

set "arg1="
call set "arg1=%%1"
set bit=64
set ARCH=amd64
set "CLARG="

REM setup Visual Studio
CALL :fn_ConfigVisualStudio

REM report the compiler architecture as a check
CALL :fn_GetCompilerArch

if defined arg1 (
    goto :arg_exists
) else (
    REM default for no argument is build release
    goto :switch-case-N-release
)
    
:arg_exists

REM now choose debug/release/install/clean
:switch-case-example
goto :switch-case-N-%arg1% 2>nul || (
    echo %arg1% is not a valid command
    goto :switch-case-end
)
goto :switch-case-end
  
:switch-case-N-release
	if not exist ".\build" ( mkdir ".\build")
	cd .\build
    cmake -S .. -B . -G "Visual Studio 17 2022"
    cmake --build . --config Release
    cd ..
    goto :switch-case-end

  
:switch-case-N-debug
	if not exist ".\build" ( mkdir ".\build")
	cd .\build
    cmake -S .. -B . -G "Visual Studio 17 2022"
    cmake --build . --config Debug
    cd ..
    goto :switch-case-end

:switch-case-N-clean
    echo clean
    rmdir /S /Q .\build 2>NUL
    goto :switch-case-end

:switch-case-end

GOTO :exit

REM ***************************************************
:fn_ConfigVisualStudio
    REM
    REM Visual Studio 2017 / 2019 / 2022 / future versions (hopefully)...
    REM
    CALL :fn_TryUseVsWhereExe
    IF NOT DEFINED VSWHEREINSTALLDIR GOTO skip_detectVisualStudio2017
    SET VSVARS32=%VSWHEREINSTALLDIR%\Common7\Tools\VsDevCmd.bat
    IF EXIST "%VSVARS32%" (
            ECHO Using Visual Studio 2017 / 2019 / 2022...
            set CLARG=-arch=%ARCH%
            %__DEBUGECHO% VSVARS32="%VSVARS32%" %CLARG%
            GOTO skip_detectVisualStudio
    )
    :skip_detectVisualStudio2017

    REM
    REM NOTE: At this point, the appropriate Visual Studio version should be
    REM       selected.
    REM
    :skip_detectVisualStudio

    SET VSVARS32=%VSVARS32:\\=\%
    %__DEBUGECHO% "%VSVARS32%" %CLARG%
    CALL "%VSVARS32%" %CLARG% 1>nul
    GOTO :EOF

REM ***************************************************
:fn_GetCompilerArch
    set "cl_arch="
    SET _cmd=cl /? 
    FOR /F "delims=" %%G IN ('%_cmd% 2^>^&1 ^| findstr /C:"Version"') DO (
        for %%A in (%%G) do (
            set cl_arch=%%A
        )
    )
    echo cl.exe compiler architectue is %cl_arch%
    GOTO :EOF

REM ***************************************************
:fn_TryUseVsWhereExe
    IF DEFINED VSWHERE_EXE GOTO skip_setVsWhereExe
    SET VSWHERE_EXE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe
    IF NOT EXIST "%VSWHERE_EXE%" SET VSWHERE_EXE=%ProgramFiles%\Microsoft Visual Studio\Installer\vswhere.exe
    :skip_setVsWhereExe

    IF NOT EXIST "%VSWHERE_EXE%" (
        ECHO The "VsWhere" tool does not appear to be installed.
        GOTO :EOF
    ) ELSE (
        %__DEBUGECHO% VSWHERE_EXE="%VSWHERE_EXE%"
    )
    SET VS_WHEREIS_CMD="%VSWHERE_EXE%" -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath -latest
    %__DEBUGECHO% VS_WHEREIS_CMD=%VS_WHEREIS_CMD%

    FOR /F "delims=" %%D IN ('%VS_WHEREIS_CMD%') DO (SET VSWHEREINSTALLDIR=%%D)

    IF NOT DEFINED VSWHEREINSTALLDIR (
        ECHO Visual Studio 2017 / 2019 / 2022 is not installed.
    GOTO :EOF
    )
    %__DEBUGECHO% Visual Studio 2017 / 2019 / 2022 is installed.
    %__DEBUGECHO% VsWhereInstallDir = '%VSWHEREINSTALLDIR%'
    GOTO :EOF

REM ***************************************************
:exit

