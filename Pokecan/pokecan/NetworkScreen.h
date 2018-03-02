#pragma once

#include "pokecan/Screen.h"

class NetworkScreen : public Screen {
public:
	NetworkScreen(Adafruit_SSD1306_I2c& _display, State& _state);

	~NetworkScreen();

	virtual void render();
	virtual int8_t key(uint8_t);
	
private:
	void update_ip(void);
	void start_setup();
	void cancel_setup();
	void disconnect();

	string ip;
	uint8_t mode;
	minar::callback_handle_t update_ip_handle;

	uint8_t menu_selected;
	vector<string> menu_text;

	string setup_ssid;

	bool in_setup;
};
