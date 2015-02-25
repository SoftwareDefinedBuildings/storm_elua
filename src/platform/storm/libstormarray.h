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
    ARR_TYPE_INT32 = 5,

    GS_TYPE_INT16_BE = 6,
    GS_TYPE_UINT16_BE = 7,
    GS_TYPE_INT32_BE = 8
};

#define ARR_START(x) (((uint8_t*)((x)) + sizeof(storm_array_t)))
int storm_array_nc_create(lua_State *L, int count, int type);

int array_get(storm_array_t *arr, int idx);
int array_set(storm_array_t *arr, int idx, int val);
int array_get_length(storm_array_t *arr);

int arr_as_str(lua_State *L);
int arr_get_pstring(lua_State *L);
int arr_set_pstring(lua_State *L);

#endif
