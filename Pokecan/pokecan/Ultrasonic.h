#pragma once

#include "mbed-drivers/mbed.h"

class Ultrasonic {
public:
	Ultrasonic(PinName, PinName, float, float);
	void start_measure(void);

	float get_distance();

private:
	/*void echo_rise();
	void echo_fall();*/

	DigitalOut trig;
	DigitalIn echo;

	Timer timer;
	Timeout tout;

	float timeout;
	float speed;

	float distance;

	bool done;

	float start;
	float end;
};