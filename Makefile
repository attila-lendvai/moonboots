# Environment

PLATFORM=vega5000

MKDIR = mkdir
CP = cp
LUA-DIR = ../dependencies/lua-5.2.0/

OBJECTFILES= \
	${OBJECTDIR}/$(PLATFORM)-main.o \
	${OBJECTDIR}/$(PLATFORM)-lua-binding.o

ifeq ($(PLATFORM),pc1000)
  CC = arm-elf-gcc
  CFLAGS = -mlittle-endian -mcpu=arm9 -B../gcc/bin/ -I../gcc/include/ -I../api/include/ -Wundef -Wstrict-prototypes -Wno-trigraphs -Wimplicit -Wformat
  OBJECTFILES="${OBJECTDIR}/init.o $(OBJECTFILES)"
endif

ifeq ($(PLATFORM),vega5000)
  CC = mipsel-linux-uclibc-gcc
  SDKV5INC = ../api/include
  SDKV5LIB = ../api/lib
  CFLAGS="-I${SDKV5INC}" -Wundef -Wstrict-prototypes -Wno-trigraphs -Wimplicit -Wformat
  LDLIBSOPTIONS=-lcaethernet -lcafont -lcafs -lcakms -lcalcd -lcamodem -lcapmodem -lcaprt -lcartc -lcauart -lcauldpm -lcausbh -lcagsm -lcabarcode -lpthread -ldl -lcaclvw -lcatls -lctosapi -lm
endif

CFLAGS += -std=c99 -I"${LUA-DIR}/src/"

PATH := ../gcc/bin/:${PATH}

OBJECTDIR ?= build/$(PLATFORM)

all: ${BUILD_SUBPROJECTS} $(OBJECTDIR)/app/bootstrap

# PC1000
build/pc1000/app/bootstrap.elf: ${OBJECTFILES} main.lua
	${MKDIR} -p ${OBJECTDIR}/app
	arm-elf-ld -o $@ ${OBJECTFILES} -T../ldscript -L../gcc/lib/ -L../gcc/libelf/ -L../api/lib/ -L . -L"${LUA-DIR}/src/" -llua -l"pc1000wlsapi(v11)" -l"pc1000api(v09)" -lc -lgcc -lm

build/pc1000/app/bootstrap: build/pc1000/app/bootstrap.elf
	elftobin build\\pc1000\\app\\bootstrap.elf build\\pc1000\\app\\bootstrap.bin PC1000---APP

build/pc1000/pc1000-main.o: build/main.lua.dump pc1000/main.c
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.c) -O3 -o $@ pc1000/main.c

build/pc1000/pc1000-lua-binding.o: pc1000/lua-binding.c
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.c) -O3 -o $@ $<

${OBJECTDIR}/%.o: %.s
	${MKDIR} -p ${OBJECTDIR}/app
	arm-elf-as -o $@ $<

# Vega5000
build/vega5000/app/bootstrap: ${OBJECTFILES} main.lua
	${MKDIR} -p ${OBJECTDIR}/app
	$(CC) -L . -L"${SDKV5LIB}" -L"${LUA-DIR}/src/" -o $@ ${OBJECTFILES} -Wl,-Bstatic -llua -Wl,-Bdynamic ${LDLIBSOPTIONS}
	mipsel-linux-uclibc-strip $@

build/vega5000/vega5000-main.o: build/main.lua.dump vega5000/main.c
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.c) -O3 -o $@ vega5000/main.c

build/vega5000/vega5000-lua-binding.o: vega5000/lua-binding.c
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.c) -O3 -o $@ $<

# Generic
build/main.lua.dump: main.lua
	xxd -i main.lua $@

${OBJECTDIR}/%.o: %.c
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.c) -O3 -o $@ $<

# Clean Targets
clean:
	-${RM} -r build/*
