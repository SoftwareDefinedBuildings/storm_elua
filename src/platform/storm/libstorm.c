
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "platform.h"
#include "lrotable.h"
#include "platform_conf.h"
#include "platform_generic.h"
#include "auxmods.h"
#include "libstormarray.h"
#include <string.h>
#include <stdint.h>
#include <interface.h>
#include <stdlib.h>

#if LUA_OPTIMIZE_MEMORY == 0
#error libstorm can only be compiled with LTR on (optram=true)
#endif

#ifndef LUA_NUMBER_INTEGRAL
#error need integral
#endif
int32_t __attribute__((naked)) k_syscall_ex_ri32_u32_u32(uint32_t id, uint32_t arg0, uint32_t arg1)
{
    __syscall_body(ABI_ID_SYSCALL_EX);
}
int32_t __attribute__((naked)) k_syscall_ex_ri32_u32(uint32_t id, uint32_t arg0)
{
    __syscall_body(ABI_ID_SYSCALL_EX);
}
int32_t __attribute__((naked)) k_syscall_ex_ru32_u32(uint32_t id, uint32_t arg0)
{
    __syscall_body(ABI_ID_SYSCALL_EX);
}
int32_t __attribute__((naked)) k_syscall_ex_ri32_u32_u32_cb_vptr(uint32_t id, uint32_t arg0, uint32_t arg1, void* cb, void *r)
{
    __syscall_body(ABI_ID_SYSCALL_EX);
}
uint32_t __attribute__((naked)) k_syscall_ex_ru32(uint32_t id)
{
    __syscall_body(ABI_ID_SYSCALL_EX);
}
int32_t __attribute__((naked)) k_syscall_ex_ri32(uint32_t id)
{
    __syscall_body(ABI_ID_SYSCALL_EX);
}
int32_t __attribute__((naked)) k_syscall_ex_ri32_u32_vptr_u32_cptr_cb_vptr(uint32_t id, uint32_t arg0, void *arg1, uint32_t arg2, char* arg3, cb_t cb, void *r)
{
    __syscall_body(ABI_ID_SYSCALL_EX);
}
int32_t __attribute__((naked)) k_syscall_ex_ri32_u32_cb_vptr(uint32_t id, uint32_t arg0, void *cb, void *r)
{
    __syscall_body(ABI_ID_SYSCALL_EX);
}
int32_t __attribute__((naked)) k_syscall_ex_ri32_cptr_u32_cptr_u32(uint32_t id, uint32_t arg0, const char* arg1, uint32_t arg2, const char* arg3, uint32_t arg4)
{
    __syscall_body(ABI_ID_SYSCALL_EX);
}
int32_t __attribute__((naked)) k_syscall_ex_ri32_u32_u32_u32_buf_u32_vptr_vptr(uint32_t id, uint32_t arg0, uint32_t arg1, uint8_t* arg2, uint32_t arg3, void* cb, void* r)
{
    __syscall_body(ABI_ID_SYSCALL_EX);
}
int32_t __attribute__((naked)) k_syscall_ex_ri32_cb_vptr_cb_vptr_cptr_u32(uint32_t id, void* arg0, void* arg1, void* arg2, void* arg3, const char* arg4, uint32_t arg5)
{
    __syscall_body(ABI_ID_SYSCALL_EX);
}
int32_t __attribute__((naked)) k_syscall_ex_ri32_u32_u32_cptr(uint32_t id, uint32_t char_handle, uint32_t len, const char* buffer)
{
    __syscall_body(ABI_ID_SYSCALL_EX);
}
void* __attribute__((naked)) k_syscall_ex_rvoid(uint32_t id)
{
    __syscall_body(ABI_ID_SYSCALL_EX);
}
int32_t __attribute__((naked)) k_syscall_ex_rcptr_u32_cptr_u32(uint32_t id, const char* arg0, uint32_t arg1, const char* arg2, uint32_t arg3)
{
    __syscall_body(ABI_ID_SYSCALL_EX);
}
int32_t __attribute__((naked)) k_syscall_ex_rcptr_u32_u32(uint32_t id, const char* arg0, uint32_t arg1, uint32_t buffer)
{
    __syscall_body(ABI_ID_SYSCALL_EX);
}
int32_t __attribute__((naked)) k_syscall_ex_ri32_cptr_u32_cptr_cptr(uint32_t id, const char* d, uint32_t a, const char *b, char* c)
{
    __syscall_body(ABI_ID_SYSCALL_EX);
}
int32_t __attribute__((naked)) k_syscall_ex_ri32_cptr(uint32_t id, char *b)
{
    __syscall_body(ABI_ID_SYSCALL_EX);
}
int32_t __attribute__((naked)) k_syscall_ex_ri32_vptr_vptr_uint32_vptr_vptr(uint32_t id, void* a, void* b, uint32_t c, void *d, void *e)
{
    __syscall_body(ABI_ID_SYSCALL_EX);
}
int32_t __attribute__((naked)) k_syscall_ex_ri32_uint32_vptr_uint32_vptr_vptr(uint32_t id, uint32_t a, void* b, uint32_t c, void *d, void *e)
{
    __syscall_body(ABI_ID_SYSCALL_EX);
}
//Some driver specific syscalls
//--------- GPIO
#define simplegpio_set_mode(dir,pinspec) k_syscall_ex_ri32_u32_u32(0x101,(dir),(pinspec))
#define simplegpio_set(value,pinspec) k_syscall_ex_ri32_u32_u32(0x102,(value),(pinspec))
#define simplegpio_get(pinspec) k_syscall_ex_ri32_u32(0x103,(pinspec))
#define simplegpio_set_pull(dir,pinspec) k_syscall_ex_ri32_u32_u32(0x104, (dir),(pinspec))
#define simplegpio_getp(pinspec) k_syscall_ex_ri32_u32(0x105,(pinspec))
#define simplegpio_enable_irq(pinspec, flag, cb, r) k_syscall_ex_ri32_u32_u32_cb_vptr(0x106,(pinspec),(flag), (cb), (r))
#define simplegpio_disable_irq(pinspec) k_syscall_ex_ri32_u32(0x107, (pinspec))

//----------- TIMER
#define timer_set(ticks,periodic, callback, r) k_syscall_ex_ri32_u32_u32_cb_vptr(0x201, (ticks), (periodic), (callback),(r))
#define timer_getnow() k_syscall_ex_ru32(0x202)
#define timer_getnow_s16() k_syscall_ex_ru32(0x203)
#define timer_getnow_s48() k_syscall_ex_ru32(0x204)
#define timer_cancel(id) k_syscall_ex_ri32_u32(0x205, (id))

//---------- UDP
#define udp_socket() k_syscall_ex_ri32(0x301)
#define udp_bind(sockid,port) k_syscall_ex_ri32_u32_u32(0x302, (sockid), (port))
#define udp_close(sockid) k_syscall_ex_ri32_u32(0x303, (sockid))
#define udp_sendto(sockid, buffer, bufferlen, addr, port) k_syscall_ex_ri32_cptr_u32_cptr_u32(0x304, (sockid), (buffer), (bufferlen), (addr), (port))
#define udp_set_recvfrom(sockid, cb, r) k_syscall_ex_ri32_u32_cb_vptr(0x305, (sockid), (cb), (r))
#define udp_get_blipstats() k_syscall_ex_rvoid(0x306)
#define udp_clear_blipstats() k_syscall_ex_rvoid(0x307)
#define udp_get_retrystats() k_syscall_ex_rvoid(0x308)
#define udp_clear_retrystats() k_syscall_ex_rvoid(0x309)

//#define udp_unset_recvfrom(sockid) k_syscall_ex_ri32_u32(0x306, (sockid))

//---------- SysInfo
#define sysinfo_nodeid() k_syscall_ex_ru32(0x401)
#define sysinfo_getmac(buffer) k_syscall_ex_ru32_u32(0x402, (buffer))
#define sysinfo_getipaddr(buffer) k_syscall_ex_ru32_u32(0x403, (buffer))
#define sysinfo_reset() k_syscall_ex_ru32(0x404);
#define sysinfo_setlocklevel(level) k_syscall_ex_ru32_u32(0x405, (level))

