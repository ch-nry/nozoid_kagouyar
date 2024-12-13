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

// optimisation : tout les nombre float sont marqué comme float : 0.6f
// sauf les * 0.5 (le compilateur utilise des decalages de bits???)

#define proto2 // commenter pour la version final
// #define fabien // CV1 et CV2 switch pour faire octave + et octave -

#include <stdio.h>
#include <string.h>
#include "daisysp.h"
#include "b_declaration.h"
#include "b_hardware.cpp"
#include "c_fonctions.h"
#include "d_voice.h"
#include "e_input.h"
#include "f_leds_menu.h"
#include "m_VCO1.h"
#include "m_VCO2.h"
#include "m_VCO3.h"
#include "m_VCF.h"
#include "m_ADSR.h"
#include "m_LFO1.h"
#include "m_LFO2.h"
#include "m_LFO3.h"
#include "m_LFO7.h"
#include "m_effect.h"
#include "x_test.h"

//14th slot to save curent preset on shutdown
#define save_pos  0x90000000 + (14*4096)

static void AudioCallback(AudioHandle::InterleavingInputBuffer  in, AudioHandle::InterleavingOutputBuffer out, size_t size) {
    hw.test_out(true); // write test_out pin;  10µs la premiere partie
    g_time++;
    g_led_blink += 1<<24; // on laisse les overflow passer de negatif a positif

    float VCO1_fq;
    float VCO2_fq;
    float VCO3_fq;
    float VCF1_fq;
    float sig, sommeADSR;
    float CV_pitch;

    // variable ne necessitant pas d'etre recalculés a chaque sample
    g_pot_audio[k_VCO1_fq] += coef_audio_to_block * g_pot_increment[k_VCO1_fq];
    VCO1_fq = VCO_CV_range(curent_config.c_VCO1_RANGE, g_pot_audio[k_VCO1_fq]);
    g_pot_audio[k_VCO2_fq] += coef_audio_to_block * g_pot_increment[k_VCO2_fq];
    VCO2_fq = VCO_CV_range(curent_config.c_VCO2_RANGE, g_pot_audio[k_VCO2_fq]);
    g_pot_audio[k_VCO3_fq] += coef_audio_to_block * g_pot_increment[k_VCO3_fq];
    VCO3_fq = VCO_CV_range(curent_config.c_VCO3_RANGE, g_pot_audio[k_VCO3_fq]);

	g_pot_audio[k_VCF1_fq] += coef_audio_to_block * g_pot_increment[k_VCF1_fq];
    VCF1_fq = 127.f * g_pot_audio[k_VCF1_fq];
    g_pot_audio[k_VCF1_q] += coef_audio_to_block * g_pot_increment[k_VCF1_q];
    g_pot_audio[k_VCF1_mod1] += coef_audio_to_block * g_pot_increment[k_VCF1_mod1];
    g_pot_audio[k_VCF1_mod2] += coef_audio_to_block * g_pot_increment[k_VCF1_mod2];

    (g_pot_audio[k_ADSR_a] += coef_audio_to_block * g_pot_increment[k_ADSR_a]);
    (g_pot_audio[k_ADSR_d] += coef_audio_to_block * g_pot_increment[k_ADSR_d]);
    (g_pot_audio[k_ADSR_r]  += coef_audio_to_block * g_pot_increment[k_ADSR_r]);

	CV_pitch = g_Modulation[CV1_OUT] * 60.f*g_CV1_gain; // de -5V a +5V = +/- 5 octaves

    if (allvoice[0].v_GATE_source == 2) allvoice[0].v_pitch = CV_pitch;
    if (allvoice[1].v_GATE_source == 2) allvoice[1].v_pitch = CV_pitch;
    if (allvoice[2].v_GATE_source == 2) allvoice[2].v_pitch = CV_pitch;
    if (allvoice[3].v_GATE_source == 2) allvoice[3].v_pitch = CV_pitch;

    LFO1_FQ  = g_knob[k_LFO1_fq]; // on ne filtre pas  les pots de frequence pour gagner en CPU
    LFO1_INC = CV2increment_lfo(curent_config.c_LFO1_RANGE, LFO1_FQ);
    LFO1_MIX = g_pot_audio[k_LFO1_mod] += coef_audio_to_block * g_pot_increment[k_LFO1_mod];

    LFO2_FQ  = g_knob[k_LFO2_fq];
    LFO2_INC = CV2increment_lfo(curent_config.c_LFO2_RANGE, LFO2_FQ);
    LFO2_MIX = g_pot_audio[k_LFO2_mod] += coef_audio_to_block * g_pot_increment[k_LFO2_mod];

    LFO3_FQ  = g_knob[k_LFO3_fq];
    LFO3_INC = CV2increment_lfo(curent_config.c_LFO3_RANGE, LFO3_FQ);
    LFO3_MIX = g_pot_audio[k_LFO3_mod] += coef_audio_to_block * g_pot_increment[k_LFO3_mod];

    LFO4_INC = CV2increment_lfo(curent_config.c_LFO4_RANGE, g_knob[k_LFO4_fq] );
    LFO5_INC = CV2increment_lfo(curent_config.c_LFO5_RANGE, g_knob[k_LFO5_fq] );
    LFO6_INC = CV2increment_lfo(curent_config.c_LFO6_RANGE, g_knob[k_LFO6_fq] );

    LFO7_INC = CV2increment_lfo(curent_config.c_LFO7_RANGE, g_knob[k_LFO7_fq]);
    LFO7_WF =  g_pot_audio[k_LFO7_wf] += coef_audio_to_block * g_pot_increment[k_LFO7_wf];
    LFO7_SYM =  g_pot_audio[k_LFO7_sym] += coef_audio_to_block * g_pot_increment[k_LFO7_sym];

    // buffered Audio Loop
    for(uint32_t i = 0; i < size; ) // size = 2* 24 : block audio de 0.5ms
    // cad on actualise les potentiomettres a 2KHz, meme si le dac est a 200Hz
    {
		sig=0.f;
		sommeADSR = 0.f;

        if ((i & 0b111) == 0) // Sample Rate LFO = SR/4 (facteur 8 a cause du cannal droit et gauche)
            LFO(); // SR = 12KHz
		//LFO(); // SR = 48KHz

        // filtre les PWM en audio, car on les utilise pour toutes les voies de polyphonie
		g_pot_audio[k_VCO1_wfm] += g_pot_increment[k_VCO1_wfm];
		g_pot_audio[k_VCO2_wfm] += g_pot_increment[k_VCO2_wfm];
		g_pot_audio[k_VCO3_wfm] += g_pot_increment[k_VCO3_wfm];

        // filtre les niveaux du mix en audio, car on les utilise pour toutes les voies de polyphonie
		float mix1, mix2, mix3;
		g_pot_audio[k_MIX1] += g_pot_increment[k_MIX1];
		mix1 = g_pot_audio[k_MIX1]  * g_pot_audio[k_MIX1];
		g_pot_audio[k_MIX2] += g_pot_increment[k_MIX2];
		mix2 = g_pot_audio[k_MIX2]  * g_pot_audio[k_MIX2];
		g_pot_audio[k_MIX3] += g_pot_increment[k_MIX3];
		mix3 = g_pot_audio[k_MIX3]  * g_pot_audio[k_MIX3];
		g_pot_audio[k_GAIN] += g_pot_increment[k_GAIN];

		// filtrage audio des modulation des VCO
		g_pot_audio[k_VCO1_mod1] += g_pot_increment[k_VCO1_mod1];
		g_pot_audio[k_VCO1_mod2] += g_pot_increment[k_VCO1_mod2];
		g_pot_audio[k_VCO1_mod3] += g_pot_increment[k_VCO1_mod3];
		g_pot_audio[k_VCO2_mod1] += g_pot_increment[k_VCO2_mod1];
		g_pot_audio[k_VCO2_mod2] += g_pot_increment[k_VCO2_mod2];
		g_pot_audio[k_VCO2_mod3] += g_pot_increment[k_VCO2_mod3];
		g_pot_audio[k_VCO3_mod1] += g_pot_increment[k_VCO3_mod1];
		g_pot_audio[k_VCO3_mod2] += g_pot_increment[k_VCO3_mod2];
		g_pot_audio[k_VCO3_mod3] += g_pot_increment[k_VCO3_mod3];

		// filtrage audio du sustain
		g_pot_audio[k_ADSR_s] += g_pot_increment[k_ADSR_s];

        for (uint32_t j=nb_voice; j--;)
        { // pour toutes les voies de polyphonie
            float sound;
            sommeADSR += ADSR(j);

            g_Modulation[LFO1_OUT] = g_LFO1_AR[j];
            g_Modulation[LFO1_OUT+modulation_source_last] = -g_LFO1_AR[j];
            g_Modulation[LFO2_OUT] = g_LFO2_AR[j];
            g_Modulation[LFO2_OUT+modulation_source_last] = -g_LFO2_AR[j];
            g_Modulation[LFO3_OUT] = g_LFO3_AR[j];
            g_Modulation[LFO3_OUT+modulation_source_last] = -g_LFO3_AR[j];

            sound  =  mix1 * VCO1(j, VCO1_fq);
            sound += mix2  * VCO2(j, VCO2_fq);
            sound += mix3  * VCO3(j, VCO3_fq);
            sound = VCF1(j, VCF1_fq, sound);

            sig += VCA(j, sound);
        }

        // pour la suite, on utilise la somme des ADSR comme g_Modulation
        sommeADSR *= 1.f/nb_voice;
        g_Modulation[ADSR_OUT] = sommeADSR;
        g_Modulation[ADSR_OUT+modulation_source_last] = - sommeADSR;
        g_Modulation[LFO1_OUT] = g_LFO1_AR[nb_voice];
        g_Modulation[LFO1_OUT+modulation_source_last] = -g_LFO1_AR[nb_voice];
        g_Modulation[LFO2_OUT] = g_LFO2_AR[nb_voice];
        g_Modulation[LFO2_OUT+modulation_source_last] = -g_LFO2_AR[nb_voice];
        g_Modulation[LFO3_OUT] = g_LFO3_AR[nb_voice];
        g_Modulation[LFO3_OUT+modulation_source_last] = -g_LFO3_AR[nb_voice];

        sig = effect1(sig);
        sig = effect2(sig);

        VCF2(sig);
        sig *= 0.5f * g_pot_audio[k_GAIN] * g_pot_audio[k_GAIN];
        if(fabs(sig)>=1.f) g_clip = 1.f; else g_clip =  _fmax(fabs(sig)*0.3f, g_clip - 0.0001f);

        out[i++] = sig; // droite
        out[i++] = -sig; // gauche
    }
    hw.test_out(false);
}

