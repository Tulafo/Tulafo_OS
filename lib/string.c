#include "string.h"
#include "../kernel/memory.h"



size_t len_str(char* str){
    unsigned int len = 0;

    while(str[++len] != 0){
        len++;
    }

    return len-1;
}

void inv_str(char* base, char* top){
    while(base < top){
        char tmp = (*top);
        (*top) = (*base);
        (*base) = tmp;

        base++;
        top--;
    }
}


string new_str(char* data){
    unsigned int len = len_str(data);

    string out_str;

    out_str.data = (char*)malloc(len);
    out_str.len = len;

    memcpy(out_str.data, data, len);

    return out_str;
}

string new_str_buffer(size_t len){
    string out_str;

    out_str.data = (char*)malloc(len);
    out_str.len = len;

    char* str_top = out_str.data + len;
    for(char* i = out_str.data; i<str_top; i++){
        (*i) = 0;
    }

    return out_str;
}

size_t find_char(string* src, char ch, size_t beg){
    for(size_t i = beg; i < src->len; i++)
        if(src->data[i] = ch)
            return i+1;

    return 0;
}

size_t find_substr(string* src, string* substr, size_t beg){
    bool found = false;

    for(size_t i = beg; i < src->len; i++){
        if(src->data[i] = substr->data[0]){
            for (size_t j = 1; j < substr->len; j++){
                if (src->data[i+j] != substr->data[j]){
                    found = false;
                    break;
                }

                found = true;
            }
        }

        if(found){
            return i+1;
        }
    }

    return 0;
}