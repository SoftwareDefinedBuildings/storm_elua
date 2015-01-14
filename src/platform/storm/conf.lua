-- Configuration file for the Storm backend

local cpumode = ( builder:get_option( 'cpumode' ) or 'arm' ):lower()

specific_files = "platform.c interface.c libstorm.c libmsgpack.c"

local ldscript = "kernelpayload.ld"
  
-- Prepend with path
specific_files = utils.prepend_path( specific_files, sf( "src/platform/%s", platform ) )
ldscript = sf( "src/platform/%s/%s", platform, ldscript )

addm{ "FOR" .. comp.cpu:upper(), 'gcc' }

-- Standard GCC flags
addcf{ '-ffunction-sections', '-fdata-sections', '-fno-strict-aliasing', "-g3", '-Wall' , '-mthumb'}
--addlf{ '-nostartfiles', '-nostdlib', '-T', ldscript, , '-mthumb','-Wl,--gc-sections', '-Wl,--allow-multiple-definition' }
addlf{ '-nostartfiles','-mcpu=cortex-m4', '-T', ldscript, '-mthumb', '-Wl,--gc-sections', "-g3", '-Wl,--allow-multiple-definition' }

addaf{ '-x', 'assembler-with-cpp', '-Wall' }
addlib{ 'c','gcc','m' }

-- Special target configuration
local target_flags = { '-mcpu=cortex-m4' }
addm( 'CPUMODE_THUMB' )
 
addcf( target_flags )
-- addlf{ target_flags, '-Wl,-e,HardReset' }
addaf{ target_flags,'-D__ASSEMBLY__' }

-- Toolset data
tools.storm = {}

-- Array of file names that will be checked against the 'prog' target; their absence will force a rebuild
tools.storm.prog_flist = { output .. ".bin" }

-- We use 'gcc' as the assembler
toolset.asm = toolset.compile

