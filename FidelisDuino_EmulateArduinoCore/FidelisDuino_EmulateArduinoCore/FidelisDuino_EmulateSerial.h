#pragma once

#include <iostream>
#include <cstdio>
#include <ctime>
#include <functional>
#include <chrono>
#include <future>
#include <cstdio>
#include <string>

namespace FidelisDuino {
	namespace EmulateSerial {
		//Local vars:
		namespace Local {
			bool _available = false;
			std::string _readString = "";
			void listen_func(std::string data) {
				//std::cout << "New Data from Serial!\n";
				FidelisDuino::EmulateSerial::Local::_readString = data;
				FidelisDuino::EmulateSerial::Local::_available = true;
			}
		}
		//Emulates arduino 'Serial' functions
		class Serial {
		private:
			void listen(std::function<void(std::string)> func)
			{
				std::cout << "Serial Listening!\n";
				bool _execute = true;
				std::thread([=]()
					{
						while (_execute) {
							std::string arrived = "";

							//https://stackoverflow.com/questions/10464344/reading-from-stdin-in-c
							for (std::string line; std::getline(std::cin, line);) {
								//std::cout << line << std::endl;
								arrived = line;
								func(line);
							}

							std::this_thread::sleep_for(
								std::chrono::milliseconds(3000));
						}
					}).detach();
			}
		public:
			Serial() {
				listen(FidelisDuino::EmulateSerial::Local::listen_func);
			}
			void println(std::string msg) {
				std::cout << msg + "\n";
			}
			void print(const char* msg) {
				std::cout << msg;
			}
			void print(int msg) {
				std::cout << msg;
			}
			bool available() {
				bool ret = FidelisDuino::EmulateSerial::Local::_available;
				FidelisDuino::EmulateSerial::Local::_available = false;
				return ret;
			}
			std::string readString() {
				return FidelisDuino::EmulateSerial::Local::_readString;
			}
		};
	}
}




//The 'Serial' object instance of '_Serial' must be global to seem the same like Arduino IDE:
FidelisDuino::EmulateSerial::Serial Serial;