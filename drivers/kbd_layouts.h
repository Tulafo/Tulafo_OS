#include "kbd_layouts.h"
typedef struct {
    char base;          
    char shifted;        
    char alt_gr;        
    char shift_alt_gr;   
} KeyMapping;

static KeyMapping qwerty_us_layout[128] = {
    [KEY_A] = {'a', 'A', 'à', 'À'},
    [KEY_B] = {'b', 'B', 0, 0},
    [KEY_C] = {'c', 'C', 0, 0},
    [KEY_D] = {'d', 'D', 0, 0},
    [KEY_E] = {'e', 'E', 'è', 'È'},
    [KEY_F] = {'f', 'F', 0, 0},
    [KEY_G] = {'g', 'G', 0, 0},
    [KEY_H] = {'h', 'H', 0, 0},
    [KEY_I] = {'i', 'I', 'ì', 'Ì'},
    [KEY_J] = {'j', 'J', 0, 0},
    [KEY_K] = {'k', 'K', 0, 0},
    [KEY_L] = {'l', 'L', 0, 0},
    [KEY_M] = {'m', 'M', 0, 0},
    [KEY_N] = {'n', 'N', 0, 0},
    [KEY_O] = {'o', 'O', 'ò', 'Ò'},
    [KEY_P] = {'p', 'P', 0, 0},
    [KEY_Q] = {'q', 'Q', 0, 0},
    [KEY_R] = {'r', 'R', 0, 0},
    [KEY_S] = {'s', 'S', 0, 0},
    [KEY_T] = {'t', 'T', 0, 0},
    [KEY_U] = {'u', 'U', 0, 0},
    [KEY_V] = {'v', 'V', 0, 0},
    [KEY_W] = {'w', 'W', 0, 0},
    [KEY_X] = {'x', 'X', 0, 0},
    [KEY_Y] = {'y', 'Y', 0, '¥'},
    [KEY_Z] = {'z', 'Z', 0, 0},

    [KEY_D1] = {'1', '!', '¹', '¡'},
    [KEY_D2] = {'2', '@', '²', 0},
    [KEY_D3] = {'3', '#', '³', 0},
    [KEY_D4] = {'4', '$', 0, 0},
    [KEY_D5] = {'5', '%', '€', 0},
    [KEY_D6] = {'6', '^', 0, 0},
    [KEY_D7] = {'7', '&', 0, 0},
    [KEY_D8] = {'8', '*', 0, 0},
    [KEY_D9] = {'9', '(', 0, 0},
    [KEY_D0] = {'0', ')', 0, 0},

    [KEY_MINUS] = {'-', '_', 0, 0},
    [KEY_EQUAL] = {'=', '+', 0, 0},
    [KEY_OSB] = {'[', '{', 0, 0},
    [KEY_CSB] = {']', '}', 0, 0},
    [KEY_SEMICOLON] = {';', ':', 0, 0},
    [KEY_QUOTE] = {'\'', '"', 0, 0},
    [KEY_BACKTICK] = {'`', '~', 0, 0},
    [KEY_BACKSLASH] = {'\\', '|', 0, 0},
    [KEY_COMMA] = {',', '<', 0, '«'},
    [KEY_DOT] = {'.', '>', '·', '»'},
    [KEY_SLASH] = {'/', '?', 0, '¿'},
    [KEY_KP_ASTERISK] = {'*', '*', '*','*'}

};



const char kbd_US [128] =
{
    0,  
    27, /* Escape*/ 
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',   /* 0x02 -> 0x0E */
    '\t', /* Tab */
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',   /* 0x10 -> 0x1C*/  
    0, /* LCtrl */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',        /* 0x1E 0x29*/ 
    0,  /*LShify*/ 
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',             /* 0x2B -> 0x35*/
    0,  /* RShift*/
    '*',            /* 0x37*/
    0,  /* LAlt */
    ' ',  /* Space bar */
    0,  /* Caps lock */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    /* F1 -> F10*/
    0,  /* Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
    '-',    /* 0x4A */
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
    '+',    /* 0x4E */
    0,  /* End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    /* All other keys are undefined */
};
const char kbd_US_shift [128] =
{
    0,  
    27, /* Escape */ 
    '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',   /* 0x02 -> 0x0E */
    '\t', /*Tab */
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',   /* 0x10 -> 0x1C*/
    0, /*Ctrl */
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',         /* 0x1E 0x29*/  
    0, /* Lshift */
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',              /* 0x2B -> 0x35*/ 
    0, /* Rshift*/ 
    '*',            /* 0x37*/
    0,  /* Alt */
    ' ',  /* Space bar */
    0,  /* Caps lock */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /* F1 -> F10 */ /* 0x3b -> 0x44*/
    0,  /* Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
    '-',    /* 0x4A */
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
    '+',    /* 0x4E */
    0,  /* End */
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    /* All other keys are undefined */
};