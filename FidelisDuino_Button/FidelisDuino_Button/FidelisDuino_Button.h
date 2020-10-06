#pragma once

#include "FidelisDuino_EdgeDetection.h"
#include "FidelisDuino_Timer.h"
#include "FidelisDuino_IO.h"

namespace FidelisDuino {
	namespace Button {
		class PushButton {
		private:
			FidelisDuino::IO::Input::InputStateMachine button;
			//Alem do 'InputStateMachine' devemos criar mais alguns estados não contemplados na classe:
			bool _AterReleasedEdge = false; //Depois de 'OnReleasedEdge' (ao apertar o btn) vem esse evento.
			bool _AterPressedEdge = false; //Depois de 'OnPressedEdge' (ao soltar o btn) vem esse evento.
		public:
			void Loop(int ParTimeTrue, int ParTimeFalse, bool input, int filter = 0) {

				
				button.Loop(ParTimeTrue, ParTimeFalse,input, filter);

				bool _r = Released();
				_AterReleasedEdge = (Released() && input) ? true : false;
				bool _p = Pressed();
				_AterPressedEdge = (_p && !input) ? true : false;
			}
			bool Released() {
				return button.StFalse();
			}
			bool Pressed() {
				return button.StTrue();
			}
			bool ChangingToReleased() {
				return button.StCtFalse();
			}
			bool ChangingToPressed() {
				return button.StCtTrue();
			}
			bool OnReleasedEdge() {
				return button.StOnFalseEdge();
			}
			bool OnPressedEdge() {
				return button.StOnTrueEdge();
			}


			bool AterReleasedEdge() { 
				return _AterReleasedEdge; 
			}
			bool AterPressedEdge() { 
				return _AterPressedEdge; 
			}

		};
		//With long click you change the menu, with short one you change the page!
		class DoubleChoicePushButtonMenu {
		private:
			PushButton MenuClick;
			PushButton PageClick;
			int ProbableNextMenu = 0;
			int ProbableNextPage = 0;
			int ActualMenu = 1; //Zero é considerado sem seleção
			int ActualPage = 0; //Zero é considerado sem seleção
			bool OnMenuChanged = false;
			bool OnPageChanged = false;

			bool ForceLoadPage = false;

			bool IgnoreMenuReleased = false; //Ignora confirmação de Seleção de Menu
		public:
			DoubleChoicePushButtonMenu() {
				
			}
			void Loop(bool input, int MenuClickPulseTime, int MenuClickInativeTime, int PageClickPulseTime, int PageClickInativeTime, int filter = 0, void(*_OnGoInPage)(int page, int menu) = [](int page, int menu)->void {}, void(*_OnGoInMenu)(int menu) = [](int menu)->void {}) {
				if (ForceLoadPage)
				{
					ForceLoadPage = false;
					_OnGoInPage(ActualPage, ActualMenu);
				}

				OnMenuChanged = false;
				OnPageChanged = false;

				MenuClick.Loop(MenuClickPulseTime, MenuClickInativeTime,input, filter);
				PageClick.Loop(PageClickPulseTime, PageClickInativeTime, input, filter);

				//Selecionado menu:
				if (MenuClick.AterPressedEdge())
				{
					ProbableNextMenu = ProbableNextMenu + 1;
					IgnoreMenuReleased = false;
				}
				//Confirmando menu:
				if (MenuClick.OnReleasedEdge())
				{
					if (!IgnoreMenuReleased)
					{
						ActualMenu = ProbableNextMenu;
						ProbableNextMenu = 0;

						ActualPage = 0;
						ProbableNextPage = 0;

						if (ActualMenu > 0)
						{
							OnMenuChanged = true;
							_OnGoInMenu(ActualMenu);
						}

						/*
						Depois de entrar na pagina o tempo continua contando e dispara um evento 'falso' de troca de menu,
						afinal o click do menu é mais longo e sempre vem depois..
						Nesses cassos 'ProbableNextMenu' é igual a zero, fazendo com que o 'ActualMenu' seja tbem zero.
						Por isso devemos validar se 'ActualMenu = 0' é igual a zero e tratar isso, mas sem disparar o evento
						de troca de menu '_OnGoInMenu' :
						*/
						if (ActualMenu == 0) { ActualMenu = 1; }
					}
				}

				//Selecionado pagina:
				if (PageClick.AterPressedEdge() && !MenuClick.AterPressedEdge())
				{
					ProbableNextPage = ProbableNextPage + 1;
					IgnoreMenuReleased = true; //Se estamos selecionando uma pagina, então ignoramos a possivel confirmção de Menu futura.
				}
				//Confirmando pagina:
				if (PageClick.OnReleasedEdge())
				{
					ActualPage = ProbableNextPage;
					ProbableNextPage = 0;

					if (ActualPage > 0)
					{
						OnPageChanged = true;
						_OnGoInPage(ActualPage, ActualMenu);
					}
				}
				
			}

