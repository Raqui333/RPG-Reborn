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

    player->npos.x = (action_on) ? 752 : 384;
    player->npos.y = (action_on) ? 412 : 320;

    player->is_in_action = action_on;

    if (!action_on) player->key_lock = false;

    if (onoff) battle = new Battle(enemy);
}

void Interface::render()
{
    if (SDL_GetTicks() - interface_tick < 5000) {
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
    std::string level = "Level " + std::to_string(player->getlvl());
    Texture::drawText(level.c_str(), fg, size, 175, 50);

    Texture::drawText("Strength", fg, size, 175, 75);
    Texture::drawText(std::to_string(player->getstr()).c_str(), fg, 10, 200, 95);

    std::string exp = std::to_string(player->getexp().second) + "/" + std::to_string(player->getexp().first);
    Texture::drawText(exp.c_str(), fg, size, 55, 171);

    // hit and mana points
    std::string hitpoints = std::to_string(player->gethp().first) + " / " + std::to_string(player->gethp().second);
    Texture::drawText(hitpoints.c_str(), fg, size + 5, 85, 205);

    std::string manapoints = std::to_string(player->getmp().first) + " / " + std::to_string(player->getmp().second);
    Texture::drawText(manapoints.c_str(), fg, size + 5, 85, 255);

    // write quest log
    std::stringstream entry(log);
    std::string line;
    int y = 340;
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
