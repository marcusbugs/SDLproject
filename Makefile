CXX = g++
CXXFLAGS = -Wall -std=c++14 $(shell pkg-config --cflags sdl2)
LDFLAGS  = $(shell pkg-config --libs sdl2)

ifeq ($(OS),Windows_NT)
    TARGET = sdl2-test.exe
else
    TARGET = sdl2-test
endif

all: $(TARGET)

# Auto-discover all .cpp files in src/
SOURCES = $(wildcard src/*.cpp)
OBJS    = $(notdir $(SOURCES:.cpp=.o))

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Pattern rule: any X.o is built from src/X.cpp
%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o $(TARGET)

.PHONY: run
run: $(TARGET)
	./$(TARGET)