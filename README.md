# Kagouyar
![Kagouyar Synthesizer](https://nozoid.com/wp-content/uploads/2022/12/DSC03690b-scaled.jpg)

Firmware for the Kagouyar synthesizer by [nozoïd](http://nozoid.com/kagouyar)

![License](https://img.shields.io/badge/license-GPL--3.0-blue.svg)

## About

**Kagouyar** is a polyphonic, virtual analogue synthesizer designed by nozoïd. It combines simple usage with a huge range of sonic possibilities. Thanks to state-of-the-art technologies, it feels like an analogue synthesizer but includes numerous features and options never seen in a vintage synthesizer.

This repository contains the firmware source code that runs on **Daisy Seed** by Electrosmith.

## Main Features

### Architecture

- **4 voices of polyphony** for creating complex harmonies
- **3 oscillators per voice** with atypical waveforms and modulations
- **7 LFOs** that can inter-modulate to create complex rhythmic sequences
- **Multi-mode filter** including a 24dB/Oct low pass
- **2 audio effects** (distortion, waveshaper, echo, Doppler effect, etc.)
- **Modulation matrix** with tactile switches

### Interface

- **46 control potentiometers**
- **16 configuration and assignation tactile switches**
- **17 modulation selection tactile switches**
- **1-octave tactile keyboard**, also used to select specific options
- **27 LEDs** to display modulations, selections and options
- **No menu**: every functionality accessible with only 2 simultaneous touches

### Connectivity

- Headphone out (stereo 1/4" jack)
- Audio out (mono - balanced 1/4" jack)
- MIDI IN (5-pin DIN)
- GATE IN (0 to 5V - 1/8" mono jack)
- CV1 IN (Pitch or modulation: -5V to 5V, 1V/Oct - 1/8" mono jack)
- CV2 IN (Modulation: -5V to 5V - 1/8" mono jack)
- Power supply (9-12V DC, 500mA max)

## What's New in Version 2

- **2× more VCO waveforms** (via long click on waveform)
- **2× more LFO waveforms** (via long click on waveform)
- **2× more audio effects** (via long click on effect)
- Increased max cutoff of the main VCF
- Added **A/D mode** (selectable in place of the A/R envelope generator)
- Added **DJ mode** to the VCF2 filter (low/high on the same potentiometer)
- Added **very slow mode** for LFO 4, 5 and 6 (up to more than 2.5h period)
- Changed LFO SYNC modulation: frequency division now depends on LFO Fq range, division or multiplication goes from /128 to ×128
- Changed automodulation in LFO SYNC: modulation potentiometer allows dividing or multiplying LFO frequency by factors 1, 2, 3, 4, 8
- New LFO CLIP modulation functionality when no modulation is set: LFO waveform is clipped in various steps (like a bitcrusher)
- When "LOAD" switch is pressed, potentiometers are no longer updated to allow cut transitions
- Added option to use CV switch to control keyboard octave (LOAD + SAVE + KB13)
- MIDI note velocity now controls note amplitude
- Triangle VCO waveform is more expressive
- Added curve to VCF Q for better mapping
- Lots of fine tuning
- Fixed "no sound" bug

## Building

### Prerequisites

To compile this code, you will need:

- A very outdated (and slightly custom) version of **libdaisy**, provided in the `git_daisy.zip` file
- GCC ARM toolchain (arm-none-eabi-gcc)
- Make

## Documentation

- [User Manual](https://nozoid.com/data/doc_kagouyar.pdf)
- [Official Website](https://nozoid.com/kagouyar/)

## License

This project is licensed under the **GPL-3.0 License**. See the [LICENSE.txt](LICENSE.txt) file for details.

## Author

Created by **Cyrille Henry -nozoïd** - [Website](https://nozoid.com) | [GitHub](https://github.com/ch-nry)

## Links

- [Vimeo](https://vimeo.com/nozoid)
- [Bluesky](https://bsky.app/profile/nozoid.bsky.social)
- [Mastodon](https://mastodon.social/@nozoid)
- [Instagram](https://www.instagram.com/nozoid_synth/)

---

*Designed for sound exploration, the Kagouyar is the ultimate synthesizer for drone-oriented performance, soft ambient music and hardcore techno.*


