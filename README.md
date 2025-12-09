This is the source code of "kagouyar", a synthesizer by nozoïd.
http://nozoid.com/kagouyar
It run on on daisy seed by electrosmith.
To compile this code, you need a very outdated (and sligtlly custom) libdaisy, provided as a zip file

new in version 2 :
- twice more VCO waveforms (via long click on the waveform)
- twice more LFO waveforms (via long click on the waveform)
- twice more audio effect (via long click on the efect)
- increase max cutoff of the main VCF
- add a A/D mode (selectable in place of the A/R envelope generator)
- add a DJ mode to the VCF2 filter (low/high on the same potentiometer)
- add a very slow mode for the lfo4, 5 and 6 (up to more than 2.5h period) 
- change the LFO SYNC modulation : The frequency division now depend on the LFO Fq range, the division or multiplication goes from /128 to *128
- change the automodulation in LFO SYNC : the modulation potentiommeter allow to divide or multiply the LFO frequency with factor 1,2,3,4,8.
- The LFO CLIP modulation have a new functionality when no modulation is set : the LFO waveform is cliped in various step (like a bitcrusher)
- when "LOAD" switch is press, the potentiometters are no more updated in order to allow cut transitions
- add an option to use CV switch to control keyboard octave (LOAD + SAVE + KB13)
- use midi note velocity for the amplitude of the note
- The triangle VCO WF is more expressive
- add a curve to VCF Q for better mapping
- lots of "fine tuning"
- fix a "no sound" bug


