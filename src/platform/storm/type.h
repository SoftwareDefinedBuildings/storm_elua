#ifndef __TYPE_H__
#define __TYPE_H__

typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef unsigned int   BOOL;

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned long u32;
typedef signed long s32;
typedef unsigned long long u64;
typedef signed long long s64;

#endif
#if 0
lua build_elua.lua   \
    board=storm   \
    target=lua       \
    allocator=newlib   \
    toolchain=codesourcery   \
    optram=true  \
    boot=standard  \
    romfs=compile  \
    cpumode=thumb  \
    bootloader=none  \
    output_dir=bprod  \
    romfs_dir=romfs  \
    skip_conf=false  \
    config_only=false  \
    build_mode=build_dir_linearized  \
    build_dir=build \
    disp_mode=all
#endif