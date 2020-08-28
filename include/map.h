#ifndef MAP_H_
#define MAP_H_

#include "game.h"
#include "texture.h"
#include "player.h"

class Map {
    SDL_Rect pos, sprites[70];
    SDL_Texture *tiles;

    std::vector<std::vector<int>> map;

    std::vector<std::pair<int,int>> colliders;
public:
    Map(std::string, int);
    ~Map();

    void render(SDL_Rect);
    void magicweb(Player*, int);

    std::vector<std::pair<int,int>> getColliders();
    void addCollider(std::pair<int,int>);
};

#endif // MAP_H_
