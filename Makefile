# Environment
MKDIR = mkdir
CP = cp
CC = arm-elf-gcc
LUA-DIR = ../dependencies/lua-5.2.0/

PATH := ../gcc/bin/:${PATH}

OBJECTDIR ?= build/pc1000

OBJECTFILES= \
	${OBJECTDIR}/init.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/lua-main.o \
	${OBJECTDIR}/josapi-lua-binding.o

CFLAGS = -std=c99 -mlittle-endian -mcpu=arm9 -B../gcc/bin/ -I../gcc/include/ -I../api/include/ -I"${LUA-DIR}/src/" -Wundef -Wstrict-prototypes -Wno-trigraphs -Wimplicit -Wformat

# Build Targets
all: ${BUILD_SUBPROJECTS} build/pc1000/app/bootstrap.elf

build/pc1000/app/bootstrap.elf: ${OBJECTFILES} main.lua
	arm-elf-ld -o $@ ${OBJECTFILES} -T../ldscript -L../gcc/lib/ -L../gcc/libelf/ -L../api/lib/ -L . -L"${LUA-DIR}/src/" -llua -l"pc1000wlsapi(v11)" -l"pc1000api(v09)" -lc -lgcc -lm
	elftobin build\\pc1000\\app\\bootstrap.elf build\\pc1000\\app\\bootstrap.bin PC1000---APP
#	$(CP) main.lua build/pc1000/app/
#	rsync --recursive --exclude "*.so" --exclude "*.o" --exclude "*.c" $(CHIBI-TARGET-DIR)/lib build/pc1000/app/

${OBJECTDIR}/main.o: main.c josapi-lua-binding.c
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.c) -O2 -o $@ main.c

${OBJECTDIR}/lua-main.o: main.lua
	xxd -i main.lua ${OBJECTDIR}/lua-main.c
	$(COMPILE.c) -O2 -o $@ ${OBJECTDIR}/lua-main.c

${OBJECTDIR}/%.o: %.c
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.c) -O2 -o $@ $<

${OBJECTDIR}/%.o: %.s
	${MKDIR} -p build/pc1000/app
	arm-elf-as -o $@ $<

%.c: %.stub
	chmod a+w $@
	$(CHIBI-HOST-DIR)/chibi-scheme -I $(CHIBI-HOST-DIR)/lib $(CHIBI-HOST-DIR)/tools/chibi-ffi $< $@
	chmod a-w $@

# Clean Targets
clean:
	-${RM} -r build/*
