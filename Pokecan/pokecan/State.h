#pragma once

#include <stdint.h>

#include "pokecan/Config.h"
#include "pokecan/MotorControl.h"
#include "pokecan/NetworkManager.h"

class State {
public:
	uint8_t status;
	float distance;
	uint8_t level;
	uint8_t distance_change;

	uint8_t startup_phase;

	MotorControl motor;
	Config config;
	NetworkManager* nm;

	State(I2C &_i2c) : motor(_i2c), config(_i2c) {
		status = IDLE;
		level = 0;
		distance = 10;
	}

	static const uint8_t IDLE;
	static const uint8_t MOVE;
	static const uint8_t DUMP;

	constexpr static const uint8_t STARTUP_NM = 1;
	constexpr static const uint8_t STARTUP_CONF = 2;
	constexpr static const uint8_t STARTUP_FIN = 3;
};