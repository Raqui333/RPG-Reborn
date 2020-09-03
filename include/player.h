#ifndef ENTITY_H_
#define ENTITY_H_

#include "game.h"
#include "texture.h"

class Player {
    bool key_lock;
    int moveX, moveY, sqmX, sqmY, velocity;

    SDL_Texture *player_texture;
    SDL_Rect pos, npos;

    bool is_in_action, moving;
    
    int current_frame, next_frame;
    int FPS, UP, LEFT, DOWN, RIGHT;

    void anime_sprite(int);

    SDL_Rect camera;

    // rpg
    std::pair<int,int> hp, mp, exp;
    int level, strength;

    std::pair<bool, int> dungeon;

    // time events
    Uint32 regen_tick;
public:
    Player(const char*, int, int);
    ~Player();

    void events(const Uint8*);
    void move(std::vector<std::pair<int,int>>);
    bool moved();
    
    SDL_Rect getpos();
    void setpos(int, int);

    // interface
    friend class Interface;

    SDL_Rect getcam();

    void render();
    bool isInAction();

    // status
    void kill();
    std::pair<int, int> gethp();

    void heal(int);
    void dmg(int);

    void regen();

    std::pair<int, int> getmp();
    void mpused(int);

    int getlvl();
    
    int getstr();

    std::pair<int, int> getexp();
    bool setexp(int);

    void setDungeon(bool, int);
    bool isInDungeon();
    int getDungeonRank();

    // time events
    Uint32 regen_time();
};

#endif // ENTITY_H_
