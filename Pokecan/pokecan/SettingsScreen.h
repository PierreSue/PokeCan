#pragma once

#include "pokecan/Screen.h"

class SettingsScreen : public Screen {
public:
	SettingsScreen(Adafruit_SSD1306_I2c& _display, State& _state) : Screen(_display, _state) {
		selected = 0;
		menu_text = vector<string>(0);
		menu_text.push_back("Moving route");
		menu_text.push_back("Motor test");
		menu_text.push_back("Dumping threshold");
		menu_text.push_back("Calibration");
		menu_text.push_back("Factory reset");
	}

	~SettingsScreen(){}

	virtual void render();
	virtual int8_t key(uint8_t);
	
private:
	uint8_t selected;
	vector<string> menu_text;
};
