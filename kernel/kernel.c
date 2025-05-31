#include "io.h"
#include "memory.h"
#include "shell.h"
#include "../lib/integer.h"
#include "../lib/string.h"
#include "../lib/math.h"
#include "../lib/conversions.h"


void init(){
    init_mem();
    init_io();
    init_console();
}


extern int main(){
    init();

    while(1){
        console_in();
    }

    while(1);
    return 1;
}





