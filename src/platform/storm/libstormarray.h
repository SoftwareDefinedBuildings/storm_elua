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

/**
 * This function can be called directly, without using lua_call
 */
int storm_array_nc_create(lua_State *L, int count, int type);
int arr_create(lua_State *L);
int arr_get(lua_State *L);
int arr_set(lua_State *L);
int arr_get_as(lua_State *L);
int arr_get_length(lua_State *L);
int arr_from_str(lua_State *L);
int arr_as_str(lua_State *L);
int arr_get_pstring(lua_State *L);
int arr_set_pstring(lua_State *L);
int arr_set_as(lua_State *L);

#endif