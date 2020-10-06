#pragma once

#include <iostream>
#include <cstdio>
#include <ctime>
#include <functional>
#include <chrono>
#include <future>
#include <cstdio>

#include "FidelisDuino_EmulateSerial.h"

namespace FidelisDuino {
	namespace EmulateArduinoCore {
		//Local vars:
		namespace Local {
			unsigned long actual_time = 0;
			int interval = 50;
		}
	}
}


class _RunCoreUtil {
public:
	_RunCoreUtil() {
		start(FidelisDuino::EmulateArduinoCore::Local::interval, RunCoreUtilFunction);
	}
	unsigned long GetMillis() {
		return FidelisDuino::EmulateArduinoCore::Local::actual_time;
	}
private:
	
	void start(int _interval, std::function<void(void)> func)
	{
		//https://stackoverflow.com/questions/30425772/c-11-calling-a-c-function-periodically/30425945#30425945
		bool _execute = true;
		std::thread([=]()
			{
				while (_execute) {
					func();
					std::this_thread::sleep_for(
						std::chrono::milliseconds(_interval));
				}
			}).detach();
	}
	static void RunCoreUtilFunction() {
		//Serial.println("One Secoond passed!");
		FidelisDuino::EmulateArduinoCore::Local::actual_time = FidelisDuino::EmulateArduinoCore::Local::actual_time + FidelisDuino::EmulateArduinoCore::Local::interval;
	}
};



_RunCoreUtil RunCoreUtil;


unsigned long millis() {
	return RunCoreUtil.GetMillis();
}

//Define the 'String' from arduino
typedef std::string String;

typedef bool boolean;

// Checks for a digit (0 through 9).
inline boolean isDigit(int c) {
	return (isdigit(c) == 0 ? false : true);
}