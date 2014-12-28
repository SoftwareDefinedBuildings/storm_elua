
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

#if LUA_OPTIMIZE_MEMORY == 0
#error libstorm can only be compiled with LTR on (optram=true)
#endif

//Some driver specific syscalls
int32_t __attribute__((naked)) k_syscall_ex_ri32_u32_u32(uint32_t id, uint32_t arg0, uint32_t arg1)
{
    __syscall_body(ABI_ID_SYSCALL_EX);
}
int32_t __attribute__((naked)) k_syscall_ex_ri32_u32(uint32_t id, uint32_t arg0)
{
    __syscall_body(ABI_ID_SYSCALL_EX);
}

#define simplegpio_set_mode(dir,pinspec) k_syscall_ex_ri32_u32_u32(0x101,(dir),(pinspec))
#define simplegpio_set(value,pinspec) k_syscall_ex_ri32_u32_u32(0x102,(value),(pinspec))
#define simplegpio_get(pinspec) k_syscall_ex_ri32_u32(0x103,(pinspec))
#define simplegpio_set_pull(dir,pinspec) k_syscall_ex_ri32_u32_u32(0x104, (dir),(pinspec))

static const u16 pinspec_map [] =
{
    0x0109, //D0 = PB09
    0x010A, //D1 = PB10
    0x0010, //D2 = PA16
    0x000C, //D3 = PA12
    0x0209, //D4 = PC09
    0x000A, //D5 = PA10
    0x000B, //D6 = PA11
    0x0013, //D7 = PA19
    0x000D, //D8 = PA13
    0x010B, //D9 = PB11
    0x010C, //D10 = PB12
    0x010F, //D11 = PB15
    0x010E, //D12 = PB14
    0x010D, //D13 = PB13
    0x0105, //A0 = PB05
    0x0104, //A1 = PB04
    0x0103, //A2 = PB03
    0x0102, //A3 = PB02
    0x0007, //A4 = PA07
    0x0005, //A5 = PA05
};

// Lua: storm.io.get(pin1, pin2, ..., pinn )
static int libstorm_io_get( lua_State *L )
{
    unsigned i;
    int pinspec;
    int rv;
    unsigned tos = lua_gettop(L);
    for( i = 1; i <= tos; i ++ )
    {
        pinspec = luaL_checkint(L, i);
        if (pinspec < 0 || pinspec > 19)
          return luaL_error( L, "invalid IO pin");

        rv = simplegpio_get(pinspec_map[pinspec]);
        lua_pushnumber(L, rv);
    }
    return tos;
}

// Lua: storm.io.set( value, pin1, pin2, ..., pinn )
static int libstorm_io_set( lua_State *L )
{
    int value;
    unsigned i;
    int pinspec;
    int rv;
    value = ( u32 )luaL_checkinteger( L, 1 );
    if (value > 1)
     return luaL_error( L, "invalid value");

    for( i = 2; i <= lua_gettop( L ); i ++ )
    {
    pinspec = luaL_checkint( L, i );
    if (pinspec < 0 || pinspec > 19)
      return luaL_error( L, "invalid IO pin");

    rv = simplegpio_set(value, pinspec_map[pinspec]);
    if (rv != 0)
    {
        return luaL_error( L, "kernel error");
    }

    }
    return 0;
}

// Lua: storm.io.set_mode( dir, pin1, pin2, ..., pinn )
static int libstorm_io_set_mode( lua_State *L )
{
    int pinspec;
    u32 dir;
    unsigned i;
    int rv;
    dir = ( u32 )luaL_checkinteger( L, 1 );
    if (dir > 2)
        return luaL_error( L, "invalid direction");

    for( i = 2; i <= lua_gettop( L ); i ++ )
    {
        pinspec = luaL_checkint( L, i );
        if (pinspec < 0 || pinspec > 19)
            return luaL_error( L, "invalid IO pin");
        rv = simplegpio_set_mode(dir, pinspec_map[pinspec]);
        if (rv != 0)
        {
            return luaL_error( L, "kernel error");
        }
    }
    return 0;
}

