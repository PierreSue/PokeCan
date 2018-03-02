#include "pokecan/MenuScreen.h"

void MenuScreen::render() {
	display.clearDisplay();
	display.printfPos(20, 0, "Menu");
	display.drawFastHLine(0, 9, 128, WHITE);
	for(unsigned i = 0; i < menu_text.size(); ++i) {
		string line;
		if(selected == i) line = "> ";
		else line = "  ";
		line += menu_text[i];
		display.printfPos(0, 10 + 8*i, line.c_str());
	}
	display.display();
}

int8_t MenuScreen::key(uint8_t keycode) {
	switch(keycode) {
	case 0:
		if(selected > 0) --selected;
		break;
	case 1:
		if(selected < menu_text.size()-1) ++selected;
		break;
	case 2:
		return Screen::SCR_MAIN;
	case 3:
		if(selected == 0) {
			return Screen::SCR_SETTINGS;
		} else if(selected == 1) {
			return Screen::SCR_NETWORK;
		} else {
			return Screen::SCR_ABOUT;
		}
	}
	return -1;
}