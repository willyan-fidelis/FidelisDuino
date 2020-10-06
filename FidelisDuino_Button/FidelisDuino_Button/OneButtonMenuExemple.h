#pragma once

#include "FidelisDuino_Button.h"

void setup() {

}

FidelisDuino::Button::DoubleChoicePushButtonMenu MenuPushButton;
bool signal_push_button = false;

FidelisDuino::Button::SingleChoiceToggleButtonMenu MenuToggleButton;
bool signal_toggle_button = false;

FidelisDuino::Button::DoubleOrSingleChoiceMenu DoubleOrSingleButton;
bool signal_DoubleOrSingle_button = false;
void loop() {
	if (false)
	{
		MenuPushButton.Loop(signal_push_button, 1000, 3000, 100, 2000, 50, [](int page, int menu)->void {
			Serial.print("Going into page ");
			Serial.print(page);
			Serial.print(" and menu ");
			Serial.print(menu);
			Serial.println("");
			}, [](int menu)->void {
				Serial.print("Going into menu ");
				Serial.print(menu);
				Serial.println("");
			});

		/*
		if (MenuPushButton.OnGoInMenu(1)) { Serial.println("OnGoInMenu 1"); }
		if (MenuPushButton.OnGoInMenu(2)) { Serial.println("OnGoInMenu 2"); }
		if (MenuPushButton.OnGoInMenu(3)) { Serial.println("OnGoInMenu 3"); }
		if (MenuPushButton.OnGoInMenu(4)) { Serial.println("OnGoInMenu 4"); }
		if (MenuPushButton.OnGoInMenu(5)) { Serial.println("OnGoInMenu 5"); }
		if (MenuPushButton.OnGoInMenu(6)) { Serial.println("OnGoInMenu 6"); }
		if (MenuPushButton.OnGoInMenu(7)) { Serial.println("OnGoInMenu 7"); }
		if (MenuPushButton.OnGoInMenu(8)) { Serial.println("OnGoInMenu 8"); }
		if (MenuPushButton.OnGoInMenu(9)) { Serial.println("OnGoInMenu 9"); }
		if (MenuPushButton.OnGoInMenu(10)) { Serial.println("OnGoInMenu 10"); }

		if (MenuPushButton.OnGoInPage(1)) { Serial.println("OnGoInPage 1"); }
		if (MenuPushButton.OnGoInPage(2)) { Serial.println("OnGoInPage 2"); }
		if (MenuPushButton.OnGoInPage(3)) { Serial.println("OnGoInPage 3"); }
		if (MenuPushButton.OnGoInPage(4)) { Serial.println("OnGoInPage 4"); }
		if (MenuPushButton.OnGoInPage(5)) { Serial.println("OnGoInPage 5"); }
		if (MenuPushButton.OnGoInPage(6)) { Serial.println("OnGoInPage 6"); }
		if (MenuPushButton.OnGoInPage(7)) { Serial.println("OnGoInPage 7"); }
		if (MenuPushButton.OnGoInPage(8)) { Serial.println("OnGoInPage 8"); }
		if (MenuPushButton.OnGoInPage(9)) { Serial.println("OnGoInPage 9"); }
		if (MenuPushButton.OnGoInPage(10)) { Serial.println("OnGoInPage 10"); }

		if (MenuPushButton.OnGoInPage(3, 1)) { Serial.println("OnGoIn Page 3 And Menu 1"); }
		*/
	}

	if (false)
	{
		MenuToggleButton.Loop(signal_toggle_button, 1000, 5000, 50, [](int page)->void {
			Serial.print("Going into page ");
			Serial.print(page);
			Serial.println("");
		});
	}

	if (true)
	{
		DoubleOrSingleButton.Loop(false,signal_push_button, 1000, 3000, 100, 2000, 50, [](int page, int menu)->void {
			Serial.print("Going into page ");
			Serial.print(page);
			Serial.print(" and menu ");
			Serial.print(menu);
			Serial.println("");
			}, [](int menu)->void {
				Serial.print("Going into menu ");
				Serial.print(menu);
				Serial.println("");
			});
	}

	if (Serial.available())
	{
		String data = Serial.readString();
		Serial.println(data);
		if (Serial.readString() == "5")
		{
			Serial.println("5 arrived from serial!");
		}
		if (Serial.readString() == "1")
		{
			signal_push_button = true;
			signal_toggle_button = true;
			signal_DoubleOrSingle_button = true;
		}
		if (Serial.readString() == "0")
		{
			signal_push_button = false;
			signal_toggle_button = false;
			signal_DoubleOrSingle_button = false;
		}
	}
}