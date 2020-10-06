#pragma once

#include "FidelisDuino_Input.h"

FidelisDuino::IO::Input::InputStateMachine input; //input state machine
bool signal = false;

FidelisDuino::IO::Input::InputStateMachine input_with_filter; //input state machine
bool signal_with_filter = false;
void setup() {

}

void loop() {
	if (true)
	{
		input.Loop(3000, 5000, signal,1000);

		//if (input.StTrue()) { Serial.println("StTrue"); }
		//if (input.StFalse()) { Serial.println("StFalse"); }
		if (input.StCtTrue()) { Serial.println("StCtTrue.."); }
		if (input.StCtFalse()) { Serial.println("StCtFalse.."); }
		if (input.StOnTrueEdge()) { Serial.println("StOnTrueEdge"); }
		if (input.StOnFalseEdge()) { Serial.println("StOnFalseEdge"); }
	}

	if (false)
	{
		input_with_filter.Loop(5000, 10000, signal_with_filter, 1000);
	}

	if (Serial.available())
	{
		if (Serial.readString() == "1") {
			Serial.println("Changed to 1");
			signal = true;
			signal_with_filter = true;
		}
		if (Serial.readString() == "0") {
			Serial.println("Changed to 0");
			signal = false;
			signal_with_filter = false;
		}
	}
}