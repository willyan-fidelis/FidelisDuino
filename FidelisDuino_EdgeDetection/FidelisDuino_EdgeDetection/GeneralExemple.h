#pragma once

#include "FidelisDuino_EdgeDetection.h"

FidelisDuino::EdgeDetection::EdgeDetection borda;
bool sinal_borda = false;

void setup() {

}

void loop()
{
	borda.Loop(sinal_borda);
	if (borda.OnDetection())
	{
		Serial.println("Edge was detected!");
	}
}