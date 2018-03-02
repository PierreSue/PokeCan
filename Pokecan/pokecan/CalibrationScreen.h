#pragma once

#include "pokecan/Screen.h"

class CalibrationScreen : public Screen {
public:
	CalibrationScreen(Adafruit_SSD1306_I2c& _display, State& _state) : Screen(_display, _state) {
		
	}

	~CalibrationScreen(){}

	virtual void render();
	virtual int8_t key(uint8_t);
	
private:
};
