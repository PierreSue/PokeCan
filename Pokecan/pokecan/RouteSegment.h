#pragma once

#include "mbed-drivers/mbed.h"

class RouteSegment {
public:
	RouteSegment();

	uint8_t direction;
	uint16_t distance;

	constexpr static const uint8_t FORWARD = 1;
	constexpr static const uint8_t BACKWARD = 2;
	constexpr static const uint8_t LEFTWARD = 3;
	constexpr static const uint8_t RIGHTWARD = 4;
	constexpr static const uint8_t TURN = 5;

};