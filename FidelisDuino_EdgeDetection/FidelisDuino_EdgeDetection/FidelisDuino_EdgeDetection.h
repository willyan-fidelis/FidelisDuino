/*
Date: 2020-09-09
Title: EdgeDetection
Namespace: FidelisDuino::EdgeDetection
Autor: Willyan Sergio Fidelis
Type file: Header
--------------------------------------------Remarks --------------------------------------------
This namespace have classes that help to handle with edge, like a button click and so on.
--------------------------------------------Remarks --------------------------------------------
*/

#pragma once

#if defined( ___DEBUG_ON___ )
#include "FidelisDuino_EmulateArduinoCore.h"
#else
#include "Arduino.h"
#endif

namespace FidelisDuino
{
	namespace EdgeDetection
	{

		class EdgeDetection
		{
		private:
			bool state = false;
			bool _input = false;
			bool state_ons = false;
			bool firstScan = true; //Isso garante que não seja disparado nehuma detection falsa no primeiro scan de leitura!
		public:
			void Loop()
			{
				state = ((_input && !state_ons) && !firstScan) ? true : false;
				state_ons = _input;

				firstScan = false;
			}
			void Loop(bool input)
			{
				_input = input;
				Loop();
			}
			bool OnDetection()
			{
				return state;
			}
			bool OnDetection(bool input)
			{
				_input = input;
				return OnDetection();
			}
			bool GetInput() {
				return _input;
			}

		};
		//Detect true and false edge's
		class TransitionDetection
		{
		private:
			EdgeDetection TrueDetection;
			EdgeDetection FalseDetection;
			int FirstTimeTrueEdge; //Guarda o status paran pegar a primeira detecçao!
			int FirstTimeFalseEdge;//Guarda o status paran pegar a primeira detecçao!


		public:
			TransitionDetection()
			{
				FirstTimeTrueEdge = 0;
				FirstTimeFalseEdge = 0;
			}
			void Loop(bool input) {
				TrueDetection.Loop(input);
				FalseDetection.Loop(!input);

				if (FirstTimeTrueEdge == 1) { FirstTimeTrueEdge = 2; }
				if (FirstTimeFalseEdge == 1) { FirstTimeFalseEdge = 2; }

				if (FirstTimeTrueEdge == 0 && TrueDetection.OnDetection()) { FirstTimeTrueEdge = 1; }
				if (FirstTimeFalseEdge == 0 && FalseDetection.OnDetection()) { FirstTimeFalseEdge = 1; }
			}
			bool OnTrueEdge() {
				return TrueDetection.OnDetection();
			}
			bool OnFalseEdge() {
				return FalseDetection.OnDetection();
			}
			bool IsFirstTrueDetection() {
				if (FirstTimeTrueEdge == 1) { return true; }
				else { return false; }
			}
			bool IsFirstFalseDetection() {
				if (FirstTimeFalseEdge == 1) { return true; }
				else { return false; }
			}
		};
	}
}