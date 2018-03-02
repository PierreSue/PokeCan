#include <cstdlib>

#include "core-util/FunctionPointer.h"
#define YOTTA_CFG_MBED_TRACE
#include "mbed-trace/mbed_trace.h"
#define TRACE_GROUP  "nm"

#include "pokecan/NetworkManager.h"

const string NetworkManager::NO_IP = "nil";

char *hex_table = "0123456789ABCDEF";

NetworkManager::NetworkManager(PinName tx, PinName rx, uint32_t baud) : esp(tx, rx), timeout(500), ip(NO_IP) {
	esp.baud(baud);

	if(!(command("node.restart()") && execute())) {
		tr_warn("Can't reset ESP");
	}

	wait_ms(1000);
	flush();

	if(!(command("wifi.setmode(wifi.STATION);wifi.sta.autoconnect(1)") && execute())) {
		tr_warn("Can't set ESP mode to STATION");
	}

	update_ip_handle = minar::Scheduler::postCallback(mbed::util::FunctionPointer0<void>(this, &NetworkManager::update_ip).bind()).period(minar::milliseconds(3000)).getHandle();
	update_ip();
}

NetworkManager::~NetworkManager() {
	minar::Scheduler::cancelCallback(update_ip_handle);
}

void NetworkManager::update_ip(void) {
	ip = get_ip();
}

string NetworkManager::start_setup() {
	char resp[256];
	int resp_len = 256;

	if(!(command("node.restart()") && execute())) {
		tr_warn("Can't reset ESP");
	}

	wait_ms(500);

	while(!(command("print(wifi.sta.getmac())")
		&& execute(resp, &resp_len) && (resp[2] != '-'))) {
		tr_warn("Can't get MAC address!");
	}

	char ssid[17] = "Pokecan_";

	char pass[9];
	pass[0] = resp[6];
	pass[1] = resp[7];
	pass[2] = resp[9];
	pass[3] = resp[10];
	pass[4] = resp[12];
	pass[5] = resp[13];
	pass[6] = resp[15];
	pass[7] = resp[16];
	pass[8] = '\0';

	strcat(ssid, pass);

	tr_info("SSID: %s", ssid);

	resp_len = 256;

	while(!(command("dofile(\"set_me_up.lua\")") && execute(resp, &resp_len))) {
		tr_warn("Can't execute script");
	}

	tr_info("Started setup with base name %s", pass);
	return string(pass);
}

void NetworkManager::cancel_setup() {
	char resp[256];
	int resp_len = 256;

	while(!(command("dofile(\"cancel_setup.lua\")") && execute(resp, &resp_len))) {
		tr_warn("Can't execute script");
	}

	return;
}

bool NetworkManager::disconnect() {
	if(!(command("wifi.sta.disconnect()") && execute())) {
		tr_warn("Can't disconnect");
		return false;
	}
	if(!(command("wifi.sta.config(\"x\",\"\")") && execute())) {
		tr_warn("Can't clear config");
		return false;
	}
	return true;
}

string NetworkManager::get_ip() {
	int ip_len = 15;
	char ip[15];
	if(!(command("ip=wifi.sta.getip();print(ip)") && execute(ip, &ip_len))) {
		return "nil";
	}
	if(ip[0] == 'n') {
		return "nil";
	}
	return string(ip, ip_len);
}

bool NetworkManager::command(const char* cmd) {
	if(strcmp(cmd, "\r\n") != 0) tr_info("ESP8266 command: %s", cmd);
	for(int i = 0; i < strlen(cmd); ++i) {
		if(send_char(cmd[i]) < 0) return false;
	}
	return true;
}

bool NetworkManager::execute(char* resp_buf, int* resp_len) {
	if(!command("\r\n")) {
		tr_warn("failed to enter newline");
		return false;
	}
	wait_ms(50);
	if(!discard_echo()) {
		tr_warn("can't discard echo");
		return false;
	}
	//tr_info("ESP8266 exec ok");
	wait_ms(100);
	if(resp_buf && resp_len) {
		for(int i = 0; i < *resp_len; ++i) {
			int c = recv_char();
			if(c < 0) return false;
			if(c == '\r') {
				*resp_len = i;
				break;
			}
			resp_buf[i] = c;
		}
		tr_info("ESP8266 response: %.*s", *resp_len, resp_buf);
	}
	return flush();
}

bool NetworkManager::discard_echo() {
	while(1) {
		int ch = recv_char();
		if(ch < 0) {
			return false;
		}
		if(ch == '\n') {
			//tr_info("Discarded %d chars", count);
			return true;
		}
	}
}

bool NetworkManager::flush() {
	while(1) {
		int ch = recv_char();
		if(ch < 0) return false;
		else if(ch == '>') {
			recv_char();
			return true;
		}
	}
}

int NetworkManager::send_char(char ch) {
	Timer timer;
	timer.start();
	while(1) {
		if(esp.writeable()) {
			return esp.putc(ch);
		}
		if(timer.read_ms() > timeout) return -1;
	}
}

int NetworkManager::recv_char() {
	Timer timer;
	timer.start();
	while(1) {
		if(esp.readable()) {
			char ch = esp.getc();
			return ch;
		}
		if(timer.read_ms() > timeout) {
			tr_warn("ESP receive timeout");
			return -1;
		}
	}
}