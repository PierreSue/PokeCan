#pragma once

#include "pokecan/Screen.h"

class MotorTestScreen : public Screen {
public:
	MotorTestScreen(Adafruit_SSD1306_I2c& _display, State& _state) : Screen(_display, _state) {
		selected = 0;
		mode = 0;
		menu_text = vector<string>(0);
		menu_text.push_back("Move forward");
		menu_text.push_back("Move left");
		menu_text.push_back("Lift and dump");
	}

	~MotorTestScreen(){}

	virtual void render();
	virtual int8_t key(uint8_t);
	
private:
	uint8_t selected;
	uint8_t mode;
	vector<string> menu_text;

	void forward();
	void backward();
	void finish_test();
};
