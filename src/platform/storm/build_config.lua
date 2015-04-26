-- This is the platform specific board configuration file
-- It is used by the generic board configuration system (config/)

module( ..., package.seeall )

-- Add specific components to the 'components' table
function add_platform_components( t, board, cpu )

end

-- Add specific configuration to the 'configs' table
function add_platform_configs( t, board, cpu )

end

-- Return an array of all the available platform modules for the given cpu
function get_platform_modules( board, cpu )
  m = {
      io = { lib='"io"', map = "libstorm_io_map", open = false },
      os = { lib='"os"', map = "libstorm_os_map", open = false },
      mp = { lib='"mp"', map = "libmsgpack_mp_map", open = false },
      net = { lib='"net"', map = "libstorm_net_map", open = false },
      array  = { lib='"array"', map = "libstorm_array_map", open = false},
      i2c  = { lib='"i2c"', map = "libstorm_i2c_map", open = false},
      n  = { lib='"n"', map = "contrib_native_map", open = false},
      bl  = { lib='"bl"', map = "libstorm_bl_map", open = false},
      aes  = { lib='"aes"', map = "libstorm_aes_map", open = false},
      spi = { lib='"spi"', map ="libstorm_spi_map", open = false},
      flash = { lib='"flash"', map ="libstorm_flash_map", open = false}
  }
  return m
end

