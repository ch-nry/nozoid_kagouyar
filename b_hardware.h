// copy and adapted from daisy_pod
// 2020 cyrille henry

#pragma once
#ifndef Kagouyar_BSP_H
#define Kagouyar_BSP_H

#include "daisy_seed.h"

namespace daisy
{
/**
    @brief Class that handles initializing all of the hardware specific to the Daisy Patch Board. \n
    Helper funtions are also in place to provide easy access to built-in controls and peripherals.
    @author Stephen Hensley
    @date November 2019
    @ingroup boards
*/
class Kagouyar
{
  public:
    Kagouyar() {}
    ~Kagouyar() {}

    /** Init related stuff. */
    void Init();
    void DelayMs(size_t del);
    void StartAudio(AudioHandle::InterleavingAudioCallback cb);
    void StartAudio(AudioHandle::AudioCallback cb);
    void ChangeAudioCallback(AudioHandle::InterleavingAudioCallback cb);
    void ChangeAudioCallback(AudioHandle::AudioCallback cb);
    void StopAudio();
    void SetAudioSampleRate(SaiHandle::Config::SampleRate samplerate);
    float AudioSampleRate();
    void SetAudioBlockSize(size_t blocksize);
    size_t AudioBlockSize();
    float AudioCallbackRate();
    void StartAdc();
    void UpdateAnalogControls();
    float GetKnobValue(uint32_t k);
    void test_out(bool out);

    void ClearLeds();
    void UpdateLeds();

    /** Public Members */
    DaisySeed     seed;
    AnalogControl knobs_[nb_CV];

    LedDriverPca9685<1, false> led_driver_;
    ShiftRegister595 binary_led;
	MidiUartTransport midi;
	
  private:
    void   InitKnobs();
    void   InitLeds();
    void   InitBinaryLeds();
    void   InitMidi();
    void   InitKeyboard();
    void   InitPin();
    float  sample_rate_;
    size_t block_size_;
};

} // namespace daisy
#endif
