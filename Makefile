# Makefile for SDL2 physics game
# Marcus Cassell -- CSCI 221 Final Project
#
# Portable across Windows (MSYS2/mingw64), Linux, and macOS.
# Uses pkg-config to auto-detect the right SDL2 paths and link flags.

CC = g++
CFLAGS = -Wall -std=c++14 $(shell pkg-config --cflags sdl2)

ifeq ($(OS),Windows_NT)
    # Windows: keep .exe extension and force a console window so std::cout works.
    TARGET  = sdl2-test.exe
    LDFLAGS = $(shell pkg-config --libs sdl2) -mconsole
else
    # Linux / macOS
    TARGET  = sdl2-test
    LDFLAGS = $(shell pkg-config --libs sdl2)
endif

$(TARGET): main.o Application.o Object.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o Application.o Object.o $(LDFLAGS)

main.o: src/main.cpp src/Application.hpp src/Vec2.hpp src/Color.hpp
	$(CC) $(CFLAGS) -c src/main.cpp

Application.o: src/Application.cpp src/Application.hpp src/Color.hpp
	$(CC) $(CFLAGS) -c src/Application.cpp

Object.o: src/Object.cpp src/Object.hpp src/Vec2.hpp
	$(CC) $(CFLAGS) -c src/Object.cpp

clean:
	rm -f *.o $(TARGET)

run: $(TARGET)
	./$(TARGET)
