#include "memory.h"
#include "shell.h"
#include "../drivers/video.h"
#include "../drivers/keyboard.h"
#include "../drivers/kbd_layouts.h"
#include "../lib/stdint.h"
#include "../lib/stdbool.h"
#include "../lib/string.h"
#include "../lib/math.h"
#include "../lib/conversions.h"
#include "fat32.h"


void init(){
    init_mem();
    init_video();
    init_console();
}


extern int main(){
    init();

    
    return 1;
}





