#pragma once

//The follow statement could be add here or before '#include' this .h file:
/*
#if defined( ___DEBUG_ON___ )
#include "FidelisDuino_EmulateArduinoCore.h"
#else
#include "Arduino.h"
#endif
*/

//https://medium.com/jungletronics/arduino-multi-file-sketch-583a7833162c
//https://github.com/giljr/Ardu_Serie/tree/master/_45_Multifile
//https://forum.arduino.cc/index.php?topic=370651.0

namespace FidelisDuino {
	namespace Timer {
		class TON
		{
		private:
			unsigned long _lastScan = 0;
			unsigned long _actualScan = 0;
			unsigned long _acc = 0;    //acumulado
			unsigned long _target = 0; //desejado
			bool _condition = false;
			bool _result = false;
			unsigned long longDelta(unsigned long& lastScan, unsigned long actualScan)
			{
				unsigned long delta;

				if (actualScan >= lastScan)//Normal condition:
				{
					delta = actualScan - lastScan;
				}
				else if (actualScan < lastScan) //Overflow condition:
				{
					delta = (4294967295 - lastScan) + lastScan;
				}
				lastScan = actualScan;
				return delta;
			}

		public:
			//Para contar chame 'count' ou 'evaluate' para verificar resultado do timer chame 'output' e be happy
			bool GetOutput() {
				return _result;
			}
			bool GetInput() {
				return _condition;
			}
			//Every time it is called the timer continue count, other wise it is stopped emediatilly.
			//'time' parametre is optional here, can be set at 'input', 'evaluate' and 'loop' methods
			bool Input(unsigned long time = 0)
			{
				if (time > 0) { _target = time; }
				_condition = true;
				return _result;
			}
			//Evaluate a specific condition
			bool Input(bool evaluate, unsigned long time = 0)
			{
				if (time > 0) { _target = time; }
				_condition = evaluate;
				return _result;
			}

			//This function evaluate multiples times. The first one(time) is mandatory and is the biggest and so on.
			int InputMultipleEvaluate(bool evaluate, unsigned long time, unsigned long time_2 = 0, unsigned long time_3 = 0, unsigned long time_4 = 0, unsigned long time_5 = 0)
			{
				_target = time;
				_condition = evaluate;
				if (_result)
				{
					return 1;
				}
				else if (time_2 >= _acc)
				{
					return 2;
				}
				else if (time_3 >= _acc)
				{
					return 3;
				}
				else if (time_4 >= _acc)
				{
					return 4;
				}
				else if (time_5 >= _acc)
				{
					return 5;
				}
				else
				{
					return 0;
				}
			}

			//Call it on loop for every things gonna be ok
			//'time' parametre is optional here, can be set at 'input', 'evaluate' and 'loop' methods
			void Loop(unsigned long time = 0)
			{
				if (time > 0) { _target = time; }

				_actualScan = millis();
				long delta = longDelta(_lastScan, _actualScan);
				if (_condition)
				{
					if (!_result)
					{
						_acc = _acc + delta;
						if (_acc >= _target)
						{
							_result = true;
						}
					}
				}
				else
				{
					_acc = 0;
					_result = false;
				}
				_condition = false;
			}
			void Loop(unsigned long time, bool evaluate)
			{
				_condition = evaluate;
				Loop(time);
			}

		};
		class TOF
		{
		private:
			unsigned long _lastScan = 0;
			unsigned long _actualScan = 0;
			unsigned long _acc = 0;    //acumulado
			unsigned long _target = 0; //desejado
			bool _condition = false;
			bool _result = false;
			bool _started = false;
			bool _done = false;
			unsigned long longDelta(unsigned long& lastScan, unsigned long actualScan)
			{
				unsigned long delta;

				if (actualScan >= lastScan)//Normal condition:
				{
					delta = actualScan - lastScan;
				}
				else if (actualScan < lastScan) //Overflow condition:
				{
					delta = (4294967295 - lastScan) + lastScan;
				}
				lastScan = actualScan;
				return delta;
			}

		public:
			//Para contar chame 'count' ou 'evaluate' para verificar resultado do timer chame 'output' e be happy
			bool GetOutput() {
				return _result;
			}
			bool GetInput() {
				return _condition;
			}
			//Every time it is called the timer continue count, other wise it is stopped emediatilly.
			//'time' parametre is optional here, can be set at 'input', 'evaluate' and 'loop' methods
			void Input(unsigned long time = 0)
			{
				_started = true;
				if (time > 0) { _target = time; }
				_condition = true;
			}
			//Evaluate a specific condition
			bool Input(bool evaluate, unsigned long time = 0)
			{
				if (evaluate) { _started = true; }
				if (time > 0) { _target = time; }
				
				_condition = evaluate;
				return evaluate || _result;
			}

			//Call it on loop for every things gonna be ok
			//'time' parametre is optional here, can be set at 'input', 'evaluate' and 'loop' methods
			void Loop(unsigned long time = 0)
			{
				if (time > 0) { _target = time; }
				_actualScan = millis();
				long delta = longDelta(_lastScan, _actualScan);

				if (_condition) { _done = false; }

				if (!_condition && _started)
				{
					if (_result)
					{
						_acc = _acc + delta;
						if (_acc >= _target)
						{
							_result = false;
							_started = false;
							_done = true;
						}
					}
				}
				else if ((!_condition && !_started && !_done) || _done)
				{
					_acc = 0;
					_result = false;
				}
				else
				{
					_acc = 0;
					_result = true;
				}

				
				_condition = false;
			}
			void Loop(unsigned long time, bool evaluate)
			{
				_condition = evaluate;
				Loop(time);
			}

		};
	}
}