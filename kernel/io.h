#pragma once

#include "../lib/integer.h"
#include "../lib/string.h"

#define VIDEO_MEMORY_START 0xB8000
#define SCREEN_HEIGHT 25
#define SCREEN_LENGTH 80
#define SCREEN_SIZE (SCREEN_HEIGHT*SCREEN_LENGTH)
#define VIDEO_MEMORY_SIZE (2*SCREEN_SIZE)
#define VIDEO_MEMORY_LIMIT (VIDEO_MEMORY_START + VIDEO_MEMORY_SIZE) 

#define CRT_INDEX_REG 0x3D4
#define CRT_DATA_REG  0x3D5
#define CURSOR_LOW_BYTE 0x0F
#define CURSOR_HIGH_BYTE 0x0E

extern uint16_t cursor_position;
extern uint8_t video_mode;




enum Video_Modes {
    WHITE_BLACK     =   0b00001111,
    LIGHTGRAY_BLAK  =   0b00000111,
    WHITE_BLUE      =   0b00011111,
    LIGHTGRAY_BLUE  =   0b00010111,

};  

typedef struct  {
    uint8_t x;
    uint8_t y;
    bool invalid;

} Screen_Coordinates;

uint8_t inb(uint16_t port);
void outb(uint16_t port, int8_t value);


void printf(char* in_str, uint16_t start_location, bool update_cursor);
void printstr(string in_str, uint16_t start_location, bool update_cursor);
void printch(char character, uint16_t position, bool update_cursor);
void hex_print(size_t number, uint16_t position, bool update_cursor);
void clear_screen();
void erase(uint16_t pos, bool update_cursor);
uint16_t get_screen_linear_position(Screen_Coordinates coords);
Screen_Coordinates get_screen_coordinates(uint16_t linear);
uint16_t get_screen_linear_position_raw(uint8_t x, uint8_t y);

void init_io();