#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "platform.h"
#include "lrotable.h"
#include "platform_conf.h"
#include "auxmods.h"
#include <string.h>
#include <stdint.h>
#include <interface.h>
#include <stdlib.h>
#include "libstormarray.h"

#if LUA_OPTIMIZE_MEMORY == 0
#error libstorm can only be compiled with LTR on (optram=true)
#endif

#ifndef LUA_NUMBER_INTEGRAL
#error need integral
#endif

#define ARRAY_SANE_SIZE 4000

const uint8_t arr_sizemap [] = {0, 1, 1, 2, 2, 4};
const uint8_t arr_shiftmap [] = {0, 0, 0, 1, 1, 2};

// Module function map
#define MIN_OPT_LEVEL 2
#include "lrodefs.h"
static int arr_create(lua_State *L);
static int arr_set_type(lua_State *L);
static int arr_get_type(lua_State *L);
static int arr_get(lua_State *L);
static int arr_set(lua_State *L);
static int arr_get_as(lua_State *L);
static int arr_set_as(lua_State *L);
static int arr_get_length(lua_State *L);
static int arr_sum(lua_State *L);
static int arr_append(lua_State *L);
static const LUA_REG_TYPE array_meta_map[] =
{
 //   { LSTRKEY( "set_type" ), LFUNCVAL ( arr_set_type ) },
  //  { LSTRKEY( "get_type" ), LFUNCVAL ( arr_get_type ) },
    { LSTRKEY( "get" ), LFUNCVAL ( arr_get ) },
    { LSTRKEY( "set" ), LFUNCVAL ( arr_set ) },
    { LSTRKEY( "get_as" ), LFUNCVAL ( arr_get_as ) },
    { LSTRKEY( "set_as" ), LFUNCVAL ( arr_set_as ) },
    { LSTRKEY( "sum" ), LFUNCVAL ( arr_sum) },
    { LSTRKEY( "append" ), LFUNCVAL ( arr_append) },
    { LSTRKEY( "__len" ), LFUNCVAL ( arr_get_length ) },
    { LSTRKEY( "__index" ), LROVAL ( array_meta_map ) },
    { LNILKEY, LNILVAL }
};

int storm_array_nc_create(lua_State *L, int count, int type)
{
    storm_array_t *arr = lua_newuserdata(L, sizeof(storm_array_t) + count*arr_sizemap[type]);
    memset(ARR_START(arr), 0, count*arr_sizemap[type]);
    arr->type = type;
    arr->len = count*arr_sizemap[type];
    lua_pushrotable(L, (void*)array_meta_map);
    lua_setmetatable(L, -2);
    return 1;
}
//lua storm.array.create(count, default_element_size)
static int arr_create(lua_State *L)
{
    uint32_t size = luaL_checkinteger(L, 1);
    uint32_t type = ARR_TYPE_INT32;
    if (lua_gettop(L) >= 2)
    {
        type = luaL_checkinteger(L, 2);
        if (type > ARR_TYPE_INT32)
        {
            return luaL_error(L, "invalid array type");
        }
    }
    if (size*arr_sizemap[type] > ARRAY_SANE_SIZE)
    {
        return luaL_error(L, "insane array size");
    }
    storm_array_t *arr = lua_newuserdata(L, sizeof(storm_array_t) + size*arr_sizemap[type]);
    memset(ARR_START(arr), 0, size*arr_sizemap[type]);
    arr->type = type;
    arr->len = size*arr_sizemap[type];
    arr->end = 0;
    lua_pushrotable(L, (void*)array_meta_map);
    lua_setmetatable(L, -2);
    return 1;
}
//lua array:get(idx)
static int arr_get(lua_State *L)
{
    int idx;
    uint16_t count;
    storm_array_t *arr = lua_touserdata(L, 1);
    idx = luaL_checkinteger(L, 2);
    if (!arr)
    {
        return luaL_error(L, "invalid array");
    }
    count = arr->len >> arr_shiftmap[arr->type];
    if (idx > count || idx == 0)
    {
        return luaL_error(L, "out of bounds");
    }
    idx--;
    switch(arr->type)
    {
        case ARR_TYPE_INT8:
            lua_pushnumber(L, ((int8_t*)ARR_START(arr))[idx]);
            break;
        case ARR_TYPE_UINT8:
            lua_pushnumber(L, ((uint8_t*)ARR_START(arr))[idx]);
            break;
        case ARR_TYPE_INT16:
            lua_pushnumber(L, ((int16_t*)ARR_START(arr))[idx]);
            break;
        case ARR_TYPE_UINT16:
            lua_pushnumber(L, ((uint16_t*)ARR_START(arr))[idx]);
            break;
        case ARR_TYPE_INT32:
            lua_pushnumber(L, ((int32_t*)ARR_START(arr))[idx]);
            break;
        default:
            return luaL_error(L, "bad array type");
    }
    return 1;
}

