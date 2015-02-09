
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
int32_t __attribute__((naked)) k_syscall_ex_ri32_u32_u32_cb_vptr(uint32_t id, uint32_t arg0, uint32_t arg1, cb_t cb, void *r)
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
//#define udp_unset_recvfrom(sockid) k_syscall_ex_ri32_u32(0x306, (sockid))

//---------- SysInfo
#define sysinfo_nodeid() k_syscall_ex_ru32(0x401)
#define sysinfo_getmac(buffer) k_syscall_ex_ru32_u32(0x402, (buffer))
#define sysinfo_getipaddr(buffer) k_syscall_ex_ru32_u32(0x403, (buffer))

#warning todo make gpio irq enforce one per pin

//---------- I2C
#define i2c_transact(iswrite, address, flags, buffer, len, callback, r) k_syscall_ex_ri32_u32_u32_u32_buf_u32_vptr_vptr((0x500 + (iswrite)), (address), (flags), (buffer), (len), (callback), (r))


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
static int libstorm_io_get( lua_State *L )
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
static int libstorm_io_getd( lua_State *L )
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
static int libstorm_os_getnodeid( lua_State *L )
{
    uint32_t rv;
    rv = sysinfo_nodeid();
    lua_pushnumber(L, rv);
    return 1;
}

// Retrieve the MAC address as an array
static int libstorm_os_getmac( lua_State *L )
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
static int libstorm_os_getmacstring( lua_State *L )
{
    uint8_t mac[6];
    static char smac[18];
    sysinfo_getmac((uint32_t)&mac);
    snprintf(smac, 18, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    lua_pushstring(L, smac);
    //lua_pushfstring(L, "%d:%d:%d:%d:%d:%d", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return 1;
}

static int libstorm_os_getipaddr( lua_State *L )
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

static int libstorm_os_getipaddrstring( lua_State *L )
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


// Lua: storm.io.set( value, pin1, pin2, ..., pinn )
static int libstorm_io_set( lua_State *L )
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
static int libstorm_os_cancel( lua_State *L )
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
static int libstorm_os_now( lua_State *L )
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
static int libstorm_os_periodically(lua_State *L)
{
    return libstorm_tmr_impl(L, 1);
}

// Lua: storm.os.invokeLater( interval, function, arg0, arg1, arg2)
int libstorm_os_invokeLater(lua_State *L)
{
    return libstorm_tmr_impl(L, 0);
}

static int libstorm_os_run_callback(lua_State *L)
{
    _cb_L = L;
    k_run_callback();
    return 0;
}

static int libstorm_os_wait_callback(lua_State *L)
{
    _cb_L = L;
    k_wait_callback();
    return 0;
}

static int libstorm_os_kyield(lua_State *L)
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
} udp_recv_params_t;
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
    if ((rv = lua_pcall(_cb_L, 3, 0, 0)) != 0)
    {
        printf("[ERROR] could not run net.recv callback (%d)\n", rv);
        msg = lua_tostring(_cb_L, -1);
        printf("[ERROR] msg: %s\n", msg);
    }
}

// Lua: storm.net.bind(port, recv_callback)
static int libstorm_net_bind(lua_State *L)
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
static int libstorm_net_close(lua_State *L)
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
static int libstorm_net_sendto(lua_State *L)
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
static int libstorm_io_watch_single(lua_State *L)
{
    return libstorm_io_watch_impl(L, 0);
}
static int libstorm_io_watch_all(lua_State *L)
{
    return libstorm_io_watch_impl(L, 1);
}
static int libstorm_io_cancel_watch(lua_State *L)
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
static int libstorm_os_read_stdin(lua_State *L) {
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
static int libstorm_i2c_write(lua_State *L)
{
    return libstorm_io_i2c_x(L, 2);
}
//lua storm.i2c.read(address, flags, array_or_number, function(status, array)) -> nil
static int libstorm_i2c_read(lua_State *L)
{
    return libstorm_io_i2c_x(L, 1);
}

static int explode(lua_State *L)
{
    return lua_yield(L, 0);
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
    { LSTRKEY( "PERIPHERAL" ), LNUMVAL(2) },
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
    { LSTRKEY( "invokePeriodically" ), LFUNCVAL ( libstorm_os_periodically ) },
    { LSTRKEY( "invokeLater" ),  LFUNCVAL ( libstorm_os_invokeLater ) },
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
    { LSTRKEY( "explode" ), LFUNCVAL ( explode ) },
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
    { LSTRKEY( "udpsocket" ),  LFUNCVAL ( libstorm_net_bind ) },
    { LSTRKEY( "close" ), LFUNCVAL ( libstorm_net_close ) },
    { LSTRKEY( "sendto" ), LFUNCVAL ( libstorm_net_sendto ) },
 //   { LSTRKEY( "set_recvfrom" ), LFUNCVAL ( libstorm_net_recvfrom ) },
 //   { LSTRKEY( "unset_recvfrom" ), LFUNCVAL ( libstorm_net_recvfrom ) },
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
