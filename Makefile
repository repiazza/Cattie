# 
#   Makefile for Cattie
#
#   Written by Renato Fermi <repiazza@gmail.com> in January 2023
#
#

CCOPT = -Wall
INCDIR= -I.
CC    = gcc

SDLADDONLIBS = -lSDL2main -lSDL2 -lSDL_ttf -lSDL2_image

ifdef _WIN32
CCOPT += -LC:/msys64/mingw64/bin/../lib -lmingw32 $(SDLADDONLIBS) -mwindows -D_WIN32
LIBS  =  -LC:/msys64/mingw64/bin/../lib -lmingw32 $(SDLADDONLIBS) -mwindows -D_WIN32
endif

ifdef LINUX
CCOPT += -Wl,-rpath,/usr/lib64 -Wl,--enable-new-dtags $(SDLADDONLIBS) -DLINUX
LIBS  =  -Wl,-rpath,/usr/lib64 -Wl,--enable-new-dtags $(SDLADDONLIBS) -DLINUX
endif

ifdef DEBUG
CCOPT += -g -ggdb
else
CCOPT += -O2
endif

CATTIE_EXEC=cattie

OBJS += \
  cattie.o \
  GXRF/trace.o \
  GXRF/GXRF.o 

all: clean $(CATTIE_EXEC)

clean:
	rm -f $(OBJS) $(CATTIE_EXEC)

cattie: $(OBJS)
	$(CC) $(LDOPT) -o $(CATTIE_EXEC) $(OBJS) $(LIBS)

%.o: %.c
	$(CC) -c $(CCOPT) $(INCDIR) $< -o $@
