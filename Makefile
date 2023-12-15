# 
#   Makefile for Cattie
#
#   Written by Renato Fermi <repiazza@gmail.com> in January 2023
#
#

CC    = gcc

SRCCWD = src/

CCOPT = -Wall -Wextra

INCDIR= -I.
INCDIR+= -Isrc/
INCDIR+= -IGXRF/
INCDIR+= -Iinclude

SDLADDONLIBS     = -lSDL2main -lSDL2 -lSDL2_image
NCURSESADDONLIBS = -lncurses

ifdef _WIN32
	NCURSESADDONLIBS += -DNCURSES_STATIC
	SDLADDONLIBS += -lSDL_ttf 
	CCOPT += -LC:/msys64/mingw64/bin/../lib -lmingw32 $(SDLADDONLIBS) $(NCURSESADDONLIBS) -mwindows -D_WIN32 
	LIBS  =  -LC:/msys64/mingw64/bin/../lib -lmingw32 $(SDLADDONLIBS) $(NCURSESADDONLIBS) -mwindows -D_WIN32 
endif

ifdef LINUX
	SDLADDONLIBS += -lSDL2_ttf
	CCOPT += -Wl,-rpath,/usr/lib64 -Wl,--enable-new-dtags $(SDLADDONLIBS) $(NCURSESADDONLIBS) -DLINUX
	LIBS  =  -Wl,-rpath,/usr/lib64 -Wl,--enable-new-dtags $(SDLADDONLIBS) $(NCURSESADDONLIBS) -DLINUX
endif

ifdef DEBUG
	CCOPT += -g -ggdb
else
	CCOPT += -O2
endif

CATTIE_EXEC=cattie

OBJS += \
	src/cattie.o \
	src/trace.o \
	GXRF/GXRF.o \
	src/cmdline.o \
	src/util.o \
	src/sl.o

all: clean $(CATTIE_EXEC)

clean:
	rm -f $(OBJS) $(CATTIE_EXEC)

cattie: $(OBJS)
	$(CC) $(LDOPT) $(INCDIR) -o $(CATTIE_EXEC) $(OBJS) $(LIBS) 

%.o: %.c
	$(CC) -c $(CCOPT) $(INCDIR) $< -o $@

ifdef LINUX
install: all
	bash ./scripts/install_linux.sh

uninstall:
	bash ./scripts/uninstall_linux.sh
endif

ifdef _WIN32
install: all
	./scripts/install_win32.bat

uninstall:
	./scripts/uninstall_win32.bat
endif

distclean: clean
	rm -rvf *.log

.PHONY: all clean install uninstall distclean

