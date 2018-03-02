#pragma once

#include <vector>
#include <string>

#include "pokecan/Screen.h"

using namespace std;

class MenuScreen : public Screen {
public:
	MenuScreen(Adafruit_SSD1306_I2c& _display, State& _state) : Screen(_display ,_state) {
		selected = 0;
		menu_text = vector<string>(0);
		menu_text.push_back("Settings");
		menu_text.push_back("Network");
		menu_text.push_back("About");
	}

	~MenuScreen(){}

	virtual void render();
	virtual int8_t key(uint8_t);
	
private:
	uint8_t selected;
	vector<string> menu_text;
};