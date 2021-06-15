#pragma once

#include <stdio.h>
#include <stdlib.h>

char* readEntireFile(const char* name) {

    FILE *file = fopen(name, "rb");
    
    if(file == 0) {
        printf("file doesnt' exist\n");
        return 0;
    }

    // Find file size
    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate big enough buffer (+ null-termiator)
    char* buffer = (char*)malloc(fsize + 1);
    fread(buffer, 1, fsize, file);
    buffer[fsize] = 0;

    fclose(file);

    return buffer;
}
