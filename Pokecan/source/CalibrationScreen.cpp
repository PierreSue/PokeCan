#include "pokecan/CalibrationScreen.h"

void CalibrationScreen::render() {
	display.clearDisplay();
	display.printfPos(0, 0, "Can depth calibration");
	display.drawFastHLine(0, 9, 128, WHITE);
	display.setTextSize(2);
	int dist = state.distance;
	display.printfPos(48, 20, "%2d cm", dist);
	display.setTextSize(1);
	display.printfPos(0, 40, "Left : cancel");
	display.printfPos(0, 50, "Right: confirm");
	display.display();
}

int8_t CalibrationScreen::key(uint8_t keycode) {
	switch(keycode) {
	case 0:
		break;
	case 1:
		break;
	case 2:
		return Screen::SCR_SETTINGS;
	case 3:
		uint8_t dist = state.distance;
		state.config.can_height = dist;
		state.config.save();
		return Screen::SCR_SETTINGS;
	}
	return -1;
}