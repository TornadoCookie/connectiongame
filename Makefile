# Generated using Helium v2.0.1 (https://github.com/tornadocookie/he)

PLATFORM?=linux64-debug
DISTDIR?=build

.PHONY: all

RAYLIB_NAME=raylib5-$(PLATFORM)

ifeq ($(PLATFORM), linux64)
EXEC_EXTENSION=
LIB_EXTENSION=.so
CC=gcc
CXX=g++
RAYLIB_DLL=-lraylib
CFLAGS+=-O2
CFLAGS+=-D RELEASE
CFLAGS+=-D EXEC_EXTENSION=\"\"
CFLAGS+=-D LIB_EXTENSION=\".so\"
endif

ifeq ($(PLATFORM), linux64-debug)
EXEC_EXTENSION=-debug
LIB_EXTENSION=-debug.so
CC=gcc
CXX=g++
RAYLIB_DLL=-lraylib
CFLAGS+=-g
CFLAGS+=-D DEBUG
CFLAGS+=-D EXEC_EXTENSION=\"-debug\"
CFLAGS+=-D LIB_EXTENSION=\"-debug.so\"
endif

ifeq ($(PLATFORM), win64)
EXEC_EXTENSION=.exe
LIB_EXTENSION=.dll
CC=x86_64-w64-mingw32-gcc
CXX=x86_64-w64-mingw32-g++
RAYLIB_DLL=-lraylibdll
CFLAGS+=-O2
CFLAGS+=-D RELEASE
CFLAGS+=-D EXEC_EXTENSION=\".exe\"
CFLAGS+=-D LIB_EXTENSION=\".dll\"
endif

ifeq ($(PLATFORM), web)
EXEC_EXTENSION=.html
LIB_EXTENSION=.a
CC=emcc
CXX=em++
RAYLIB_DLL=-lraylib
CFLAGS+=-O2
CFLAGS+=-D RELEASE
CFLAGS+=-D EXEC_EXTENSION=\".html\"
CFLAGS+=-D LIB_EXTENSION=\".a\"
endif

PROGRAMS=fsg
LIBRARIES=

all: $(DISTDIR) $(DISTDIR)/src $(foreach prog, $(PROGRAMS), $(DISTDIR)/$(prog)$(EXEC_EXTENSION)) $(foreach lib, $(LIBRARIES), $(DISTDIR)/$(lib)$(LIB_EXTENSION)) deps

ifneq ($(DISTDIR), .)
deps:
	mkdir -p $(DISTDIR)/lib
	if [ -d lib/$(RAYLIB_NAME) ]; then cp -r lib/$(RAYLIB_NAME) $(DISTDIR)/lib; fi
else
deps:
endif

$(DISTDIR)/src:
	mkdir -p $@

$(DISTDIR):
	mkdir -p $@

CFLAGS+=-Isrc
CFLAGS+=-Iinclude
CFLAGS+=-D PLATFORM=\"$(PLATFORM)\"


CFLAGS+=-Ilib/$(RAYLIB_NAME)/include

LDFLAGS+=-lm
LDFLAGS+=-Llib/$(RAYLIB_NAME)/lib
LDFLAGS+=$(RAYLIB_DLL)
LDFLAGS+=-Wl,-rpath,lib/$(RAYLIB_NAME)/lib

fsg_SOURCES+=$(DISTDIR)/src/main.o

$(DISTDIR)/fsg$(EXEC_EXTENSION): $(fsg_SOURCES)
	$(CC) -o $@ $^ $(LDFLAGS)

$(DISTDIR)/%.o: %.c
	$(CC) -c $^ $(CFLAGS) -o $@

clean:
	rm -f $(DISTDIR)/src/main.o
	rm -f $(DISTDIR)/fsg$(EXEC_EXTENSION)

all_dist:
	DISTDIR=$(DISTDIR)/dist/linux64 PLATFORM=linux64 $(MAKE)
	DISTDIR=$(DISTDIR)/dist/linux64-debug PLATFORM=linux64-debug $(MAKE)
	DISTDIR=$(DISTDIR)/dist/win64 PLATFORM=win64 $(MAKE)
	DISTDIR=$(DISTDIR)/dist/web PLATFORM=web $(MAKE)
