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

    char test[] = ",,Hallo; mein..freund\0";
    char delim[] = ",; .\0";


    printf("%s %i", test, 15);
    putchar('\n');
    printf("%s %x", test, 0x0F);

    while(1);

    char* token = strtok(test, delim);
    printf("%s", token);
    putchar('\n');

    do{
        token = strtok(NULL, delim);
        print_string(token, cursor_position, true);
        print_string("\n\0", cursor_position, true);
    }while(token != NULL);

    while(1);
    return 1;
}





