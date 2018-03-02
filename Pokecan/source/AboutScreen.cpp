#include "pokecan/AboutScreen.h"

void AboutScreen::render() {
	display.clearDisplay();
	for(uint8_t i = 0; i < 44; ++i) {
		for(uint8_t j = 0; j < 128; ++j) {
			display.drawPixel(j, i, px[i*128 + j]);
		}
	}
	display.printfPos(0, 50, "by Dodo, Pierre, Sasa");
	display.display();
}

int8_t AboutScreen::key(uint8_t keycode) {
	return Screen::SCR_MENU;
}