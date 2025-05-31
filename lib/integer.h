#pragma once

#ifdef __INT8_TYPE__
    typedef __INT8_TYPE__ int8_t;
    typedef __UINT8_TYPE__ uint8_t;
#else
    typedef signed char int8_t;
    typedef unsigned char uint8_t;
#endif

#ifdef __INT16_TYPE__
    typedef __INT16_TYPE__ int16_t;
    typedef __UINT16_TYPE__ uint16_t;
#else
    typedef signed short int16_t;
    typedef unsigned short uint16_t;
#endif

#ifdef __INT32_TYPE__
    typedef __INT32_TYPE__ int32_t;
    typedef __UINT32_TYPE__ uint32_t;
#else
    typedef signed int int32_t;
    typedef unsigned int uint32_t;
#endif

#ifdef __INT64_TYPE__
    typedef __INT64_TYPE__ int64_t;
    typedef __UINT64_TYPE__ uint64_t;
#else
    typedef signed long long int64_t;
    typedef unsigned long long uint64_t;
#endif

#ifdef __SIZE_TYPE__
    typedef __SIZE_TYPE__ size_t;
#endif


#define NULL 0

typedef enum {
    false = 0,
    true = 1
} bool;


