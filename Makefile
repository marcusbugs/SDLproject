CXX = g++
CXXFLAGS = -Wall -std=c++17 $(shell pkg-config --cflags sdl2)
LDFLAGS  = $(shell pkg-config --libs sdl2)

ifeq ($(OS),Windows_NT)
    TARGET = sdl2-test.exe
    RM = del /Q /F
else
    TARGET = sdl2-test
    RM = rm -f
endif



all: $(TARGET)

$(TARGET): main.o
	$(CXX) main.o -o $@ $(LDFLAGS)

main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) main.o $(TARGET)

.PHONY: run
run: $(TARGET)
	./$(TARGET)