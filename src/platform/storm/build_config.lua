-- This is the platform specific board configuration file
-- It is used by the generic board configuration system (config/)

module( ..., package.seeall )

print ("[!!] BUILD CONFIG RAN ")
-- Add specific components to the 'components' table
function add_platform_components( t, board, cpu )

end

-- Add specific configuration to the 'configs' table
function add_platform_configs( t, board, cpu )

end

-- Return an array of all the available platform modules for the given cpu
function get_platform_modules( board, cpu )
  m = {
      io = { lib='"io"', map = "libstorm_io_map", open = false }
  }
  return m
end

