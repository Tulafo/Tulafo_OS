#include "conversions.h"
#include "string.h"
#include "math.h"
#include "../kernel/memory.h"


//Converts a number to decimal C-style string
char* to_cstr(long int in_num){
    char* out_buff = (char*)malloc(12);
    char* ptr = out_buff;

    do{
        char remainder = in_num%10;
        in_num = in_num/10;

        (*ptr) = remainder | 0b00110000;
        ptr++;
    } while(in_num != 0);

    (*ptr) = 0;

    //Inverting string
    char* end = --ptr;
    char* start = out_buff;
    inv_str(start, end);


    return out_buff;
}

//Converts a number to hexadecimal C-style string
char* to_cstr_h(long int in_num){
    //10 bytes
    char* out_buff = (char*)malloc(12);
    char* ptr = out_buff;
    
    *ptr = '0';
    ptr++;
    *ptr = 'x';
    ptr++;

    while(in_num != 0){
        char remainder = in_num%16;
        in_num = in_num/16;

        remainder = remainder | 0b00110000;

        if(remainder > '9'){
            remainder+=('A' - '9' - 1);
        }

        (*ptr) = remainder;

        ptr++;
    }

    (*ptr) = 0;

    //Inverting string
    char* end = --ptr;
    char* start = out_buff+2;
    inv_str(start, end);


    return out_buff;

}


string to_str(long int in_num){
    //12 bytes
    char out_buff [12];
    char* ptr = out_buff;

    while(in_num != 0){
        char remainder = in_num%10;
        in_num = in_num/10;

        (*ptr) = remainder | 0b00110000;
        ptr++;
    }

    (*ptr) = 0;

    //Inverting string
    char* end = --ptr;
    char* start = out_buff;
    inv_str(start, end);

    string out_str = new_str(out_buff);


    return out_str;
}

string to_str_h(long int in_num){
    //10 bytes
    char out_buff [10];
    char* ptr = out_buff;

    while(in_num != 0){
        char remainder = in_num%16;
        in_num = in_num/16;

        remainder = remainder | 0b00110000;

        if(remainder > '9'){
            remainder+=('A' - '9' - 1);
        }

        (*ptr) = remainder;

        ptr++;
    }

    (*ptr) = 0;

    //Inverting string
    char* end = --ptr;
    char* start = out_buff;
    inv_str(start, end);

    string out_str = new_str(out_buff);
    return out_str;
}



long int to_int(char* in_str){
    int len = len_str(in_str);

    char* data = in_str;

    long int res = 0;

    for(int i = len - 1; i>=0; i--){
        long int power = pow(10, i);

        char digit = (*data) - '0';
        res += (digit*power);
        data++;
    }

    return res;
}

long int to_int(string in_str){
    char* data = in_str.data;

    long int res = 0;

    for(int i = in_str.len - 1; i>=0; i--){
        long int power = pow(10, i);
        
        char digit = (*data) - '0';
        res += (digit*power);
        data++;
    }

    return res;
}
