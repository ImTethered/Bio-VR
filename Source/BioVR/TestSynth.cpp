// Fill out your copyright notice in the Description page of Project Settings.

#include "TestSynth.h"

void UTestSynth::Init(const int32 SampleRate)
{
	NumChannels = 1;

	// Initialize the DSP objects
	Osc.Init(SampleRate);
	Osc.SetFrequency(440.0f);
	Osc.Start();
}

void UTestSynth::OnGenerateAudio(TArray<float>& OutAudio)
{
	// Perform DSP operations here
	for (int32 Sample = 0; Sample < OutAudio.Num(); ++Sample)
	{
		OutAudio[Sample] = Osc.Generate();
	}
}

void UTestSynth::SetFrequency(const float InFrequencyHz)
{
	// Use this protected base class method to push a lambda function which will safely execute in the audio render thread.
	SynthCommand([this, InFrequencyHz]()
	{
		Osc.SetFrequency(InFrequencyHz);
		Osc.Update();
	});
}