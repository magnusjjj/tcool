#ifndef COMMON_H
#define COMMON_H
#include "printf.h"

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long int uint64_t;

extern void print(const char*);
extern void print_specific(const char* c, int size);
extern int cmp(char *a, char *b, int length);
extern void print_dec(uint32_t n);
extern void memset(void *, uint8_t, uint32_t);
extern void clear_screen();

static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ __volatile__ ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ __volatile__ ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

static inline void cpuGetMSR(uint32_t msr, uint32_t *lo, uint32_t *hi)
{
   __asm__ __volatile__("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}
 
static inline void cpuSetMSR(uint32_t msr, uint32_t lo, uint32_t hi)
{
   __asm__ volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
}

extern void memcpy(void *p1, void *p2, uint32_t len);
extern void setup_printf();
#endif
