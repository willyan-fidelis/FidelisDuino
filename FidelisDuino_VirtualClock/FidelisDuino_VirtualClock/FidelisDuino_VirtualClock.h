#pragma once

namespace FidelisDuino {
	namespace VirtualClock{
		class VirtualClock {
		private:
			bool _isSet = false;
			unsigned long _hrs, _min, _sec, _weekDay;
			unsigned long _lastScan = 0;
			unsigned long _actualScan = 0;
			unsigned long _accMillis = 0;

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

			//Converte uma unidade mais baixa em outra de maior valor. Exemplo: converte milli sec em sec:
			unsigned long ExtractUpperUnit(unsigned long lowUnitValue, unsigned long UnitsToChangeToUp, unsigned long& remainde) {
				if (lowUnitValue >= UnitsToChangeToUp)
				{
					unsigned long _ToAdd = (int)lowUnitValue / UnitsToChangeToUp;
					remainde = lowUnitValue - (_ToAdd * UnitsToChangeToUp);
					return _ToAdd;
				}
				else {
					remainde = lowUnitValue;
					return 0;
				}
			}
		public:
			VirtualClock() {
				_hrs = 0;
				_min = 0;
				_sec = 0;
				_weekDay = 0;
			}
			void SetTime(int hrs, int min, int sec, int weekDay) {
				_isSet = true;
				_hrs = hrs;
				_min = min;
				_sec = sec;
				_weekDay = weekDay;
			}
			bool IsSet() {
				return true;
			}
			void Loop() {

			}
			void GetTime(unsigned long& hrs, unsigned long& min, unsigned long& sec, unsigned long& weekday) {
				hrs = _hrs;
				min = _min;
				sec = _sec;
				weekday = _weekDay;
			}
			void Update() {
				_actualScan = millis();
				unsigned long delta = longDelta(_lastScan, _actualScan);

				_accMillis = _accMillis + delta;
				
				unsigned long _UpperUnits;
				//_accMillis = 127007;

				//Milli seconds to seconds:
				_UpperUnits = ExtractUpperUnit(_accMillis, 1000, _accMillis);
				if (_UpperUnits == 0) { return; }
				_sec = _sec + _UpperUnits;

				//Seconds to minutes:
				_UpperUnits = ExtractUpperUnit(_sec, 60, _sec);
				if (_UpperUnits == 0) { return; }
				_min = _min + _UpperUnits;

				//Minutes to hours:
				_UpperUnits = ExtractUpperUnit(_min, 60, _min);
				if (_UpperUnits == 0) { return; }
				_hrs = _hrs + _UpperUnits;

				//Hours to week day:
				_UpperUnits = ExtractUpperUnit(_hrs, 24, _hrs);
				if (_UpperUnits == 0) { return; }
				_weekDay = _weekDay + _UpperUnits;

				//Week day to limbo...:
				//_UpperUnits = ExtractUpperUnit(_weekDay, 6, _weekDay);
				//if (_UpperUnits == 0) { return; }
				//limbo = limbo + _UpperUnits;

				if (_weekDay > 6)
				{
					_weekDay = 0;
				}

				//ExtractUpperUnit(_accMillis, 1000, _accMillis);
			}
		};
	}
}