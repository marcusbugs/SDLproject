# Makefile for SDL2 physics game
# Marcus Cassell -- CSCI 221 Final Project
#
# Portable across Windows (MSYS2/mingw64), Linux, and macOS.
# Uses pkg-config to auto-detect the right SDL2 paths and link flags.

CC = g++
CFLAGS = -Wall -std=c++14 $(shell pkg-config --cflags sdl2)

ifeq ($(OS),Windows_NT)
    # Windows: keep .exe extension and force a console window so std::cout works.
    TARGET  = goblin-game.exe
    LDFLAGS = $(shell pkg-config --libs sdl2) -mconsole
else
    # Linux / macOS
    TARGET  = goblin-game
    LDFLAGS = $(shell pkg-config --libs sdl2)
endif

$(TARGET): main.o Application.o Goblin.o StaticFont.o Player.o Config.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o Application.o Goblin.o StaticFont.o Player.o Config.o $(LDFLAGS)

main.o: src/main.cpp src/Application.hpp src/Vec2.hpp src/Color.hpp gorbie/StaticFont.hpp src/Config.hpp src/Player.hpp
	$(CC) $(CFLAGS) -c src/main.cpp

StaticFont.o: gorbie/StaticFont.cpp gorbie/StaticFont.hpp
	$(CC) $(CFLAGS) -c "gorbie/StaticFont.cpp"

Application.o: src/Application.cpp src/Application.hpp src/Color.hpp src/Goblin.hpp src/Vec2.hpp src/Config.hpp src/Player.hpp
	$(CC) $(CFLAGS) -c src/Application.cpp

Goblin.o: src/Goblin.cpp src/Goblin.hpp src/Vec2.hpp src/Color.hpp
	$(CC) $(CFLAGS) -c src/Goblin.cpp

Player.o: src/Player.cpp src/Player.hpp src/Vec2.hpp
	$(CC) $(CFLAGS) -c src/Player.cpp

Config.o: src/Config.cpp src/Config.hpp src/Color.hpp
	$(CC) $(CFLAGS) -c src/Config.cpp

clean:
	rm -f *.o $(TARGET)

run: $(TARGET)
	./$(TARGET)

