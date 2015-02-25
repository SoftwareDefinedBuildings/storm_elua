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
static const LUA_REG_TYPE array_meta_map[] =
{
 //   { LSTRKEY( "set_type" ), LFUNCVAL ( arr_set_type ) },
  //  { LSTRKEY( "get_type" ), LFUNCVAL ( arr_get_type ) },
    { LSTRKEY( "get" ), LFUNCVAL ( arr_get ) },
    { LSTRKEY( "set" ), LFUNCVAL ( arr_set ) },
    { LSTRKEY( "get_as" ), LFUNCVAL ( arr_get_as ) },
    { LSTRKEY( "set_as" ), LFUNCVAL ( arr_set_as ) },
    { LSTRKEY( "__len" ), LFUNCVAL ( arr_get_length ) },
    { LSTRKEY( "__index" ), LROVAL ( array_meta_map ) },
    { LSTRKEY( "get_pstring" ), LFUNCVAL ( arr_get_pstring ) },
    { LSTRKEY( "set_pstring" ), LFUNCVAL ( arr_set_pstring ) },
    { LSTRKEY( "as_str" ), LFUNCVAL ( arr_as_str ) },


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
    lua_pushrotable(L, (void*)array_meta_map);
    lua_setmetatable(L, -2);
    return 1;
}

int array_get(storm_array_t *arr, int idx){
  switch(arr->type)
    {
    case ARR_TYPE_INT8:
      return ((int8_t*)ARR_START(arr))[idx];
    case ARR_TYPE_UINT8:
      return ((uint8_t*)ARR_START(arr))[idx];
    case ARR_TYPE_INT16:
      return ((int16_t*)ARR_START(arr))[idx];
    case ARR_TYPE_UINT16:
      return ((uint16_t*)ARR_START(arr))[idx];
    case ARR_TYPE_INT32:
      return ((int32_t*)ARR_START(arr))[idx];
    default:
      printf("error: storm array - invalid type"); //?
      return 0;
    }
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

int array_set(storm_array_t *arr, int idx, int val){
  switch(arr->type)
    {
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
      return 0;
    }
  return 1;
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
    if (! array_set(arr, idx, val)){
      return luaL_error(L, "bad array type");
    }
    return 0;
}

int array_get_as2(storm_array_t *arr, int type, int idx, int *err)
{
  uint8_t rv [] __attribute__((aligned(4))) = {0,0,0,0};
  uint8_t* ptr = ARR_START(arr) + idx;
  *err = 0;
  switch(type){
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
  switch(type){
  case ARR_TYPE_INT8:
    return *((int8_t*) &rv[0]);
  case ARR_TYPE_UINT8:
    return *((uint8_t*) &rv[0]);
  case ARR_TYPE_INT16:
  case GS_TYPE_INT16_BE:
    return *((int16_t*) &rv[0]);
  case ARR_TYPE_UINT16:
  case GS_TYPE_UINT16_BE:
    return *((uint16_t*) &rv[0]);
  case ARR_TYPE_INT32:
  case GS_TYPE_INT32_BE:
    return *((int32_t*) &rv[0]);
  default:
    *err = 1;
    return 0;
  }
}

int array_get_as(storm_array_t *arr, int type, int idx){
  int _;
  return array_get_as2(arr, type, idx, &_);
}

//lua array:get_as(type, byte_idx)
static int arr_get_as(lua_State *L)
{
    int idx;
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
    int err;
    int val = array_get_as2(arr, type, idx, &err);
    if (err){
      return luaL_error(L, "bad array type");
    }
    return val;
}

int array_get_length(storm_array_t *arr){
  return arr->len >> arr_shiftmap[arr->type];
}

//lua #array
static int arr_get_length(lua_State *L)
{
    storm_array_t *arr = lua_touserdata(L, 1);
    lua_pushnumber(L, array_get_length(arr));
    return 1;
}

//lua array.from_string(str)
//safe to call from C, pops off string and replaces with array
int arr_from_str(lua_State *L)
{
    int top = lua_gettop(L);
    size_t len;
    const char* pay;
    pay = lua_tolstring(L, -1, &len);
    storm_array_t *arr = lua_newuserdata(L, sizeof(storm_array_t) + len);
    memcpy(ARR_START(arr), pay, len);
    arr->type = ARR_TYPE_UINT8;
    arr->len = len;
    lua_pushrotable(L, (void*)array_meta_map);
    lua_setmetatable(L, -2);
    lua_replace(L, top);
    lua_settop(L, top);
    return 1;
}

//lua arr:as_str()
int arr_as_str(lua_State *L)
{
    storm_array_t *arr = lua_touserdata(L, 1);
    lua_pushlstring(L, (char*)(ARR_START(arr)), arr->len);
    return 1;
}


//lua: array:get_pstring(byte_index)
int arr_get_pstring(lua_State *L)
{
    storm_array_t *arr = lua_touserdata(L, 1);
    int idx = luaL_checkinteger(L, 2);
    if (idx > arr->len) return luaL_error(L, "out of bounds");
    int strlen = ARR_START(arr)[idx];
    if (idx + strlen >= arr->len) return luaL_error(L, "bad string");
    lua_pushlstring(L, (char*) (&(ARR_START(arr)[idx+1])), strlen);
    return 1;
}

//lua: array:get_pstring(byte_index, string)
int arr_set_pstring(lua_State *L)
{
    storm_array_t *arr = lua_touserdata(L, 1);
    int idx = luaL_checkinteger(L, 2);
    size_t strlen;
    const char* str = lua_tolstring(L, 3, &strlen);
    if (strlen > 255) strlen = 255;
    if (idx + strlen >= arr->len) return luaL_error(L, "out of bounds");
    memcpy(ARR_START(arr) + idx + 1, str, strlen);
    ARR_START(arr)[idx] = strlen;
    return 0;
}

//lua array:set_as(type, byte_idx, val)
static int arr_set_as(lua_State *L)
{
    int idx;
    uint8_t* srcptr;
    uint8_t* dstptr;
    int val;
    int type;
    storm_array_t *arr = lua_touserdata(L, 1);
    type = luaL_checkinteger(L, 2);
    idx = luaL_checkinteger(L, 3);
    val = luaL_checknumber(L, 4);
    srcptr = (uint8_t*) (&val);
    if (!arr)
    {
        return luaL_error(L, "invalid array");
    }
    if (idx >= arr->len)
    {
        return luaL_error(L, "out of bounds");
    }
    dstptr = &(ARR_START(arr)[idx]);

    switch(type)
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
    { LSTRKEY( "fromstr" ), LFUNCVAL ( arr_from_str ) },
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
