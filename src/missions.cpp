#include "../include/interface.h"

void Interface::quest_completed(int xp)
{
    if (player->setexp(xp))
        set_screen_log("You leveled up");
    else
        set_screen_log("You earned " + std::to_string(xp) + " exp from a quest");

    current_quest++;
    to_completed = {0, 0};
}

void Interface::first_mission()
{
    to_completed.second = 1;

    std::stringstream tmp;

    tmp << "You was summoned in an\n";
    tmp << "odd world. There are some\n";
    tmp << "strange symbols on the\n";
    tmp << "island.\n\n";
    tmp << "Find the big one in the\n";
    tmp << "south.\n\n";
    tmp << "Completed (" << to_completed.first << "/" << to_completed.second << ")\n";

    if (player->getpos().x == 800 && player->getpos().y == 960) {
        to_completed.first = 1;
        player->setpos(1792, 2432);
    }

    log = tmp.str();

    if (to_completed.first == to_completed.second)
        quest_completed(100);
}

void Interface::second_mission()
{
    to_completed.second = 1;

    std::stringstream tmp;

    tmp << "Go to the Rank E gate in\n";
    tmp << "the Northeast.\n\n";
    tmp << "Completed (" << to_completed.first << "/" << to_completed.second << ")\n";

    if (player->isInDungeon() && player->getDungeonRank() == 0)
        to_completed.first = 1;
    
    log = tmp.str();

    if (to_completed.first == to_completed.second)
        quest_completed(100);
}

void Interface::third_mission()
{
    to_completed.second = 5;

    std::stringstream tmp;

    tmp << "Monsters are getting hard\n";
    tmp << "you need to go stronger.\n\n";
    tmp << "Get level 5\n\n";
    tmp << "Completed (" << to_completed.first << "/" << to_completed.second << ")\n";

    to_completed.first = player->getlvl();
    
    log = tmp.str();

    if (to_completed.first >= to_completed.second)
        quest_completed(100);
}

void Interface::fourth_mission()
{
    std::stringstream tmp;

    tmp << "Thanks for testing the\n";
    tmp << "game, it is still in\n";
    tmp << "development.\n";

    log = tmp.str();
}
