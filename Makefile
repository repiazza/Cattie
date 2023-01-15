CCOPT = -Wall

ifdef   DEBUG
CCOPT += -g
else
CCOPT += -O2
endif

INCDIR=
CC     = gcc

LIBS=  
ifdef _WIN32
LIBS+= -LC:/msys64/mingw64/bin/../lib -lSDL2_image -lmingw32 -lSDL2main -lSDL2 -mwindows -lSDL_ttf
endif

ifdef LINUX
LIBS+= -Wl,-rpath,/usr/lib64 -Wl,--enable-new-dtags -lSDL2 -lSDL2main -lSDL_ttf -lSDL2_image -DLINUX
endif

CATTIE_EXEC=cattie

OBJS += \
  cattie.o 

all: clean $(CATTIE_EXEC)

clean:
	rm -f $(OBJS) $(CATTIE_EXEC)

cattie: $(OBJS)
	$(CC) $(LDOPT) -o $(CATTIE_EXEC) $(OBJS) $(LIBS)

%.o: %.c
	$(CC) -c $(CCOPT) $(INCDIR) $< -o $@
