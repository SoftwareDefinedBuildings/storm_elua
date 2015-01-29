-- STORM build configuration


return {
  cpu = 'atsam4lc8',
  components = {
     sercon = { uart = 0, speed = 115200, timer = 0 },
    romfs = true,
    shell = true,
    -- linenoise = { shell_lines = 10, lua_lines = 50 },
    -- xmodem = true,
    -- rpc = { uart = 0, speed = 115200 }
  },
  config = {
    egc = { mode = "alloc" }
  },
  modules = {
    generic = { 'all_lua', 'bit'},
    --generic = { '-pio', '-tmr', '-pd', '-uart', 'term', '-pack', '-bit', 'elua', '-cpu', '-rpc', 'all_lua' },
    platform = { 'all' }
  }
}