static int arr_sum(lua_State *L)
{
  storm_array_t *arr = lua_touserdata(L, 1);
  int len  = arr->len >> arr_shiftmap[arr->type];
  int sum = 0, i;
  void *a = ARR_START(arr);
#define SUM(type) for (i = 0; i < len; i++){sum = sum + ((type*)a)[i];} break;
  switch(arr->type){
  case ARR_TYPE_INT8:   SUM(int8_t)
  case ARR_TYPE_UINT8:  SUM(uint8_t)
  case ARR_TYPE_INT16:  SUM(int16_t)
  case ARR_TYPE_UINT16: SUM(uint16_t)
  case ARR_TYPE_INT32:  SUM(int32_t)
  default: return luaL_error(L, "bad array type");
  }
#undef SUM
  lua_pushnumber(L, sum);
  return 1;
}

static int arr_set_index(storm_array_t *arr, int idx, int val){
  if (idx >= arr->end){
    arr->end = idx + 1;
  }
  switch (arr->type){
  case ARR_TYPE_INT8:
    ((int8_t*)ARR_START(arr))[idx] = val;
    break;
  case ARR_TYPE_UINT8:
    ((uint8_t*)ARR_START(arr))[idx] = val;
    break;
  case ARR_TYPE_INT16:
    ((int16_t*)ARR_START(arr))[idx] = val;
    break;
  case ARR_TYPE_UINT16:
    ((uint16_t*)ARR_START(arr))[idx] = val;
    break;
  case ARR_TYPE_INT32:
    ((int32_t*)ARR_START(arr))[idx] = val;
    break;
  default:
    return 1;
  }
  return 0;
}

//lua array:set(idx, val)
static int arr_set(lua_State *L)
{
    int idx;
    uint16_t count;
    storm_array_t *arr = lua_touserdata(L, 1);
    idx = luaL_checkinteger(L, 2);
    int val = luaL_checknumber(L, 3);
    if (!arr)
    {
        return luaL_error(L, "invalid array");
    }
    count = arr->len >> arr_shiftmap[arr->type];
    if (idx > count || idx == 0)
    {
        return luaL_error(L, "out of bounds");
    }
    idx--;
    if (arr_set_index(arr, idx, val)){
      luaL_error(L, "bad array type");
    }
    return 0;
}

static int arr_append(lua_State *L){
  storm_array_t *arr = lua_touserdata(L, 1);
  int idx = arr->end;
  int val = luaL_checknumber(L, 2);
  if (!arr){
      return luaL_error(L, "invalid array");
  }
  if (idx + 1 > (arr->len >> arr_shiftmap[arr->type])){
      return luaL_error(L, "out of bounds");
  }
  if (arr_set_index(arr, idx, val)){
    luaL_error(L, "bad array type");
  }
  return 0;
}

