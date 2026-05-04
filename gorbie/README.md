# `gn::StaticFont` and `gn::Text` Documentation

A lightweight pixel-font rendering system for SDL2 without `SDL_ttf` dependencies.

## `StaticFont`

Static utility for font initialization and immediate-mode rendering.

### Usage

```c++
// 1. Initialize once
gn::StaticFont::initialize(renderer);

// 2. Immediate-mode rendering
gn::StaticFont::setScale(2);
gn::StaticFont::setColor(255, 255, 255);
gn::StaticFont::render(renderer, "Hello World", {100, 100});

// 3. Create persistent Text object (efficient)
gn::Text myText = gn::StaticFont::createText("Persistent Text", 2);
myText.pos = {200, 200};

// 4. Cleanup
gn::StaticFont::destroy();
```

### Functions

| Function                                               | Description                               |
|--------------------------------------------------------|-------------------------------------------|
| `initialize(SDL_Renderer*);`                           | Initializes font resources.               |
| `render(SDL_Renderer*, const char[], SDL_Point);`      | Draws text directly to the render target. |
| `createText(const char[], uint32_t scale, SDL_Color);` | Creates a `Text` object.                  |
| `setColor(uint8_t r, g, b, a = 255);`                  | Sets global color for `render`.           |
| `setScale(uint32_t);`                                  | Sets global scale for `render`.           |
| `destroy();`                                           | Releases font resources.                  |

---

## `Text`

Pre-rendered text block. Supports rotation, scaling, and flipping. More performant for static content.

### Usage

```c++
gn::Text label = gn::StaticFont::createText("Goblin", 2);
label.setColor(255, 0, 0);  // Red
label.flipHorizontal();     // Flip
label.pos = {100, 100};     // Position
label.angle = 45.0f;        // Rotate
label.render(renderer);

// Cloning and Moving
gn::Text label2 = label.clone(renderer);
gn::Text label3 = std::move(label2);

gn::StaticFont::destroy();
```

### Functions

| Function | Description |
|----------|-------------|
| `render(SDL_Renderer*) const;` | Renders the text with current state. |
| `clone(SDL_Renderer*) const;` | Returns a copy of the text object. |
| `setColor(uint8_t r, g, b, a = 255) const;` | Sets text color/alpha. |
| `flipHorizontal();` | Flips rendering horizontally. |
| `flipVertical();` | Flips rendering vertically. |
| `setScale(uint32_t);` | Sets uniform scale. |
| `setScale(std::pair<uint32_t, uint32_t>);` | Sets independent X/Y scale. |
| `center(SDL_Rect);` | Centers `pos` within a rectangle. |
| `getWidth() const;` | Returns scaled width. |
| `getHeight() const;` | Returns scaled height. |
