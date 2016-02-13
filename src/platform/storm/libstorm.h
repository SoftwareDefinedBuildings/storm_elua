#ifndef __LIBSTORM_H__
#define __LIBSTORM_H__

int libmsgpack_mp_pack(lua_State *L);
int libmsgpack_mp_unpack(lua_State *L);

int libstorm_io_get( lua_State *L );
int libstorm_io_getd( lua_State *L );
int libstorm_os_getnodeid( lua_State *L );
int libstorm_os_getmac( lua_State *L );
int libstorm_os_getmacstring( lua_State *L );
int libstorm_os_getipaddr( lua_State *L );
int libstorm_os_getipaddrstring( lua_State *L );
int libstorm_io_set( lua_State *L );
int libstorm_io_set_mode( lua_State *L );
int libstorm_io_set_pull( lua_State *L );
int libstorm_os_cancel( lua_State *L );
int libstorm_os_now( lua_State *L );
int libstorm_os_invoke_periodically(lua_State *L);
int libstorm_os_invoke_later(lua_State *L);
int libstorm_os_setoutputhook(lua_State* L);
int libstorm_net_udpsocket(lua_State *L);
int libstorm_net_close(lua_State *L);
int libstorm_net_sendto(lua_State *L);
int libstorm_net_stats(lua_State *L);
int libstorm_net_clear_stats(lua_State *L);
int libstorm_net_retry_stats(lua_State *L);
int libstorm_net_clear_retry_stats(lua_State *L);
int libstorm_io_watch_single(lua_State *L);
int libstorm_io_watch_all(lua_State *L);
int libstorm_io_cancel_watch(lua_State *L);
int libstorm_os_read_stdin(lua_State *L);
int libstorm_i2c_write(lua_State *L);
int libstorm_i2c_read(lua_State *L);
int libstorm_bl_enable(lua_State *L);
int libstorm_bl_addservice(lua_State *L);
int libstorm_bl_addcharacteristic(lua_State *L);
int libstorm_bl_notify(lua_State *L);

void libstorm_os_calloutputhook(const char* buffer, size_t length);

#endif
