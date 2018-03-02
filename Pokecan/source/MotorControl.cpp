#include "pokecan/MotorControl.h"

MotorControl::MotorControl(I2C& _i2c, uint8_t address) : i2c(_i2c), pca(_i2c, address), mcp(_i2c) {
	pca.set_frequency(500);
	for(uint8_t i = 0; i < channels*2; ++i) {
		mcp.setup(i, MCP23017::OUT);
	}
	for(uint8_t i = 0; i < 16; ++i) stop(i);
}

void MotorControl::stop(uint8_t channel) {
	if(channel > channels) return;
	pca.set_pwm(channel, 0, 0);
	mcp.output(channel*2, MCP23017::LOW);
	mcp.output(channel*2+1, MCP23017::LOW);
}

void MotorControl::forward(uint8_t channel, uint8_t percentage) {
	if(channel > channels) return;
	float p = ((float) percentage) / 100;
	pca.set_pwm(channel, 0, p * 4095);
	mcp.output(channel*2, MCP23017::HIGH);
	mcp.output(channel*2+1, MCP23017::LOW);
}

void MotorControl::backward(uint8_t channel, uint8_t percentage) {
	if(channel > channels) return;
	float p = ((float) percentage) / 100;
	pca.set_pwm(channel, 0, p * 4095);
	mcp.output(channel*2, MCP23017::LOW);
	mcp.output(channel*2+1, MCP23017::HIGH);
}