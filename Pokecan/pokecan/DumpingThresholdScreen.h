#pragma once

#include "pokecan/Screen.h"

class DumpingThresholdScreen : public Screen {
public:
	DumpingThresholdScreen(Adafruit_SSD1306_I2c& _display, State& _state) : Screen(_display, _state) {
		selected = 0;
	}

	~DumpingThresholdScreen(){}

	virtual void render();
	virtual int8_t key(uint8_t);
	
private:
	uint8_t selected;
};