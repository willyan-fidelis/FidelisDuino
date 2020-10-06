#pragma once

#include "FidelisDuino_EdgeDetection.h"

FidelisDuino::EdgeDetection::TransitionDetection borda;
bool sinal_borda = false;

void setup() {

}

void loop()
{
	borda.Loop(sinal_borda);

	if (borda.IsFirstTrueDetection())
	{
		Serial.println("Primeiro evento 'True Detection' ocorrido!");
	}
	if (borda.IsFirstFalseDetection())
	{
		Serial.println("Primeiro evento 'False Detection' ocorrido!");
	}
	if (borda.OnTrueEdge())
	{
		Serial.println("Borda 'True' detectada!");
	}
	if (borda.OnFalseEdge())
	{
		Serial.println("Borda 'False' detectada!");
	}
}