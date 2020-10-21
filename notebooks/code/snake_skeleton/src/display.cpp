#include "stdlib.h"
#include <iostream>

const char* cmd_clear="clear";

void backgroundClear(){
    int out = system( cmd_clear);
    if( out != 0){
        std::cerr << "clear command failed" << std::endl;
        exit(1);
    }
}

void printFrame(const int& nx, const int& ny, const int* bg){
    for( int j=0; j<ny; j++){
        for( int i=0; i<nx; i++){
            if( bg[i+j*nx] == 1 ){
                std::cout << "#";
            }
            else if( bg[i+j*nx] == 2 ){
                std::cout << "@" ; 
            }    
            else{
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
}
