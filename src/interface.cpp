#include "../include/interface.h"

Battle *battle = NULL;

Interface::Interface(Player* obj)
{
    interface_tick = SDL_GetTicks();

    main_interface = Texture::loadIMG("assets/interface/main.png");

    action_interface = Texture::loadIMG("assets/interface/action.png");
    action_on = false;

    viewport = {800, 0, 288, 608};

    player = obj;

    to_completed = {0,0};

    current_quest = 0;

    quest_list = {
        &Interface::first_mission,
        &Interface::second_mission,
        &Interface::third_mission,
        &Interface::fourth_mission,
    };

    log = in_screen_log = "";
}

Interface::~Interface()
{
    SDL_DestroyTexture(main_interface);
    SDL_DestroyTexture(action_interface);
    delete battle;
}

void Interface::toggle_battle(bool onoff, Enemy* enemy = NULL)
{
    action_on = onoff;

    player->current_frame = player->DOWN;

    player->npos.x = (action_on) ? 745 : 384;
    player->npos.y = (action_on) ? 412 : 320;

    player->is_in_action = action_on;

    if (!action_on) player->key_lock = false;

    if (onoff) battle = new Battle(enemy);
}

void Interface::render()
{
    if (SDL_GetTicks() - interface_tick < 1000) {
        int size = 10, x = 400 - (in_screen_log.length() * size/2);
        Texture::drawText(in_screen_log.c_str(), {255,255,255}, size, x, 295);
    }

    // action interface
    if (action_on) {
        player->key_lock = true;
        SDL_RenderCopy(Game::renderer, action_interface, NULL, NULL);
        if (battle) battle->init(this);
    }

    // text proprieties
    SDL_Color fg = {150, 150, 150};
    int size = 10;

    // main interface
    SDL_RenderSetViewport(Game::renderer, &viewport);
    SDL_RenderCopy(Game::renderer, main_interface, NULL, NULL);

    // status
    std::string level = "Lv. " + std::to_string(player->getlvl());
    Texture::drawText(level.c_str(), fg, size, 203, 25);

    Texture::drawText(std::to_string(player->getstr()).c_str(), fg, size, 148, 70); // strength

    // bars
    SDL_Rect bar = {50, 128, 0, 15};
    float width_percent = 1.7;

    bar.w = (player->gethp().second * 100 / player->gethp().first) * width_percent;
    
    if (bar.w < 45)
        SDL_SetRenderDrawColor(Game::renderer, 0xaa, 0x00, 0x00, 0xAA);
    else if (bar.w < 50 * width_percent)
        SDL_SetRenderDrawColor(Game::renderer, 0xaa, 0xa8, 0x00, 0xAA);
    else // full life
        SDL_SetRenderDrawColor(Game::renderer, 0x17, 0xaa, 0x00, 0xAA);

    SDL_RenderFillRect(Game::renderer, &bar);

    Texture::drawText(std::to_string(player->gethp().second).c_str(), fg, size, 224, bar.y - 3); // health

    bar.y += 20;

    bar.w = (player->getmp().second * 100 / player->getmp().first) * width_percent;
    SDL_SetRenderDrawColor(Game::renderer, 0x33, 0x70, 0xd9, 0xAA);
    SDL_RenderFillRect(Game::renderer, &bar);

    Texture::drawText(std::to_string(player->getmp().second).c_str(), fg, size, 224, bar.y - 3); // mana

    bar.y += 20;
    bar.h = 10;

    bar.w = (player->getexp().second * 100 / player->getexp().first) * width_percent;
    SDL_SetRenderDrawColor(Game::renderer, 0xbf, 0x2a, 0x2a, 0xAA);
    SDL_RenderFillRect(Game::renderer, &bar); // exp

    // write quest log
    std::stringstream entry(log);
    std::string line;
    int y = 240;
    while (std::getline(entry, line)) {
        Texture::drawText(line.c_str(), fg, size, 20, y);
        y += 15;
    }
}

void Interface::set_screen_log(std::string slog)
{
    in_screen_log = slog;
    interface_tick = SDL_GetTicks();
}

void Interface::events(const Uint8 *state)
{
    interface_event = state;
}

void Interface::quest()
{
    (this->*quest_list[current_quest])();
}

int Interface::get_current_quest()
{
    return current_quest;
}
