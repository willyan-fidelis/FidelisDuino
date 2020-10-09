#pragma once

#include "FidelisDuino_Timer.h"
#include "FidelisDuino_EdgeDetection.h"
#include "FidelisDuino_Util.h"

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
				void Toggle() {
					if (IsStarted())
					{
						TurnOff();
					}
					else
					{
						TurnOn();
					}
				}
				bool IsStarted() {
					return TurnState || clockCounter.IsStarted();
				}
				bool Out() {
					return TurnState || clockCounter.Out();
				}
			};
			class AnalogicOutput {
			private:
				typedef void(*OnChangeFunction)(int Target, int Out);
				OnChangeFunction _OnChange;
				int OutValue = 0;
				int TargetValue = 0;

				int TargetActual = 0;
				bool StFadeOn = false;
				bool StFadeOff = false;

				FidelisDuino::Timer::Ticker ticker;
				int _msPerPercent = 0;
				void _Write(int target) {
					OutValue = FidelisDuino::Util::Math::map_int(target, 0, 100, 0, 1023);
					_OnChange(target, OutValue);
				}
			public:
				void Loop(int target_min, int target_max, int out_min, int out_max,OnChangeFunction OnChange) {
					_OnChange = OnChange;
					ticker.Loop();
					if (ticker.OnTickerEvent())
					{
						if (StFadeOn)
						{
							TargetActual = TargetActual + 1;
							_Write(TargetActual);
							if (TargetActual >= TargetValue)
							{
								ticker.detach();
								//StFadeOn = false;
							}
						}
						if (StFadeOff)
						{
							TargetActual = TargetActual - 1;
							_Write(TargetActual);
							if (TargetActual <= TargetValue)
							{
								ticker.detach();
								//StFadeOff = false;
							}
						}
					}
					if (ticker.OnStopTickerEvent()) {
						if (StFadeOn)
						{
							_Write(TargetValue);
							StFadeOn = false;
						}
						if (StFadeOff)
						{
							_Write(0);
							StFadeOff = false;
						}
					}
				}
				void Write(int target) {
					TargetActual = target;
					StFadeOn = false;
					StFadeOff = false;
					OutValue = FidelisDuino::Util::Math::map_int(target, 0, 100, 0, 1023);
					_OnChange(target, OutValue);
				}
				void Fade(int value, int msPerPercent) {
					if (value > TargetActual)
					{
						FadeOn(value, msPerPercent);
					}
					if (value < TargetActual)
					{
						FadeOff(value, msPerPercent);
					}
				}
				void FadeOn(int value, int msPerPercent) {
					_msPerPercent = msPerPercent;
					TargetValue = value;

					//TargetActual = 0;
					StFadeOn = true;
					StFadeOff = false;
					ticker.attach(msPerPercent, []()->void {});//OnFadeOnTicker(this);
				}
				void FadeOff(int value, int msPerPercent) {
					_msPerPercent = msPerPercent;
					TargetValue = value;

					//TargetActual = 0;
					StFadeOn = false;
					StFadeOff = true;
					ticker.attach(msPerPercent, []()->void {});
				}
				void ToggleFade(int value, int msPerPercent) {
					if (TargetActual > 0)
					{
						FadeOff(0, msPerPercent);
					}
					else {
						FadeOn(value, msPerPercent);
					}
				}
			};
		}
	}
}