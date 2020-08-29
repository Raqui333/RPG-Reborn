#include "../include/game.h"

int main() {
	Game reborn("RPG Reborn", 1088, 608);
	
	while (reborn.isRunning()) {
		reborn.event_loop();
		reborn.update();
		reborn.render();
	}

	return 0;
}
