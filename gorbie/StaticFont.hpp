#ifndef GORDIE_STATIC_FONT_MAPPING_HPP
#define GORDIE_STATIC_FONT_MAPPING_HPP

// Holds the interface for rendering a pixel-font with the SDL Renderer without an SDL_TTF dependency.
// Created by Gordie Novak, 2026, on a thursday (i forget which one)

// free to distribute/change

#include <SDL.h>
#include <cstdint>

namespace gord {

    class StaticFont {
    public:
        /** Initializes the font.
         * @warning Remember to call @code StaticFont::destroy()@endcode on cleanup.*/
        static void initialize(SDL_Renderer* renderer);

        /** Renders a given piece of text to the provided position.
         * @param renderer SDL_Renderer instance
         * @param text The text you want to render
         * @param pos The position you want to render the text at.
         * @param scale The scale of the text.
         * @warning This function does not check if the font is intialized yet. Please ensure to call @code initialize
         @endcode first. 
         * @note Newlines '\\n' function, but tabs '\\t' don't.
         * @code SDL_SetRenderTarget(renderer, nullptr)@endcode if you want to render to the window. */
        static void renderText(SDL_Renderer* renderer, const char text[], SDL_Point pos, int scale = 1);

        /** Sets the color of the text you will render!
         * @param r 'Red' {0 - 255}
         * @param g 'Green' {0 - 255}
         * @param b 'Blue' {0 - 255}
         * @param a 'Alpha' (Transparency) */
        static void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

        /** Cleans up font resources */
        static void destroy();

    private:
        static SDL_Texture* font_texture;

        // This is not a class you construct. 
        StaticFont() = delete;
    };

}


#endif