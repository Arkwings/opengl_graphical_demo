CC = g++
SDL_CFLAGS = $(shell sdl2-config --cflags)
SDL_LDFLAGS = $(shell sdl2-config --libs)
CPPFLAGS = -std=c++11 -I. $(SDL_CFLAGS)
CFLAGS = -Wall -O3 -g
LDFLAGS = -lm $(SDL_LDFLAGS) -lSDL2_mixer -lpng -lSOIL -lfreetype -lftgl
UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
	MACOSX_DEPLOYMENT_TARGET = 10.8
        CFLAGS += -I/opt/local/include/ -I/usr/include/freetype2 -I/usr/include/FTGL -mmacosx-version-min=$(MACOSX_DEPLOYMENT_TARGET)
        LDFLAGS += -framework OpenGL -mmacosx-version-min=$(MACOSX_DEPLOYMENT_TARGET)
else
        LDFLAGS +=  -L/usr/lib -L/usr/X11R6/lib -lGL -lGLEW -lglfw -lassimp 
endif

PROGNAME = onedaydirectx
PACKAGE=$(PROGNAME)
SOURCES = sources/init.cpp sources/objloader.cpp sources/data.cpp sources/controls.cpp sources/shaders.cpp sources/main.cpp
OBJ = $(SOURCES:.cpp=.o)

all: $(PROGNAME)

$(PROGNAME): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $(PROGNAME)
	@echo 'Make done!'

%.o: %.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

