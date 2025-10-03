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


float g_LFO1_noise[2]; // pour le noise avec interpolation cubic
float g_LFO1_AR[5]; // nb_voice+1
float g_phase_LFO1_div;
uint32_t g_LFO1_last_step, g_LFO1_reset;

inline void LFO1(float const fq, float const mix_factor, float const increment) {
    float modulation = 0.f;

    if (curent_config.c_LFO1_WF == WF_AR) {
		float const A_time = ADSR_time2filter(fq);
		float const D_time = ADSR_time2filter(mix_factor);
        for (uint32_t j=nb_voice; j--;) {
            float const tmp = allvoice[j].v_GATE >= 1.;
            float const time = tmp?A_time : D_time;
            _fonepole(g_LFO1_AR[j], tmp, time);
            modulation += g_LFO1_AR[j];
        }
        modulation /= nb_voice;
        g_LFO1_AR[nb_voice] = modulation;
        g_Modulation[LFO1_OUT] = modulation;
        g_Modulation_Phase[LFO1_OUT] = modulation;
        g_Modulation_Reset[LFO1_OUT] = g_new_gate; // signal provenant de la fct "add_voice"
        g_new_gate = 0;

    } else {
        float WF1, phase = 0.f, overflow_phase;
        uint32_t source_addresse = curent_config.c_Modulation_Source[LFO1_MOD];
        switch (curent_config.c_Modulation_Type[LFO1_MOD]) {
        case LFO_Mix : // ok
            {
				if (source_addresse != LFO1_OUT) { //pas de modulation
                    WF1 = g_Modulation[source_addresse];
                } else {
                    WF1 = 1.f;
                }
                phase = g_Modulation_Phase[LFO1_OUT] + increment;                               // calcul de la phase
                overflow_phase = (int)phase;
                phase -= overflow_phase;
                g_Modulation_Reset[LFO1_OUT] = overflow_phase;
                g_Modulation_Phase[LFO1_OUT] = phase;

                float const WF = LFO_compute_WF(phase, curent_config.c_LFO1_WF, g_LFO1_noise, g_Modulation_Reset[LFO1_OUT]);
                modulation = mix(WF, WF1, mix_factor);
            }
            break;
        case LFO_AM : // ok
            {
                WF1 = g_Modulation[source_addresse];
                phase = g_Modulation_Phase[LFO1_OUT] + increment;                               // calcul de la phase
                overflow_phase = (int)phase;
                phase -= overflow_phase;
                g_Modulation_Reset[LFO1_OUT] = overflow_phase;
                g_Modulation_Phase[LFO1_OUT] = phase;

                float const WF = LFO_compute_WF(phase, curent_config.c_LFO1_WF, g_LFO1_noise, g_Modulation_Reset[LFO1_OUT]);
                modulation = WF * (1.f - (WF1+1.f) * mix_factor);
            }
            break;
        case LFO_FM : // ok
            {
                WF1 = g_Modulation[source_addresse];
                float const l_increment = increment * CV2freq( -36.3763 + WF1 * mix_factor * 50.f);

                phase = g_Modulation_Phase[LFO1_OUT] + l_increment;                               // calcul de la phase
                overflow_phase = _floor(phase);
                phase -= overflow_phase;
                g_Modulation_Phase[LFO1_OUT] = phase;
                g_Modulation_Reset[LFO1_OUT] = overflow_phase != 0.f;
                modulation = LFO_compute_WF(phase, curent_config.c_LFO1_WF, g_LFO1_noise, g_Modulation_Reset[LFO1_OUT]);
            }
            break;
        case LFO_PM : // ok
            {
                WF1 = g_Modulation[source_addresse];
                phase = g_Modulation_Phase[LFO1_OUT] + increment;                               // calcul de la phase
                overflow_phase = (int)phase;
                phase -= overflow_phase;
                g_Modulation_Reset[LFO1_OUT] = overflow_phase;
                g_Modulation_Phase[LFO1_OUT] = phase;

                phase = wrap2(phase + 2.f * WF1 * mix_factor);
                float const WF = LFO_compute_WF(phase, curent_config.c_LFO1_WF, g_LFO1_noise, g_Modulation_Reset[LFO1_OUT]);
                modulation = WF;
            }
            break;
        case LFO_CLIP : // OK
           {
                if (source_addresse != LFO1_OUT) {
                    WF1 = g_Modulation[source_addresse];
                } else {//automodulation
                    WF1 = 1.f;
                }

                phase = g_Modulation_Phase[LFO1_OUT] + increment;                               // calcul de la phase
                overflow_phase = (int)phase;
                phase -= overflow_phase;
                g_Modulation_Reset[LFO1_OUT] = overflow_phase;
                g_Modulation_Phase[LFO1_OUT] = phase;

                float const WF = LFO_compute_WF(phase, curent_config.c_LFO1_WF, g_LFO1_noise, g_Modulation_Reset[LFO1_OUT]);

                modulation = _fmin(WF, WF1+2.f*(1.f-mix_factor));
                modulation = _fmax(modulation, WF1-2.f*(1.f-mix_factor));
            }
            break;
        case LFO_Fold : // fold : OK
            {
                WF1 = g_Modulation[source_addresse];
                phase = g_Modulation_Phase[LFO1_OUT] + increment;                               // calcul de la phase
                overflow_phase = (int)phase;
                phase -= overflow_phase;
                g_Modulation_Reset[LFO1_OUT] = overflow_phase;
                g_Modulation_Phase[LFO1_OUT] = phase;

                float WF = LFO_compute_WF(phase, curent_config.c_LFO1_WF, g_LFO1_noise, g_Modulation_Reset[LFO1_OUT]);
                WF *= 1.f + (WF1+1.f)* mix_factor; // gain from 1 to 3
                WF = (WF >  1.f)?  2.f - WF : WF; // fold
                WF = (WF < -1.f)? -2.f - WF : WF;
                modulation = WF;
            }
            break;
        case LFO_Xor : // OK
            {
                WF1 = g_Modulation[source_addresse];
                phase = g_Modulation_Phase[LFO1_OUT] + increment;                               // calcul de la phase
                overflow_phase = (int)phase;
                phase -= overflow_phase;
                g_Modulation_Reset[LFO1_OUT] = overflow_phase;
                g_Modulation_Phase[LFO1_OUT] = phase;

                float const WF = LFO_compute_WF(phase, curent_config.c_LFO1_WF, g_LFO1_noise, g_Modulation_Reset[LFO1_OUT]);

                uint32_t i_WF1 = (WF1+1.f) * 2147483648.f;
                uint32_t i_WF2 = (WF+1.f)  * 2147483648.f;
                float const tmp = mix_factor * (mix_factor+1.f) * 7.9f;
                uint32_t ui_tmp = tmp;                                                           //0 a 15 (0b1111)
                ui_tmp <<= 28;
                i_WF1 &= ui_tmp;
                i_WF2 ^= i_WF1;                                                         // on inverse les bits de poids fort en premier
                modulation =  (i_WF2 * int2float) - 1.f;
            }
            break;
        case LFO_FILTER :
            {
                float filter_fq;
                if (source_addresse != NONE_OUT) {
                    filter_fq = mix(127.f, 127.f * g_Modulation[source_addresse], mix_factor);
                } else {//pas de modulation : le pot est le % de modulation
                    filter_fq = (0.5f-mix_factor) * 254.f;
                }
                phase = g_Modulation_Phase[LFO1_OUT] + increment;                               // calcul de la phase
                overflow_phase = (int)phase;
                phase -= overflow_phase;
                g_Modulation_Reset[LFO1_OUT] = overflow_phase;
                g_Modulation_Phase[LFO1_OUT] = phase;

                filter_fq = CV2freq(filter_fq)*(1.f/13000.f);
                float const WF = LFO_compute_WF(phase, curent_config.c_LFO1_WF, g_LFO1_noise, g_Modulation_Reset[LFO1_OUT]);
                modulation = g_Modulation[LFO1_OUT];
                _fonepole(modulation, WF, filter_fq);
            }
            break;
        case LFO_RESET : // OK
            {
                uint32_t WF1_reset;
                float l_mix_factor;
                if (source_addresse != NONE_OUT) {
                    WF1_reset = g_Modulation_Reset[source_addresse];
                    l_mix_factor = mix_factor;
                } else { //reset manuel
                    if (g_LFO1_reset == 0) {
                        if (mix_factor > 0.51f) {
                            WF1_reset = 1;
                            g_LFO1_reset = 1;
                        } else {
                            WF1_reset = 0;
                        }
                    } else {
                        WF1_reset = 0;
                        if (mix_factor < 0.49f) {
                            g_LFO1_reset = 0;
                        }
                    }
                    l_mix_factor = 1.f;
                }
                phase = g_Modulation_Phase[LFO1_OUT] + increment;                               // calcul de la phase
                overflow_phase = (int)phase;
                phase -= overflow_phase;
                g_Modulation_Reset[LFO1_OUT] = overflow_phase;
                if(WF1_reset) { phase = wrap(mix(wrap(phase+0.5f), 0.5f, l_mix_factor) + 0.5f); }
                g_Modulation_Phase[LFO1_OUT] = phase;
                modulation = LFO_compute_WF(phase, curent_config.c_LFO1_WF, g_LFO1_noise, g_Modulation_Reset[LFO1_OUT]);
            }
            break;
        case LFO_SYNC : // on se syncronise sur la modulation extern,
            if ((source_addresse != NONE_OUT) && (source_addresse != LFO1_OUT) && (source_addresse != LFO1_OUT + modulation_source_last)) {
				// clock divid/ mult : FQ : frequency divider or multiplier; MOD: add an offset to the input phase.
                float const tmp = fq * 8.99f;
                switch ((uint32_t) tmp) {
                    case 0 : // div 8
                        g_Modulation_Reset[LFO1_OUT] = 0;
                        if (g_Modulation_Reset[source_addresse]) {
                            g_phase_LFO1_div++;
                            if ( g_phase_LFO1_div >= 8) {
                                g_phase_LFO1_div = 0;
                                g_Modulation_Reset[LFO1_OUT] = 1;
                            }
                        }
                        phase = ( g_phase_LFO1_div + g_Modulation_Phase[source_addresse]) * 0.125f;
                    break;
                    case 1 : // div 4
                        g_Modulation_Reset[LFO1_OUT] = 0;
                        if (g_Modulation_Reset[source_addresse]) {
                            g_phase_LFO1_div++;
                            if ( g_phase_LFO1_div >= 4) {
                                g_phase_LFO1_div = 0;
                                g_Modulation_Reset[LFO1_OUT] = 1;
                            }
                        }
                        phase = ( g_phase_LFO1_div + g_Modulation_Phase[source_addresse])*0.25f;
                    break;
                    case 2 : // div 3
                        g_Modulation_Reset[LFO1_OUT] = 0;
                        if (g_Modulation_Reset[source_addresse]) {
                            g_phase_LFO1_div++;
                            if ( g_phase_LFO1_div >= 3) {
                                g_phase_LFO1_div = 0;
                                g_Modulation_Reset[LFO1_OUT] = 1;
                            }
                        }
                        phase = ( g_phase_LFO1_div + g_Modulation_Phase[source_addresse])/3.0f;
                    break;
                    case 3 : // div 2
                        g_Modulation_Reset[LFO1_OUT] = 0;
                        if (g_Modulation_Reset[source_addresse]) {
                            g_phase_LFO1_div++;
                            if ( g_phase_LFO1_div >= 2) {
                                g_phase_LFO1_div = 0;
                                g_Modulation_Reset[LFO1_OUT] = 1;
                            }
                        }
                        phase = ( g_phase_LFO1_div + g_Modulation_Phase[source_addresse])*0.5f;
                    break;
                    case 4 : // Id
                        g_Modulation_Reset[LFO1_OUT] = g_Modulation_Reset[source_addresse];
                        phase = g_Modulation_Phase[source_addresse];
                    break;
                    case 5 : // mult 2
                        phase = g_Modulation_Phase[source_addresse] * 2.f;
                        g_Modulation_Reset[LFO1_OUT] = (phase < g_Modulation_Phase[source_addresse]);
                    break;
                    case 6 : // mult 3
                        phase = g_Modulation_Phase[source_addresse] * 3.f;
                        g_Modulation_Reset[LFO1_OUT] = (phase < g_Modulation_Phase[source_addresse]);
                    break;
                    case 7 : // mult 4
                        phase = g_Modulation_Phase[source_addresse] * 4.f;
                        g_Modulation_Reset[LFO1_OUT] = (phase < g_Modulation_Phase[source_addresse]);
                    break;
                    case 8 : // mult 8
                        phase = g_Modulation_Phase[source_addresse] * 8.f;
                        g_Modulation_Reset[LFO1_OUT] = (phase < g_Modulation_Phase[source_addresse]);
                    break;
                }
                phase = wrap(phase + mix_factor);
                g_Modulation_Phase[LFO1_OUT] = phase;
                modulation = LFO_compute_WF(phase, curent_config.c_LFO1_WF, g_LFO1_noise, g_Modulation_Reset[LFO1_OUT]);
                break;
            } else {//automodulation donc autre algo : on change juste le PWM
                WF1 = mix_factor + mix_factor - 1.f;

                phase = g_Modulation_Phase[LFO1_OUT] + increment;                               // calcul de la phase
                overflow_phase = (int)phase;
                phase -= overflow_phase;
                g_Modulation_Reset[LFO1_OUT] = overflow_phase;
                g_Modulation_Phase[LFO1_OUT] = phase;

                if (WF1 > 0.f) {
                    phase *= mix(1.f, phase, WF1);
                    phase *= mix(1.f, phase, WF1);
                    phase *= mix(1.f, phase, WF1);
                    phase *= mix(1.f, phase, WF1);
                }
                else {
                    WF1 = -WF1;
                    phase = 1.f - phase;
                    phase *= mix(1.f, phase, WF1);
                    phase *= mix(1.f, phase, WF1);
                    phase *= mix(1.f, phase, WF1);
                    phase *= mix(1.f, phase, WF1);
                    phase = 1.f - phase;
                }
                modulation = LFO_compute_WF(phase, curent_config.c_LFO1_WF, g_LFO1_noise, g_Modulation_Reset[LFO1_OUT]);
                break;
            }
        case LFO_GATE : // ok
            {
                float l_mix_factor;
                if ((source_addresse != NONE_OUT) && (source_addresse != LFO1_OUT) && (source_addresse != LFO1_OUT + modulation_source_last))  { // pas automodulation
                    WF1 = g_Modulation[source_addresse];
                    l_mix_factor = mix_factor;
                } else {
                    WF1 = 0.f;
                    if(source_addresse != NONE_OUT)
						l_mix_factor = 1.f-mix_factor;
					else l_mix_factor = mix_factor;
                }
                phase = g_Modulation_Phase[LFO1_OUT] + increment;                               // calcul de la phase
                overflow_phase = (int)phase;
                phase -= overflow_phase;
                g_Modulation_Reset[LFO1_OUT] = overflow_phase;
                g_Modulation_Phase[LFO1_OUT] = phase;
                if (WF1 > ( (2.f*l_mix_factor)-1.f) ) {
                    modulation = LFO_compute_WF(phase, curent_config.c_LFO1_WF, g_LFO1_noise, g_Modulation_Reset[LFO1_OUT]);
                } else modulation = g_Modulation[LFO1_OUT];
            }
            break;
        case LFO_TH : // ok
            {
                float l_mix_factor;
                if ((source_addresse != NONE_OUT) && (source_addresse != LFO1_OUT) && (source_addresse != LFO1_OUT + modulation_source_last))  { // pas automodulation
                    WF1 = g_Modulation[source_addresse];
                    l_mix_factor = mix_factor;;
                } else {
                    WF1 = 0.f;
					l_mix_factor = 1.f-mix_factor;
                }
                phase = g_Modulation_Phase[LFO1_OUT] + increment;                               // calcul de la phase
                overflow_phase = (int)phase;
                phase -= overflow_phase;
                g_Modulation_Reset[LFO1_OUT] = overflow_phase;
                g_Modulation_Phase[LFO1_OUT] = phase;

                modulation = g_Modulation[LFO1_OUT];
                if (g_LFO1_last_step == 0) {
                    if ( WF1 > 0.01f + ((2.f*l_mix_factor)-1.f) )
                    {
                        modulation = LFO_compute_WF(phase, curent_config.c_LFO1_WF, g_LFO1_noise, g_Modulation_Reset[LFO1_OUT]);
                        g_LFO1_last_step = 1;
                    }
                }
                else {
                    if ( WF1 < -0.01f + ((2.f*l_mix_factor)-1.f) )
                    {
                        g_LFO1_last_step = 0;
                    }
                }
            }
            break;
        case LFO_RndLoop :
        // utilise la freqauence du LFO de modulation
        // sans automodulation : fq = nb de step, mix_factor = variation speed
        // en automod : 'mod' permet de passer les step, il y a 2 step
            {
                uint32_t WF1_reset, nb_step;
                float l_mix_factor;
                if ((source_addresse != NONE_OUT) && (source_addresse != LFO1_OUT) && (source_addresse != LFO1_OUT + modulation_source_last))  { // pas automodulation
                    WF1_reset = g_Modulation_Reset[source_addresse];
                    nb_step = table_step[(int)(fq*5.9999f)];
                    l_mix_factor = mix_factor;
                } else { //automodulation ou pas de modulation
					nb_step = 1;
                    if (g_LFO1_reset == 0) {
                        if (mix_factor > 0.51f) {
                            WF1_reset = 1;
                            g_LFO1_reset = 1;
                        } else {
                            WF1_reset = 0;
                        }
                    } else {
                        WF1_reset = 0;
                        if (mix_factor < 0.49f) {
                            g_LFO1_reset = 0;
                        }
                    }
                    l_mix_factor = 1.f;
                }
                {
                    if (WF1_reset) {
                        if(++g_LFO1_last_step >= (uint)nb_step) g_LFO1_last_step=0;
                        uint32_t num_drunk = drunk_LFO1 + 3*g_LFO1_last_step;
                        drunk_lfo(num_drunk, l_mix_factor*l_mix_factor);
                        g_LFO1_noise[1] = g_LFO1_noise[0];
                        g_LFO1_noise[0] = g_drunk_lfo[num_drunk]*2.f - 1.f;
                        g_Modulation_Reset[LFO1_OUT] = 1;

                    } else {
                        g_Modulation_Reset[LFO1_OUT] = 0;
                    }
                    modulation = Interpolation_Curve(g_Modulation_Phase[source_addresse], curent_config.c_LFO1_WF, g_LFO1_noise);
                    g_Modulation_Phase[LFO1_OUT] = g_Modulation_Phase[source_addresse];
                }
            }
            break;
        }
        g_LFO1_AR[0]= modulation; // sauvegarde pour ne pas avoir a tester la WF du LFO ds la boucle audio
        g_LFO1_AR[1]= modulation;
        g_LFO1_AR[2]= modulation;
        g_LFO1_AR[3]= modulation;
        g_LFO1_AR[4]= modulation; // la somme de tout les AR = LFO en mode normal
        g_Modulation[LFO1_OUT] = modulation;
        g_Modulation[LFO1_OUT+modulation_source_last] = -modulation;
        g_Modulation[LFO1_OUT_FILTER] += 0.003 * (modulation - g_Modulation[LFO1_OUT_FILTER]); // pour les leds
    }
}

float g_LFO4_noise[4]; // pour le noise avec interpolation cubic

inline void LFO4(float increment) {
    float phase, overflow_phase, tmp;

    phase = g_Modulation_Phase[LFO4_OUT] + increment;                               // calcul de la phase
    overflow_phase = (int)phase;
    phase -= overflow_phase;
    g_Modulation_Reset[LFO4_OUT] = overflow_phase;
    g_Modulation_Phase[LFO4_OUT] = phase;

    tmp = LFO_compute_WF(phase, curent_config.c_LFO4_WF, g_LFO4_noise, g_Modulation_Reset[LFO4_OUT]);
    g_Modulation[LFO4_OUT] = tmp;
    g_Modulation[LFO4_OUT+modulation_source_last] = -tmp;
    g_Modulation[LFO4_OUT_FILTER] += 0.003 * (tmp - g_Modulation[LFO4_OUT_FILTER]);
}
