#include "string.h"
#include "../kernel/memory.h"

static char *saved_pos_strtok;


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

int8_t strcmp(const char* s1, const char* s2){
    for(size_t i = 0;  ; i++){
        if(s1[i] != s2[i])
            (unsigned char)s1[i] - (unsigned char)s2[i];

        
        if (s1[i] == 0)
            break;
    }
    return 0;
}

bool strtok_find_delim(char ch, const char* delim){
    for(const char* i = delim; *i != 0; i++ ){
        if(ch == *i)
            return true;
    }

    return false;
}


char* strtok(char* str, const char* delim){
    if(str == NULL) str = saved_pos_strtok;

    if(str != NULL){
        for(; strtok_find_delim(*str, delim)==true && *str != 0; str++){
            *str = 0;
        }
        
        if(*str == 0){
            saved_pos_strtok = str;
            return NULL;
        }

        char* i = str;
        for(; strtok_find_delim(*i, delim)==false && *i != 0; i++);

        if(*i == 0){
            char* tmp = saved_pos_strtok+1;
            saved_pos_strtok = i;
            return tmp;
        }

        *i = 0;
        saved_pos_strtok = i + 1;
        return str;
    }

    return NULL;        // Both str and saved_pos_strtok were 0. Bad call
}

char* strcpy(char* dest, const char* src){
    char* ret = dest;
    while(*src) *dest++ = *src++;
    *dest = 0;
    return ret;
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