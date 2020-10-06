#pragma once

#include "FidelisDuino_Timer.h"
#include "FidelisDuino_EdgeDetection.h"

namespace FidelisDuino {
	namespace IO {
		namespace Output {
			class ClockGenerator {
			private:
				FidelisDuino::Timer::TON TimeOn;
				FidelisDuino::Timer::TON TimeOff;
				bool StStarted = false;
				bool StOff = false;
				bool OutpRun = false;
				
			public:
				void Loop(bool evaluate, int OnTime, int OffTime) {
					StStarted = evaluate;

					TimeOff.Loop(OffTime);
					TimeOn.Loop(OnTime);

					if (TimeOff.Input(StOff)) { StOff = false; }
					if (TimeOn.Input(StStarted && !StOff)) { StOff = true; }

					OutpRun = StStarted && !StOff;
				}
				bool Out() { return OutpRun; }
			};
			//Gera uma quantidade especifica de pulsos na saida.
			class ClockGeneratorCount {
			private:
				ClockGenerator clock;
				FidelisDuino::EdgeDetection::EdgeDetection detectChanges;
				bool started = false;
				int actual_cycle = 0;
				int _OnTime = 0; int _OffTime = 0; int _count = 0;
			public:
				void Count(int OnTime, int OffTime, int count) {
					_OnTime = OnTime;
					_OffTime = OffTime;
					_count = count;
				}
				void Loop() {
					Loop(_OnTime, _OffTime, _count);
				}
				void Loop(int OnTime, int OffTime, int count) {

					clock.Loop(started, OnTime, OffTime);
					detectChanges.Loop(!clock.Out());
					if (started && detectChanges.OnDetection())
					{
						actual_cycle = actual_cycle + 1;
						if (actual_cycle >= count)
						{
							started = false;
						}
					}
				}
				void Start() {
					started = true;
					actual_cycle = 0;
				}
				bool Out() {
					return clock.Out();
				}
			};
		}
	}
}