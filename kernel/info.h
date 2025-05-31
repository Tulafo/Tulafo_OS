#include "../lib/integer.h"

#define KERNEL_POSITION 0X1000


bool is_in_rodata(const int8_t* ptr);
bool is_in_code(const int8_t* ptr);