// Lua: storm.io.set_pull( pullmode, pin1, pin2, ..., pinn )
static int libstorm_io_set_pull( lua_State *L )
{
    int pinspec;
    u32 dir;
    unsigned i;
    int rv;
    dir = ( u32 )luaL_checkinteger( L, 1 );
    if (dir > 3)
        return luaL_error( L, "invalid pullmode");

    for( i = 2; i <= lua_gettop( L ); i ++ )
    {
        pinspec = luaL_checkint( L, i );
        if (pinspec < 0 || pinspec > 19)
            return luaL_error( L, "invalid IO pin");
        rv = simplegpio_set_pull(dir, pinspec_map[pinspec]);
        if (rv != 0)
        {
            return luaL_error( L, "kernel error");
        }
    }
    return 0;
}

// Module function map
#define MIN_OPT_LEVEL 2
#include "lrodefs.h" 
const LUA_REG_TYPE libstorm_io_map[] =
{
    { LSTRKEY( "set_mode" ), LFUNCVAL ( libstorm_io_set_mode ) },
    { LSTRKEY( "set" ),  LFUNCVAL ( libstorm_io_set ) },
    { LSTRKEY( "get" ), LFUNCVAL ( libstorm_io_get ) },
    { LSTRKEY( "set_pull" ), LFUNCVAL ( libstorm_io_set_pull ) },
    { LSTRKEY( "D0" ), LNUMVAL ( 0 ) },
    { LSTRKEY( "D1" ), LNUMVAL ( 1 ) },
    { LSTRKEY( "D2" ), LNUMVAL ( 2 ) },
    { LSTRKEY( "D3" ), LNUMVAL ( 3 ) },
    { LSTRKEY( "D4" ), LNUMVAL ( 4 ) },
    { LSTRKEY( "D5" ), LNUMVAL ( 5 ) },
    { LSTRKEY( "D6" ), LNUMVAL ( 6 ) },
    { LSTRKEY( "D7" ), LNUMVAL ( 7 ) },
    { LSTRKEY( "D8" ), LNUMVAL ( 8 ) },
    { LSTRKEY( "D9" ), LNUMVAL ( 9 ) },
    { LSTRKEY( "D10" ), LNUMVAL ( 10 ) },
    { LSTRKEY( "D11" ), LNUMVAL ( 11 ) },
    { LSTRKEY( "D12" ), LNUMVAL ( 12 ) },
    { LSTRKEY( "D13" ), LNUMVAL ( 13 ) },
    { LSTRKEY( "A0" ), LNUMVAL ( 14 ) },
    { LSTRKEY( "A1" ), LNUMVAL ( 15 ) },
    { LSTRKEY( "A2" ), LNUMVAL ( 16 ) },
    { LSTRKEY( "A3" ), LNUMVAL ( 17 ) },
    { LSTRKEY( "A4" ), LNUMVAL ( 18 ) },
    { LSTRKEY( "A5" ), LNUMVAL ( 19 ) },
    { LSTRKEY( "OUTPUT" ), LNUMVAL(0) },
    { LSTRKEY( "INPUT" ), LNUMVAL(1) },
    { LSTRKEY( "PERIPHERAL" ), LNUMVAL(2) },
    { LSTRKEY( "LOW" ), LNUMVAL(0) },
    { LSTRKEY( "HIGH" ), LNUMVAL(1) },
    { LSTRKEY( "PULL_NONE" ), LNUMVAL(0) },
    { LSTRKEY( "PULL_UP" ), LNUMVAL(1) },
    { LSTRKEY( "PULL_DOWN" ), LNUMVAL(2) },
    { LSTRKEY( "PULL_KEEP" ), LNUMVAL(3) },
    { LNILKEY, LNILVAL }
};
/*
const LUA_REG_TYPE libstorm_map[] =
{
    { LSTRKEY( "io" ), LROVAL( io_map ) },

    { LNILKEY, LNILVAL }
};
LUALIB_API int luaopen_libstorm( lua_State *L )
{
  printf("\nregister called\n");
  return 0;
}
*/