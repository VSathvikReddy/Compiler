#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "file_handle.h"

Source file_open(int argc, char* argv[]){
    if(argc<=1){
        fprintf(stderr, "No arguments provided\n");
        exit(EXIT_FAILURE);
    }
    FILE* fptr = fopen(argv[1], "r"); 
    if(!fptr){
        fprintf(stderr, "Unable to open %s: %s\n",argv[1], strerror(errno));
        exit(EXIT_FAILURE);
    }


    fseek(fptr, 0, SEEK_END);
    size_t size = ftell(fptr);
    rewind(fptr);

    if(size<=0) {
        fprintf(stderr, "Empty or invalid file\n");
        fclose(fptr);
        exit(EXIT_FAILURE);
    }
    unsigned char *buffer = malloc(size);
    if(!buffer){
        perror("malloc");
        fclose(fptr);
        exit(EXIT_FAILURE);
    }
    size_t read = fread(buffer, 1, size, fptr);
    fclose(fptr);
    if(read!=size){
        perror("fread");
    }
    Source out = {buffer,size,0,1,1};
    return out;
}