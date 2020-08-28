#ifndef ENEMY_H_
#define ENEMY_H_

#include "game.h"
#include "texture.h"

class Enemy {
    std::string enemy_name;

    SDL_Texture *enemy_texture;
    SDL_Rect battle_pos;

    std::pair<int, int> hp;
    int rank;
public:
    Enemy(const char*, int, std::string, int);
    ~Enemy();

    void render_in_battle();

    // status
    std::string getname();

    std::pair<int,int> gethp();
    void dmg(int);
    void reset_hp();

    int getrank();
    int expfromdeath();
};

#endif // ENEMY_H_
