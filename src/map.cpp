#include "../include/map.h"

Map::Map(std::string path, int map_width)
{
    tiles = Texture::loadIMG("assets/tiles/ground.png");

    // load map from file
    std::ifstream file(path);
    if (file.is_open()) {
        std::string line;
        std::vector<int> tmp;
        int count = 0;
        
        while (std::getline(file, line, ',')) {
            tmp.push_back(std::stoi(line));
            if (count == map_width) {
                map.push_back(tmp);
                tmp.clear();
                count = 0;
            } else {
                count++;
            }
        }
        
        file.close();
    }

    // set colliders
    for (int line = 0; line < map.size(); line++) {
        for (int col = 0; col < map[0].size(); col++) {
            if (map[line][col] <= 0) {
                map[line][col] *= -1;
                colliders.push_back({col * 32, line * 32});
            }
        }
    }

    // clipping sprite set
    for (int tile = 0; tile < sizeof(sprites)/16; tile++) {
        for (int y = 1; y <= 199; y += 33) {
            for(int x = 1; x <= 298; x += 33) {
                sprites[tile] = {x, y, 32, 32};
                tile++;
            }
        }
    }

    pos = {0, 0, 32, 32};
}

Map::~Map()
{
    SDL_DestroyTexture(tiles);
}

void Map::render(SDL_Rect camera)
{
    int tile_type = 0;

    for (int line = 0; line < map.size(); line++) {
        for (int col = 0; col < map[0].size(); col++) {
            pos.x = col * 32 - camera.x;
            pos.y = line * 32 - camera.y;

            tile_type = map[line][col];

            SDL_RenderCopy(Game::renderer, tiles, &sprites[tile_type], &pos);
        }
    }
}

std::vector<std::pair<int,int>> Map::getColliders()
{
    return colliders;
}

void Map::addCollider(std::pair<int,int> new_collider)
{
    colliders.push_back(new_collider);
}

void Map::magicweb(Player* player, int current_quest)
{
    int x = player->getpos().x;
    int y = player->getpos().y;
    int quest = current_quest;

    if (x == 800 && y == 960 && quest > 1) {
        player->setpos(1792, 2432);
    } else if (x == 2144 && y == 1952) {
        // dungeon rank E gate, enter
        player->setpos(2304, 512);
        player->setDungeon(true, 0);
    } else if (x == 2304 && y == 544) {
        // dungeon rank E gate, leave
        player->setpos(2144, 1984);
        player->setDungeon(false, 0);
    }
}

