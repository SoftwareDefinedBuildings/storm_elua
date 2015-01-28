#ifndef __LIBSTORM_ARRAY_H__
#define __LIBSTORM_ARRAY_H__

#include <stdint.h>

typedef struct
{
    uint8_t type;
    uint8_t reserved;
    uint16_t len;
} __attribute__((packed)) storm_array_t;

enum {
    ARR_TYPE_INT8 = 1,
    ARR_TYPE_UINT8 = 2,
    ARR_TYPE_INT16 = 3,
    ARR_TYPE_UINT16 = 4,
    ARR_TYPE_INT32 = 5
};

#define ARR_START(x) (((uint8_t*)((x)) + sizeof(storm_array_t)))

#endif