int main(void)
{
    volatile uint32_t i; // get potentiometter loop

////////////////////////////////////////////////////////////////////////
// attend que l'allimentation se stabilise
////////////////////////////////////////////////////////////////////////
	if (!dsy_gpio_read(&low_power_pin)) { // si l'allim est en dessous des 7V
		hw.test_out(true); hw.seed.SetLed(true);
		hw.test_out(false); hw.seed.SetLed(false);
	}

////////////////////////////////////////////////////////////////////////
// initialisation hardware et variable
////////////////////////////////////////////////////////////////////////
    hw.Init(); // Init the seed + hardware
    hw.StartAdc();// start ADC callbacks
    init_variables(); // init variables
  	//g_delay_effect1.Init(); // init delay line
    g_delay_effect2.Init();
    //g_delay_effect3.Init();
    hw.midi.StartRx(); // init MIDI


////////////////////////////////////////////////////////////////////////
// initialisation des memoires
////////////////////////////////////////////////////////////////////////
	if (true) { // normal operation
		load_config(13); // only for the calibration
		g_CV1_offset = curent_config.c_CV1_offset;
		g_CV2_offset = curent_config.c_CV2_offset;
		g_CV1_gain = curent_config.c_CV1_gain;
		if(!load_config(14)) empty_config(); // load saved config, if failled, load an empty config
	}
	else {	// for test purpose
		empty_config();
	}

////////////////////////////////////////////////////////////////////////
// hardware test
////////////////////////////////////////////////////////////////////////
	//if(true) // test only
    if( dsy_gpio_read(&HW_test) == 0)
    { // test mode
        hw.StartAudio(AudioCallbackTest); // just for testing the audio dac
        while(1) // on est en mode de test
            test();
    } // end of test mode

////////////////////////////////////////////////////////////////////////
// erase memory that will be used to save the curent preset when shutdown
// doing this now to gain time when shutdown
	hw.seed.qspi.Erase(save_pos, save_pos + sizeof(CONFIGURATION));

////////////////////////////////////////////////////////////////////////
// start audio
    hw.StartAudio(AudioCallback); // normal operation

////////////////////////////////////////////////////////////////////////
// main loop
////////////////////////////////////////////////////////////////////////
    while(1) { // loop for low piority task

		// hw.test_out(i>20); // test de performance

        if(++i > nb_CV) {
			i=0;
		    get_analog_in(); // analog gate and CV in
		}
        get_pot(i); // get potentiometters value and filter them
        get_keyboard(); // test keyboard and display leds accordingly;
        get_midi(); // test reception de midi data

       // test shutdown
        if (!dsy_gpio_read(&low_power_pin)) { // si l'allim passe en dessous des 7V,
			// on arrete le son et tout les process qui prennent du temps ou du courant,
			// ensuite, on sauve la config actuel pendant les qqs ms restante de courant ds les condensateurs

			hw.test_out(true); // for oscillo debug only
			hw.StopAudio(); //stop audio
			set_all_led(0,0,0,0,0); //extinction des leds analogique pour economiser du courant
			write_binary_led(0); // extinction des leds du clavier
			hw.test_out(false);

			hw.seed.qspi.Write(save_pos, sizeof(CONFIGURATION), (uint8_t*)&curent_config); // 4.5ms

			while(true) { // boucle pour montrer la fin  de la procedure de sauvegarde
				hw.test_out(true); hw.seed.SetLed(true);
				hw.test_out(false); hw.seed.SetLed(false);
			}
		}

    } // everything else is done on the audio interuption
}
