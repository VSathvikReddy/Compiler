#include <stdio.h>
#include <stdlib.h>

#include "file_handle.h"

int main(int argc, char* argv[]){
    Source file = file_open(argc,argv);
    

    free((void*)file.data); //This is the only place file.data owns the data to free ir
    return 0;
}