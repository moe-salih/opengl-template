@echo off

set "LOG=./build.log"

set "ProjectDir=%~dp0" 
if "%ProjectDir:~-1%"=="\" set "ProjectDir=%ProjectDir:~0,-1%"
for %%a in ("%ProjectDir:\=" "%") do set "ProjectName=%%~a"


set "CC=cl"

set "_MAIN_SRC=./src/*.c*"
@REM set "_3RD_PARTY_SRC=./inc/3rd-party/src/*.c*"
set "SOURCES=%_MAIN_SRC%"
@REM  %_3RD_PARTY_SRC%"

set _MAIN_INC="./inc"
set _3RD_PARTY_INC="./inc/vendor"
set "INCLUDS=/I%_MAIN_INC% /I%_3RD_PARTY_INC%"

set "LIB_DIR=./lib"
set "LIBS=glfw3_mt.lib glad.lib user32.lib shell32.lib kernel32.lib gdi32.lib dwmapi.lib opengl32.lib fmt.lib imgui.lib imgui_impl.lib"

set "CFLAGS=/EHsc /std:c++20 /MT"
set "LDFLAGS=/link /LIBPATH:%LIB_DIR% %LIBS%"

set "TARGET=target\"
set "INTERMEDIATE=target\intermediate\"

goto:MAIN


:Print
    SETLOCAL ENABLEDELAYEDEXPANSION
        SET message=%~1
        ECHO %message%
        ECHO %message% >> %LOG%
    ENDLOCAL
EXIT /B 0

:Log
    SETLOCAL ENABLEDELAYEDEXPANSION
        SET level=%~1
        SET message=%~2
                
        ECHO [%level% : %time%] : %message%
        ECHO [%level% : %time%] : %message% >> %LOG%
    ENDLOCAL
EXIT /B 0

:MAIN
    call:Print " ------------- BUILD ATEMPT -------------- "
    call:Log INFO "Project Name: %ProjectName%"
    call:Print " ----------------------------------------- "

    @REM check target directory if not exist create it
    if not exist "%TARGET%" (
        call:Log INFO "Target directory not exist, creating..."
        mkdir "%TARGET%"
        call:Log INFO "Target directory created."
    ) else (
        call:Log INFO "Cleaning target directory..."
        del /q "%TARGET%"    

        if not exist "%INTERMEDIATE%" (
            mkdir "%INTERMEDIATE%"
        ) else (
            call:Log INFO "Cleaning intermediates directory..."
            del /q "%INTERMEDIATE%"
        )
    )

    @REM compile
    call:Log: INFO "Compiling : %CC% %CFLAGS% %SOURCES% /Fo:%INTERMEDIATE% /Fd:%INTERMEDIATE% /Fe:%TARGET%\%ProjectName%.exe %INCLUDS% /link %LDFLAGS%"
    @REM call:Print "------------- COMPILATION ---------------"
    echo ------------- COMPILATION --------------- >> %LOG%
    
    %CC% %CFLAGS% %SOURCES% /Fo:%INTERMEDIATE% /Fd:%INTERMEDIATE% /Fe:%TARGET%\%ProjectName%.exe %INCLUDS% /link %LDFLAGS% >> %LOG% 2>&1
    set "ERROR_CODE=%ERRORLEVEL%"
    echo ----------------------------------------- >> %LOG%
    @REM call:Print " ----------------------------------------- "
    call:Log INFO "Compilation done."

    call:Print " ------------- BUILD FINISHED ------------ "

    if %ERROR_CODE% NEQ 0 (
        call:Log ERROR "Build failed. Exit code: %ERROR_CODE%"
    ) else (
        call:Log INFO "Build succeeded. Exit code: %ERROR_CODE%"
    )

    call:Print " ----------------------------------------- "
    exit /b %ERROR_CODE%
