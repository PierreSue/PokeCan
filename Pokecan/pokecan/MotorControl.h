#pragma once

#include "mbed-drivers/mbed.h"
#include "pca9685/PCA9685.h"
#include "mcp230xx/MCP230xx.h"

#include "pokecan/Pins.h"

#include <vector>

using std::vector;

class MotorControl {
public:
	MotorControl(I2C&, uint8_t = 0x80);

	void stop(uint8_t);
	void forward(uint8_t, uint8_t);
	void backward(uint8_t, uint8_t);

private:
	I2C& i2c;
	PCA9685 pca;
	MCP23017 mcp;

	uint8_t channels = 8;
};