#include "../kernel/info.h"
#include "../lib/stdint.h"



#define nullptr 0

extern uint8_t __KERNEL_START[], __KERNEL_END[];
extern uint8_t __TEXT_START[], __TEXT_END[];
extern uint8_t __RODATA_START[], __RODATA_END[];
extern uint8_t __DATA_START[], __DATA_END[];
extern uint8_t __BSS_START[], __BSS_END[];

#define KERNEL_SIZE ((size_t)__KERNEL_END - (size_t)__KERNEL_START)
#define TEXT_SIZE ((size_t)__TEXT_END - (size_t)(__TEXT_START))
#define RODATA_SIZE ((size_t)__RODATA_END - (size_t)(__RODATA_START))
#define DATA_SIZE ((size_t)__DATA_END - (size_t)(__DATA_START))
#define BSS_SIZE ((size_t)__BSS_END - (size_t)(__BSS_START))

#define MEM_START       ((size_t)(__KERNEL_END + 0x1000))  // 4KiB after kernel end 
#define MEM_SIZE        0x40000000             // 1GiB
#define MEM_TOP         (MEM_START + MEM_SIZE)
#define BLOCK_SIZE      0x400                  // 1 KiB
#define BLOCK_NUMBER    (MEM_SIZE/BLOCK_SIZE)
#define MEM_MAP_LEN     (BLOCK_NUMBER/(sizeof(size_t) * 8))


extern char* mem_pos;

void* is_valid_ptr(*ptr);
void* malloc(size_t bytes);
int8_t memcpy(void* dest, void* src, size_t bytes);
int8_t free(void* ptr);
void* realloc(void* data, size_t bytes);
void* memset(void* ptr, uint8_t value, size_t  count)


void init_mem();