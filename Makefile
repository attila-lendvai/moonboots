# Environment

PLATFORM=vega5000
OBJECTDIR ?= build

MKDIR = mkdir
CP = cp
LUA-DIR = ../dependencies/lua-5.2.0/
LUASOCKET-DIR = ../dependencies/luasocket-2.0.2/

OBJECTFILES= \
	${OBJECTDIR}/generic-main.o \
	${OBJECTDIR}/$(PLATFORM)/main.o \
	${OBJECTDIR}/$(PLATFORM)/lua-binding.o

ifeq ($(PLATFORM),pc1000)
  CC = arm-elf-gcc
  CFLAGS = -mlittle-endian -mcpu=arm9 -B../gcc/bin/ -I../gcc/include/ -I../api/include/ -Wundef -Wstrict-prototypes -Wno-trigraphs -Wimplicit -Wformat
  OBJECTFILES="${OBJECTDIR}/$(PLATFORM)/init.o $(OBJECTFILES)"
endif

ifeq ($(PLATFORM),vega5000)
  CC = mipsel-linux-uclibc-gcc
  SDKV5INC = ../api/include
  SDKV5LIB = ../api/lib
  CFLAGS="-I${SDKV5INC}" -Wundef -Wstrict-prototypes -Wno-trigraphs -Wimplicit -Wformat
  LDLIBSOPTIONS=-lcaethernet -lcafont -lcafs -lcakms -lcalcd -lcamodem -lcapmodem -lcaprt -lcartc -lcauart -lcauldpm -lcausbh -lcagsm -lcabarcode -lpthread -ldl -lcaclvw -lcatls -lctosapi -lm
endif

CFLAGS += -std=c99 -O3 -I"${LUA-DIR}/src/" -I"${LUASOCKET-DIR}/src/"

PATH := ../gcc/bin/:${PATH}

all: prepare ${BUILD_SUBPROJECTS} $(OBJECTDIR)/$(PLATFORM)/app/bootstrap

prepare:
	${MKDIR} -p ${OBJECTDIR}/$(PLATFORM)/app

# PC1000
$(OBJECTDIR)/pc1000/app/bootstrap.elf: ${OBJECTFILES} main.lua
	arm-elf-ld -o $@ ${OBJECTFILES} -T../ldscript -L../gcc/lib/ -L../gcc/libelf/ -L../api/lib/ -L . -L"${LUA-DIR}/src/" -llua -l"pc1000wlsapi(v11)" -l"pc1000api(v09)" -lc -lgcc -lm

$(OBJECTDIR)/pc1000/app/bootstrap: build/pc1000/app/bootstrap.elf
	elftobin build\\pc1000\\app\\bootstrap.elf build\\pc1000\\app\\bootstrap.bin PC1000---APP

$(OBJECTDIR)/pc1000/pc1000/main.o: build/main.lua.dump pc1000/main.c
	$(COMPILE.c) -O3 -o $@ pc1000/main.c

$(OBJECTDIR)/pc1000/pc1000/lua-binding.o: pc1000/lua-binding.c
	$(COMPILE.c) -O3 -o $@ $<

${OBJECTDIR}/%.o: %.s
	arm-elf-as -o $@ $<

# Vega5000
$(OBJECTDIR)/vega5000/app/bootstrap: ${OBJECTFILES}
	$(CC) -L . -L"${SDKV5LIB}" -L"${LUA-DIR}/src/" -L"${LUASOCKET-DIR}/src/" -o $@ ${OBJECTFILES} -Wl,-Bstatic -llua -lmime -lsocket -Wl,-Bdynamic ${LDLIBSOPTIONS}
	mipsel-linux-uclibc-strip $@
	rsync --recursive --exclude "*.so" $(LUASOCKET-DIR)/install/ build/vega5000/app/lua/

$(OBJECTDIR)/vega5000/vega5000/main.o: vega5000/main.c
	$(COMPILE.c) -o $@ vega5000/main.c

$(OBJECTDIR)/vega5000/vega5000/lua-binding.o: vega5000/lua-binding.c
	$(COMPILE.c) -o $@ $<

# Generic
$(OBJECTDIR)/generic-main.o: $(OBJECTDIR)/main.lua.dump generic-main.c
	$(COMPILE.c) -o $@ generic-main.c

$(OBJECTDIR)/main.lua.dump: main.lua
	xxd -i main.lua $@

${OBJECTDIR}/%.o: %.c
	$(COMPILE.c) -o $@ $<

# Clean Targets
clean:
	-${RM} -r build/*
