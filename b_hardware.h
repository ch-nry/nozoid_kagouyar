// --------------------------------------------------------------------------
// This file is part of the KAGOUYAR firmware.
//
//    KAGOUYAR firmware is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    KAGOUYAR firmware is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with KAGOUYAR firmware. If not, see <http://www.gnu.org/licenses/>.
// --------------------------------------------------------------------------

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
    double AudioSampleRate();
    void SetAudioBlockSize(size_t blocksize);
    size_t AudioBlockSize();
    double AudioCallbackRate();
    void StartAdc();
    void UpdateAnalogControls();
    double GetKnobValue(uint32_t k);
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
    double  sample_rate_;
    size_t block_size_;
};

} // namespace daisy
#endif
