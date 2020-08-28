#include "../include/enemy.h"

Enemy::Enemy(const char *file, int init_hp, std::string name, int givenrank)
{
    enemy_name = name;
    enemy_texture = Texture::loadIMG(file);
    battle_pos = {269, 71, 262, 262};

    hp = {init_hp, init_hp};
    rank = givenrank;
}

Enemy::~Enemy()
{
    SDL_DestroyTexture(enemy_texture);
}

void Enemy::render_in_battle()
{
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