			//Possibilta forçar a troca de pagina manualmente:
			void LoadPage(int menu, int page) {
				ActualMenu = menu;
				ActualPage = page;
				ForceLoadPage = true;
			}

			bool OnGoInMenu(int menu) {
				return OnMenuChanged && ActualMenu == menu;
			}
			bool OnGoInPage(int page) {
				return OnPageChanged && ActualPage == page;
			}
			bool OnGoInPage(int page, int menu) {
				return OnPageChanged && ActualPage == page && ActualMenu == menu;
			}

			int GetCurrentMenu() {
				return ActualMenu;
			}
			int GetCurrentPage() {
				return ActualPage;
			}
		};
		class SingleChoiceToggleButtonMenu {
		private:
			int ProbableNextPage = 0;
			int ActualPage = 0; //Zero é considerado sem seleção
			FidelisDuino::IO::Input::InputStateMachine button;
			FidelisDuino::Timer::TOF ChangingTime;
			FidelisDuino::EdgeDetection::EdgeDetection TransitionToChanged;

			bool ForceLoadPage = false;
		public:
			void Loop(bool input, int PageClickPulseTime, int PageClickInativeTime, int filter, void(*_OnGoInPage)(int page, int menu)/* = [](int page)->void {}*/) {
				if (ForceLoadPage)
				{
					ForceLoadPage = false;
					_OnGoInPage(ActualPage,0);
				}

				button.Loop(PageClickPulseTime, PageClickPulseTime,input,filter);
				ChangingTime.Loop(PageClickInativeTime);
				TransitionToChanged.Loop(!ChangingTime.GetOutput());
				//Selecionado pagina:
				if (button.StOnTrueEdge() || button.StOnFalseEdge())
				{
					ProbableNextPage = ProbableNextPage + 1;
					ChangingTime.Input();
				}
				//Confirmando pagina:
				if (TransitionToChanged.OnDetection())
				{
					ActualPage = ProbableNextPage;
					ProbableNextPage = 0;

					_OnGoInPage(ActualPage,0);
				}
			}
			//Possibilta forçar a troca de pagina manualmente:
			void LoadPage(int page) {
				ActualPage = page;
				ForceLoadPage = true;
			}
		};
		class DoubleOrSingleChoiceMenu {
		private:
			DoubleChoicePushButtonMenu pushButton;
			SingleChoiceToggleButtonMenu toggleButton;
			int Forced_ActualMenu = 0;
			int Forced_ActualPage = 0;
			bool ForceLoadPage = false;
		public:
			void Loop(bool IsPushButton, bool input, int MenuClickPulseTime, int MenuClickInativeTime, int PageClickPulseTime, int PageClickInativeTime, int filter, void(*_OnGoInPage)(int page, int menu) = [](int page, int menu)->void {}, void(*_OnGoInMenu)(int menu) = [](int menu)->void {}) {
				if (IsPushButton)
				{
					if (ForceLoadPage)
					{
						ForceLoadPage = false;
						pushButton.LoadPage(Forced_ActualMenu, Forced_ActualPage);
					}
					pushButton.Loop(input, MenuClickPulseTime, MenuClickInativeTime, PageClickPulseTime, PageClickInativeTime, filter, _OnGoInPage, _OnGoInMenu);
				}
				else {
					if (ForceLoadPage)
					{
						ForceLoadPage = false;
						toggleButton.LoadPage(Forced_ActualPage);
					}
					toggleButton.Loop(input, PageClickPulseTime, PageClickInativeTime, filter, _OnGoInPage);
				}
			}
			void LoadPage(int menu,int page) {
				Forced_ActualMenu = menu;
				Forced_ActualPage = page;
				ForceLoadPage = true;
			}
		};
	}
}