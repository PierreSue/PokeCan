#include "pokecan/Config.h"

#define YOTTA_CFG_MBED_TRACE
#include "mbed-trace/mbed_trace.h"
#define TRACE_GROUP  "cfg"

#include <string>
#include <sstream>

#define ADDR_FIRSTTIME 0x00
#define ADDR_THRESHOLD 0x01
#define ADDR_CAN_HEIGHT 0x02
#define ADDR_LIFT_HEIGHT 0x03

using namespace std;

Config::Config(I2C& _i2c) : i2c(_i2c), address(0xA0), route(0) {
	threshold = 80;
	can_height = lift_height = 0;
}

void Config::load() {
	tr_info("start to read");
	uint8_t result = read(0x00, ADDR_FIRSTTIME);
	if(result == 0xFF) {
		tr_info("it's first use, so we save default config first");
		save();
	}
	result = read(0x00, ADDR_THRESHOLD);
	threshold = result;
	result = read(0x00, ADDR_CAN_HEIGHT);
	can_height = result;
	result = read(0x00, ADDR_LIFT_HEIGHT);
	lift_height = result;

	// read size
	uint8_t size = read(0x01, 0x00);
	uint8_t direction, distance;
	route = vector<RouteSegment>(size);
	for(uint8_t i = 0; i < size; ++i) {
		direction = read(0x01, 0x01 + (3*i));
		distance = read(0x01, 0x02 + (3*i));
		distance <<= 8;
		distance += read(0x01, 0x03 + (3*i));
		route[i].direction = direction;
		route[i].distance = distance;
	}
}

void Config::save() {
	// normal parameters
	write(0x00, ADDR_FIRSTTIME, 0x00);
	write(0x00, ADDR_THRESHOLD, threshold);
	write(0x00, ADDR_CAN_HEIGHT, can_height);
	write(0x00, ADDR_LIFT_HEIGHT, lift_height);

	// route, 1st byte for size
	uint8_t size = (route.size() > 80) ? 80 : route.size();
	write(0x01, 0x00, size);
	for(uint8_t i = 0; i < size; ++i) {
		write(0x01, 0x01 + (3*i), route[i].direction);
		write(0x01, 0x02 + (3*i), ((route[i].distance >> 8) & 0xFF));
		write(0x01, 0x03 + (3*i), (route[i].distance & 0xFF));
	}
}

uint8_t Config::read(uint8_t reg1, uint8_t reg2) {
	char cmd[2] = { reg1, reg2 };
	char ret;
	i2c.write(address, cmd, 2, true);
	i2c.read(address, &ret, 1);
	tr_info("read EEPROM %u,%u = %u", reg1, reg2, ret);
	return ret;
}

void Config::write(uint8_t reg1, uint8_t reg2, uint8_t val) {
	char cmd[3] = { reg1, reg2, val };
	i2c.write(address, cmd, 3);
	tr_info("write EEPROM %u,%u = %u", reg1, reg2, val);
	return;
}