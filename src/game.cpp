#include "../include/game.h"
#include "../include/texture.h"
#include "../include/player.h"
#include "../include/map.h"
#include "../include/interface.h"
#include "../include/enemy.h"

SDL_Renderer *Game::renderer = NULL;

Map *map = NULL;
Player *player = NULL;
Interface *interface = NULL;

Enemy *wolf = NULL;
Enemy *goblin = NULL;
Enemy *leprechaun = NULL;
Enemy *mud_golem = NULL;
Enemy *slime = NULL;

std::vector<Enemy*> enemies_list;

Game::Game(const char* window_name, int height, int width)
: running(false)
{
    srand(time(NULL));
    game_tick = SDL_GetTicks();

    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        window = SDL_CreateWindow(window_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, height, width, SDL_WINDOW_SHOWN);
        
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        running = true;
    }

    TTF_Init();

    map = new Map("assets/reborn.map", 99);
    player = new Player("assets/entities/character.png", 32, 32);

    interface = new Interface(player);
    
    wolf = new Enemy("assets/entities/wolf.png", 20, "Wolf", 2);
    goblin = new Enemy("assets/entities/goblin.png", 20, "Goblin", 2);
    leprechaun = new Enemy("assets/entities/leprechaun.png", 30, "Leprechaun", 2);
    mud_golem = new Enemy("assets/entities/mud_golem.png", 50, "Mud Golem", 2);
    slime = new Enemy("assets/entities/slime.png", 10, "Slime", 2);

    enemies_list = {
        wolf,
        goblin,
        leprechaun,
        mud_golem,
        slime,
    };
}

Game::~Game()
{
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    delete map;
    delete player;

    delete interface;
    
    for (auto enemy : enemies_list)
        delete enemy;
}

SDL_Window* Game::getWindow()
{
    return window;
}

bool Game::isRunning()
{
    return running;
}

void Game::update()
{
    player->move(map->getColliders());

    if (player->isInDungeon() && player->moved() && rand() % 21 == 10) {
        interface->toggle_battle(true, enemies_list[rand() % 5]);
    }

    if (!player->isInAction()) {
        if (SDL_GetTicks() - player->regen_time() > 2000)
            player->regen();
    }

    map->magicweb(player, interface->get_current_quest());
    
    interface->quest();
}

void Game::render()
{
    // main viewport
    SDL_Rect viewport = {0, 0, 800, 608};

    // settings
    SDL_RenderSetViewport(renderer, &viewport);
    SDL_SetRenderDrawColor(renderer, 0x33, 0x70, 0xd9, 0xFF);

    SDL_RenderClear(renderer);
    
    map->render(player->getcam());

    // player
    if (!player->isInAction())
        player->render();

    // interface viewport
    interface->render();

    SDL_RenderPresent(renderer);
}

void Game::event_loop() {
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT)
            running = false;
    }

    const Uint8 *currentState = SDL_GetKeyboardState(NULL);

    player->events(currentState);
    interface->events(currentState);
}
