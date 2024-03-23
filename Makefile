# 
#   Makefile for Cattie
#
#   Written by Renato Fermi <repiazza@gmail.com> in January 2023
#
#


UNAME_S := $(shell uname -s)

ifneq ($(findstring _NT-, $(UNAME_S)),)
	_WIN32 = 1
else
	LINUX = 1
endif

CC    = gcc

SRC_PATH = src
GXRF_PATH = GXRF
SCRIPTS_PATH = scripts
INCLUDE_PATH = include
CCOPT = -Wall -Wextra

INCDIR=  -I.
INCDIR+= -I$(SRC_PATH)/
INCDIR+= -I$(GXRF_PATH)/
INCDIR+= -I$(INCLUDE_PATH)/

SDL_ADD_LIBS     = -lSDL2main -lSDL2 -lSDL2_image
NCURSES_ADD_LIBS = -lncurses

ifdef _WIN32
	NCURSES_ADD_LIBS += -DNCURSES_STATIC
	SDL_ADD_LIBS += -lSDL2_ttf 
	CCOPT += -LC:/msys64/mingw32/bin/../lib -lmingw32 $(SDL_ADD_LIBS) $(NCURSES_ADD_LIBS) -mwindows -D_WIN32 
	LIBS  =  -LC:/msys64/mingw32/bin/../lib -lmingw32 $(SDL_ADD_LIBS) $(NCURSES_ADD_LIBS) -mwindows -D_WIN32 
endif

ifdef LINUX
	SDL_ADD_LIBS += -lSDL2_ttf
	CCOPT += -Wl,-rpath,/usr/lib64 -Wl,--enable-new-dtags $(SDL_ADD_LIBS) $(NCURSES_ADD_LIBS) -DLINUX
	LIBS  =  -Wl,-rpath,/usr/lib64 -Wl,--enable-new-dtags $(SDL_ADD_LIBS) $(NCURSES_ADD_LIBS) -DLINUX
endif

DEBUG_ADD_FLAGS = -O2
ifdef DEBUG
	DEBUG_ADD_FLAGS = -g -ggdb
endif

CATTIE_EXEC=cattie

OBJS += \
	$(SRC_PATH)/cattie.o \
	$(SRC_PATH)/trace.o \
	$(SRC_PATH)/cmdline.o \
	$(SRC_PATH)/util.o \
	$(SRC_PATH)/sl.o \
	$(SRC_PATH)/player.o \
	$(GXRF_PATH)/GXRF.o

all: clean $(CATTIE_EXEC)

clean:
	rm -f $(OBJS) $(CATTIE_EXEC)

cattie: $(OBJS)
	$(CC) $(LDOPT) $(INCDIR) -o $(CATTIE_EXEC) $(OBJS) $(LIBS) 

%.o: %.c
	$(CC) -c $(CCOPT) $(DEBUG_ADD_FLAGS) $(INCDIR) $< -o $@

ifdef LINUX
install: all
	bash ./$(SCRIPTS_PATH)/install_linux.sh

uninstall:
	bash ./$(SCRIPTS_PATH)/uninstall_linux.sh
endif

ifdef _WIN32
install: all
	./$(SCRIPTS_PATH)/install_win32.bat

uninstall:
	./$(SCRIPTS_PATH)/uninstall_win32.bat
endif

distclean: clean
	rm -rvf *.log

.PHONY: all clean install uninstall distclean

