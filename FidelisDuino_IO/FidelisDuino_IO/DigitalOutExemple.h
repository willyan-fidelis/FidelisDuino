#pragma once

#include "FidelisDuino_Output.h"
#include "FidelisDuino_EdgeDetection.h"

FidelisDuino::IO::Output::DigitalOutput _DigitalOutput;
FidelisDuino::EdgeDetection::TransitionDetection startedDetection;

void setup() {

}

void loop()
{
	_DigitalOutput.Loop([](bool OnChangeState)->void {
		if (OnChangeState)
		{
			Serial.println("Turned On!");
		}
		else {
			Serial.println("Turned Off!");
		}
	});

	startedDetection.Loop(_DigitalOutput.IsStarted());
	if (startedDetection.OnTrueEdge()) { Serial.println("Started!"); }
	if (startedDetection.OnFalseEdge()) { Serial.println("Stopped!"); }
	
	if (Serial.available()) {
		if (Serial.readString() == "blink")
		{
			Serial.println("Started!");
			_DigitalOutput.Blink(3000, 3);
		}
		if (Serial.readString() == "blink-ever")
		{
			Serial.println("Started!");
			_DigitalOutput.Blink(1000);
		}
		if (Serial.readString() == "turn-on")
		{
			Serial.println("Started!");
			_DigitalOutput.TurnOn(3000);
		}
		if (Serial.readString() == "turn-ever")
		{
			Serial.println("Started!");
			_DigitalOutput.TurnOn();
		}
		if (Serial.readString() == "turn-off")
		{
			Serial.println("Started!");
			_DigitalOutput.TurnOff();
		}
		if (Serial.readString() == "toggle")
		{
			Serial.println("Started!");
			_DigitalOutput.Toggle();
		}
		
	}
}

