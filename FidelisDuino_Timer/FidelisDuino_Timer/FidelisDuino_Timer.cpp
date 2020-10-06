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
#include "FidelisDuino_Timer.h"

FidelisDuino::Timer::TON TestTimerTON;
bool TonEval = false;
FidelisDuino::Timer::TOF TestTimerTOF;
bool TofEval = false;

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
void loop_test() {
	TestTimerTON.Loop(15000);
	TestTimerTOF.Loop(5000);
	if (millis() < 5000)
	{
		Serial.println("Less than 5 seconds passed after arduino become on!");
	}
	if (TestTimerTON.Input(TonEval))
	{
		Serial.println("Timer TON done!");
		TestTimerTOF.Input();
	}
	
	if (TestTimerTOF.GetOutput())
	{
		Serial.println("Timer TOF done!");
	}
}

int main()
{
    std::cout << "Started! \n";
	Serial.println("Testando TON!");
	start(1000, loop_test);
	while (true) {}
}

// Executar programa: Ctrl + F5 ou Menu Depurar > Iniciar Sem Depuração
// Depurar programa: F5 ou menu Depurar > Iniciar Depuração

// Dicas para Começar: 
//   1. Use a janela do Gerenciador de Soluções para adicionar/gerenciar arquivos
//   2. Use a janela do Team Explorer para conectar-se ao controle do código-fonte
//   3. Use a janela de Saída para ver mensagens de saída do build e outras mensagens
//   4. Use a janela Lista de Erros para exibir erros
//   5. Ir Para o Projeto > Adicionar Novo Item para criar novos arquivos de código, ou Projeto > Adicionar Item Existente para adicionar arquivos de código existentes ao projeto
//   6. No futuro, para abrir este projeto novamente, vá para Arquivo > Abrir > Projeto e selecione o arquivo. sln
