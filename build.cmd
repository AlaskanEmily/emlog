@echo off

rem Any copyright of this file is dedicated to the Public Domain.
rem http://creativecommons.org/publicdomain/zero/1.0/

set EM_LOG_BITS=32
set EM_LOG_MODE=RELEASE
set EM_LOG_VERBOSE=n
set EM_LOG_CRT=T
set EM_LOG_TARGET=

:loop
if not "%1"=="" (
    if "%1"=="/32" (
        set EM_LOG_BITS=32
        shift
        goto :loop
    )
    if "%1"=="/64" (
        set EM_LOG_BITS=64
        shift
        goto :loop
    )
    if "%1"=="/release" (
        set EM_LOG_MODE=RELEASE
        shift
        goto :loop
    )
    if "%1"=="/rdebug" (
        set EM_LOG_MODE=RDEBUG
        shift
        goto :loop
    )
    if "%1"=="/relwithasserts" (
        set EM_LOG_MODE=RDEBUG
        shift
        goto :loop
    )
    if "%1"=="/debug" (
        set EM_LOG_MODE=DEBUG
        shift
        goto :loop
    )
    if "%1"=="/v" (
        set EM_LOG_VERBOSE=y
        shift
        goto :loop
    )
    if "%1"=="/q" (
        set EM_LOG_VERBOSE=n
        shift
        goto :loop
    )
    if "%1"=="/MT" (
        set EM_LOG_CRT=T
        shift
        goto :loop
    )
    if "%1"=="/MTd" (
        set EM_LOG_CRT=Td
        shift
        goto :loop
    )
    if "%1"=="/MD" (
        set EM_LOG_CRT=D
        shift
        goto :loop
    )
    if "%1"=="/MDd" (
        set EM_LOG_CRT=Dd
        shift
        goto :loop
    )
    if "%1"=="/clean" (
        set EM_LOG_TARGET=clean
        shift
        goto :loop
    )
    
    shift goto :loop
)

if "%EM_LOG_VERBOSE%"=="y" (
    echo Building Em Log for %EM_LOG_BITS% bits in %EM_LOG_MODE% mode
)

mkdir objects\%EM_LOG_MODE% 2> nul
mkdir lib 2> nul

nmake /nologo /f makefile.msvc %EM_LOG_TARGET% CRT=%EM_LOG_CRT% MODE=%EM_LOG_MODE% BITS=%EM_LOG_BITS%