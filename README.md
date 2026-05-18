# Goblin Game CS 221

Author: Marcus Cassell

Course: CSCI 221, Spring 2026

A sdl 2 game where you hunt goblins

---

## Must be run locally (not over SSH)

There is a window popup and won't popup over ssh

---
## Building and running

```bash
make            # builds the game
make run        # builds AND runs it
make clean      # cleans it
```

The make file will auto-detect OS and SDL2 installation with pkg-config.

### Running

You can pass a config file when running like so:

```bash
./goblin-game my_config.txt
```

If no argument is given it will run with defaults defined in config.cpp


### Controls

- W A S D move the player
- Left click — attack any goblin within range (red mean in range)

---

## Where the grading criteria are

### 1. User-created data structure (stack)
- `src/Node.hpp` and `src/Node.cpp` the node class. Stores times as Uint32 type
- `src/KillStack.hpp` and `src/KillStack.cpp` are the linked-list stack
  class.

### 2. Sorting algorithm (insertion sort)
- `src/Application.cpp` / `Application::sortGoblinsByDistance()`
  at the bottom of the file
- after the sort, `goblins[0]` is the closest goblin
  and gets a red outline

### 3. File I/O
- `src/Config.hpp` and `src/Config.cpp`  the Config struct and
  its `loadConfig(const char* path)` method
- uses sdl get line and reads key = value type arguments
- Example config.txt in the project root.

