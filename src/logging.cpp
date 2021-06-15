#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>


FILE *logFile = nullptr;
bool verboseMode = false;
bool output = true;


// Outputs error into logFile and stderr even if verboseMode is disabled & stops the compiler
void abortf(const char* str, ...) {
    if(!output) return;
    
    va_list argptr;
    va_start(argptr, str);

    vfprintf(stderr, str, argptr);
    
    if(logFile != nullptr) {
        vfprintf(logFile, str, argptr);
        fflush(logFile);
    }

    fflush(stderr);
    
    va_end(argptr);
}