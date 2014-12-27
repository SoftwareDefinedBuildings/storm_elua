#include "interface.h"
#include <stdint.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//------------------------------
// LIBC SUPPORT STUBS
//------------------------------

extern void* __ram_end__;
void* _sbrk(uint32_t increment)
{
    extern uint8_t *end;
    static uint8_t *heap_end = 0;
    uint8_t *prev_heap_end;
    if (heap_end == 0)
    {
        heap_end =(uint8_t*) &end;
    }
    prev_heap_end = heap_end;
    heap_end += increment;
    return prev_heap_end;
}
int _isatty(int fd)
{
    return 1;
    if (fd == 0)
    {
        return 1;
    }
    return 0;
}
int _write(int fd, const void *buf, uint32_t count)
{
    k_write(fd, buf, count);
    return count;
}
int _close(int fd)
{
    return -1;
}
int _fstat(int fd, struct stat *buf)
{
    return -1;
}
int _lseek(int fd, uint32_t offset, int whence)
{
    return -1;
}
int _read(int fd, void *buf, uint32_t count)
{
    return k_read(fd, (uint8_t*) buf, count);
    /*
 uint32_t got = 0;
    uint32_t tmp;
    int i;
    while(got < count) {
        tmp = k_read(fd, &((uint8_t*)buf)[got], count - got);
        if (tmp >= 0) {
            got += tmp;
        } else {
            return tmp;
        }
        if (tmp == 0 && got > 0) break;
    }
    return got;
    */
}

void _fini()
{
}

//------------------------------
// _start and _start2 entrypoints
//------------------------------

extern uint32_t _sfixed;
extern uint32_t _efixed;
extern uint32_t _etext;
extern uint32_t _srelocate;
extern uint32_t _erelocate;
extern uint32_t _szero;
extern uint32_t _ezero;
extern uint32_t _sstack;
extern uint32_t _estack;
extern void (*_init)();
extern void main();

void __attribute__(( naked , used )) _start()
{
    asm volatile("ldr r0, =_start2\n\t"
                 "ldr r1, =_estack\n\t"
                 "bx lr" : : : "r0", "r1" );
}

/* This function is not intended to return! */
void __attribute__((used)) _start2()
{
    uint8_t* ptr;

    //asm volatile(" LDR sp, =_estack");
    uint32_t *pSrc, *pDest;

    /* Move the relocate segment */
	pSrc = &_etext;
	pDest = &_srelocate;
	if (pSrc != pDest) for (; pDest < &_erelocate;) *pDest++ = *pSrc++;

	/* Clear the zero segment */
	for (pDest = &_szero; pDest < &_ezero;) *pDest++ = 0;

    #if 0
    //This is a libc sanity check, many problems with malloc/stacks
    //and weak symbol overrides are caught by this
    printf("==[TESTSUITE_STARTING]==\n");
    {
        int x = 5;
        int y = -5;
        float z = 2.34;

        printf("Test printf\n");
        printf(" > %d, %d, %f\n", x, y, z);
    }
    {
        printf("Test malloc\n");
        ptr = malloc(512);
        printf(" > 0x%08x\n", (uint32_t)ptr);
        if (ptr == 0)
        {
            printf("Malloc fail\n");
            while(1);
        }
    }
    {
        volatile unsigned int x = 5;
        volatile int y = -5;
        volatile float z = 2.34;
        volatile char *s = "hello world";
        printf("Test snprintf\n");
        snprintf(ptr, 500, "%u %d %.2f '%s'\n", x, y, z, s);
        printf(" > %s\n", ptr);
    }
    {
        volatile unsigned int x = 5;
        volatile int y = -5;
        volatile float z = 2.34;
        volatile char *s = "hello world";
        char stackbuf[512];
        printf("Test stack snprintf\n");
        snprintf(stackbuf, 500, "%u %d %.2f '%s'\n", x, y, z, s);
        printf(" > %s\n", stackbuf);
    }
    {
        char* rv;
        printf("Test gets\n");
        rv = gets(ptr);
        printf(" > 0x%08x '%s'\n", (uint32_t) rv, rv);
    }
    {
        char* rv;
        printf("Test gets2\n");
        rv = gets(ptr);
        printf(" > 0x%08x '%s'\n", (uint32_t) rv, rv);
    }
    {
        char stackbuf[512];
        char* rv;
        printf("Test stack gets\n");
        rv = gets(stackbuf);
        printf(" > 0x%08x '%s'\n", (uint32_t) rv, rv);
    }
    printf("==[TESTSUITE COMPLETE]==\n");
    #endif

    main();

    // It's not technically the end of the world if main exits
    // we would still execute any callbacks that it asked for
    // don't _fini here obviously.
    return;
}

//------------------------------
// Kernel ABI functions
//------------------------------

uint32_t __attribute__((naked)) k_get_kernel_version()
{
    __syscall_body(ABI_ID_GET_KERNEL_VERSION);
}
int32_t __attribute__((naked)) k_write(uint32_t fd, uint8_t const *src, uint32_t size)
{
    __syscall_body(ABI_ID_WRITE);
}
void __attribute__((naked)) k_yield()
{
    __syscall_body(ABI_ID_YIELD);
}
int32_t __attribute__((naked)) k_read(uint32_t fd, uint8_t *dst, uint32_t size)
{
    __syscall_body(ABI_ID_READ);
}
int32_t __attribute__((naked)) k_read_async(uint32_t fd, uint8_t *dst, uint32_t size, cb_i32_t cb, void* r)
{
    __syscall_body(ABI_ID_READ_ASYNC);
}
uint8_t __attribute__((naked)) k_run_callback()
{
    __syscall_body(ABI_ID_RUN_CALLBACK);
}
void __attribute__((naked)) k_wait_callback()
{
    __syscall_body(ABI_ID_WAIT_CALLBACK);
}