#include "shell.h"
#include "memory.h"
#include "../lib/string.h"
#include "../lib/conversions.h"
#include "../drivers/keyboard.h"
#include "../drivers/video.h"

string in_buffer;

string command;
string* flags;
string* args;

bool initialized = false;


void init_console(){
    if(initialized){
        return;
    }

    in_buffer = new_str_buffer(IN_BUFF_SIZE);
    in_buffer.len = 0;
    initialized = true;
}


void eval_buffer(){
    printch('\n', cursor_position, true);

    printstr(in_buffer, cursor_position, true);
    
    in_buffer.len = 0;
}


bool console_in(){
    if(!initialized){
        return false;
    }

    uint8_t code = get_keyboard_byte();
    
    if(!code){
        return false;
    }


    Key_Info info = get_key_info(code);

    if(info.key == KEY_NULL){
        return false;
    }

    if(info.key == KEY_RETURN){
        eval_buffer();
        return true;
    }

    char ch  = get_char(info);

    if(!ch){
        return false;
    }

    if(ch == '\t'){
        for(uint8_t i = 0; i<4; ++i){
            if(in_buffer.len>=IN_BUFF_SIZE){
                break;
            }

            printch(' ', cursor_position, true);
            in_buffer.data[in_buffer.len] = ' ';
            in_buffer.len++;
        }
    }

    else if(ch == '\b'){
        if(in_buffer.len > 0){
            in_buffer.len--;
            in_buffer.data[in_buffer.len] = 0;

            erase(cursor_position, true);
        }
    }

    else{
        if(in_buffer.len < IN_BUFF_SIZE){
            in_buffer.data[in_buffer.len] = ch;
            in_buffer.len++;
            
            printch(ch, cursor_position, true);
        }
    }   

    return true;
}

bool console_out(){
    return true;
}

void error(int8_t code, string message){

}