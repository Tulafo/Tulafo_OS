#include "io.h"
#include "../lib/conversions.h"
#include "../lib/math.h"
#include "../lib/string.h"
#include "../lib/kbd_layouts.h"


uint16_t cursor_position = 0;
uint8_t video_mode = LIGHTGRAY_BLAK;

bool lshift = false;
bool rshift = false;
bool lctrl = false;
bool lalt = false;


uint8_t inb(uint16_t port) {
    uint8_t result;
    asm volatile ("in %1, %0" : "=a"(result) : "d"(port));
    return result;
}
void outb(uint16_t port, int8_t value) {
    asm volatile ("out %0, %1" : : "a"(value), "Nd"(port));
}

void move_cursor(uint32_t offset){
    outb(CRT_INDEX_REG, CURSOR_LOW_BYTE);
    outb(CRT_DATA_REG, offset & 0x00FF);
    outb(CRT_INDEX_REG, CURSOR_HIGH_BYTE);
    outb(CRT_DATA_REG, (offset >> 8) & 0xFF);

}
void move_cursor(Screen_Coordinates coords){
    long offset = get_screen_linear_position(coords);

    if(offset == -1)
        return

    move_cursor(offset);
}
void update_cursor_pos(){
    move_cursor(cursor_position);
}

void new_line(){
    uint16_t line = cursor_position/SCREEN_LENGTH +1;

    cursor_position = line * SCREEN_LENGTH;
}

void printf(char* in_str, uint16_t start_location, bool update_cursor){
    if (update_cursor){
        cursor_position = start_location;
        
        for(uint16_t i = 0; in_str[i] != 0 && cursor_position <= SCREEN_SIZE; i++)
        {
            char ch = in_str[i];
            int8_t* addr = (int8_t*)(2*cursor_position + VIDEO_MEMORY_START);

            if(ch == '\n'){
                new_line();
                continue;
            }

            *addr = ch;
            *(addr+1) = video_mode;

            cursor_position++;
        }

        update_cursor_pos();
    }

    else{
        int8_t* addr = (int8_t*)(2*start_location + VIDEO_MEMORY_START);

        for(size_t i = 0; in_str[i] != 0; i++)
        {
            char ch = in_str[i];    
            if(ch == '\n'){
                uint16_t line = ((size_t)addr - VIDEO_MEMORY_START) / 2/SCREEN_HEIGHT + 1;
                addr = (int8_t*)(VIDEO_MEMORY_START + line*SCREEN_LENGTH*2);
                continue;
            }

            *addr = ch;
            *(addr+1) = video_mode;

            addr+=2;
        }
    }
    
}

void printstr(string in_str, uint16_t start_location, bool update_cursor){
    char* data = in_str.data;

    if (update_cursor){
        cursor_position = start_location;
        
        for(uint16_t i = 0; i < in_str.len && cursor_position <= SCREEN_SIZE; i++)
        {
            char ch = data[i];
            uint8_t* addr = (uint8_t*)(2*cursor_position + VIDEO_MEMORY_START);

            if(ch == '\n'){
                new_line();
                continue;
            }

            *addr = ch;
            *(addr+1) = video_mode;

            cursor_position++;
        }

        update_cursor_pos();
    }

    else{
        uint8_t* addr = (uint8_t*)(2*start_location + VIDEO_MEMORY_START);

        for(size_t i = 0; i < in_str.len; i++){
            char ch = data[i];

            if(ch == '\n'){
                uint16_t line = ((size_t)addr - VIDEO_MEMORY_START) / 2/SCREEN_HEIGHT + 1;
                addr = (uint8_t*)(VIDEO_MEMORY_START + line*SCREEN_LENGTH*2);
                continue;
            }

            *addr = ch;
            *(addr+1) = video_mode;

            addr+=2;
        }
    }
}

void printch(char character, uint16_t position, bool update_cursor){

    if(character == '\n' && update_cursor){
        new_line();
        update_cursor_pos();
        return;
    }

    else if(character == 0){
        return;
    }

    uint32_t addr = VIDEO_MEMORY_START + position*2;
    *(volatile int8_t*)addr = character;
    *(volatile int8_t*)(addr+1) = video_mode;

    if(update_cursor){
        ++cursor_position;
        update_cursor_pos();
    }
}

void hex_print(size_t number, uint16_t position, bool update_cursor){
    printf(to_cstr_h(number), position, update_cursor);
}

void erase(uint16_t pos, bool update_cursor){
    printch(' ', pos, false);

    if(update_cursor){
        cursor_position = pos;
        update_cursor_pos();
    }
}

void clear_screen(){
    volatile int8_t* vidmem = (volatile int8_t*) VIDEO_MEMORY_START;
    for(uint16_t i = 0; i<2*SCREEN_SIZE; i += 2){
        vidmem[i] = ' ';
        vidmem[i+1] = video_mode;
    }
}

uint16_t get_screen_linear_position(Screen_Coordinates coords){
    uint8_t x = coords.x;
    uint8_t y = coords.y;

    if(x >= SCREEN_LENGTH || y >= SCREEN_HEIGHT)
        return -1;

    return y*(uint16_t)SCREEN_LENGTH + x;
}

uint16_t get_screen_linear_position(uint8_t x, uint8_t y){
    if(x >= SCREEN_LENGTH || y >= SCREEN_HEIGHT)
        return - 1;

    return y*SCREEN_LENGTH + x;
}

Screen_Coordinates get_screen_coordinates(uint16_t linear){
    Screen_Coordinates pos;

    if(linear > SCREEN_SIZE)
        pos.invalid = true;
        return pos;

    pos.x = linear % SCREEN_LENGTH;
    pos.y = linear / SCREEN_LENGTH;
    pos.invalid = false;
    return pos;
}









void init_io(){
    video_mode = LIGHTGRAY_BLAK;
    cursor_position = 0;
    update_cursor_pos();
}