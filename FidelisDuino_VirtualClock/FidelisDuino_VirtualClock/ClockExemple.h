#pragma once

#include "FidelisDuino_VirtualClock.h"

FidelisDuino::VirtualClock::VirtualClock clk;

void setup() {
	clk.SetTime(23, 59, 45, 6);
}

void loop() {
	clk.Update();

	unsigned long _hrs, _min, _sec, _weekDay;
	clk.GetTime(_hrs, _min, _sec, _weekDay);
	Serial.println("Time is: ");
	Serial.print(_hrs);Serial.print(":");Serial.print(_min);Serial.print(" - ");Serial.print(_sec);
	Serial.print(" week: ");Serial.println(_weekDay);
}