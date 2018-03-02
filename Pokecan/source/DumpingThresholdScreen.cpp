#include "pokecan/DumpingThresholdScreen.h"

void DumpingThresholdScreen::render() {
	display.clearDisplay();
	display.printfPos(20, 0, "Dumping threshold");
	display.drawFastHLine(0, 9, 128, WHITE);
	display.setTextSize(2);
	display.printfPos(52, 20, "%2d%%", selected);
	display.setTextSize(1);
	display.printfPos(0, 40, "Left : cancel");
	display.printfPos(0, 50, "Right: confirm");
	display.display();
}

int8_t DumpingThresholdScreen::key(uint8_t keycode) {
	switch(keycode) {
	case 0:
		if(selected < 100) ++selected;
		else selected = 0;
		break;
	case 1:
		if(selected > 0) --selected;
		else selected = 100;
		break;
	case 2:
		return Screen::SCR_SETTINGS;
	case 3:
		return Screen::SCR_SETTINGS;
	}
	return -1;
}