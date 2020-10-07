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
				void Count(int OnTime, int OffTime, int count = 0) {
					_OnTime = OnTime;
					_OffTime = OffTime;
					_count = count;
				}
				void Loop() {
					Loop(_OnTime, _OffTime, _count);
				}
				void Loop(int OnTime, int OffTime, int count = 0) {

					clock.Loop(started, OnTime, OffTime);
					detectChanges.Loop(!clock.Out());
					if (count == 0 && started)
					{
						return;
					}
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
				void Stop() {
					started = false;
					actual_cycle = 0;
				}
				bool IsStarted() {
					return started;
				}
				bool Out() {
					return clock.Out();
				}
			};
			class DigitalOutput {
			private:
				FidelisDuino::IO::Output::ClockGeneratorCount clockCounter;
				FidelisDuino::EdgeDetection::TransitionDetection detectChangesCC;

				bool TurnState = false;
				FidelisDuino::EdgeDetection::TransitionDetection detectChangesTS;

				typedef void(*OnChangeFunction)(bool ChangeTo);
				OnChangeFunction _OnChange;
			public:
				void Loop(OnChangeFunction OnChange) {
					_OnChange = OnChange;

					clockCounter.Loop();

					detectChangesCC.Loop(clockCounter.Out());
					detectChangesTS.Loop(TurnState);

					if (detectChangesCC.OnTrueEdge()) { OnChange(true); }
					if (detectChangesCC.OnFalseEdge()) { OnChange(false); }

					if (detectChangesTS.OnTrueEdge()) { OnChange(true); }
					if (detectChangesTS.OnFalseEdge()) { OnChange(false); }
				}
				void TurnOn() {
					TurnState = true;
					clockCounter.Stop();
				}
				void TurnOff() {
					TurnState = false;
					clockCounter.Stop();
				}
				void TurnOn(int time) {
					clockCounter.Count(time, 1000, 1);
					clockCounter.Start();
				}
				void Blink(int time) {
					TurnState = false;
					clockCounter.Count(time, time);
					clockCounter.Start();
				}
				void Blink(int time, int times) {
					TurnState = false;
					clockCounter.Count(time, time, times);
					clockCounter.Start();
				}
				bool IsStarted() {
					return TurnState || clockCounter.IsStarted();
				}
				bool Out() {
					return TurnState || clockCounter.Out();
				}
			};
		}
	}
}