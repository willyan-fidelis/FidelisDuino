#pragma once

#include "FidelisDuino_Util_Math.h"
#include "FidelisDuino_Util_StringFormat.h"

bool test = false;
String toBeTested = "";
void setup() {

}

void loop() {
	test = FidelisDuino::Util::Math::isValidNumber(toBeTested);
	
	if (Serial.available())
	{
		String data = Serial.readString();
		Serial.println("New data arrived: "+data);
		toBeTested = data;
	}
	uint8_t abc[6] = { 0xD,3,4,5,6,0x7A };
	Serial.println(FidelisDuino::Util::Text::Format::MACFormat(abc));
}