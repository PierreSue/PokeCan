#include "mbed-drivers/mbed.h"
#include "pokecan/MotorTestScreen.h"

void MotorTestScreen::render() {
	display.clearDisplay();
	if(mode == 0) {
		display.printfPos(20, 0, "Motor test menu");
		display.drawFastHLine(0, 9, 128, WHITE);
		for(unsigned i = 0; i < menu_text.size(); ++i) {
			string line;
			if(selected == i) line = "> ";
			else line = "  ";
			line += menu_text[i];
			display.printfPos(0, 10 + 8*i, line.c_str());
		}
	} else if(mode == 1) {
		display.printfPos(0, 0, "Move forward..");
	} else if(mode == 2) {
		display.printfPos(0, 0, "Move backward..");
	}
	display.display();
}

void MotorTestScreen::forward() {
	mode = 1;
	state.motor.forward(4, 100);
	minar::Scheduler::postCallback(FunctionPointer0<void>(this, &MotorTestScreen::backward).bind()).delay(minar::milliseconds(1000));
}
void MotorTestScreen::backward() {
	mode = 2;
	state.motor.backward(4, 100);
	minar::Scheduler::postCallback(FunctionPointer0<void>(this, &MotorTestScreen::finish_test).bind()).delay(minar::milliseconds(1000));
}
void MotorTestScreen::finish_test() {
	state.motor.stop(4);
	mode = 0;
}

int8_t MotorTestScreen::key(uint8_t keycode) {
	if(mode == 0) {
		switch(keycode) {
		case 0:
			if(selected > 0) --selected;
			break;
		case 1:
			if(selected < menu_text.size()-1) ++selected;
			break;
		case 2:
			return Screen::SCR_MENU;
		case 3:
			if(selected == 0) {
				forward();
			}
			break;
		}
		return -1;
	}
	return -1;
}