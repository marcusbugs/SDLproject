/*Player header file
*
 * Created by MJC on 5/7/26
 *
 */

#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "Vec2.hpp"
#include <SDL2/SDL.h>

class Player {
    private:
        Vec2 position;
        Vec2 velocity;
        Vec2 facing;
        float radius;
        float speed;
        int hp;
        int maxHp;
        SDL_Texture* texture;
        bool attack;

    public:
        Player(Vec2 pos, float r, float s, int maxHp_);
        ~Player();

        bool loadTexture(SDL_Renderer* renderer, const char* path);

        void input(const Uint8* keys);
        void update(float dt);
        void render(SDL_Renderer* renderer);

        Vec2 getPosition() {return position;};
        Vec2 getVelocity() {return velocity;};
        float getRadius() {return radius;};
        int getHp() {return hp;};
        int getMaxHp() {return maxHp;};

        void setPosition(Vec2 pos);
        void setVelocity(Vec2 vel);
        void takeDamage(int damage);
};


#endif //PLAYER_HPP