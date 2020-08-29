#ifndef INTERFACE_H_
#define INTERFACE_H_

#include "game.h"
#include "player.h"
#include "texture.h"
#include "enemy.h"

class Interface {
    SDL_Texture* main_interface;
    SDL_Rect viewport;
    Player* player;

    SDL_Texture* action_interface;
    bool action_on;

    std::string log, in_screen_log;
    std::pair<int,int> to_completed;

    std::vector<void (Interface::*)()> quest_list;
    int current_quest;

    const Uint8 *interface_event;
    Uint32 interface_tick;
public:
    Interface(Player*);
    ~Interface();

    void render();
    void set_screen_log(std::string);

    void quest();
    int get_current_quest();
    void quest_completed(int);

    void events(const Uint8*);

    // modifier
    void toggle_battle(bool, Enemy*);

    // missions
    void first_mission();
    void second_mission();
    void third_mission();
    void fourth_mission();

    friend class Battle;
};

class Battle {
    Enemy* enemy;

    int turn;
    std::string log;
    Uint32 time;

    enum { NEXT, PREV };
    int selection;

    // text
    SDL_Color fg, sel;
    int size;

    // menu
    const Uint8* menu_event;
    std::vector<std::string> menu;
public:
    Battle(Enemy*);
    ~Battle();

    void init(Interface*);
    void events(Interface*);

    void select_menu(int);
};

#endif // INTERFACE_H_
