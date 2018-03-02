#pragma once

#include <vector>

#include "mbed-drivers/mbed.h"

#include "pokecan/RouteSegment.h"

using namespace std;

class Config {
public:
	uint8_t threshold;
	uint8_t can_height;
	uint8_t lift_height;
	vector<RouteSegment> route;

	Config(I2C&);

	void load();
	void save();

private:
	void write(uint8_t, uint8_t, uint8_t);
	uint8_t read(uint8_t, uint8_t);

	I2C& i2c;
	uint8_t address;
};