//---------- I2C
#define i2c_transact(iswrite, address, flags, buffer, len, callback, r) k_syscall_ex_ri32_u32_u32_u32_buf_u32_vptr_vptr((0x500 + (iswrite)), (address), (flags), (buffer), (len), (callback), (r))

//---------- Bluetooth
#define bl_enable(on_ready, on_ready_r, on_changed, on_changed_r, adv, advlen) k_syscall_ex_ri32_cb_vptr_cb_vptr_cptr_u32(0x601, (on_ready), (on_ready_r), (on_changed), (on_changed_r), (adv), (advlen))
#define bl_addservice(uuid) k_syscall_ex_ri32_u32(0x602, (uuid))
#define bl_addcharacteristic(svc_handle, uuid, on_write, on_write_r) k_syscall_ex_ri32_u32_u32_cb_vptr(0x603, (svc_handle), (uuid), (on_write), (on_write_r))
#define bl_notify(char_handle, buffer_len, buffer) k_syscall_ex_ri32_u32_u32_cptr(0x604, (char_handle), (buffer_len), (buffer) )

//---------- Routing Table
#define routingtable_addroute(prefix, prefix_len, nexthop, ifindex) k_syscall_ex_rcptr_u32_cptr_u32(0x701, (prefix), (prefix_len), (nexthop), (ifindex))
#define routingtable_delroute(key) k_syscall_ex_ri32_u32(0x702, (key))
#define routingtable_getroute(key, buffer) k_syscall_ex_ri32_u32_u32(0x703, (key), (buffer))
#define routingtable_lookuproute(prefix, prefix_len, buffer) k_syscall_ex_rcptr_u32_u32(0x704, (prefix), (prefix_len), (buffer))
#define routingtable_gettable(size, buffer) k_syscall_ex_ri32_u32_u32(0x705, (size), (buffer))

#define aes_encrypt(iv, mlen, message, dest) \
    k_syscall_ex_ri32_cptr_u32_cptr_cptr(0x801, (iv),(mlen),(message),(dest))
#define aes_decrypt(iv, mlen, message, dest) \
    k_syscall_ex_ri32_cptr_u32_cptr_cptr(0x802, (iv),(mlen),(message),(dest))
#define aes_setkey(key) k_syscall_ex_ri32_cptr(0x803, (key))

#define spi_set_cs(state) k_syscall_ex_ri32_u32(0x901, (state))
#define spi_init(mode, baudrate) k_syscall_ex_ri32_u32_u32(0x902, (mode), (baudrate))
#define spi_write(txbuf, rxbuf, len, cb, r) k_syscall_ex_ri32_vptr_vptr_uint32_vptr_vptr(0x903, (txbuf), (rxbuf), (len), (cb), (r))

#define flash_write(addr, buf, len, cb, r) k_syscall_ex_ri32_uint32_vptr_uint32_vptr_vptr(0xa02, (addr), (buf),(len),(cb),(r))
#define flash_read(addr, buf, len, cb, r) k_syscall_ex_ri32_uint32_vptr_uint32_vptr_vptr(0xa01, (addr), (buf),(len),(cb),(r))

static lua_State *_cb_L;
#define MAXPINSPEC 20
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
    0x020A, //GP0 = PC10
};

// Lua: storm.io.get(pin1, pin2, ..., pinn )
int libstorm_io_get( lua_State *L )
{
    unsigned i;
    int pinspec;
    int rv;
    unsigned tos = lua_gettop(L);
    for( i = 1; i <= tos; i ++ )
    {
        pinspec = luaL_checkint(L, i);
        if (pinspec < 0 || pinspec > MAXPINSPEC)
          return luaL_error( L, "invalid IO pin");

        rv = simplegpio_get(pinspec_map[pinspec]);
        lua_pushnumber(L, rv);
    }
    return tos;
}

// Lua: storm.io.getd(pin1, pin2, ..., pinn )
int libstorm_io_getd( lua_State *L )
{
    unsigned i;
    int pinspec;
    int rv;
    unsigned tos = lua_gettop(L);
    for( i = 1; i <= tos; i ++ )
    {
        pinspec = luaL_checkint(L, i);
        if (pinspec < 0 || pinspec > MAXPINSPEC)
          return luaL_error( L, "invalid IO pin");

        rv = simplegpio_getp(pinspec_map[pinspec]);
        lua_pushnumber(L, rv);
    }
    return tos;
}

// returns the NODE ID
int libstorm_os_getnodeid( lua_State *L )
{
    uint32_t rv;
    rv = sysinfo_nodeid();
    lua_pushnumber(L, rv);
    return 1;
}

// Retrieve the MAC address as an array
int libstorm_os_getmac( lua_State *L )
{
    uint8_t mac[6];
    int i;
    sysinfo_getmac((uint32_t)&mac);
    lua_createtable(L, 6, 0);
    for (i=0; i<6; i++) {
        lua_pushinteger(L, mac[i]);
        lua_rawseti(L, -2, i);
    }
    return 1;
}

