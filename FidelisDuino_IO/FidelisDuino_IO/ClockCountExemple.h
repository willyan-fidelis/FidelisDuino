#pragma once

#include "FidelisDuino_Output.h"
#include "FidelisDuino_EdgeDetection.h"

FidelisDuino::IO::Output::ClockGeneratorCount _clockCounter;
FidelisDuino::EdgeDetection::TransitionDetection detectChanges;
bool signal = false;
String ReadStringFromSerial() {
	String conteudo = "";

	while (Serial.available() > 0) {
		conteudo = Serial.readString();
	}

	return conteudo;
}

void setup() {

}

void loop() 
{
	_clockCounter.Count( 3000, 6000, 3);
	_clockCounter.Loop();

	detectChanges.Loop(_clockCounter.Out());

	if (detectChanges.OnTrueEdge()) { Serial.println("Output was turned on!"); }
	if (detectChanges.OnFalseEdge()) { Serial.println("Output was turned off!"); }

	if (ReadStringFromSerial() == "start")
	{
		Serial.println("Started!");
		_clockCounter.Start();
	}
}

