#pragma once

#include "FidelisDuino_Output.h"
#include "FidelisDuino_EdgeDetection.h"

FidelisDuino::IO::Output::ClockGenerator _clock;
FidelisDuino::EdgeDetection::TransitionDetection detectChanges;
bool signal = false;
void setup() {

}

void loop() {
	_clock.Loop(true, 3000, 6000);

	detectChanges.Loop(_clock.Out());

	if (detectChanges.OnTrueEdge()) { Serial.println("Output was turned on!"); }
	if (detectChanges.OnFalseEdge()) { Serial.println("Output was turned off!"); }
}

String ReadStringFromSerial() {
	String conteudo = "";

	while (Serial.available() > 0) {
		conteudo =  Serial.readString();
	}

	return conteudo;
}