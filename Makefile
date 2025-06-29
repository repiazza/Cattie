# Makefile for Cattie
# Written by Renato Fermi <repiazza@gmail.com> in January 2023

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
INCDIR+= -ID:/msys64/mingw64/include


SDL_ADD_LIBS     = -lSDL2main -lSDL2 -lSDL2_image
# NCURSES_ADD_LIBS = -lncurses -ltinfo
NCURSES_ADD_LIBS = -lncurses 

OBJ_DIR = obj
BIN_DIR = bin

ifdef _WIN32
	NCURSES_ADD_LIBS += -DNCURSES_STATIC
	SDL_ADD_LIBS += -lSDL2_ttf 
	CCOPT += -LD:/msys64/usr/lib -LD:/msys64/mingw64/lib -lmingw32 $(SDL_ADD_LIBS) $(NCURSES_ADD_LIBS) -mwindows -D_WIN32 
	LIBS  =  -LD:/msys64/usr/lib -LD:/msys64/mingw64/lib -lmingw32 $(SDL_ADD_LIBS) $(NCURSES_ADD_LIBS) -mwindows -D_WIN32 
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

# CATTIE_EXEC=$(BIN_DIR)/cattie
CATTIE_EXEC=cattie

OBJS = \
	$(OBJ_DIR)/cattie.o \
	$(OBJ_DIR)/trace.o \
	$(OBJ_DIR)/cmdline.o \
	$(OBJ_DIR)/util.o \
	$(OBJ_DIR)/sl.o \
	$(OBJ_DIR)/player.o \
	$(OBJ_DIR)/GXRF.o

all: clean directories $(CATTIE_EXEC)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) *.log

directories:
	mkdir -p $(OBJ_DIR) $(BIN_DIR)

$(CATTIE_EXEC): $(OBJS)
	$(CC) $(LDOPT) $(INCDIR) -o $(BIN_DIR)/$@ $(OBJS) $(LIBS) 

$(OBJ_DIR)/%.o: $(SRC_PATH)/%.c
	$(CC) -c $(CCOPT) $(DEBUG_ADD_FLAGS) $(INCDIR) $< -o $@

$(OBJ_DIR)/%.o: $(GXRF_PATH)/%.c
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

.PHONY: all clean install uninstall distclean directories
