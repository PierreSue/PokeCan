#include <string>

#include "pokecan/NetworkScreen.h"

using std::string;

NetworkScreen::NetworkScreen(Adafruit_SSD1306_I2c& _display, State& _state) : Screen(_display, _state), ip(NetworkManager::NO_IP), mode(0), menu_selected(0), in_setup(false) {
	update_ip_handle = minar::Scheduler::postCallback(FunctionPointer0<void>(this, &NetworkScreen::update_ip).bind()).period(minar::milliseconds(1000)).getHandle();
	menu_text = vector<string>(0);
	menu_text.push_back("Set up");
	menu_text.push_back("Information");
	update_ip();
}

NetworkScreen::~NetworkScreen() {
	if(update_ip_handle != 0) minar::Scheduler::cancelCallback(update_ip_handle);
}

void NetworkScreen::update_ip(void) {
	ip = state.nm->ip;
	if(ip == NetworkManager::NO_IP) {
		menu_text[0] = "Set up";
	} else {
		if(in_setup) {
			mode = 0;
			cancel_setup();
		}
		menu_text[0] = "Disconnect";
	}
}

void NetworkScreen::start_setup() {
	//if(update_ip_handle != 0) minar::Scheduler::cancelCallback(update_ip_handle);
	//update_ip_handle = 0;

	in_setup = true;

	setup_ssid = state.nm->start_setup();
}

void NetworkScreen::cancel_setup() {
	in_setup = false;
	state.nm->cancel_setup();
}

void NetworkScreen::disconnect() {
	state.nm->disconnect();
}

void NetworkScreen::render() {
	display.clearDisplay();
	display.printfPos(20, 0, "Networking");
	display.drawFastHLine(0, 9, 128, WHITE);
	if(mode == 0) {
		if(ip == NetworkManager::NO_IP) {
			display.setTextSize(2);
			display.printfPos(8, 15, "DISCONNECT");
			display.setTextSize(1);
		} else {
			display.setTextSize(2);
			display.printfPos(10, 15, "CONNECTED");
			display.setTextSize(1);
		}
		for(unsigned i = 0; i < menu_text.size(); ++i) {
			string line;
			if(menu_selected == i) line = "> ";
			else line = "  ";
			line += menu_text[i];
			display.printfPos(0, 34 + 8*i, line.c_str());
		}
	} else if(mode == 1) {
		display.printfPos(0, 15, "Please connect to the");
		display.printfPos(0, 23, "Wi-Fi and set me up.");
		display.printfPos(0, 37, "Name:Pokecan_%s", setup_ssid.c_str());
		display.printfPos(0, 45, "Pass:%s", setup_ssid.c_str());
	} else if(mode == 2) {
		display.printfPos(0, 15, "IP: %s", ip.c_str());
	} else {
		display.printfPos(0, 15, "Wait...");
	}
	display.display();
}

int8_t NetworkScreen::key(uint8_t keycode) {
	switch(keycode) {
	case 0:
		if(mode == 0 && menu_selected > 0) --menu_selected;
		break;
	case 1:
		if(mode == 0 && menu_selected < menu_text.size()-1) ++menu_selected;
		break;
	case 2:
		if(mode == 0) {
			return Screen::SCR_MENU;
		} else if(mode == 1) {
			// cancel setup
			cancel_setup();
			mode = 0;
		} else if(mode == 2) {
			// back
			mode = 0;
		}
		break;
	case 3:
		if(mode != 0) break;
		if(menu_selected == 0) {
			if(ip == NetworkManager::NO_IP) {
				// setup
				mode = 3;
				render();
				start_setup();
				mode = 1;
			} else {
				disconnect();
			}
		} else if(menu_selected == 1) {
			// about
			mode = 2;
		}
		break;
	}
	return -1;
}