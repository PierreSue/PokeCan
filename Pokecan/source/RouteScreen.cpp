#include "pokecan/RouteScreen.h"

#include <sstream>

using std::stringstream;

RouteScreen::RouteScreen(Adafruit_SSD1306_I2c& _display, State& _state) : Screen(_display, _state), cursor(0), mode(0) {
	segments = state.config.route;
	refresh_list();
	scroll_top = 0;
	scroll_bot = (list_items.size() > 6 ? 6 : list_items.size());
}

void RouteScreen::render() {
	display.clearDisplay();
	display.printfPos(20, 0, "Route definition");
	display.drawFastHLine(0, 9, 128, WHITE);
	switch(mode) {
	case 0:
		for(unsigned i = scroll_top; i < scroll_bot; ++i) {
			string line;
			if(cursor == i) line = "> ";
			else line = "  ";
			line += list_items[i];
			display.printfPos(0, 10 + 8*(i-scroll_top), line.c_str());
		}
		break;
	case 1:
		display.printfPos(0, 10, "Remove?");
		display.printfPos(0, 18, "Left=Cancel, Right=OK");
		display.printfPos(0, 30, list_items[cursor].c_str());
		break;
	case 2:
		display.printfPos(30, 15, "Move");
		display.printfPos(40, 25, "(up/dn)");
		display.setTextSize(2);
		if(new_segment.direction == RouteSegment::FORWARD) {
			display.printfPos(0, 35, "FORWARD");
		} else if(new_segment.direction == RouteSegment::BACKWARD) {
			display.printfPos(0, 35, "BACKWARD");
		} else if(new_segment.direction == RouteSegment::LEFTWARD) {
			display.printfPos(0, 35, "LEFTWARD");
		} else if(new_segment.direction == RouteSegment::RIGHTWARD) {
			display.printfPos(0, 35, "RIGHTWARD");
		} else {
			display.printfPos(0, 35, "CLOCKWISE");
		}
		display.setTextSize(1);
		break;
	case 3:
		display.printfPos(30, 15, "For");
		display.setTextSize(2);
		if(new_segment.direction == RouteSegment::TURN) {
			display.printfPos(0, 25, "%3u deg", new_segment.distance);
		} else {
			display.printfPos(0, 25, "%4u cm", new_segment.distance);
		}
		display.setTextSize(1);
		break;
	}
	display.display();
}

int8_t RouteScreen::key(uint8_t keycode) {
	switch(keycode) {
	case 0:
		if(mode == 0) {
			if(cursor > 0) {
				if(--cursor < scroll_top) {
					--scroll_top;
					--scroll_bot;
				}
			}
		} else if(mode == 2) {
			if(new_segment.direction < RouteSegment::TURN) ++new_segment.direction;
			else new_segment.direction = RouteSegment::FORWARD;
		} else if(mode == 3) {
			++new_segment.distance;
		}
		break;
	case 1:
		if(mode == 0) {
			if(cursor < list_items.size()-1) {
				if(++cursor >= scroll_bot) {
					++scroll_bot;
					++scroll_top;
				}
			}
		} else if(mode == 2) {
			if(new_segment.direction > RouteSegment::FORWARD) --new_segment.direction;
			else new_segment.direction = RouteSegment::TURN;
		} else if(mode == 3) {
			if(new_segment.distance > 0) --new_segment.distance;
		}
		break;
	case 2:
		if(mode == 0) {
			return Screen::SCR_SETTINGS;
		} else if(mode == 1) {
			mode = 0;
		} else if(mode == 2) {
			mode = 0;
		} else if(mode == 3) {
			mode = 2;
		}
		break;
	case 3:
		if(mode == 0) {
			if(cursor == list_items.size()-1) {
				// save
				state.config.route = segments;
				state.config.save();
				return Screen::SCR_SETTINGS;
			} else if(cursor == list_items.size()-2) {
				// add
				new_segment = RouteSegment();
				new_segment.direction = RouteSegment::FORWARD;
				new_segment.distance = 0;
				mode = 2;
			} else {
				// confirm remove
				mode = 1;
			}
		} else if(mode == 1) {
			// remove
			segments.erase(segments.begin()+cursor);
			refresh_list();
			cursor = 0;
			scroll_top = 0;
			scroll_bot = (list_items.size() > 6 ? 6 : list_items.size());
			mode = 0;
		} else if(mode == 2) {
			mode = 3;
		} else if(mode == 3) {
			segments.push_back(new_segment);
			refresh_list();
			cursor = 0;
			scroll_top = 0;
			scroll_bot = (list_items.size() > 6 ? 6 : list_items.size());
			mode = 0;
		}
		break;
	}
	return -1;
}

void RouteScreen::refresh_list() {
	list_items.clear();
	for(unsigned i = 0; i < segments.size(); ++i) {
		stringstream ss;
		RouteSegment seg = segments[i];
		switch(seg.direction) {
		case RouteSegment::FORWARD:
			ss << "Move FORWARD ";
			break;
		case RouteSegment::BACKWARD:
			ss << "Move BACKWARD ";
			break;
		case RouteSegment::LEFTWARD:
			ss << "Move LEFTWARD ";
			break;
		case RouteSegment::RIGHTWARD:
			ss << "Move RIGHTWARD";
			break;
		case RouteSegment::TURN:
			ss << "TURN clockwise ";
			break;
		}
		ss << seg.distance;
		if(seg.direction == RouteSegment::TURN) {
			ss << "deg";
		} else {
			ss << "cm";
		}
		list_items.push_back(ss.str());
	}
	list_items.push_back("Add..");
	list_items.push_back("Save..");
}