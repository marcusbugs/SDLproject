#ifndef GORDIE_STATIC_FONT_MAPPING_HPP
#define GORDIE_STATIC_FONT_MAPPING_HPP

// Holds the interface for rendering a pixel-font with the SDL Renderer without an SDL_TTF dependency.
// Created by Gordie Novak, 2026, on a thursday (i forget which one)

// free to distribute/change

#include <cstdint>
#include <SDL.h>
#include <utility>

// gordie novak's namespace
namespace gn {

    /** @brief Pre-rendered text object: very efficient. Create Text objects with the @code StaticFont::createText()@endcode
     * Function.
     *
     * @details Use @code Text@endcode objects rather than the StaticFont class for rendering as it is much more efficient
     * and has many nice functions and features. . */
    class Text {
    public:
    // ************ Member functions ***************
        void render(SDL_Renderer* renderer) const noexcept; ///< Renders the given text
        Text clone(SDL_Renderer* renderer) const noexcept;  ///< Clones the current piece of text.

        void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) const noexcept;
        void flipHorizontal() noexcept;                 ///< Flips text horizontally
        void flipVertical() noexcept;                   ///< Flips text vertically.

        void setScale(uint32_t scale) noexcept;         ///< Sets scale of the text evenly across x andy components.
        void setScale(std::pair<uint32_t, uint32_t> scale) noexcept;    ///< Sets scale of x and y individually.

        void center(SDL_Rect region) noexcept;          ///< Centers the text within a given region.

        [[nodiscard]] int getWidth() const noexcept;    ///< Gets the width of the textin pixels
        [[nodiscard]] int getHeight() const noexcept;   ///< Returns the height of the text in pixels.

    // *********** Constructors/Assignment Operators/Destructors ****************** //

        Text(const Text&)               = delete; ///< Copying is not allowed for object lifetime purposes.
        Text& operator=(const Text&)    = delete; ///< Copying is not allowed for object lifetime purposes.

        Text(Text&&)                    noexcept; ///< Move construction allowed.
        Text& operator=(Text&&)         noexcept; ///< Move assignment allowed.

        ~Text();                        ///< Destructor automatically cleans up texture resources.

    // **************** Member vars ****************** //

        float               angle;      ///< Angle of rotation of the text.
        SDL_Point           pos;        ///< Position of the text @warning DEV: Do not move location within class.
                                                                        // ^
    // ************** Private implementation details *****************  // |
    private:                                                            // ^
        int r_width, r_height;          ///< Scaled width/height  @warning DEV: Do not move location within the class.

        SDL_Texture*        texture;    ///< Texture that contains the rendered text.
        uint32_t            bw, bh;     ///< Base width/base height
        int                 sx, sy;     ///< Current Scale of the Text.
        SDL_RendererFlip    flip;       ///< Whether we are currently flipped or not.

        Text(
            SDL_Texture* tex,
            SDL_Color col,
            int base_w,
            int base_h,
            uint32_t scale_x,
            uint32_t scale_y,
            SDL_RendererFlip flip = SDL_FLIP_NONE,
            float angle = 0,
            SDL_Point pos = {0,0}
        );
        friend class StaticFont;
    };

    /** Holds resources for rendering a font */
    class StaticFont {
    public:
        /** Initializes the font.
         * @warning Remember to call @code StaticFont::destroy()@endcode on cleanup.*/
        static void initialize(SDL_Renderer* renderer) noexcept;

        /** Renders a given piece of text to the provided position.
         * @param text The text you want to render
         * @param pos The position you want to render the text at.
         * @warning This function does not check if the font is initialized yet. Please ensure to call @code initialize
         @endcode first. 
         * @note Newlines '\\n' work, but tabs '\\t' don't. Remember to call @code SDL_SetRenderTarget(renderer, nullptr)@endcode
         *  if you want to render to the window. Otherwise it will render to the current render target.  */
        static void render(SDL_Renderer*, const char text[], SDL_Point pos) noexcept;

        /** Renders a text object that can be used repeatedly. Much more performant than calling drawText
         * repeatedly. Also has access to rotation/scaling and other functions
         * @param text The text you want to render
         * @note Initializes all text to be of scale 1 with color {255, 255, 255, 255}. */
        [[nodiscard]] static Text createText(const char text[], uint32_t scale = 1, SDL_Color color = {255, 255, 255, 255});

        /** Sets the color of the text you will render!
         * @param r 'Red' {0 to 255}
         * @param g 'Green' {0 to 255}
         * @param b 'Blue' {0 to 255}
         * @param a 'Alpha' (Transparency) {0 to 255}*/
        static void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) noexcept;

        /** Sets the scale of the text being rendered to the display.
         * @warning Don't set the scale too high or the program may lag (try to keep <100)*/
        static void setScale(uint32_t scale) noexcept;

        /** Cleans up font resources */
        static void destroy() noexcept;

        // This is not a class you construct.
        StaticFont() = delete;
    public:
        static SDL_Renderer* sdl_renderer;  ///< Stores the renderer so we don't get mismatched renderers.
        static SDL_Texture* font_texture;   ///< Texture that stores font information
        static uint32_t font_scale;         ///< Current scale of the renderer.
        static SDL_Color font_color;        ///< Current color of the font.
    };

}


#endif