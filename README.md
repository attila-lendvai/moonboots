<!-- -*- mode: markdown; coding: utf-8 -*- -->

# MoonBoots #

Compile and integrate [Lua](http://www.lua.org/) in various environments, focusing on embedded devices. Integration means a platform specific FFI and a layer abstracting away (some of) the differences.

## Status ##

2013-01-23 First publishing:

Lua works stable on the two supported platforms ([_vega5000_](#vega5000) and [_pc1000_](#pc1000)), and basic FFI works together with the beginnings of a platform abstraction layer.

## Implementation policies ##

Lua FFI bindings:

* Every parameter is in its natural unit regardless of the API (e.g. time is always expected and returned in seconds)
* Functions and variables are never renamed (not even DelayMS, even though it accepts seconds), but redundant prefixes are removed.
* Functions raise Lua errors, unless return codes are part of the normal usage (e.g. Wls_CheckNetLink returns the network link state)

## Supported hardware platforms ##

### <a id="vega5000"></a>[Vega 5000](http://www.castech.com.tw/en/in-products-03.aspx?id=P_00000109&cid=C_00000018&pname=VEGA5000&cname=EFT-POS+Terminal) by [Castles Technology](http://www.castech.com.tw/) ###

A MIPS based POS terminal with 32MB flash and 32MB RAM running Linux.


### <a id=pc1000"></a>[PC 1000](http://www.justtide.com/products_detail/&productId=897969ac-a238-4fc8-b2b6-163e506edf8c.html) by [Shenzhen Justtide Technology](http://www.justtide.com) ###

An ARM (ARM9 32-bit) based POS terminal with 8MB flash and 16MB RAM running a custom OS.


#### Issues ####

* There's no malloc/free on this platform (the firmware binaries need to be raw memory bits at a specific address). I had to write a custom _ldscript_ to layout a heap and integrate my own malloc.c.
* %f in printf doesn't work (C side issue)

## Credits ##

The project's name was suggested by Rob Hoelz (hoelzro). 'Lua' means 'moon' in Portuguese, and 'boot' comes from 'bootstrap'.

The project was initially written by Attila Lendvai (attila.lendvai@gmail.com) and used in projects at [iSoft.kz](http://isoft.kz)
