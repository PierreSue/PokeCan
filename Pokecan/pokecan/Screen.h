#pragma once

#include "adafruit-ssd1306/Adafruit_SSD1306.h"

#include "pokecan/State.h"

class Screen {
public:
	Screen(Adafruit_SSD1306_I2c& _display, State& _state) : display(_display), state(_state) {
	}

	virtual ~Screen(){}

	virtual void render() = 0;
	virtual int8_t key(uint8_t) = 0;

	constexpr static const int8_t SCR_SPLASH = 0;
	constexpr static const int8_t SCR_MAIN = 1;
	constexpr static const int8_t SCR_MENU = 2;
	constexpr static const int8_t SCR_SETTINGS = 3;
	constexpr static const int8_t SCR_ABOUT = 4;
	constexpr static const int8_t SCR_NETWORK = 5;
	constexpr static const int8_t SCR_DUMP_TH = 6;
	constexpr static const int8_t SCR_MOTOR_TEST = 7;
	constexpr static const int8_t SCR_CALIBRATION = 8;
	constexpr static const int8_t SCR_ROUTE = 9;
	
protected:
	Adafruit_SSD1306_I2c& display;
	State& state;
};