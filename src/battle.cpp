#include "../include/interface.h"

Battle::Battle(Enemy* one)
{
    enemy = one;

    time = SDL_GetTicks();
    turn = 0;
    log = "A monster has appeared";

    fg = { 150, 150, 150 };
    sel = { 51, 112, 217 };
    size = 10;

    menu = { "Attack", "Heal", "Escape" };

    selection = 0;
}

Battle::~Battle()
{}

void Battle::init(Interface* sys)
{
    sys->player->render();
    enemy->render_in_battle();

    // hp bar
    SDL_SetRenderDrawColor(Game::renderer, 0x17, 0xaa, 0x00, 0xAA);
    SDL_Rect hp_bar = {13, 13, (enemy->gethp().second * 100 / enemy->gethp().first) * 3, 20};
    SDL_RenderFillRect(Game::renderer, &hp_bar);

    // name & rank
    std::stringstream title;

    title << "Rank";
    
    switch (enemy->getrank()) {
        case 2: title << " E "; break;
        case 3: title << " D "; break;
        case 4: title << " C "; break;
        case 5: title << " B "; break;
        case 6: title << " A "; break;
        case 7: title << " S "; break;
        default:
            title << "Unknown";
            break;
    }

    title << enemy->getname();
    
    Texture::drawText(title.str().c_str(), fg, size, 15, 38);

    // menu
    int y = 420;
    for (int i = 0; i < menu.size(); ++i) {
        Texture::drawText(menu[i].c_str(), (selection == i) ? sel : fg, size * 1.5, 50, y);
        y += 40;
    }

    // player turn
    if (SDL_GetTicks() - time > 200 && turn == 0) {
        Battle::events(sys);
    }

    // enemy turn
    if (turn == 1 && SDL_GetTicks() - time > 1000) {
        int dmg_r = rand() % (enemy->getrank() * 10) + 1;
        sys->player->dmg(dmg_r);
        log = "You lost " + std::to_string(dmg_r) + " of health due to ";
        log += enemy->getname() + "'s attack";
        turn -= 1;
    }

    Texture::drawText(log.c_str(), sel, size, 15, 370);

    if (enemy->gethp().second <= 0) {
        sys->player->setexp(enemy->expfromdeath());
        enemy->reset_hp();
        sys->toggle_battle(false, NULL);
    } else if (sys->player->gethp().second <= 0) {
        sys->player->kill();
        sys->toggle_battle(false, NULL);
        sys->player->setDungeon(false, 0);
    }
}

void Battle::select_menu(int item)
{
    switch (item) {
        case NEXT:
            selection++;
            break;
        case PREV:
            selection--;
            break;
        default: // nothing
            break;
    }

    if (selection == menu.size()) {
        selection = 0;
    } else if (selection < 0) {
        selection = menu.size() - 1;
    }
}

void Battle::events(Interface* sys)
{
    menu_event = sys->interface_event;
    
    if (menu_event[SDL_SCANCODE_DOWN]) {
        select_menu(NEXT);
        time = SDL_GetTicks();
    } else if (menu_event[SDL_SCANCODE_UP]) {
        select_menu(PREV);
        time = SDL_GetTicks();
    } else if (menu_event[SDL_SCANCODE_RETURN] || menu_event[SDL_SCANCODE_KP_ENTER]) {
        switch (selection) {
            case 0: {
                int dmg_r = rand() % (sys->player->getstr() + sys->player->getlvl()) + 1;
                enemy->dmg(dmg_r);
                log = enemy->getname() + " lost " + std::to_string(dmg_r) + " of health due to your attack";
            } break;
            case 1: {
                if (sys->player->getmp().second >= 10) {
                    int healing = rand() % sys->player->gethp().first;
                    sys->player->heal(healing);
                    sys->player->mpused(10);
                    log = "You healed " + std::to_string(healing) + " of health";
                } else {
                    log = "You don't have enough mana";
                    turn -= 1;
                }
            } break;
            case 2:
                sys->toggle_battle(false, NULL);
                break;
            default: // nothing
                break;
        }

        time = SDL_GetTicks();
        turn += 1;
    }
}
