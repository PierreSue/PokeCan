#pragma once

#include <vector>
#include <string>

#include "pokecan/Screen.h"
#include "pokecan/RouteSegment.h"

using std::vector;
using std::string;

class RouteScreen : public Screen {
public:
	RouteScreen(Adafruit_SSD1306_I2c& _display, State& _state);

	~RouteScreen(){}

	virtual void render();
	virtual int8_t key(uint8_t);
	
private:
	void refresh_list();

	uint8_t scroll_top;
	uint8_t scroll_bot;
	uint8_t cursor;

	uint8_t mode;

	RouteSegment new_segment;

	vector<string> list_items;
	vector<RouteSegment> segments;
};
