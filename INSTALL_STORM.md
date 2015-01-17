The purpose of this document is to get a Lua shell running on a Storm mote. You will need:

* the `kernel0` branch of [https://github.com/SoftwareDefinedBuildings/stormport](https://github.com/SoftwareDefinedBuildings/stormport)
* TinyOS toolchain installed (follow instructions in the above repository)
* the `master` branch of this repository, [https://github.com/SoftwareDefinedBuildings/storm_elua](https://github.com/SoftwareDefinedBuildings/storm_elua)
* Python pip installation
* a FireStorm mote with USB cable

## Preparing Kernel

Inside the `stormport` repository, find the Kernel application at
`stormport/apps/Kernel`. To limit the number of items that print out on your
screen and obfuscate the output you care about, open up the Makefile and make
sure you have the following 2 lines commented/uncommented:

```make
PFLAGS += -DBLIP_STFU
# PFLAGS += -DWITH_WIZ
```

then flash your attached mote with

```bash
make storm install
```

You will need the Kernel installed on the mote in order to run Lua.

## Preparing ELua

Prepare your system to compile ELua. Run the following from inside the base
directory of the `storm_elua` repository:

```bash
sudo apt-get install -y lua5.1 luarocks
sudo pip install --upgrade stormloader
sudo luarocks install luafilesystem
sudo luarocks install lpack
sudo luarocks install md5
lua cross-lua.lua
```

Then, to install ELua on your mote, run

```bash
. gobuild
```

If this fails and gives you an error related to not finding a luarocks library, try running `sudo apt-get remove lua5.2`.

If successful, this will give you some output like 

```
[SLOADER] Attached
SERIAL NUMBER 0xfeeb
IPAddress - Setting global address: 2001:470:4885:a:212:6d02::feeb
XX ipdispatchp splitcontrol.start
Booting kernel 1.0.0.0 (dc97c8af2b49204568f0bd9908633ba3b68c06e6)
Found payload start at 0x50001
Payload stack generated
XX radio control start done err: 0
um
lua: ?:0: attempt to call global 'x' (a number value)
stack traceback:
?: in main chunk
[C]: ?

IoET 2015 embedded Lua
eLua# 
```

and then drop you into a shell indicated by `eLua#`. To get the Lua
interpreter, simply run `lua`, and you should get

```
You are now in a Lua shell
Lua 5.1.4  Copyright (C) 1994-2011 Lua.org, PUC-Rio
> 
```

Note: do not worry about the error `attempt to call global 'x' (a number value) -- it means nothing other than indicating stack traces work