//lua array:get_as(type, byte_idx)
static int arr_get_as(lua_State *L)
{
    int idx;
    uint8_t* ptr;
    int type;
    uint8_t rv [] __attribute__((aligned(4))) = {0,0,0,0};
    storm_array_t *arr = lua_touserdata(L, 1);
    idx = luaL_checkinteger(L, 3);
    type = luaL_checkinteger(L, 2);
    if (!arr)
    {
        return luaL_error(L, "invalid array");
    }
    if (idx >= arr->len)
    {
        return luaL_error(L, "out of bounds");
    }
    ptr = ARR_START(arr) + idx;
    switch(type)
    {
        case GS_TYPE_INT16_BE:
        case GS_TYPE_UINT16_BE:
            rv[0] = ptr[1];
            rv[1] = ptr[0];
            break;
        case GS_TYPE_INT32_BE:
            rv[0] = ptr[3];
            rv[1] = ptr[2];
            rv[2] = ptr[1];
            rv[3] = ptr[0];
            break;
        default: //little endian
            rv[0] = ptr[0];
            rv[1] = ptr[1];
            rv[2] = ptr[2];
            rv[3] = ptr[3];
    }
    switch(type)
    {
        case ARR_TYPE_INT8:
            lua_pushnumber(L, *((int8_t*) &rv[0]));
            break;
        case ARR_TYPE_UINT8:
            lua_pushnumber(L, *((uint8_t*) &rv[0]));
            break;
        case ARR_TYPE_INT16:
        case GS_TYPE_INT16_BE:
            lua_pushnumber(L, *((int16_t*) &rv[0]));
            break;
        case ARR_TYPE_UINT16:
        case GS_TYPE_UINT16_BE:
            lua_pushnumber(L, *((uint16_t*) &rv[0]));
            break;
        case ARR_TYPE_INT32:
        case GS_TYPE_INT32_BE:
            lua_pushnumber(L, *((int32_t*) &rv[0]));
            break;
        default:
            return luaL_error(L, "bad array type");
    }
    return 1;
}

//lua #array
static int arr_get_length(lua_State *L)
{
    int count;
    storm_array_t *arr = lua_touserdata(L, 1);
    count = arr->len >> arr_shiftmap[arr->type];
    lua_pushnumber(L, count);
    return 1;
}

//lua array:set_as(type, byte_idx, val)
static int arr_set_as(lua_State *L)
{
    int idx;
    uint8_t* srcptr;
    uint8_t* dstptr;
    int val;
    storm_array_t *arr = lua_touserdata(L, 1);
    idx = luaL_checkinteger(L, 2);
    val = luaL_checknumber(L, 3);
    srcptr = (uint8_t*) (&val);
    if (!arr)
    {
        return luaL_error(L, "invalid array");
    }
    if (idx >= arr->len)
    {
        return luaL_error(L, "out of bounds");
    }
    dstptr = ((uint8_t*)arr) + sizeof(storm_array_t) + idx;

    switch(arr->type)
    {
        case ARR_TYPE_INT32:
            dstptr[3] = srcptr[3];
            dstptr[2] = srcptr[2];
        case ARR_TYPE_UINT16:
        case ARR_TYPE_INT16:
            dstptr[1] = srcptr[1];
        case ARR_TYPE_INT8:
        case ARR_TYPE_UINT8:
            dstptr[0] = srcptr[0];
            break;
        case GS_TYPE_INT16_BE:
        case GS_TYPE_UINT16_BE:
            dstptr[1] = srcptr[0];
            dstptr[0] = srcptr[1];
            break;
        case GS_TYPE_INT32_BE:
            dstptr[3] = srcptr[0];
            dstptr[2] = srcptr[1];
            dstptr[1] = srcptr[2];
            dstptr[0] = srcptr[3];
            break;
        default:
            return luaL_error(L, "bad array type");
    }
    return 0;
}


const LUA_REG_TYPE libstorm_array_map[] =
{
    { LSTRKEY( "create" ), LFUNCVAL ( arr_create ) },
    { LSTRKEY( "INT8" ),  LNUMVAL ( ARR_TYPE_INT8 ) },
    { LSTRKEY( "UINT8" ), LNUMVAL ( ARR_TYPE_UINT8 ) },
    { LSTRKEY( "INT16" ), LNUMVAL ( ARR_TYPE_INT16 ) },
    { LSTRKEY( "UINT16" ),LNUMVAL ( ARR_TYPE_UINT16 ) },
    { LSTRKEY( "INT32" ), LNUMVAL ( ARR_TYPE_INT32 ) },
    { LSTRKEY( "INT16_BE" ), LNUMVAL ( GS_TYPE_INT16_BE ) },
    { LSTRKEY( "UINT16_BE" ), LNUMVAL ( GS_TYPE_UINT16_BE ) },
    { LSTRKEY( "INT32_BE" ), LNUMVAL ( GS_TYPE_INT32_BE ) },
    { LSTRKEY( "_arrmeta" ), LROVAL ( array_meta_map ) },
    { LNILKEY, LNILVAL }
};