// Retrieve the MAC address as an (integer-formatted) string
//TODO: handle hex string formatting
int libstorm_os_getmacstring( lua_State *L )
{
    uint8_t mac[6];
    static char smac[18];
    sysinfo_getmac((uint32_t)&mac);
    snprintf(smac, 18, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    lua_pushstring(L, smac);
    //lua_pushfstring(L, "%d:%d:%d:%d:%d:%d", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return 1;
}

int libstorm_os_getipaddr( lua_State *L )
{
    uint8_t ip[16];
    int i;
    sysinfo_getipaddr((uint32_t)&ip);
    lua_createtable(L, 16, 0);
    for (i=0; i<16; i++) {
        lua_pushinteger(L, ip[i]);
        lua_rawseti(L, -2, i);
    }
    return 1;
}

int libstorm_os_getipaddrstring( lua_State *L )
{
    uint8_t ip[16];
    static char sip[40];
    sysinfo_getipaddr((uint32_t)&ip);
    snprintf(sip, 40, "%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
                 ip[0], ip[1], ip[2], ip[3], ip[4], ip[5], ip[6], ip[7],
                 ip[8], ip[9], ip[10], ip[11], ip[12], ip[13], ip[14], ip[15]);
    lua_pushstring(L, sip);
    return 1;
}

int libstorm_os_addroute( lua_State *L )
{
    const char* prefix;
    size_t prefixlen;
    const char* nexthop;
    uint8_t ifindex;
    int rv;
    char *errparam = "expected(prefix, prefixlen, nexthop, interface)";
    if (lua_gettop(L) != 4) return luaL_error(L, errparam);

    if (lua_isnil(L, 1))
    {
        prefix = NULL;
    }
    else
    {
        prefix = luaL_checkstring(L, 1);
    }
    prefixlen = luaL_checknumber(L, 2);
    if (lua_isnil(L, 3))
    {
        nexthop = NULL;
    }
    else
    {
        nexthop = luaL_checkstring(L, 3);
    }
    ifindex = luaL_checknumber(L, 4);
    rv = routingtable_addroute(prefix, prefixlen, nexthop, ifindex);
    lua_pushnumber(L, rv);
    return 1;
}

int libstorm_os_delroute( lua_State *L )
{
    int route_key;
    int rv;
    char *errparam = "expected (keyindex)";
    if (lua_gettop(L) != 1) return luaL_error(L, errparam);
    route_key = luaL_checknumber(L, 1);
    rv = routingtable_delroute(route_key);
    lua_pushnumber(L, rv);
    return 1;
}

int libstorm_os_getroute( lua_State *L )
{
    // this is a packed version of the route_entry struct from BLIP
    // uint8_t route key, uint8_t prefix[16], uint8_t prefix length, uint8_t next_hop[16], uint8_t ifindex = 35 bytes
    uint8_t routeentry[35];
    uint8_t prefix[16];
    static char prefix_s[40];
    uint8_t nexthop[16];
    uint8_t route_key;
    uint8_t prefixlen;
    uint8_t ifindex;
    char *errparam = "expected (keyindex)";
    if (lua_gettop(L) != 1) return luaL_error(L, errparam);

    route_key = luaL_checknumber(L, 1);
    routingtable_getroute(route_key, (uint32_t)&routeentry);
    { // parse route entry
        int i;
        route_key = routeentry[0];
        prefixlen = routeentry[17];
        ifindex = routeentry[34];
        for (i=0;i<16;i++)
        {
            prefix[i] = routeentry[1+i];
        }
        for (i=0;i<16;i++)
        {
            nexthop[i] = routeentry[18+i];
        }

    }
    lua_createtable(L, 0, 5);
    lua_pushstring(L, "route_key");
    lua_pushnumber(L, route_key);
    lua_settable(L, 2);

    lua_pushstring(L, "prefix");
    snprintf(prefix_s, 40, "%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
                 prefix[0], prefix[1], prefix[2], prefix[3], prefix[4], prefix[5], prefix[6], prefix[7],
                 prefix[8], prefix[9], prefix[10], prefix[11], prefix[12], prefix[13], prefix[14], prefix[15]);
    lua_pushstring(L, prefix_s);
    lua_settable(L, 2);

    lua_pushstring(L, "prefixlen");
    lua_pushnumber(L, prefixlen);
    lua_settable(L, 2);

    lua_pushstring(L, "next_hop");
    snprintf(prefix_s, 40, "%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
                 nexthop[0], nexthop[1], nexthop[2], nexthop[3], nexthop[4], nexthop[5], nexthop[6], nexthop[7],
                 nexthop[8], nexthop[9], nexthop[10], nexthop[11], nexthop[12], nexthop[13], nexthop[14], nexthop[15]);
    lua_pushstring(L, prefix_s);
    lua_settable(L, 2);

    lua_pushstring(L, "ifindex");
    lua_pushnumber(L, ifindex);
    lua_settable(L, 2);
    return 1;
}

int libstorm_os_lookuproute( lua_State *L )
{
    uint8_t routeentry[35];
    const char *inprefix;
    uint8_t prefix[16];
    static char prefix_s[40];
    uint8_t nexthop[16];
    uint8_t route_key;
    uint8_t prefixlen;
    uint8_t ifindex;
    char *errparam = "expected (prefix, prefix len)";
    if (lua_gettop(L) != 2) return luaL_error(L, errparam);
    if (lua_isnil(L, 1))
    {
        inprefix = NULL;
    }
    else
    {
        inprefix = luaL_checkstring(L, 1);
    }
    prefixlen = luaL_checknumber(L, 2);
    routingtable_lookuproute(inprefix, prefixlen, (uint32_t)&routeentry);

    { // parse route entry
        int i;
        route_key = routeentry[0];
        prefixlen = routeentry[17];
        ifindex = routeentry[34];
        for (i=0;i<16;i++)
        {
            prefix[i] = routeentry[1+i];
        }
        for (i=0;i<16;i++)
        {
            nexthop[i] = routeentry[18+i];
        }

    }
    lua_createtable(L, 0, 5);
    lua_pushstring(L, "route_key");
    lua_pushnumber(L, route_key);
    lua_settable(L, 3);

    lua_pushstring(L, "prefix");
    snprintf(prefix_s, 40, "%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
                 prefix[0], prefix[1], prefix[2], prefix[3], prefix[4], prefix[5], prefix[6], prefix[7],
                 prefix[8], prefix[9], prefix[10], prefix[11], prefix[12], prefix[13], prefix[14], prefix[15]);
    lua_pushstring(L, prefix_s);
    lua_settable(L, 3);

    lua_pushstring(L, "prefixlen");
    lua_pushnumber(L, prefixlen);
    lua_settable(L, 3);

    lua_pushstring(L, "next_hop");
    snprintf(prefix_s, 40, "%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
                 nexthop[0], nexthop[1], nexthop[2], nexthop[3], nexthop[4], nexthop[5], nexthop[6], nexthop[7],
                 nexthop[8], nexthop[9], nexthop[10], nexthop[11], nexthop[12], nexthop[13], nexthop[14], nexthop[15]);
    lua_pushstring(L, prefix_s);
    lua_settable(L, 3);

    lua_pushstring(L, "ifindex");
    lua_pushnumber(L, ifindex);
    lua_settable(L, 3);
    return 1;
}

int libstorm_os_gettable( lua_State *L )
{
    int tablesize = 0;
    int i;
    uint8_t table[35*20]; // max possible size of routing table
    uint8_t prefix[16];
    uint8_t nexthop[16];
    uint8_t route_key;
    uint8_t prefixlen;
    uint8_t ifindex;
    static char prefix_s[40];

    routingtable_gettable((uint32_t)&tablesize, (uint32_t) table);
    lua_createtable(L, tablesize, 0);
    for (i=0; i<tablesize; i++)
    {
        int j;
        route_key = (table+i*35)[0];
        prefixlen = (table+i*35)[17];
        ifindex = (table+i*35)[34];
        for (j=0;j<16;j++)
        {
            prefix[j] = (table+i*35)[1+j];
        }
        for (j=0;j<16;j++)
        {
            nexthop[j] = (table+i*35)[18+j];
        }

        // push array index
        lua_pushnumber(L, i+1); // arrays start at 1 in Lua

        lua_createtable(L, 0, 5);
        lua_pushstring(L, "route_key");
        lua_pushnumber(L, route_key);
        lua_settable(L, 3);

        lua_pushstring(L, "prefix");
        snprintf(prefix_s, 40, "%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
                     prefix[0], prefix[1], prefix[2], prefix[3], prefix[4], prefix[5], prefix[6], prefix[7],
                     prefix[8], prefix[9], prefix[10], prefix[11], prefix[12], prefix[13], prefix[14], prefix[15]);
        lua_pushstring(L, prefix_s);
        lua_settable(L, 3);

        lua_pushstring(L, "prefixlen");
        lua_pushnumber(L, prefixlen);
        lua_settable(L, 3);

        lua_pushstring(L, "next_hop");
        snprintf(prefix_s, 40, "%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
                     nexthop[0], nexthop[1], nexthop[2], nexthop[3], nexthop[4], nexthop[5], nexthop[6], nexthop[7],
                     nexthop[8], nexthop[9], nexthop[10], nexthop[11], nexthop[12], nexthop[13], nexthop[14], nexthop[15]);
        lua_pushstring(L, prefix_s);
        lua_settable(L, 3);

        lua_pushstring(L, "ifindex");
        lua_pushnumber(L, ifindex);
        lua_settable(L, 3);

        lua_settable(L, 1);
    }

    return 1;
}


// Lua: storm.io.set( value, pin1, pin2, ..., pinn )
int libstorm_io_set( lua_State *L )
{
    int value;
    unsigned i;
    int pinspec;
    int rv;
    value = ( u32 )luaL_checkinteger( L, 1 );
    if (value > 2)
     return luaL_error( L, "invalid value");

    for( i = 2; i <= lua_gettop( L ); i ++ )
    {
    pinspec = luaL_checkint( L, i );
    if (pinspec < 0 || pinspec > MAXPINSPEC)
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
int libstorm_io_set_mode( lua_State *L )
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
        if (pinspec < 0 || pinspec > MAXPINSPEC)
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
int libstorm_io_set_pull( lua_State *L )
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
        if (pinspec < 0 || pinspec > MAXPINSPEC) 
            return luaL_error( L, "invalid IO pin");
        rv = simplegpio_set_pull(dir, pinspec_map[pinspec]);
        if (rv != 0)
        {
            return luaL_error( L, "kernel error");
        }
    }
    return 0;
}

static void libstorm_tmr_free_context( lua_State *L, void* ctx)
{
    uint32_t i;
    uint32_t *context = ctx;
    for (i=0;i<context[0];i++)
    {
        luaL_unref(L, LUA_REGISTRYINDEX, context[i+2]); //parameters;
    }
    luaL_unref(L, LUA_REGISTRYINDEX, context[1]); //function
    free(context);
}
int libstorm_os_cancel( lua_State *L )
{
    const uint32_t *context;
    if (lua_gettop( L ) != 1)
        return luaL_error( L, "cancel takes a single timer pointer");
    context = lua_touserdata(L, 1);
    timer_cancel(context[context[0]+2]);
    libstorm_tmr_free_context(L, (void*) context);
    lua_pop(L, 1);
    return 0;
}

// Lua: storm.os.now(type)
int libstorm_os_now( lua_State *L )
{
    int type = luaL_checkint(L, 1);
    uint32_t rv;
    switch(type)
    {
        case 1: //SHIFT_0
            rv = timer_getnow();
            break;
        case 2: //SHIFT_16
            rv = timer_getnow_s16();
            break;
        case 3: //SHIFT_48
            rv = timer_getnow_s48();
            break;
        default:
            return luaL_error( L, "invalid shift type");
    }
    lua_pushnumber(L, rv);
    return 1;
}

static void libstorm_tmr_periodic_callback(void* ctx)
{
    uint32_t *context = ctx;
    uint32_t i;
    const char* msg;
    int rv;
    lua_rawgeti(_cb_L, LUA_REGISTRYINDEX, context[1]);
    for (i=0;i<context[0];i++)
    {
        lua_rawgeti(_cb_L, LUA_REGISTRYINDEX, context[i+2]);
    }
    if ((rv = lua_pcall(_cb_L, context[0], 0, 0)) != 0)
    {
        printf("[ERROR] could not run tmr.periodic callback (%d)\n", rv);
        msg = lua_tostring(_cb_L, -1);
        printf("[ERROR] msg: %s\n", msg);
    }
}
static void libstorm_tmr_oneshot_callback(void* ctx)
{
    uint32_t *context = ctx;
    uint32_t i;
    int rv;
    const char* msg;
    lua_rawgeti(_cb_L, LUA_REGISTRYINDEX, context[1]);
    for (i=0;i<context[0];i++)
    {
        lua_rawgeti(_cb_L, LUA_REGISTRYINDEX, context[i+2]);
    }
    if ((rv = lua_pcall(_cb_L, context[0], 0, 0)) != 0)
    {
        printf("[ERROR] could not run tmr.oneshot callback (%d)\n", rv);
        msg = lua_tostring(_cb_L, -1);
        printf("[ERROR] msg: %s\n", msg);
    }
    libstorm_tmr_free_context(_cb_L, context);
}

static int libstorm_tmr_impl( lua_State *L, uint32_t periodic)
{
    u32 ticks;
    int rv;
    int i;
    int tos;
    uint32_t *context;
    tos = lua_gettop( L );
    if (tos < 2)
        return luaL_error( L, "need interval and function");
    context = (uint32_t*) malloc(4*(tos+1));
    if (!context)
        return luaL_error( L, "out of memory");
    ticks = ( u32 )luaL_checkinteger( L, 1 );
    context[0] = tos-2;

    for ( i = tos; i >= 2; i --)
    {
        context[i-1] = luaL_ref(L, LUA_REGISTRYINDEX);
    }
    if (periodic)
    {
        rv = timer_set(ticks, 1, libstorm_tmr_periodic_callback, context);
    }
    else
    {
        rv = timer_set(ticks, 0, libstorm_tmr_oneshot_callback, context);
    }
    if (rv < 0)
    {
        free(context); context = NULL;
        return luaL_error( L, "kernel error");
    }
    context[tos] = rv;
    lua_pushlightuserdata ( L, context);
    return 1;
}

// Lua: storm.os.invokePeriodically( interval, function, arg0, arg1, arg2)
int libstorm_os_invoke_periodically(lua_State *L)
{
    return libstorm_tmr_impl(L, 1);
}

// Lua: storm.os.invokeLater( interval, function, arg0, arg1, arg2)
int libstorm_os_invoke_later(lua_State *L)
{
    return libstorm_tmr_impl(L, 0);
}

int libstorm_os_run_callback(lua_State *L)
{
    _cb_L = L;
    k_run_callback();
    return 0;
}

int libstorm_os_wait_callback(lua_State *L)
{
    _cb_L = L;
    k_wait_callback();
    return 0;
}

int libstorm_os_kyield(lua_State *L)
{
    k_yield();
    return 0;
}

typedef struct
{
    int recv_cb_ref;
    uint16_t sockid;
    uint16_t _;
} __attribute__((packed)) storm_socket_t;
typedef struct
{
    uint32_t reserved1;
    uint32_t reserved2;
    uint8_t* buffer;
    uint32_t buflen;
    uint8_t src_address [16];
    uint32_t port;
    uint8_t lqi;
    uint8_t rssi;
} __attribute__((__packed__)) udp_recv_params_t;
//lua callback signature recv(data, address, port)
static void libstorm_net_recv_cb(void* sock_ptr, udp_recv_params_t *params, char* addr)
{
    storm_socket_t *sock = sock_ptr;
    int rv;
    const char* msg;
    if (sock->recv_cb_ref == 0) return;
    lua_rawgeti(_cb_L, LUA_REGISTRYINDEX, sock->recv_cb_ref);
    lua_pushlstring(_cb_L, (char*)params->buffer, params->buflen);
    lua_pushstring(_cb_L, addr); //addr is in p form
    lua_pushnumber(_cb_L, params->port);
    lua_pushnumber(_cb_L, params->lqi);
    lua_pushnumber(_cb_L, params->rssi);
    if ((rv = lua_pcall(_cb_L, 5, 0, 0)) != 0)
    {
        printf("[ERROR] could not run net.recv callback (%d)\n", rv);
        msg = lua_tostring(_cb_L, -1);
        printf("[ERROR] msg: %s\n", msg);
    }
}

// Lua: storm.net.udpsocket(port, recv_callback)
int libstorm_net_udpsocket(lua_State *L)
{
    storm_socket_t *sock;
    int32_t rv;
    int32_t socknum;
    uint16_t port;
    if (lua_gettop(L) != 2)
    {
        return luaL_error( L, "expected (port, recv_callback)");
    }
    port = (uint16_t) luaL_checkinteger(L, 1);
    socknum = udp_socket();
    if (socknum == -1)
    {
        return luaL_error( L, "ran out of sock FD's");
    }
    sock = (storm_socket_t*) malloc(sizeof(storm_socket_t));
    if (!sock)
    {
        return luaL_error( L, "out of memory");
    }
    sock->sockid = (uint16_t) socknum;
    //--
    //call kernel bind(socknum, port)
    rv = udp_bind(socknum, port);
    if (rv != 0)
    {
        free(sock);
        return luaL_error( L, "could not bind socket");
    }
    sock->recv_cb_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    //Also tell kernel to bind callback
    udp_set_recvfrom(socknum, libstorm_net_recv_cb, sock);
    lua_pushlightuserdata ( L, sock);
    return 1;
}
// Lua: storm.net.close(socket_handle)
int libstorm_net_close(lua_State *L)
{
    //First free the callback
    storm_socket_t *sock;
    sock = lua_touserdata(L, 1);
    if (!sock)
    {
        return luaL_error(L, "expected (sock)");
    }
    luaL_unref(L, LUA_REGISTRYINDEX, sock->recv_cb_ref);
    //tell kernel to free socket
    udp_close(sock->sockid);
    free(sock);
    return 0;
}
// Lua: storm.net.sendto(socket_handle, buffer, addrstr, port)
int libstorm_net_sendto(lua_State *L)
{
    storm_socket_t *sock;
    int32_t rv;
    const char* buffer;
    size_t buflen;
    const char* addrstr;
    uint32_t port;
    char *errparam = "expected (sock, buffer, addrstr, port)";
    if (lua_gettop(L) != 4) return luaL_error(L, errparam);
    sock = lua_touserdata(L, 1);
    if (!sock) return luaL_error(L, errparam);
    buffer = luaL_checklstring(L, 2, &buflen);
    addrstr = luaL_checkstring(L, 3);
    port = luaL_checknumber(L,4);
    if (port > 65535 || port == 0) return luaL_error(L, errparam);
    rv = udp_sendto(sock->sockid, buffer, buflen, addrstr, port);
    if (rv == 0) lua_pushnumber(L, 1);
    else lua_pushnumber(L, 0);
    return 1;
    //call kernel sendto(socknum, buffer, bufferlen, toaddrstr(nulterm), function_on_complete(linkresult))
}

static int traceback (lua_State *L) {
  if (!lua_isstring(L, 1))  /* 'message' not a string? */
    return 1;  /* keep it intact */
  lua_getfield(L, LUA_GLOBALSINDEX, "debug");
  if (!lua_istable(L, -1) && !lua_isrotable(L, -1)) {
    lua_pop(L, 1);
    return 1;
  }
  lua_getfield(L, -1, "traceback");
  if (!lua_isfunction(L, -1) && !lua_islightfunction(L, -1)) {
    lua_pop(L, 2);
    return 1;
  }
  lua_pushvalue(L, 1);  /* pass error message */
  lua_pushinteger(L, 2);  /* skip this function and traceback */
  lua_call(L, 2, 1);  /* call debug.traceback */
  return 1;
}

static void l_message (const char *pname, const char *msg) {
  if (pname) fprintf(stdout, "%s: ", pname);
  fprintf(stdout, "%s\n", msg);
  fflush(stdout);
}

static int docall (lua_State *L, int narg, int clear) {
  int status;
  int base = lua_gettop(L) - narg;  /* function index */
  lua_pushcfunction(L, traceback);  /* push traceback function */
  lua_insert(L, base);  /* put it under chunk and args */
  status = lua_pcall(L, narg, (clear ? 0 : LUA_MULTRET), base);
  lua_remove(L, base);  /* remove traceback function */
  /* force a complete garbage collection in case of errors */
  if (status != 0) lua_gc(L, LUA_GCCOLLECT, 0);
  return status;
}

static int incomplete (lua_State *L, int status) {
  if (status == LUA_ERRSYNTAX) {
    size_t lmsg;
    const char *msg = lua_tolstring(L, -1, &lmsg);
    const char *tp = msg + lmsg - (sizeof(LUA_QL("<eof>")) - 1);
    if (strstr(msg, LUA_QL("<eof>")) == tp) {
      //lua_pop(L, 1);
      return 1;
    }
  }
  return 0;  /* else... */
}

static int report (lua_State *L, int status) {
  if (status && !lua_isnil(L, -1)) {
    const char *msg = lua_tostring(L, -1);
    if (msg == NULL) msg = "(error object is not a string)";
    l_message("stormshell", msg);
    lua_pop(L, 1);
  }
  return status;
}

//lua : storm.os.stormshell(line)
static int libstorm_os_stormshell(lua_State *L) {
  int status;
  const char* chnk;
  if (lua_gettop(L) != 1)
  {
    return luaL_error(L, "expected (line)");
  }
  //load the buffered text
  lua_pushstring(L, "clibuf");
  lua_gettable(L, LUA_REGISTRYINDEX);
  if(lua_isnil(L, -1))
  {
    lua_pop(L, 1);
    lua_pushstring(L, "");
  }
  lua_insert(L, 1);
  //1 == prev buffer
  //2 == new part
  lua_concat(L, 2);
  //If the chunk does not end with a newline, then wait
  chnk = lua_tostring(L, 1);
  if (chnk[lua_strlen(L, 1)-1] != '\n')
  {
    lua_pushstring(L, "clibuf");
    lua_insert(L, 1); //put it below buffer string
    //clibuf
    //string
    lua_settable(L, LUA_REGISTRYINDEX);
    return 0;
  }
  status = luaL_loadbuffer(L, lua_tostring(L, 1), lua_strlen(L, 1), "=stdin");
  if (incomplete(L, status))
  {
    lua_pop(L, 1); //push off chunk/error_message
    lua_pushstring(L, "clibuf");
    lua_insert(L, 1); //put it below buffer string
    //clibuf
    //string
    lua_settable(L, LUA_REGISTRYINDEX);
    return 0;
  }
  lua_remove(L, 1);
  //zero out buffer, we have complete chunk
  lua_pushstring(L, "clibuf");
  lua_pushstring(L, "");
  lua_settable(L, LUA_REGISTRYINDEX);
  //TOS is chunk
  if (status != 0)
  {
    report(L, status);
    fputs("\033[34;1mstormsh> \033[0m", stdout);
    return 0;
  }
  status = docall(L, 0, 0);
  report(L, status);
  if (status == 0 && lua_gettop(L) > 0) {  /* any result to print? */
      lua_getglobal(L, "print");
      lua_insert(L, 1);
      if (lua_pcall(L, lua_gettop(L)-1, 0, 0) != 0)
        l_message("stormshell", lua_pushfstring(L,
                               "error calling " LUA_QL("print") " (%s)",
                               lua_tostring(L, -1)));
  }
  lua_settop(L, 0);  /* clear stack */
  fputs("\033[34;1mstormsh> \033[0m", stdout);
  fflush(stdout);
  return 0;
}

char stdin_buffer[128];

typedef struct
{
    int cbref;
    uint16_t pinspec;
} io_watch_t;

static void libstorm_io_watch_single_cb(void* watch_ptr)
{
    io_watch_t *watch = watch_ptr;
    int rv;
    const char* msg;
    if (watch->cbref == 0) return;
    lua_rawgeti(_cb_L, LUA_REGISTRYINDEX, watch->cbref);
    if ((rv = lua_pcall(_cb_L, 0, 0, 0)) != 0)
    {
        printf("[ERROR] could not run io.watch1 callback (%d)\n", rv);
        msg = lua_tostring(_cb_L, -1);
        printf("[ERROR] msg: %s\n", msg);
    }
    luaL_unref(_cb_L, LUA_REGISTRYINDEX, watch->cbref);
    free(watch);
}
static void libstorm_io_watch_all_cb(void* watch_ptr)
{
    io_watch_t *watch = watch_ptr;
    int rv;
    const char* msg;
    if (watch->cbref == 0) return;
    lua_rawgeti(_cb_L, LUA_REGISTRYINDEX, watch->cbref);
    if ((rv = lua_pcall(_cb_L, 0, 0, 0)) != 0)
    {
        printf("[ERROR] could not run io.watchA callback (%d)\n", rv);
        msg = lua_tostring(_cb_L, -1);
        printf("[ERROR] msg: %s\n", msg);
    }
}
// Lua: storm.io.watch_single(changetype, pin, function)
static int libstorm_io_watch_impl(lua_State *L, int repeat)
{
    int pin;
    int watchtype;
    io_watch_t *watch;
    if (lua_gettop(L) != 3)
    {
        return luaL_error(L, "expected (changetype, pin, function())");
    }
    watchtype = luaL_checkinteger(L, 1);
    pin = luaL_checkinteger(L, 2);
    if (pin < 0 || pin > MAXPINSPEC)	
        return luaL_error( L, "invalid IO pin");
    if (watchtype < 0 || watchtype > 2)
        return luaL_error(L, "invalid change type");

    watch = (io_watch_t*) malloc(sizeof(io_watch_t));
    if (!watch)
    {
        return luaL_error( L, "out of memory");
    }
    watch->pinspec = pinspec_map[pin];
    watch->cbref = luaL_ref(L, LUA_REGISTRYINDEX);
    if (repeat)
        simplegpio_enable_irq(pinspec_map[pin], watchtype | 4, libstorm_io_watch_all_cb, watch);
    else
        simplegpio_enable_irq(pinspec_map[pin], watchtype, libstorm_io_watch_single_cb, watch);
    //--
    lua_pushlightuserdata ( L, watch);
    return 1;
}
int libstorm_io_watch_single(lua_State *L)
{
    return libstorm_io_watch_impl(L, 0);
}
int libstorm_io_watch_all(lua_State *L)
{
    return libstorm_io_watch_impl(L, 1);
}
int libstorm_io_cancel_watch(lua_State *L)
{
    io_watch_t *watch;
    if (lua_gettop(L) != 1)
    {
        return luaL_error(L, "expected (handle)");
    }
    watch = lua_touserdata(L, 1);
    simplegpio_disable_irq(watch->pinspec);
    luaL_unref(L, LUA_REGISTRYINDEX, watch->cbref);
    free(watch);
    return 0;
}

static void libstorm_os_read_stdin_callback(void* r, int32_t v)
{
    int cbindex = (int) r;
    int rv;
    const char *msg;
    lua_rawgeti(_cb_L, LUA_REGISTRYINDEX, cbindex);
    lua_pushlstring(_cb_L, stdin_buffer, v);
    if ((rv = lua_pcall(_cb_L, 1, 0, 0)) != 0)
    {
        printf("[ERROR] could not run stdin callback (%d)\n", rv);
        msg = lua_tostring(_cb_L, -1);
        printf("[ERROR] msg: %s\n", msg);
    }
    luaL_unref(_cb_L, LUA_REGISTRYINDEX, cbindex);
}
//lua: storm.os.read_stdin(func(txt))
int libstorm_os_read_stdin(lua_State *L)
{
    int cbindex;
    //luaL_checkfunction(L, 1);
    cbindex = luaL_ref(L, LUA_REGISTRYINDEX);
    k_read_async(0, (uint8_t*)stdin_buffer, 128, libstorm_os_read_stdin_callback, (void*)cbindex);
    return 0;
}

extern uint32_t _ebss;
static int libstorm_os_freeram(lua_State *L) {
    uint32_t freeram;
    freeram = (uint32_t)&freeram - (uint32_t)(&_ebss);
    lua_pushnumber(L, freeram);
    return 1;
}

typedef struct
{
    int cbref;
    int arrayref;
    uint32_t len;
} i2c_transact_t;

static void libstorm_i2c_transact_callback(void* tr, int status)
{
    i2c_transact_t *t = tr;
    int rv;
    const char* msg;
    lua_rawgeti(_cb_L, LUA_REGISTRYINDEX, t->cbref);
    lua_pushnumber(_cb_L, status);
    lua_rawgeti(_cb_L, LUA_REGISTRYINDEX, t->arrayref);
    if ((rv = lua_pcall(_cb_L, 2, 0, 0)) != 0)
    {
        printf("[ERROR] could not run i2c callback (%d)\n", rv);
        msg = lua_tostring(_cb_L, -1);
        printf("[ERROR] msg: %s\n", msg);
    }
    luaL_unref(_cb_L, LUA_REGISTRYINDEX, t->cbref);
    luaL_unref(_cb_L, LUA_REGISTRYINDEX, t->arrayref);
    free(t);
}

static int libstorm_io_i2c_x(lua_State *L, uint8_t iswrite)
{
    uint32_t address;
    uint32_t flags;
    storm_array_t *arr;
    i2c_transact_t *t;
    int rv;
    if (lua_gettop(L) != 4)
    {
        return luaL_error(L, "expected (address, flags, array, function())");
    }
    address = luaL_checkinteger(L, 1);
    flags = luaL_checkinteger(L, 2);
    arr = lua_touserdata(L, 3);
    if (((address & 0xFF00) < 0x100) || ((address && 0xFF00) > 0x200))
        return luaL_error( L, "invalid address");
    //check flags?
    t = (i2c_transact_t*) malloc(sizeof(i2c_transact_t));
    if (!t)
    {
        return luaL_error( L, "out of memory");
    }
    t->len = arr->len;
    rv = i2c_transact(iswrite, address, flags, ARR_START(arr), t->len, (void*)libstorm_i2c_transact_callback, t);
    if (rv != 0)
    {
        free(t);
        lua_pushnil(L);
        return 1;
    }
    else
    {
        t->cbref = luaL_ref(L, LUA_REGISTRYINDEX);
        t->arrayref = luaL_ref(L, LUA_REGISTRYINDEX);
        lua_pushlightuserdata ( L, t);
        return 1;
    }
}
//lua storm.i2c.write(address, flags, array, function(status)) -> nil
int libstorm_i2c_write(lua_State *L)
{
    return libstorm_io_i2c_x(L, 2);
}
//lua storm.i2c.read(address, flags, array_or_number, function(status, array)) -> nil
int libstorm_i2c_read(lua_State *L)
{
    return libstorm_io_i2c_x(L, 1);
}

static int bl_onready_cb_key = 0;
static int bl_connect_cb_key = 0;

int libstorm_net_stats(lua_State *L)
{
    void* blipstats = udp_get_blipstats();
    lua_pushlstring(L, blipstats, 20);
    return 1;
}
int libstorm_net_clear_stats(lua_State *L)
{
    udp_clear_blipstats();
    return 0;
}
int libstorm_net_retry_stats(lua_State *L)
{
    storm_array_t *arr;
    void* retrystats = udp_get_retrystats();

    // retrystats is
    // struct
    // {
    //     uint8_t pkt_cnt[256];
    //     uint8_t tx_cnt[256];
    // } __attribute__((packed));

    // pkt_cnt
    storm_array_nc_create(L, 256, ARR_TYPE_UINT8);
    arr = lua_touserdata(L, -1);
    memcpy(ARR_START(arr), retrystats, 256 * sizeof(uint8_t));

    // tx_cnt
    storm_array_nc_create(L, 256, ARR_TYPE_UINT8);
    arr = lua_touserdata(L, -1);
    memcpy(ARR_START(arr), retrystats+256, 256 * sizeof(uint8_t));
    return 2;
}
int libstorm_net_clear_retry_stats(lua_State *L)
{
    udp_clear_retrystats();
    return 0;
}
static void libstorm_bl_onready_callback(void *r)
{
    int rv;
    const char* msg;
    lua_rawgeti(_cb_L, LUA_REGISTRYINDEX, bl_onready_cb_key);
    if ((rv = lua_pcall(_cb_L, 0, 0, 0)) != 0)
    {
        printf("[ERROR] could not run bl onready callback (%d)\n", rv);
        msg = lua_tostring(_cb_L, -1);
        printf("[ERROR] msg: %s\n", msg);
    }
    luaL_unref(_cb_L, LUA_REGISTRYINDEX, bl_onready_cb_key);
    bl_onready_cb_key = 0;
}
static void libstorm_bl_onchanged_callback(void *r, uint32_t status)
{
    int rv;
    const char* msg;

    lua_rawgeti(_cb_L, LUA_REGISTRYINDEX, bl_connect_cb_key);
    lua_pushnumber(_cb_L, status);
    if ((rv = lua_pcall(_cb_L, 1, 0, 0)) != 0)
    {
        printf("[ERROR] could not run bl onchanged callback (%d)\n", rv);
        msg = lua_tostring(_cb_L, -1);
        printf("[ERROR] msg: %s\n", msg);
    }
}

//onready, onconnect
int libstorm_bl_enable(lua_State *L)
{
    size_t len;
    const char* advdata;
    if (lua_gettop(L) != 3)
    {
        return luaL_error(L, "expected (adv_data, onconnectchanged, onready)");
    }

    bl_onready_cb_key = luaL_ref(L, LUA_REGISTRYINDEX);
    bl_connect_cb_key = luaL_ref(L, LUA_REGISTRYINDEX);
    advdata = lua_tolstring(L, 1, &len);
    printf("payload len=%d\n",len);
    bl_enable(&libstorm_bl_onready_callback, NULL, &libstorm_bl_onchanged_callback, NULL, advdata, len);
    return 0;
}

//addservice(
int libstorm_bl_addservice(lua_State *L)
{
    int handle;
    if (lua_gettop(L) != 1)
    {
        return luaL_error(L, "expected (uuid)");
    }
    handle = bl_addservice(lua_tonumber(L, 1));
    lua_pushnumber(L, handle);
    return 1;
}

static void bl_write_callback(uint32_t cbref, uint32_t buflen, uint8_t *buffer)
{
    int rv;
    const char* msg;
    lua_rawgeti(_cb_L, LUA_REGISTRYINDEX, cbref);
    lua_pushlstring(_cb_L, (char*)buffer, buflen);
    if ((rv = lua_pcall(_cb_L, 1, 0, 0)) != 0)
    {
        printf("[ERROR] could not run bl write callback (%d)\n", rv);
        msg = lua_tostring(_cb_L, -1);
        printf("[ERROR] msg: %s\n", msg);
    }
}
//addcharacteristic(svc_handle, uuid, on_write)
int libstorm_bl_addcharacteristic(lua_State *L)
{
    int handle;
    int svc_handle, uuid;
    int cbref;
    if (lua_gettop(L) != 3)
    {
        return luaL_error(L, "expected (handle, uuid, on_write)");
    }
    cbref = luaL_ref(L, LUA_REGISTRYINDEX); //callback
    uuid = lua_tonumber(L, 2);
    svc_handle = lua_tonumber(L, 1);
    
    handle = bl_addcharacteristic(svc_handle, uuid, bl_write_callback, (void*) cbref);
    lua_pushnumber(L, handle);
    return 1;
}

//Lua signature storm.bl.notify(char_handle, payload)
int libstorm_bl_notify(lua_State *L)
{
    int handle;
    size_t paylen;
    const char* payload;
    handle = lua_tonumber(L, 1);
    payload = luaL_checklstring(L, 2, &paylen);
    if (paylen > 20)
        return luaL_error(L, "payload must be shorter than 20 bytes");
    bl_notify(handle, paylen, payload);
    return 0;
}

int libstorm_os_reset(lua_State *L)
{
    sysinfo_reset();
    printf("You should never see this\n");
    return 0;
}

int libstorm_os_setpowerlock(lua_State *L)
{
    int level = lua_tonumber(L, 1);
    sysinfo_setlocklevel(level);
    return 0;
}

//lua: storm.aes.encrypt(iv, msg) -> encmsg
int libstorm_aes_encrypt(lua_State *L)
{
    const char *iv;
    const char *msg;
    char *rv;
    size_t len;
    iv = lua_tolstring(L, 1, &len);
    if (len != 16) {
        return luaL_error(L, "Expected iv length 16");
    }
    msg = lua_tolstring(L, 2, &len);
    if (len % 16 != 0)
    {
        return luaL_error(L, "You need to pad ur message to %16");
    }
    rv = malloc(len);
    aes_encrypt(iv, len, msg, &rv[0]);
    lua_pushlstring(L, rv, len);
    free(rv);
    return 1;
}

//lua: storm.aes.decrypt(iv, msg) -> decmsg
int libstorm_aes_decrypt(lua_State *L)
{
    const char *iv;
    const char *msg;
    char *rv;
    size_t len;
    iv = lua_tolstring(L, 1, &len);
    if (len != 16) {
        return luaL_error(L, "Expected iv length 16");
    }
    msg = lua_tolstring(L, 2, &len);
    if (len % 16 != 0)
    {
        return luaL_error(L, "Message should be padded?");
    }
    rv = malloc(len);
    aes_decrypt(iv, len, msg, &rv[0]);
    lua_pushlstring(L, rv, len);
    free(rv);
    return 1;
}

//lua: storm.aes.setkey(key) -> nil
int libstorm_aes_setkey(lua_State *L)
{
    const char *key;
    size_t len;
    key = lua_tolstring(L, 1, &len);
    if (len != 32) {
        return luaL_error(L, "Expected key length 32");
    }
    aes_setkey((char*)key);
    return 0;
}

//lua: storm.spi.setcs(val) -> nil
int libstorm_spi_set_cs(lua_State *L)
{
    int val = luaL_checkinteger(L, 1);
    spi_set_cs(val);
    return 0;
}

//lua: storm.spi.init(mode, val) -> nil
int libstorm_spi_init(lua_State *L)
{
    int mode = luaL_checkinteger(L, 1);
    int baud = luaL_checkinteger(L, 2);
    spi_init(mode, baud);
    return 0;
}

typedef struct {
    int tx_ref;
    int rx_ref;
    int cb_ref;
} spi_xfer_t;

void spi_xfer_cb(void *r)
{
    spi_xfer_t *t = r;
    int rv;
    const char* msg;
    lua_rawgeti(_cb_L, LUA_REGISTRYINDEX, t->cb_ref);
    if ((rv = lua_pcall(_cb_L, 0, 0, 0)) != 0)
    {
        printf("[ERROR] could not run spi callback (%d)\n", rv);
        msg = lua_tostring(_cb_L, -1);
        printf("[ERROR] msg: %s\n", msg);
    }
    luaL_unref(_cb_L, LUA_REGISTRYINDEX, t->cb_ref);
    luaL_unref(_cb_L, LUA_REGISTRYINDEX, t->tx_ref);
    luaL_unref(_cb_L, LUA_REGISTRYINDEX, t->rx_ref);
    free(t);
}
//lua: storm.spi.xfer(txarr, rxarr, cb) -> nil
int libstorm_spi_xfer(lua_State *L)
{
    spi_xfer_t *t;
    int rv;
    storm_array_t *txarr = lua_touserdata(L, 1);
    storm_array_t *rxarr = lua_touserdata(L, 2);
    t = malloc(sizeof(spi_xfer_t));
    if (!t)
    {
        return luaL_error( L, "out of memory");
    }
    t->cb_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    t->rx_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    t->tx_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    rv = spi_write(ARR_START(txarr), ARR_START(rxarr), txarr->len, spi_xfer_cb, t);
    if (rv != 0)
    {
        free(t);
        return luaL_error( L, "bad spi tx");
    }
    return 0;
}

typedef struct {
    int buf_ref;
    int cb_ref;
} flash_xfer_t;

void flash_xfer_cb(void *r)
{
    flash_xfer_t *t = r;
    int rv;
    const char* msg;
    lua_rawgeti(_cb_L, LUA_REGISTRYINDEX, t->cb_ref);
    if ((rv = lua_pcall(_cb_L, 0, 0, 0)) != 0)
    {
        printf("[ERROR] could not run flash callback (%d)\n", rv);
        msg = lua_tostring(_cb_L, -1);
        printf("[ERROR] msg: %s\n", msg);
    }
    luaL_unref(_cb_L, LUA_REGISTRYINDEX, t->buf_ref);
    luaL_unref(_cb_L, LUA_REGISTRYINDEX, t->cb_ref);
    free(t);
}

//lua: storm.flash.write(addr, txarr, cb)
int libstorm_flash_write(lua_State *L)
{
    flash_xfer_t *t;
    int rv;
    uint32_t addr = lua_tonumber(L, 1);
    storm_array_t *txarr = lua_touserdata(L, 2);
    t = malloc(sizeof(flash_xfer_t));
    if (!t)
    {
        return luaL_error( L, "out of memory");
    }
    t->cb_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    t->buf_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    rv = flash_write(addr, ARR_START(txarr), txarr->len, flash_xfer_cb, t);
    if (rv != 0)
    {
        free(t);
        return luaL_error( L, "bad flash tx");
    }
    return 0;
}

//lua: storm.flash.read(addr, rxarr, cb)
int libstorm_flash_read(lua_State *L)
{
    flash_xfer_t *t;
    int rv;
    uint32_t addr = lua_tonumber(L, 1);
    storm_array_t *rxarr = lua_touserdata(L, 2);
    t = malloc(sizeof(flash_xfer_t));
    if (!t)
    {
        return luaL_error( L, "out of memory");
    }
    t->cb_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    t->buf_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    rv = flash_read(addr, ARR_START(rxarr), rxarr->len, flash_xfer_cb, t);
    if (rv != 0)
    {
        free(t);
        return luaL_error( L, "bad flash rx");
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
    { LSTRKEY( "getd" ), LFUNCVAL ( libstorm_io_getd ) },
    { LSTRKEY( "set_pull" ), LFUNCVAL ( libstorm_io_set_pull ) },
    { LSTRKEY( "watch_single" ), LFUNCVAL ( libstorm_io_watch_single ) },
    { LSTRKEY( "watch_all" ), LFUNCVAL ( libstorm_io_watch_all ) },
    { LSTRKEY( "cancel_watch" ), LFUNCVAL ( libstorm_io_cancel_watch ) },
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
    { LSTRKEY( "GP0"), LNUMVAL ( 20 ) }, /* GP0 pin 75 LED */
    { LSTRKEY( "OUTPUT" ), LNUMVAL(0) },
    { LSTRKEY( "INPUT" ), LNUMVAL(1) },
    { LSTRKEY( "ANALOG" ), LNUMVAL(2) },
    { LSTRKEY( "LOW" ), LNUMVAL(0) },
    { LSTRKEY( "HIGH" ), LNUMVAL(1) },
    { LSTRKEY( "TOGGLE" ), LNUMVAL(2) },
    { LSTRKEY( "PULL_NONE" ), LNUMVAL(0) },
    { LSTRKEY( "PULL_UP" ), LNUMVAL(1) },
    { LSTRKEY( "PULL_DOWN" ), LNUMVAL(2) },
    { LSTRKEY( "PULL_KEEP" ), LNUMVAL(3) },
    { LSTRKEY( "FALLING" ), LNUMVAL(2) },
    { LSTRKEY( "RISING" ), LNUMVAL(1) },
    { LSTRKEY( "CHANGE" ), LNUMVAL(0) },

    { LNILKEY, LNILVAL }
};

const LUA_REG_TYPE libstorm_i2c_map[] =
{
    { LSTRKEY( "write" ), LFUNCVAL ( libstorm_i2c_write ) },
    { LSTRKEY( "read" ), LFUNCVAL ( libstorm_i2c_read ) },
    { LSTRKEY( "INT" ), LNUMVAL(0x200) },
    { LSTRKEY( "EXT" ), LNUMVAL(0x100) },
    { LSTRKEY( "START" ), LNUMVAL(1) },
    { LSTRKEY( "RSTART" ), LNUMVAL(1) },
    { LSTRKEY( "ACKLAST" ), LNUMVAL(2) },
    { LSTRKEY( "STOP" ), LNUMVAL(4) },
    { LSTRKEY( "OK" ), LNUMVAL(0) },
    { LSTRKEY( "DNAK" ), LNUMVAL(1) },
    { LSTRKEY( "ANAK" ), LNUMVAL(2) },
    { LSTRKEY( "ERR" ), LNUMVAL(3) },
    { LSTRKEY( "ARBLST" ), LNUMVAL(4) },
    { LNILKEY, LNILVAL }
};

const LUA_REG_TYPE libstorm_os_map[] =
{
    { LSTRKEY( "invokePeriodically" ), LFUNCVAL ( libstorm_os_invoke_periodically ) },
    { LSTRKEY( "invokeLater" ),  LFUNCVAL ( libstorm_os_invoke_later ) },
    { LSTRKEY( "cancel" ), LFUNCVAL ( libstorm_os_cancel ) },
    { LSTRKEY( "now" ), LFUNCVAL ( libstorm_os_now ) },
    { LSTRKEY( "run_callback" ), LFUNCVAL ( libstorm_os_run_callback ) },
    { LSTRKEY( "wait_callback" ), LFUNCVAL ( libstorm_os_wait_callback ) },
    { LSTRKEY( "kyield" ), LFUNCVAL ( libstorm_os_kyield ) },
    { LSTRKEY( "stormshell"), LFUNCVAL ( libstorm_os_stormshell) },
    { LSTRKEY( "read_stdin"), LFUNCVAL ( libstorm_os_read_stdin) },
    { LSTRKEY( "imageram"), LFUNCVAL ( libstorm_os_freeram) },
    { LSTRKEY( "nodeid" ), LFUNCVAL ( libstorm_os_getnodeid ) },
    { LSTRKEY( "getmac" ), LFUNCVAL ( libstorm_os_getmac ) },
    { LSTRKEY( "getmacstring" ), LFUNCVAL ( libstorm_os_getmacstring ) },
    { LSTRKEY( "getipaddr" ), LFUNCVAL ( libstorm_os_getipaddr ) },
    { LSTRKEY( "getipaddrstring" ), LFUNCVAL ( libstorm_os_getipaddrstring ) },
    { LSTRKEY( "reset"), LFUNCVAL ( libstorm_os_reset ) },
    { LSTRKEY( "addroute" ), LFUNCVAL ( libstorm_os_addroute ) },
    { LSTRKEY( "delroute" ), LFUNCVAL ( libstorm_os_delroute ) },
    { LSTRKEY( "getroute" ), LFUNCVAL ( libstorm_os_getroute ) },
    { LSTRKEY( "lookuproute" ), LFUNCVAL ( libstorm_os_lookuproute ) },
    { LSTRKEY( "gettable" ), LFUNCVAL ( libstorm_os_gettable ) },
    { LSTRKEY( "setpowerlock"), LFUNCVAL ( libstorm_os_setpowerlock) },
    { LSTRKEY( "ROUTE_IFACE_ALL" ), LNUMVAL ( 0 ) },
    { LSTRKEY( "ROUTE_IFACE_154" ), LNUMVAL ( 1 ) },
    { LSTRKEY( "ROUTE_IFACE_PPP" ), LNUMVAL ( 2 ) },
    { LSTRKEY( "ROUTE_IFACE_ETH0" ), LNUMVAL ( 3 ) },
    { LSTRKEY( "SHIFT_0" ), LNUMVAL ( 1 ) },
    { LSTRKEY( "SHIFT_16" ), LNUMVAL ( 2 ) },
    { LSTRKEY( "SHIFT_48" ), LNUMVAL ( 3 ) },
    { LSTRKEY( "MILLISECOND" ), LNUMVAL ( MILLISECOND_TICKS ) },
    { LSTRKEY( "SECOND" ), LNUMVAL ( SECOND_TICKS ) },
    { LSTRKEY( "SECOND_S16" ), LNUMVAL ( SECOND_TICKS>>16 ) },
    { LSTRKEY( "MINUTE" ), LNUMVAL ( MINUTE_TICKS ) },
    { LSTRKEY( "MINUTE_S16" ), LNUMVAL ( MINUTE_TICKS>>16 ) },
    { LSTRKEY( "HOUR" ), LNUMVAL ( HOUR_TICKS ) },
    { LSTRKEY( "HOUR_S16" ), LNUMVAL ( HOUR_TICKS>>16 ) },
    { LNILKEY, LNILVAL }
};
const LUA_REG_TYPE libstorm_net_map[] =
{
    { LSTRKEY( "udpsocket" ),  LFUNCVAL ( libstorm_net_udpsocket ) },
    { LSTRKEY( "close" ), LFUNCVAL ( libstorm_net_close ) },
    { LSTRKEY( "sendto" ), LFUNCVAL ( libstorm_net_sendto ) },
    { LSTRKEY( "stats" ), LFUNCVAL ( libstorm_net_stats )},
    { LSTRKEY( "retrystats" ), LFUNCVAL ( libstorm_net_retry_stats )},
    { LSTRKEY( "clearstats" ), LFUNCVAL ( libstorm_net_clear_stats )},
    { LSTRKEY( "clearretrystats" ), LFUNCVAL ( libstorm_net_clear_retry_stats )},
 //   { LSTRKEY( "set_recvfrom" ), LFUNCVAL ( libstorm_net_recvfrom ) },
 //   { LSTRKEY( "unset_recvfrom" ), LFUNCVAL ( libstorm_net_recvfrom ) },
    { LNILKEY, LNILVAL }
};
const LUA_REG_TYPE libstorm_bl_map[] =
{
    { LSTRKEY( "enable" ),  LFUNCVAL ( libstorm_bl_enable ) },
    { LSTRKEY( "addservice" ),  LFUNCVAL ( libstorm_bl_addservice ) },
    { LSTRKEY( "addcharacteristic" ),  LFUNCVAL ( libstorm_bl_addcharacteristic ) },
    { LSTRKEY( "notify" ),  LFUNCVAL ( libstorm_bl_notify ) },
    { LNILKEY, LNILVAL }
};
const LUA_REG_TYPE libstorm_aes_map[] =
{
    { LSTRKEY( "encrypt" ),  LFUNCVAL ( libstorm_aes_encrypt ) },
    { LSTRKEY( "decrypt" ),  LFUNCVAL ( libstorm_aes_decrypt ) },
    { LSTRKEY( "setkey" ),  LFUNCVAL ( libstorm_aes_setkey ) },
    { LNILKEY, LNILVAL }
};
const LUA_REG_TYPE libstorm_spi_map[] =
{
    { LSTRKEY( "setcs" ),  LFUNCVAL ( libstorm_spi_set_cs ) },
    { LSTRKEY( "init" ),  LFUNCVAL ( libstorm_spi_init ) },
    { LSTRKEY( "xfer" ), LFUNCVAL ( libstorm_spi_xfer) },
    { LNILKEY, LNILVAL }
};
const LUA_REG_TYPE libstorm_flash_map[] =
{
    { LSTRKEY( "write" ),  LFUNCVAL ( libstorm_flash_write ) },
    { LSTRKEY( "read" ),  LFUNCVAL ( libstorm_flash_read ) },
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
