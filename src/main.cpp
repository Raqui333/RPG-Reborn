#include "../include/game.h"

int main() {
	Game game("RPG Reborn", 1088, 608);
	
	while (game.isRunning()) {
		game.event_loop();
		game.update();
		game.render();
	}

	return 0;
}
