#pragma once

#include <string>

#include "mbed-drivers/mbed.h"

#include "pokecan/BufferedSerial.h"

using std::string;

class NetworkManager {
public:
	NetworkManager(PinName, PinName, uint32_t);
	~NetworkManager();

	string start_setup();
	void cancel_setup();
	bool disconnect();

	constexpr static const uint8_t DISCONNECTED = 0;
	constexpr static const uint8_t CONNECTED = 1;

	static const string NO_IP;

	string ip;
private:
	bool command(const char*);
	bool execute(char* = 0, int* = 0);
	bool flush();
	bool discard_echo();

	void update_ip(void);

	int send_char(char);
	int recv_char();

	string get_ip();

	BufferedSerial esp;
	uint16_t timeout;
	bool in_setup;

	minar::callback_handle_t update_ip_handle;
};