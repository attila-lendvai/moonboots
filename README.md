<!-- -*- mode: markdown; coding: utf-8 -*- -->

# MoonBoots #

Compile and integrate the [Lua](http://www.lua.org/) language in various hardware/software environments, but focusing on embedded devices. Integration means a platform specific FFI and a layer abstracting away (some of) the differences between the supported environments.

License: BSD 3-clause.

## Status ##

2013-01-23 First publishing:

Lua works stable on the two supported platforms ([_vega5000_](#vega5000) and [_pc1000_](#pc1000)), and basic FFI works together with the beginnings of a platform abstraction layer.

The project was initially written as a proof of concept with internal use in mind, so as of now approachability is probably less than ideal.

Contribution is welcome.

### Partial list of possible improvements ###

Consider using [luabind](https://github.com/luabind/luabind).

## Implementation policies ##

Regarding the Lua FFI bindings:

* Every parameter is in its natural unit regardless of the API (e.g. time is always expected and returned in seconds)
* To keep the platform docs useful, functions and variables are never renamed (not even e.g. DelayMS, even though its Lua binding expects seconds). Redundant prefixes are removed though.
* Functions raise Lua errors, unless return codes are part of the normal usage (e.g. Wls_CheckNetLink returns the network link state)

## Build instructions ##

The makefiles expect the following directories:

* <code>../dependencies/squish</code> (build-time Lua lib to concatenate many .lua files into one; <code>hg clone http://code.matthewwild.co.uk/squish/</code>)
* <code>../$(PLATFORM)/dependencies/lua-5.2.0</code> (Lua compiled with the platform compiler. The source distribution is available from [Lua.org](http://www.lua.org/download.html). Look for possible diff files in $(PLATFORM)/patches that helps the compilation.)
* <code>../$(PLATFORM)/api/include</code> (C header files of the platform API)
* <code>../$(PLATFORM)/api/lib</code> (binary link libraries of the platform)
* <code>../$(PLATFORM)/gcc/bin/</code> (a gcc from the platform toolchain, can be copied from the platform SDK, but take care of multiple incompatible cygwin.dll's lying around)

## Supported hardware platforms ##

### <a id="vega5000"></a>[Vega 5000](http://www.castech.com.tw/en/in-products-03.aspx?id=P_00000109&cid=C_00000018&pname=VEGA5000&cname=EFT-POS+Terminal) by [Castles Technology](http://www.castech.com.tw/) ###

A MIPS based POS terminal with 32MB flash and 32MB RAM running Linux.

#### SDK ####

Cygwin compiled gcc binary (v3.4.6).

#### Issues ####

* no support for floats on the C side (unless my memory fails me)

### <a id=pc1000"></a>[PC 1000](http://www.justtide.com/products_detail/&productId=897969ac-a238-4fc8-b2b6-163e506edf8c.html) by [Shenzhen Justtide Technology](http://www.justtide.com) ###

An ARM (ARM9 32-bit) based POS terminal with 8MB flash and 16MB RAM running a custom kernel.

#### SDK ####

Cygwin compiled gcc binary (v3.0.2).

#### Issues ####

* There's no malloc/free on this platform (the firmware binaries need to be raw bits compiled to a specific memory address). I had to write a custom _ldscript_ to layout a heap and compile in an external malloc.c.
* %f in printf doesn't work (C side issue)
* no support for floats on the C side (unless my memory fails me)

## Credits ##

The project was initially written by Attila Lendvai (attila.lendvai@gmail.com) and used in projects at [iSoft.kz](http://isoft.kz)

The project's name was suggested by Rob Hoelz (hoelzro). 'Lua' means 'moon' in Portuguese, and 'boot' comes from 'bootstrap'.
