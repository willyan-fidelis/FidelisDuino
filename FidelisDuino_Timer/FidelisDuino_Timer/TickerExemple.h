#pragma once

#include "FidelisDuino_Timer.h"

FidelisDuino::Timer::Ticker ticker;


void setup() {

}

void loop()
{
	ticker.Loop();

	if (Serial.available()) {
		if (Serial.readString() == "attach")
		{
			Serial.println("attach!");
			ticker.attach(3000, []()->void {
				Serial.println("Ticked one more time!");
				}, []()->void {
					Serial.println("Last time!");
				},3);
		}
		if (Serial.readString() == "detach")
		{
			Serial.println("detach!");
			ticker.detach();
		}
	}
}

