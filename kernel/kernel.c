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


void init(){
    init_mem();
    init_video();
    init_console();
}


extern int main(){
    init();

    uint16_t code = 0x00;
    uint16_t pre_code = 0x00;


    while(1){
        code = get_keyboard_code();
        if(code != pre_code){
            clear_screen();
            hex_print(code, cursor_position, true); 
            cursor_position = 0;
        }

    }


    while(1);
    return 1;
}





