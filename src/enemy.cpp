#include "../include/enemy.h"

// each rank has an int value E(2) D(3) C(4) B(5) A(6) S(7)
Enemy::Enemy(const char *file, int init_hp, std::string name, int givenrank)
{
    enemy_name = name;
    
    enemy_texture = Texture::loadIMG(file);
    SDL_SetTextureBlendMode(enemy_texture, SDL_BLENDMODE_BLEND);
    
    battle_pos = {269, 71, 262, 262};

    hp = {init_hp, init_hp};
    rank = givenrank;

    enemy_tick = SDL_GetTicks();
}

Enemy::~Enemy()
{
    SDL_DestroyTexture(enemy_texture);
}

void Enemy::render_in_battle()
{
    if (SDL_GetTicks() - enemy_tick > 100) SDL_SetTextureAlphaMod(enemy_texture, 255);
    SDL_RenderCopy(Game::renderer, enemy_texture, NULL, &battle_pos);
}

std::string Enemy::getname()
{
    return enemy_name;
}

std::pair<int,int> Enemy::gethp()
{
    return hp;
}

void Enemy::dmg(int new_hp)
{
    SDL_SetTextureAlphaMod(enemy_texture, 0);
    enemy_tick = SDL_GetTicks();
    hp.second -= (new_hp > 0) ? new_hp : 0;
}

void Enemy::reset_hp()
{
    hp.second = hp.first;
}

int Enemy::getrank()
{
    return rank;
}

int Enemy::expfromdeath()
{
    return pow(rank, 5);
}
