#pragma once

#include "integer.h"

typedef struct{
    unsigned int len;
    char* data;
}string;

size_t len_str(char* str);
void inv_str(char* base, char* top);
size_t find(string* src, char ch, size_t beg = 0);
size_t find(string* src, string* substr, size_t beg = 0);
string new_str(char* data);
string new_str(size_t length);
