#include "StaticFont.hpp"
#include <SDL_surface.h>
#include <SDL_render.h>
#include <array>
#include <iostream>

// *****************************
// TEXT GOOBLERS
// ****************************

void gn::Text::render(SDL_Renderer* renderer) const noexcept {
	SDL_RenderCopyEx(
		renderer,
		texture,
		nullptr,
		reinterpret_cast<const SDL_Rect*>(&pos),
		angle,
		nullptr,
		flip
	);
}

gn::Text gn::Text::clone(SDL_Renderer* renderer) const noexcept {
	SDL_Texture* clone = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, r_width, r_height);

	Uint8 r, g, b, a;
	SDL_GetTextureColorMod(texture, &r, &g, &b);
	SDL_GetTextureAlphaMod(texture, &a);

	setColor(255, 255, 255, 255);

	SDL_Texture* user_render_target = SDL_GetRenderTarget(renderer);
	SDL_SetRenderTarget(renderer, clone);	//< Swap render targets
	SDL_RenderCopy(
		renderer,
		texture,
		nullptr,
		nullptr
	);
	SDL_SetRenderTarget(renderer, user_render_target); //< Swap it back to whatever the user had.

	setColor(r, g, b, a);

	return Text(clone, {r, g, b, a}, static_cast<int>(this->bw), static_cast<int>(this->bh), sx, sy, flip, angle, pos);
}

void gn::Text::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) const noexcept {
	SDL_SetTextureColorMod(texture, r, g, b);
	SDL_SetTextureAlphaMod(texture, a);
}

void gn::Text::flipHorizontal() noexcept {
	flip = SDL_FLIP_HORIZONTAL;
}

void gn::Text::flipVertical() noexcept {
	flip = SDL_FLIP_VERTICAL;
}

void gn::Text::setScale(uint32_t scale) noexcept {
	r_width		= static_cast<int>(bw * scale);
	r_height	= static_cast<int>(bh * scale);
	sx			= static_cast<int>(scale);
	sy			= static_cast<int>(scale);
}

void gn::Text::setScale(std::pair<uint32_t, uint32_t> scale) noexcept {
	r_width  = static_cast<int>(scale.first * bw);
	r_height = static_cast<int>(scale.second * bh);
	sx       = static_cast<int>(scale.first);
	sy       = static_cast<int>(scale.second);
}

void gn::Text::center(const SDL_Rect region) noexcept {
	pos.x = region.x + (region.w - r_width)/2;
	pos.y = region.y + (region.h - r_height)/2;
}

int gn::Text::getWidth() const noexcept {
	return r_width;
}

int gn::Text::getHeight() const noexcept {
	return r_height;
}

// Initializer order matches member declaration order in StaticFont.hpp
// (angle, pos, r_width, r_height, texture, bw, bh, sx, sy, flip).
// C++ initializes members in declaration order regardless; matching here
// silences the -Wreorder warning.
gn::Text::Text(Text&& o) noexcept :
	angle(o.angle),
	pos(o.pos),
	r_width(o.r_width),
	r_height(o.r_height),
	texture(o.texture),
	bw(o.bw),
	bh(o.bh),
	sx(o.sx),
	sy(o.sy),
	flip(o.flip)
{
	o.texture = nullptr;
}

gn::Text& gn::Text::operator=(Text&& o) noexcept {
	std::swap(angle, o.angle);
	std::swap(pos, o.pos);
	std::swap(r_width, o.r_width);
	std::swap(r_height, o.r_height);
	std::swap(sx, o.sx);
	std::swap(sy, o.sy);
	std::swap(flip, o.flip);
	std::swap(bw, o.bw);
	std::swap(bh, o.bh);
	std::swap(texture, o.texture);
	return *this;
}

gn::Text::~Text() {
	if (texture) {
		SDL_DestroyTexture(texture);
	}
}

gn::Text::Text(
			SDL_Texture* tex,
			const SDL_Color col,
			const int base_w,
			const int base_h,
			const uint32_t scale_x,
			const uint32_t scale_y,
			const SDL_RendererFlip flip,
			const float angle,
			const SDL_Point pos
)
	:	angle(angle),
		pos(pos),
		r_width(base_w),
		r_height(base_h),
		texture(tex),
		bw(base_w),
		bh(base_h),
		sx(static_cast<int>(scale_x)),
		sy(static_cast<int>(scale_y)),
		flip(flip)
{
	r_width *= this->sx;
	r_height *= this->sy;
	// Set the texture to be the color that we want.
	SDL_SetTextureColorMod(texture, col.r, col.g, col.b);
	SDL_SetTextureAlphaMod(texture, col.a);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
}









