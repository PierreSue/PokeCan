#include "pokecan/Ultrasonic.h"

Ultrasonic::Ultrasonic(PinName _trig, PinName _echo, float _speed, float _timeout) : trig(_trig), echo(_echo), speed(_speed), timeout(_timeout) {
	done = false;
}

float Ultrasonic::get_distance() {
	return distance;
}

/*void Ultrasonic::echo_rise() {
	printf("rise\r\n");
	start = 0;
	timer.reset();
	timer.start();
}

void Ultrasonic::echo_fall() {
	printf("fall\r\n");
	end = timer.read();
	timer.stop();
	done = true;
	printf("%f\r\n", (end-start)*17150);
	distance = (end - start) * 17150;
	tout.detach();
	tout.attach(this, &Ultrasonic::start_measure, speed);
}*/

void Ultrasonic::start_measure(void) {
	/*printf("start_measure\r\n");
	trig = 0;
	wait_ms(5);
	timer.reset();
	tout.detach();
	trig = 1;
	wait_us(10);
	done = false;

	echo.rise(this, &Ultrasonic::echo_rise);
	echo.fall(this, &Ultrasonic::echo_fall);
	echo.enable_irq();*/

	trig = 0;
	wait_ms(5);
	timer.reset();
	timer.start();
	trig = 1;
	wait_us(10);
	trig = 0;
	while(!echo) {
		start = timer.read();
	}
	while(echo) {
		end = timer.read();
	}
	distance = (end - start) * 17150;

	tout.attach(this, &Ultrasonic::start_measure, speed);

	//tout.attach(this, &Ultrasonic::start_measure, speed);
	//trig = 0;
}