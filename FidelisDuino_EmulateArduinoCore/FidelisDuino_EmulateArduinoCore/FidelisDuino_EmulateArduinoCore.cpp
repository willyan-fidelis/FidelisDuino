//Esse arquivo so serve para proposito de teste. Os arquivos .h contem o codigo a ser reutilizado em outros projetos.

#include <iostream>
#include <cstdio>
#include <ctime>
#include <functional>
#include <chrono>
#include <future>
#include <cstdio>

#include "FidelisDuino_EmulateArduinoCore.h"

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
	if (millis() > 5000)
	{
		Serial.println("More than 5 seconds passed after arduino become on!");
	}
}
int main()
{
    std::cout << "Hello World!\n";
	Serial.println("Serial function emulated!");
	start(1000, loop_test);
	while (true){}
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
