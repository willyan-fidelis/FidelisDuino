#pragma once

namespace FidelisDuino {
	namespace Util {
		namespace Math {
			bool InRange(int toBeTested, int upLevel, int lowLevel)
			{
				if ((upLevel >= toBeTested) & (lowLevel <= toBeTested))
				{
					return true;
				}
				else
				{
					return false;
				}
			}

			bool InRangeCompare(int toBeTested, int toBeCompared, int tolerance)
			{

				if (InRange(toBeTested, toBeCompared + tolerance, toBeCompared - tolerance))
				{
					return true;
				}
				else
				{
					return false;
				}
			}

			long unsigned longDelta(long unsigned& lastScan, long unsigned actualScan)
			{
				long unsigned delta;

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

			int scale(int _totalValue, int _totalPercent, int _targetPercent)
			{
				return (_totalValue * _targetPercent) / _totalPercent;
			}

			int map_int(int x, int in_min, int in_max, int out_min, int out_max) {
				return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
			}

			bool isValidNumber(String str) {
				for (int i = 0;i < str.length();i++)
				{
				#if defined( ___DEBUG_ON___ )
					if (isDigit(str.at(i))) return true;
				#else
					if (isDigit(str.charAt(i))) return true;
				#endif
				}
				return false;
			}
		}
	}
}