# Environment

#PLATFORM ?= vega5000
PLATFORM ?= pc1000
OBJECTDIR ?= build

MKDIR = mkdir
CP = cp
LUA-DIR = ../$(PLATFORM)/dependencies/lua-5.2.0/
SQUISH = ../dependencies/lua-5.1.5/src/lua ../dependencies/squish/squish

BASE_OBJECTFILES = \
	$(OBJECTDIR)/bootstrap.o \
	$(OBJECTDIR)/$(PLATFORM)/main.o \
	$(OBJECTDIR)/$(PLATFORM)/lua-binding.o

LUA_FILES = \
	lua/main.lua \
	lua/$(PLATFORM).lua

ifeq ($(PLATFORM),pc1000)
  CC = arm-elf-gcc
  CFLAGS = -mlittle-endian -mcpu=arm9 -B ../pc1000/gcc/bin/ -I ../pc1000/gcc/include/ -I ./pc1000/ -I ../pc1000/api/include/ -Dpc1000=1
  OBJECTFILES = $(OBJECTDIR)/pc1000/init.o $(OBJECTDIR)/pc1000/user-malloc.o $(BASE_OBJECTFILES)
endif

ifeq ($(PLATFORM),vega5000)
  CC = mipsel-linux-uclibc-gcc
  SDKV5INC = ../vega5000/api/include
  SDKV5LIB = ../vega5000/api/lib
  CFLAGS = -I${SDKV5INC} -Dvega5000=1
  LDLIBSOPTIONS=-lcaethernet -lcafont -lcafs -lcakms -lcalcd -lcamodem -lcapmodem -lcaprt -lcartc -lcauart -lcauldpm -lcausbh -lcagsm -lcabarcode -lpthread -ldl -lcaclvw -lcatls -lctosapi -lm
endif

OBJECTFILES ?= $(BASE_OBJECTFILES)

CFLAGS += -std=c99 -O3 -Wall -DLUA_COMPAT_MODULE -I. -I"${LUA-DIR}/src/"

PATH := ../$(PLATFORM)/gcc/bin/:${PATH}

all: prepare ${BUILD_SUBPROJECTS} $(OBJECTDIR)/$(PLATFORM)/app/bootstrap

prepare:
	${MKDIR} -p $(OBJECTDIR)/$(PLATFORM)/app

# PC1000
$(OBJECTDIR)/pc1000/app/bootstrap.elf: ${OBJECTFILES}
	arm-elf-ld --wrap=exit --wrap=malloc --wrap=free --wrap=calloc --wrap=realloc -o $@ $(OBJECTFILES) \
		-T ./pc1000/ldscript -L ../pc1000/gcc/lib/ -L ../pc1000/gcc/libelf/ -L ../pc1000/api/lib/ -L . -L"${LUA-DIR}/src/" \
		-llua -l"pc1000wlsapi(v11)" -l"pc1000api(v09)" -lc -lgcc -lm

$(OBJECTDIR)/pc1000/app/bootstrap: $(OBJECTDIR)/pc1000/app/bootstrap.elf
	elftobin_n $(OBJECTDIR)\\pc1000\\app\\bootstrap.elf $(OBJECTDIR)\\pc1000\\app\\bootstrap.bin PC1000---APP
#	elftobin $(OBJECTDIR)\\pc1000\\app\\bootstrap.elf $(OBJECTDIR)\\pc1000\\app\\bootstrap.bin PC1000---APP
#	elf2bin $(OBJECTDIR)\\pc1000\\app\\bootstrap.elf $(OBJECTDIR)\\pc1000\\app\\bootstrap.bin PC1000---APP

$(OBJECTDIR)/pc1000/main.o: pc1000/main.c
	$(COMPILE.c) -O3 -o $@ pc1000/main.c

$(OBJECTDIR)/pc1000/lua-binding.o: pc1000/lua-*
	$(COMPILE.c) -O3 -o $@ pc1000/lua-binding.c

$(OBJECTDIR)/pc1000/%.o: pc1000/%.c
	$(COMPILE.c) -O3 -o $@ $<

$(OBJECTDIR)/pc1000/%.o: pc1000/%.s
	arm-elf-as -o $@ $<

# Vega5000
$(OBJECTDIR)/vega5000/app/bootstrap: ${OBJECTFILES}
	$(CC) -L . -L"${SDKV5LIB}" -L"${LUA-DIR}/src/" -o $@ ${OBJECTFILES} -Wl,-Bstatic -llua -Wl,-Bdynamic ${LDLIBSOPTIONS}
	mipsel-linux-uclibc-strip $@
#	rsync --recursive --exclude "*.so" ../vega5000/dependencies/install/share/lua/5.2/ build/vega5000/app/lua/

$(OBJECTDIR)/vega5000/vega5000/main.o: vega5000/main.c
	$(COMPILE.c) -o $@ vega5000/main.c

$(OBJECTDIR)/vega5000/vega5000/lua-binding.o: vega5000/lua-binding.c
	$(COMPILE.c) -o $@ $<

# Generic
$(OBJECTDIR)/bootstrap.o: $(OBJECTDIR)/squished.lua.dump bootstrap.c
	$(COMPILE.c) -o $@ bootstrap.c

$(OBJECTDIR)/squished.lua.dump: $(LUA_FILES)
	$(SQUISH) --no-minify --debug --model=$(PLATFORM)
	cd $(OBJECTDIR) && xxd -i squished.lua squished.lua.dump

$(OBJECTDIR)/%.o: %.c
	$(COMPILE.c) -o $@ $<

# Clean Targets
clean:
	-${RM} -r build/*
