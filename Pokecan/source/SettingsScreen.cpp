#include "pokecan/SettingsScreen.h"

void SettingsScreen::render() {
	display.clearDisplay();
	display.printfPos(20, 0, "Settings menu");
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

int8_t SettingsScreen::key(uint8_t keycode) {
	switch(keycode) {
	case 0:
		if(selected > 0) --selected;
		break;
	case 1:
		if(selected < menu_text.size()-1) ++selected;
		break;
	case 2:
		return Screen::SCR_MENU;
	case 3:
		if(selected == 0) return Screen::SCR_ROUTE;
		else if(selected == 1) return Screen::SCR_MOTOR_TEST;
		else if(selected == 2) return Screen::SCR_DUMP_TH;
		else if(selected == 3) return Screen::SCR_CALIBRATION;
		break;
	}
	return -1;
}