//Teste the lib functions here!

/*
Date: 2020-09-09
Title: Teste purpose
Namespace: None
Autor: Willyan Sergio Fidelis
Type file: C plus plus file
--------------------------------------------Remarks --------------------------------------------
This .cpp file is 'teste purpose' only.
Please see the correct .h file and import in your app!

In your VS project properties 'Configuration Properties -> Directory VC++ -> Include Directory' add follow path and all your lib folders:
C:\arduino_portable\arduino_esp8266\portable\packages\esp8266\hardware\esp8266\2.7.4\cores\esp8266
C:\arduino_portable\arduino_esp8266\portable\sketchbook\libraries\FidelisDuino
--------------------------------------------Remarks --------------------------------------------
*/

/*Fisrt define '___DEBUG_ON___' var:*/
#if !defined( ___DEBUG_ON___ )
#define ___DEBUG_ON___
#endif

/*Now define the arduno lib or emulate it if in debug mode is on:*/
#if defined( ___DEBUG_ON___ )
#include "FidelisDuino_EmulateArduinoCore.h"
#else
#include "Arduino.h"
#endif

/*Now afetr 'defining the arduno lib or emulate it' we can call all files becouse the 'arduno lib/emulate' context already exists: */
#include <iostream>
/*Exemples:*/
#include "TickerExemple.h"

void start(int interval, std::function<void(void)> func)
{
	//https://stackoverflow.com/questions/30425772/c-11-calling-a-c-function-periodically/30425945#30425945
	bool _execute = true;
	std::thread([=]()
		{
			while (_execute) {
				func();
				std::this_thread::sleep_for(
					std::chrono::milliseconds(interval));
			}
		}).detach();
}


int main()
{
	setup();
	std::cout << "Hello World at fisrt time!\n";
	Serial.println("This has printed in 'Serial.println' format, like arduino do it!");
	start(10, loop);
	while (true) {}
}