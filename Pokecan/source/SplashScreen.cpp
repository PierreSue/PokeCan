#include "pokecan/SplashScreen.h"

void SplashScreen::render() {
	display.clearDisplay();
	for(uint8_t i = 0; i < 44; ++i) {
		for(uint8_t j = 0; j < 128; ++j) {
			display.drawPixel(j, i, px[i*128 + j]);
		}
	}
	display.printfPos(0, 48, "By Dodo, Pierre, Sasa");
	switch(state.startup_phase) {
	case State::STARTUP_CONF:
		display.printfPos(0, 56, "Reading config..");
		break;
	case State::STARTUP_NM:
		display.printfPos(0, 56, "Initialize wireless..");
		break;
	case State::STARTUP_FIN:
	default:
		display.printfPos(0, 56, "One moment please..");
		break;
	}
	display.display();
}

int8_t SplashScreen::key(uint8_t keycode) {
	
}