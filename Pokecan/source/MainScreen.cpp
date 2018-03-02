#include "pokecan/MainScreen.h"

#include "core-util/FunctionPointer.h"

MainScreen::MainScreen(Adafruit_SSD1306_I2c& _display, State& _state) : Screen(_display, _state), connected(false) {
	update_ip_handle = minar::Scheduler::postCallback(FunctionPointer0<void>(this, &MainScreen::update_ip).bind()).period(minar::milliseconds(1000)).getHandle();
	update_ip();
}

MainScreen::~MainScreen() {
	minar::Scheduler::cancelCallback(update_ip_handle);
}

void MainScreen::update_ip(void) {
	string ip = state.nm->ip;
	if(ip == NetworkManager::NO_IP) {
		connected = false;
	} else {
		connected = true;
	}
}

void MainScreen::render() {
	display.clearDisplay();
	display.setTextSize(2);
	display.printfPos(20, 0, "Pokecan");
	if(state.status == State::IDLE) {
		display.printfPos(0, 40, "Idle");
	}
	display.setTextSize(1);
	if(connected) {
		for(int i = 0; i < 15; ++i) {
			for(int j = 0; j < 15; ++j) {
				display.drawPixel(111+j, i, wifi[15*i + j]);
			}
		}
	}
	display.printfPos(54, 56-(36*(((float)state.level)/100)), "%2d%%-", state.level);
	display.drawFastHLine(0, 17, 128, WHITE);
	for(int i = 0; i < 50; ++i) {
		for(int j = 0; j < 37; ++j) {
			display.drawPixel(80 + j, 19 + i, trashcan[37*i + j]);
		}
	}
	display.display();
}

int8_t MainScreen::key(uint8_t keycode) {
	return Screen::SCR_MENU;
}