#pragma once

#include "FidelisDuino_Output.h"
#include "FidelisDuino_EdgeDetection.h"

FidelisDuino::IO::Output::AnalogicOutput analog;

void setup() {

}

void loop()
{
	analog.Loop(0, 100, 0, 1023, [](int Target, int Out)->void {
		Serial.print("Changed to: ");
		Serial.println(Target);
	});

	if (Serial.available()) {
		if (Serial.readString() == "100")
		{
			Serial.println("100!");
			analog.Write(100);
		}
		if (Serial.readString() == "5")
		{
			Serial.println("5!");
			analog.Write(5);
		}
		if (Serial.readString() == "0")
		{
			Serial.println("0!");
			analog.Write(0);
		}
		if (Serial.readString() == "fade-on")
		{
			Serial.println("fade-on!");
			analog.FadeOn(12, 1000);
		}
		if (Serial.readString() == "fade-off")
		{
			Serial.println("fade-off!");
			analog.FadeOff(1000);
		}
	}
}