// *****************************
// FONT GOOBLERS
// ****************************

// STATIC VARIABLE INITIALIZATION
SDL_Renderer*	gn::StaticFont::sdl_renderer = nullptr;
SDL_Texture*	gn::StaticFont::font_texture = nullptr;
uint32_t		gn::StaticFont::font_scale = 1;
SDL_Color		gn::StaticFont::font_color{255, 255, 255, 255};

// Width, height, and pitch of the final font render
static constexpr uint32_t fontWidth = 132, fontHeight = 28, pitch = 528;
// Compression & encoding o the font
static constexpr int fontEncoding[] =
{	5, (1 | 128u), 11, (1 | 128u), 7, (2 | 128u), 5, (1 | 128u), 8, (1 | 128u), 4, (1 | 128u), 37, (1 | 128u), 47, 
	5, (1 | 128u), 11, (1 | 128u), 6, (1 | 128u), 7, (1 | 128u), 4, (1 | 128u), 1, (1 | 128u), 1, (1 | 128u), 4, (1 | 128u), 37, (1 | 128u), 47, 
	1, (2 | 128u), 2, (3 | 128u), 3, (2 | 128u), 2, (3 | 128u), 2, (2 | 128u), 2, (2 | 128u), 2, (2 | 128u), 2, (3 | 128u), 6, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (2 | 128u), 1, (2 | 128u), 3, (2 | 128u), 3, (2 | 128u), 2, (3 | 128u), 3, (2 | 128u), 3, (2 | 128u), 2, (2 | 128u), 1, (2 | 128u), 2, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 1, (1 | 128u), 5, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (3 | 128u), 11, 
	3, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 1, (1 | 128u), 1, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 3, (1 | 128u), 3, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 3, (1 | 128u), 11, 
	1, (3 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 2, (1 | 128u), 1, (4 | 128u), 1, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 1, (1 | 128u), 1, (2 | 128u), 3, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 4, (1 | 128u), 2, (1 | 128u), 3, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 3, (1 | 128u), 3, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 12, 
	0, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 4, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 1, (1 | 128u), 1, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 5, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 13, 
	1, (3 | 128u), 1, (3 | 128u), 3, (2 | 128u), 2, (3 | 128u), 2, (2 | 128u), 2, (1 | 128u), 3, (3 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 1, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 2, (2 | 128u), 2, (3 | 128u), 3, (3 | 128u), 1, (1 | 128u), 3, (2 | 128u), 3, (2 | 128u), 2, (2 | 128u), 4, (1 | 128u), 4, (2 | 128u), 1, (2 | 128u), 2, (1 | 128u), 3, (1 | 128u), 2, (3 | 128u), 1, (3 | 128u), 11, 
	30, (1 | 128u), 8, (1 | 128u), 26, (1 | 128u), 7, (1 | 128u), 41, (1 | 128u), 15, 
	28, (2 | 128u), 8, (1 | 128u), 27, (1 | 128u), 7, (2 | 128u), 38, (2 | 128u), 16, 
	127, 5, 
	1, (2 | 128u), 2, (3 | 128u), 3, (2 | 128u), 1, (3 | 128u), 2, (3 | 128u), 1, (3 | 128u), 2, (2 | 128u), 2, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 1, (2 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 3, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 2, (2 | 128u), 2, (3 | 128u), 3, (2 | 128u), 2, (3 | 128u), 3, (2 | 128u), 1, (3 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 1, (3 | 128u), 10, 
	0, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 3, (1 | 128u), 4, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 3, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 4, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 3, (1 | 128u), 10, 
	0, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 3, (1 | 128u), 4, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 3, (2 | 128u), 1, (2 | 128u), 1, (2 | 128u), 1, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 4, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 11, 
	0, (4 | 128u), 1, (3 | 128u), 2, (1 | 128u), 3, (1 | 128u), 2, (1 | 128u), 1, (3 | 128u), 1, (3 | 128u), 1, (1 | 128u), 1, (2 | 128u), 1, (4 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (2 | 128u), 3, (1 | 128u), 3, (1 | 128u), 1, (1 | 128u), 1, (1 | 128u), 1, (1 | 128u), 1, (2 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (3 | 128u), 2, (1 | 128u), 2, (1 | 128u), 1, (3 | 128u), 3, (1 | 128u), 3, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 5, (1 | 128u), 4, (1 | 128u), 11, 
	0, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 3, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 3, (1 | 128u), 3, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 4, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 3, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (2 | 128u), 1, (2 | 128u), 2, (1 | 128u), 1, (1 | 128u), 4, (1 | 128u), 4, (1 | 128u), 11, 
	0, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 3, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 3, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 4, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 3, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 3, (1 | 128u), 3, (1 | 128u), 3, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 3, (1 | 128u), 3, (1 | 128u), 12, 
	0, (1 | 128u), 2, (1 | 128u), 1, (3 | 128u), 3, (2 | 128u), 1, (3 | 128u), 2, (3 | 128u), 1, (1 | 128u), 4, (2 | 128u), 2, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 1, (3 | 128u), 1, (1 | 128u), 3, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 2, (2 | 128u), 2, (1 | 128u), 5, (1 | 128u), 1, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (2 | 128u), 3, (1 | 128u), 3, (2 | 128u), 4, (1 | 128u), 3, (1 | 128u), 3, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 3, (1 | 128u), 3, (3 | 128u), 10, 
	127, 5, 
	1, (2 | 128u), 2, (2 | 128u), 2, (2 | 128u), 2, (2 | 128u), 2, (1 | 128u), 4, (3 | 128u), 2, (2 | 128u), 2, (3 | 128u), 2, (2 | 128u), 3, (2 | 128u), 3, (2 | 128u), 9, (2 | 128u), 1, (2 | 128u), 2, (1 | 128u), 1, (1 | 128u), 4, (1 | 128u), 1, (1 | 128u), 4, (2 | 128u), 2, (1 | 128u), 1, (5 | 128u), 43,
	0, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 4, (1 | 128u), 3, (1 | 128u), 1, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 3, (1 | 128u), 6, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 8, (1 | 128u), 3, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 3, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 1, (2 | 128u), 1, (2 | 128u), 43,
	0, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 4, (1 | 128u), 3, (1 | 128u), 1, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 3, (1 | 128u), 6, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 3, (1 | 128u), 4, (1 | 128u), 3, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 2, (1 | 128u), 3, (1 | 128u), 5, (1 | 128u), 1, (1 | 128u), 1, (1 | 128u), 1, (1 | 128u), 1, (1 | 128u), 43,
	0, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 3, (1 | 128u), 3, (1 | 128u), 2, (4 | 128u), 1, (2 | 128u), 2, (3 | 128u), 3, (1 | 128u), 3, (2 | 128u), 3, (3 | 128u), 1, (1 | 128u), 2, (1 | 128u), 8, (1 | 128u), 3, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 2, (1 | 128u), 3, (1 | 128u), 4, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 1, (1 | 128u), 1, (1 | 128u), 43,
	0, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 5, (1 | 128u), 3, (1 | 128u), 4, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 4, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 8, (1 | 128u), 3, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 2, (1 | 128u), 3, (1 | 128u), 3, (1 | 128u), 3, (1 | 128u), 1, (1 | 128u), 1, (1 | 128u), 1, (1 | 128u), 43,
	0, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 2, (1 | 128u), 5, (1 | 128u), 3, (1 | 128u), 4, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 2, (1 | 128u), 4, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 3, (1 | 128u), 4, (1 | 128u), 3, (1 | 128u), 1, (1 | 128u), 3, (1 | 128u), 1, (1 | 128u), 5, (1 | 128u), 8, (2 | 128u), 1, (2 | 128u), 43,
	1, (2 | 128u), 2, (3 | 128u), 1, (3 | 128u), 1, (2 | 128u), 4, (1 | 128u), 2, (2 | 128u), 3, (2 | 128u), 2, (1 | 128u), 4, (2 | 128u), 3, (2 | 128u), 3, (2 | 128u), 2, (1 | 128u), 4, (1 | 128u), 1, (2 | 128u), 1, (2 | 128u), 2, (1 | 128u), 1, (1 | 128u), 2, (1 | 128u), 5, (1 | 128u), 2, (1 | 128u), 3, (1 | 128u), 1, (5 | 128u), 43,
	54, (1 | 128u), 77,
	127, 5,
	127, 5,
};



// Rectangles that bind to the src texture for each letter. 
static constexpr std::array<SDL_Rect, 75> character_rects = {
    SDL_Rect{0, 0, 4, 9},	// a
    {5, 0, 4, 9},		// b
    {10, 0, 3, 9},		// c
    {14, 0, 4, 9},		// d
    {19, 0, 4, 9},		// e
    {24, 0, 3, 9},		// f
	{27, 0, 4, 9},		// g
    {32, 0, 4, 9},
    {37, 0, 1, 9},		// i
	{38, 0, 2, 9},		// j
	{41, 0, 4, 9},		// k
    {46, 0, 1, 9},
    {48, 0, 7, 9},
    {56, 0, 4, 9},
    {61, 0, 4, 9},
    {66, 0, 4, 9},
    {71, 0, 5, 9},
	{76, 0, 3, 9},
    {80, 0, 3, 9},		// s
    {84, 0, 3, 9},
    {88, 0, 4, 9},
    {93, 0, 5, 9},		// v
    {99, 0, 7, 9},
    {107, 0, 5, 9},
    {113, 0, 4, 9},
    {118, 0, 3, 9},		// z
    {0, 10, 4, 7},		// A
    {5, 10, 4, 7},
    {10, 10, 3, 7},
    {14, 10, 4, 7},
    {19, 10, 3, 7},
    {23, 10, 3, 7},
    {27, 10, 4, 7},
    {32, 10, 4, 7},		// H
    {37, 10, 1, 7},
    {39, 10, 2, 7},
    {42, 10, 4, 7},
    {47, 10, 3, 7},
    {51, 10, 5, 7},
    {57, 10, 4, 7},
    {62, 10, 4, 7},
    {67, 10, 4, 7},		// P
    {72, 10, 4, 7},
    {77, 10, 4, 7},
    {82, 10, 3, 7},
    {86, 10, 3, 7},
    {90, 10, 4, 7},
    {95, 10, 5, 7},		// V
    {101, 10, 5, 7},
    {107, 10, 5, 7},
    {113, 10, 5, 7},
    {119, 10, 3, 7},		// Z
    {0, 18, 4, 8},		// 0
    {5, 18, 3, 8},		// 1
    {9, 18, 3, 8},		// 2
    {13, 18, 3, 8},		// 3
    {17, 18, 4, 8},		// 4
    {22, 18, 3, 8},		// 5
    {26, 18, 4, 8},		// 6
    {31, 18, 3, 8},		// 7
    {35, 18, 4, 8},		// 8
    {40, 18, 4, 8},		// 9
    {45, 18, 4, 8},		// .
    {50, 18, 1, 8},		// :
    {52, 18, 1, 8},		// ,
    {54, 18, 2, 8},		// [
    {57, 18, 2, 8},		// ]
    {60, 18, 2, 8},		// (
    {63, 18, 2, 8},		// )
    {66, 18, 2, 8},		// /
    {69, 18, 3, 8},		// '\'
    {73, 18, 3, 8},		// ?
    {77, 18, 4, 8},		// ?
    {82, 18, 1, 8},
	{0, 0, 4, 0} // space
};

// Constructs a c-style array into a constexpr std::array
template <typename T, std::size_t N, std::size_t... I>
	constexpr std::array<T, N> to_array_constexpr(const T (&raw)[N], std::index_sequence<I...>) {
	return {{ raw[I]... }};
}

// Creates a link table between characters and their respective character rectangles. (performance optimization)
constexpr std::array<const SDL_Rect*, 256> create_link_table() {
	const SDL_Rect* link_table[256]{};

	for (int i = 'a'; i <= 'z'; ++i)
		link_table[i] = &character_rects[i - 'a'];
	for (int i = 'A'; i <= 'Z'; ++i)
		link_table[i] = &character_rects[i - 'A' + 26];
	for (int i = '0'; i <= '9'; ++i)
		link_table[i] = &character_rects[i - '0' + 52];

	link_table['.'] = &character_rects[63];
	link_table[':'] = &character_rects[64];
	link_table[','] = &character_rects[65];
	link_table['['] = &character_rects[66];
	link_table[']'] = &character_rects[67];
	link_table['('] = &character_rects[68];
	link_table[')'] = &character_rects[69];
	link_table['/'] = &character_rects[70];
	link_table['\\'] = &character_rects[71];
	link_table['?'] = &character_rects[72];
	link_table['!'] = &character_rects[73];
	link_table[' '] = &character_rects[74];

	return to_array_constexpr(link_table, std::make_index_sequence<256>{});
}

static constexpr auto link_table = create_link_table();


[[nodiscard]] SDL_Surface* decodeFontDataToSurface() {
	SDL_Surface* surf =
		SDL_CreateRGBSurfaceWithFormat(
			0,	// flags
			fontWidth, fontHeight,	// width and height
			1,
			SDL_PIXELFORMAT_RGBA8888 // depth
	);

	// Static cast our surface to unsigned ints so it's easier to update (usually void*)
	auto* pixels = static_cast<uint32_t*>(surf->pixels);
	const int expected_pixels = surf->w * surf->h;

	// Loop through and fill in relevant pixels.
	int pixel_idx = 0;
	for (const uint_fast8_t val : fontEncoding) {
		// If our pixel is currently filled or not
		const bool fill = val & 128u;
		// The number of times we fill in pixels.
		const uint32_t count = val & 127u;

		for (size_t i = 0; i < count && pixel_idx < expected_pixels; i++) {
			pixels[pixel_idx++] = fill ? 0xFFFFFFFF : 0x00000000;
		}
	}

	// Return the surface back to the user. 
	return surf;
}


void gn::StaticFont::initialize(SDL_Renderer* renderer) noexcept {
	// initialized ensures we don't accidentally not destroy a texture
	// we aren't guaranteed font_texture is initialized because it is a static member var.
	static bool initialized = false;
	if (initialized && font_texture != nullptr) {
		SDL_DestroyTexture(font_texture);
	}
	initialized = true;

	SDL_Surface* surf = decodeFontDataToSurface();
	font_texture = SDL_CreateTextureFromSurface(renderer, surf);
	sdl_renderer = renderer;
	SDL_FreeSurface(surf);

	SDL_SetTextureBlendMode(font_texture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(font_texture, 255);
	SDL_SetTextureColorMod(font_texture, 255, 255, 255);

}

void gn::StaticFont::render(SDL_Renderer* renderer, const char text[], const SDL_Point pos) noexcept {
	SDL_Rect dst = { pos.x, pos.y, 0, 0 };
	const size_t len = strlen(text);

	// First iterate through all of our characters.
	for (size_t i = 0; i < len; i++) {
		const char c = text[i];

		// STEP 1: Grab the src rectangle.
		if (const SDL_Rect* src = link_table[c]) {
			dst.w = src->w * font_scale;
			dst.h = src->h * font_scale;
			SDL_RenderCopy(renderer, font_texture, src, &dst);
			dst.x += (src->w + 1) * font_scale;
		} else if (c == '\n') {
			// Otherwise if newline, move draw cursor down
			dst.x = pos.x;
			dst.y += character_rects[26].h * (font_scale*1.25f);
		}
	}
}
SDL_Point getTextSize(const char text[]) {
	int tot_y{}, curr_y{};
	int max_x{}, curr_x{};
	const size_t len = strlen(text);

	for (size_t i = 0; i < len; i++) {
		const char c = text[i];
		if (const SDL_Rect* r = link_table[c]) {
			curr_y = std::max(curr_y, r->h);
			curr_x += (r->w + 1);
		} else if (c == '\n') {
			max_x = std::max(max_x, curr_x);
			tot_y += curr_y;
			curr_x = 0;
			curr_y = 0;
		}
	}

	tot_y += curr_y;
	max_x = std::max(max_x, curr_x);

	return { max_x, tot_y };
}

gn::Text gn::StaticFont::createText(const char text[], const uint32_t scale, const SDL_Color color) {
	// First reset so we don't run into issue.
	SDL_SetTextureColorMod(font_texture, 255, 255, 255);
	SDL_SetTextureAlphaMod(font_texture, 255);

	const SDL_Point dim = getTextSize(text);
	SDL_Texture* tex = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dim.x, dim.y);
	if (!tex) {
		throw std::runtime_error("gn::StaticFont::createText(): Could not create texture for font.");
	}
	SDL_Texture* user_target = SDL_GetRenderTarget(sdl_renderer);
	SDL_SetRenderTarget(sdl_renderer, tex);

	// Save the scale for later.
	const auto temp_scale = font_scale;
	setScale(1);

	render(sdl_renderer, text, { 0, 0 });

	setColor(font_color.r, font_color.g, font_color.b, font_color.a);
	setScale(temp_scale);

	SDL_SetRenderTarget(sdl_renderer, user_target);

	return Text{tex, color, dim.x, dim.y, scale, scale};
}

void gn::StaticFont::setColor(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) noexcept {
	font_color = { r, g, b, a };
	SDL_SetTextureColorMod(font_texture, r, g, b);
	SDL_SetTextureAlphaMod(font_texture, a);
}

void gn::StaticFont::setScale(const uint32_t scale) noexcept {
	font_scale = static_cast<int>(scale);
}

void gn::StaticFont::destroy() noexcept {
	SDL_DestroyTexture(font_texture);
	font_texture = nullptr;
}

