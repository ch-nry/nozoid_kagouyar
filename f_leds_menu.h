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

#define LED_VCO1            7
#define LED_VCO2            6
#define LED_VCO3            5
#define LED_notused         4
#define LED_ADSR            3
#define LED_LFO1            12
#define LED_LFO2            11
#define LED_LFO3            10
#define LED_LFO4            15
#define LED_LFO5            14
#define LED_LFO6            13
#define LED_LFO7            9
#define LED_GAIN            8
#define LED_MIDI            0
#define LED_CV1             1
#define LED_CV2             2


#define BIT_LED_MENU_VCO        6
#define BIT_LED_MENU_VCO_MOD    5
#define BIT_LED_MENU_VCF        4
#define BIT_LED_MENU_ADSR       3
#define BIT_LED_MENU_LFO        2
#define BIT_LED_MENU_LFO_MOD    1
#define BIT_LED_MENU_EFFECTS    0
#define BIT_LED_MENU_KEY0       10
#define BIT_LED_MENU_KEY1       11
#define BIT_LED_MENU_KEY2       12
#define BIT_LED_MENU_KEY3       13
#define BIT_LED_MENU_KEY4       14
#define BIT_LED_MENU_KEY5       16
#define BIT_LED_MENU_KEY6       17
#define BIT_LED_MENU_KEY7       18
#define BIT_LED_MENU_KEY8       19
#define BIT_LED_MENU_KEY9       20
#define BIT_LED_MENU_KEY10      21
#define BIT_LED_MENU_KEY11      22
#define BIT_LED_MENU_KEY12      23
#define BIT_LAST                20

#define ALL_KB_LEDS (((int32_t)1 << BIT_LED_MENU_KEY0) + ((int32_t)1 << BIT_LED_MENU_KEY1) + \
            ((int32_t)1 << BIT_LED_MENU_KEY2) + ((int32_t)1 << BIT_LED_MENU_KEY3) + ((int32_t)1 << BIT_LED_MENU_KEY4) + (1 << BIT_LED_MENU_KEY5) + \
            ((int32_t)1 << BIT_LED_MENU_KEY6) + ((int32_t)1 << BIT_LED_MENU_KEY7) + ((int32_t)1 << BIT_LED_MENU_KEY8) + (1 << BIT_LED_MENU_KEY9) + \
            ((int32_t)1 << BIT_LED_MENU_KEY10) + ((int32_t)1 << BIT_LED_MENU_KEY11) + ((int32_t)1 << BIT_LED_MENU_KEY12))

const uint32_t table_led_key[] = { BIT_LED_MENU_KEY0, BIT_LED_MENU_KEY1, BIT_LED_MENU_KEY2, BIT_LED_MENU_KEY3,   \
    BIT_LED_MENU_KEY4, BIT_LED_MENU_KEY5, BIT_LED_MENU_KEY6, BIT_LED_MENU_KEY7, BIT_LED_MENU_KEY8,          \
    BIT_LED_MENU_KEY9, BIT_LED_MENU_KEY10, BIT_LED_MENU_KEY11, BIT_LED_MENU_KEY12,  \
    BIT_LED_MENU_VCO, BIT_LED_MENU_VCO_MOD, BIT_LED_MENU_VCF, BIT_LED_MENU_ADSR, BIT_LED_MENU_LFO, \
    BIT_LED_MENU_LFO_MOD, BIT_LED_MENU_EFFECTS };


const uint32_t table_led_modulation[] = { LED_VCO1, LED_VCO1, LED_VCO1, LED_VCO1, LED_VCO1, LED_VCO1, \
LED_VCO2, LED_VCO2, LED_VCO2, LED_VCO2, LED_VCO2, LED_VCO2, \
LED_VCO3, LED_VCO3, LED_VCO3, LED_VCO3, LED_VCO3, LED_VCO3, \
LED_ADSR, LED_LFO1, LED_LFO2, LED_LFO3, LED_LFO4, LED_LFO5, LED_LFO6, LED_LFO7, \
LED_MIDI, LED_CV1, LED_CV2, LED_notused};
//VCO1_out, VCO1_SIN, VCO1_TRI... etc

float g_clip;

void leds_mode_normal() {
    hw.led_driver_.SetLed_ch(LED_VCO1, (float)0.f);
    hw.led_driver_.SetLed_ch(LED_VCO2, (float)0.f);
    hw.led_driver_.SetLed_ch(LED_VCO3, (float)0.f);
    hw.led_driver_.SetLed_ch(LED_ADSR, g_Modulation[ADSR_OUT]);
    if ( (curent_config.c_LFO1_WF != WF_AR) && (curent_config.c_LFO1_WF != WF_AR2) )  { hw.led_driver_.SetLed_ch(LED_LFO1, (float)(0.55f + g_Modulation[LFO1_OUT_FILTER]*0.45f)); }
    else { hw.led_driver_.SetLed_ch(LED_LFO1, g_Modulation[LFO1_OUT]); }
    if ( (curent_config.c_LFO2_WF != WF_AR) && (curent_config.c_LFO2_WF != WF_AR2) )  { hw.led_driver_.SetLed_ch(LED_LFO2, (float)(0.55f + g_Modulation[LFO2_OUT_FILTER]*0.45f)); }
    else { hw.led_driver_.SetLed_ch(LED_LFO2, g_Modulation[LFO2_OUT]); }
    if ( (curent_config.c_LFO3_WF != WF_AR) && (curent_config.c_LFO3_WF != WF_AR2) )  { hw.led_driver_.SetLed_ch(LED_LFO3, (float)(0.55f + g_Modulation[LFO3_OUT_FILTER]*0.45f)); }
    else { hw.led_driver_.SetLed_ch(LED_LFO3, g_Modulation[LFO3_OUT]); }
    hw.led_driver_.SetLed_ch(LED_LFO4, (float)(0.55f + g_Modulation[LFO4_OUT_FILTER]*0.45f));
    hw.led_driver_.SetLed_ch(LED_LFO5, (float)(0.55f + g_Modulation[LFO5_OUT_FILTER]*0.45f));
    hw.led_driver_.SetLed_ch(LED_LFO6, (float)(0.55f + g_Modulation[LFO6_OUT_FILTER]*0.45f));
    hw.led_driver_.SetLed_ch(LED_LFO7, (float)(0.55f + g_Modulation[LFO7_OUT_FILTER]*0.45f));
    if(g_last_switch_configuration != MENU_MIDI) {
        hw.led_driver_.SetLed_ch(LED_MIDI, (float)(g_Modulation[MIDI_modulation]));
	}
    else {
        hw.led_driver_.SetLed_ch(LED_MIDI, (float)g_MIDI_led_time);
	}
	if (!g_CV2KB) {
		hw.led_driver_.SetLed_ch(LED_CV1,  (float)(0.55f + g_Modulation[CV1_OUT]*0.45f));
		hw.led_driver_.SetLed_ch(LED_CV2,  (float)(0.55f + g_Modulation[CV2_OUT]*0.45f));
	} else {
		hw.led_driver_.SetLed_ch(LED_CV1, 0.f );
		hw.led_driver_.SetLed_ch(LED_CV2, 0.f );
		switch (curent_config.c_KEYBOARD_octave+3) {
		case 0 :
			hw.led_driver_.SetLed_ch(LED_CV2, 1.f);
		break;
		case 1 :
			hw.led_driver_.SetLed_ch(LED_CV2, 0.5f);
		break;
		case 2 :
			hw.led_driver_.SetLed_ch(LED_CV2, 0.1f);
		break;
		case 3 :
			hw.led_driver_.SetLed_ch(LED_CV1, 0.1f);
		break;
		case 4 :
			hw.led_driver_.SetLed_ch(LED_CV1, 0.5f);
		break;
		case 5 :
			hw.led_driver_.SetLed_ch(LED_CV1, 1.f);
		break;
		}
	}
    hw.led_driver_.SetLed_ch(LED_GAIN, g_clip);

    g_MIDI_led_time = fmaxf(0.f, g_MIDI_led_time - 0.02f);

    hw.led_driver_.SwapBuffersAndTransmit();
}


void leds_keyboard() {
    uint32_t led_keyboard = 0;
    int32_t tmp;
    for (int i=nb_voice; i--;) {
        if ( (allvoice[i].v_GATE_source < 2) && (allvoice[i].v_GATE >= 1) ) {
            tmp = allvoice[i].v_pitch; // pitch = 0 sur le "LA 440", touche N°9
            tmp -= 12 * curent_config.c_KEYBOARD_octave;
            if ( (tmp >= 0) && (tmp < 13) ) {
                led_keyboard |= 1<<table_led_key[tmp];
            }
        }
    }
    write_binary_led(led_keyboard);
}

void leds_key_modulation(uint32_t my_modulation) { // switch de choix de la source de g_Modulation + configurations pour les MOD VCO + configuration des LFO mod
    uint32_t led_keyboard = 0;
    for (uint32_t i=0; i<16; i++) hw.led_driver_.SetLed_ch(i, 0.f); //RaZ des leds

	uint32_t tmp = curent_config.c_Modulation_Source[my_modulation];
	uint32_t test = 1;
	if ( (my_modulation == EFFECT1_MOD) && (1<<curent_config.c_EFFECT1_TYPE & ((1<<1)+(1<<5)+(1<<14)) ) ) test = 0;
	if ( (my_modulation == EFFECT2_MOD) && (1<<curent_config.c_EFFECT2_TYPE & ((1<<7)+(1<<10)+(1<<12)) ) ) test = 0;
	if  ( test ) { // on ne l'affiche pas pour les effet qui n'utilisent pas la modulation
		if ( (tmp <modulation_source_last) || (g_led_blink>0)  ) // soit on as une modulation positive, soit on affiche une fois sur deux a cause du blink
			hw.led_driver_.SetLed_ch(table_led_modulation[tmp % modulation_source_last], 1.f );
	}

    switch (my_modulation) { // leds du clavier : ligne et configuration
        case VCO1_MOD1:
        case VCO1_MOD2:
        case VCO1_MOD3:
        case VCO2_MOD1:
        case VCO2_MOD2:
        case VCO2_MOD3:
        case VCO3_MOD1:
        case VCO3_MOD2:
        case VCO3_MOD3:
            led_keyboard |= 1<< BIT_LED_MENU_VCO_MOD; // alume la led de ligne VCO MOD
            led_keyboard |= 1<<table_led_key[curent_config.c_Modulation_Type[my_modulation]]; // allume la led correspondant au type de modulation
            switch ( curent_config.c_Modulation_Source[my_modulation]) {
                case VCO1_OUT:
                case VCO2_OUT:
                case VCO3_OUT:
                    led_keyboard |= 1<< BIT_LED_MENU_KEY7;
                break;
                case VCO1_SIN:
                case VCO2_SIN:
                case VCO3_SIN:
                    led_keyboard |= 1<< BIT_LED_MENU_KEY8;
                break;
                case VCO1_TRI:
                case VCO2_TRI:
                case VCO3_TRI:
                    led_keyboard |= 1<< BIT_LED_MENU_KEY9;
                break;
                case VCO1_SQUARE:
                case VCO2_SQUARE:
                case VCO3_SQUARE:
                    led_keyboard |= 1<< BIT_LED_MENU_KEY10;
                break;
                case VCO1_RAMP:
                case VCO2_RAMP:
                case VCO3_RAMP:
                    led_keyboard |= 1<< BIT_LED_MENU_KEY11;
                break;
                case VCO1_SAW:
                case VCO2_SAW:
                case VCO3_SAW:
                    led_keyboard |= 1<< BIT_LED_MENU_KEY12;
                break;
            //default: ADSR_out, CVEXT, AUDIO_IN1, AUDIO_IN2, MIDI_wheel etc, : on fait rien
            }
        break;
        case VCF1_MOD1:
            led_keyboard |= 1 << BIT_LED_MENU_VCF;
            if(curent_config.c_VCF1_MOD1_TYPE == 0)
                led_keyboard |= 1 << BIT_LED_MENU_KEY9;
            else
                led_keyboard |= 1 << BIT_LED_MENU_KEY10;
            break;
        case VCF1_MOD2:
            led_keyboard |= 1 << BIT_LED_MENU_VCF;
            if(curent_config.c_VCF1_MOD2_TYPE == 0)
                led_keyboard |= 1 << BIT_LED_MENU_KEY9;
            else
                led_keyboard |= 1 << BIT_LED_MENU_KEY10;
            break;
        case LFO1_MOD:
            led_keyboard |= 1 << BIT_LED_MENU_LFO_MOD;
            led_keyboard |= 1 << table_led_key[curent_config.c_Modulation_Type[LFO1_MOD]];
            break;
        case LFO2_MOD:
            led_keyboard |= 1 << BIT_LED_MENU_LFO_MOD;
            led_keyboard |= 1 << table_led_key[curent_config.c_Modulation_Type[LFO2_MOD]];
            break;
        case LFO3_MOD:
            led_keyboard |= 1 << BIT_LED_MENU_LFO_MOD;
            led_keyboard |= 1 << table_led_key[curent_config.c_Modulation_Type[LFO3_MOD]];
            break;
        // case NONE_MOD:
        // on fait rien
        // default: // rien a faire
    }
    write_binary_led(led_keyboard);
    hw.led_driver_.SwapBuffersAndTransmit();
}

void leds_key_configuration(uint32_t my_menu_switch) { // led lorsque on appuie sur switch de configuration du module
    uint32_t led_keyboard = 0;
    switch (my_menu_switch) {
    case MENU_VCO1 :
        led_keyboard |= 1 << BIT_LED_MENU_VCO;
        if (curent_config.c_VCO1_WF<9)  led_keyboard |= 1 << table_led_key[curent_config.c_VCO1_WF];
        else if ((curent_config.c_VCO1_WF<18)  &&  (g_led_blink>0))  led_keyboard |= 1 << table_led_key[curent_config.c_VCO1_WF-9];
        led_keyboard |= 1 << table_led_key[10+curent_config.c_VCO1_RANGE];
        break;
    case MENU_VCO2 :
        led_keyboard |= 1 << BIT_LED_MENU_VCO;
        if (curent_config.c_VCO2_WF<9) led_keyboard |= 1 << table_led_key[curent_config.c_VCO2_WF];
        else if ((curent_config.c_VCO2_WF<18)  &&  (g_led_blink>0))  led_keyboard |= 1 << table_led_key[curent_config.c_VCO2_WF-9];
        if(curent_config.c_VCO2_LINK) led_keyboard |= 1<< BIT_LED_MENU_KEY9;
        led_keyboard |= 1 << table_led_key[10 + curent_config.c_VCO2_RANGE];
        break;
    case MENU_VCO3 :
        led_keyboard |= 1 << BIT_LED_MENU_VCO;
        if (curent_config.c_VCO3_WF<9) led_keyboard |= 1 << table_led_key[curent_config.c_VCO3_WF];
        else if ((curent_config.c_VCO3_WF<18)  &&  (g_led_blink>0))  led_keyboard |= 1 << table_led_key[curent_config.c_VCO3_WF-9];
        if(curent_config.c_VCO3_LINK) led_keyboard |= 1<< BIT_LED_MENU_KEY9;
        led_keyboard |= 1 << table_led_key[10 + curent_config.c_VCO3_RANGE];
        break;
    case MENU_VCF1 :  // VCF option
        led_keyboard |= 1 << BIT_LED_MENU_VCF;
        led_keyboard |= 1 << table_led_key[curent_config.c_VCF1_TYPE];
        if(curent_config.c_VCF1_pitch_TRACK == 0)
            led_keyboard |= 1<< BIT_LED_MENU_KEY6;
        else
            if(curent_config.c_VCF1_pitch_TRACK == 1) {
                led_keyboard |= 1<< BIT_LED_MENU_KEY7; }
            else {
                led_keyboard |= 1<< BIT_LED_MENU_KEY8; }
		switch(curent_config.c_VCF2_TYPE) {
		case 0 :
			break;
		case 1 :
			led_keyboard |= 1<< BIT_LED_MENU_KEY11;
			break;
		case 2 :
			led_keyboard |= 1<< BIT_LED_MENU_KEY12;
			break;
		case 3 :
			led_keyboard |= 1<< BIT_LED_MENU_KEY11;
			led_keyboard |= 1<< BIT_LED_MENU_KEY12;
			break;
		}
    break;
    case MENU_ADSR :  // ADSR option
        led_keyboard |= 1<< BIT_LED_MENU_ADSR;
        led_keyboard |= 1 << table_led_key[curent_config.c_ADSR_LOOP];
        led_keyboard |= 1 << table_led_key[3+curent_config.c_VCA_TYPE];
        led_keyboard |= 1 << table_led_key[5+3+curent_config.c_KEYBOARD_octave];
        led_keyboard |= 1 << table_led_key[11+curent_config.c_KEYBOARD_toggle];
    break;
    case MENU_LFO1 :
        led_keyboard |= 1 << BIT_LED_MENU_LFO;
        if(curent_config.c_LFO1_WF<9) led_keyboard |= 1 << table_led_key[curent_config.c_LFO1_WF];
        else if ((curent_config.c_LFO1_WF<18)  &&  (g_led_blink>0))  led_keyboard |= 1 << table_led_key[curent_config.c_LFO1_WF-9];
        led_keyboard |= 1 << table_led_key[9+curent_config.c_LFO1_RANGE];
    break;
    case MENU_LFO2 :
        led_keyboard |= 1 << BIT_LED_MENU_LFO;
        if(curent_config.c_LFO2_WF<9) led_keyboard |= 1 << table_led_key[curent_config.c_LFO2_WF];
        else if ((curent_config.c_LFO2_WF<18)  &&  (g_led_blink>0))  led_keyboard |= 1 << table_led_key[curent_config.c_LFO2_WF-9];
        led_keyboard |= 1 << table_led_key[9+curent_config.c_LFO2_RANGE];
    break;
    case MENU_LFO3 :
        led_keyboard |= 1 << BIT_LED_MENU_LFO;
        if(curent_config.c_LFO3_WF<9) led_keyboard |= 1 << table_led_key[curent_config.c_LFO3_WF];
        else if ((curent_config.c_LFO3_WF<18)  &&  (g_led_blink>0))  led_keyboard |= 1 << table_led_key[curent_config.c_LFO3_WF-9];
        led_keyboard |= 1 << table_led_key[9+curent_config.c_LFO3_RANGE];
    break;
    case MENU_LFO4 :
        led_keyboard |= 1 << BIT_LED_MENU_LFO;
        if(curent_config.c_LFO4_WF<9) led_keyboard |= 1 << table_led_key[curent_config.c_LFO4_WF];
        else if ((curent_config.c_LFO4_WF<18)  &&  (g_led_blink>0))  led_keyboard |= 1 << table_led_key[curent_config.c_LFO4_WF-9];
        if(curent_config.c_LFO4_RANGE<4) led_keyboard |= 1 << table_led_key[9+curent_config.c_LFO4_RANGE];
        //else if ((curent_config.c_LFO4_RANGE==4)  &&  (g_led_blink>0)) led_keyboard |= 1 << table_led_key[9];
    break;
    case MENU_LFO5 :
        led_keyboard |= 1 << BIT_LED_MENU_LFO;
        if(curent_config.c_LFO5_WF<9) led_keyboard |= 1 << table_led_key[curent_config.c_LFO5_WF];
        else if ((curent_config.c_LFO5_WF<18)  &&  (g_led_blink>0))  led_keyboard |= 1 << table_led_key[curent_config.c_LFO5_WF-9];
        if(curent_config.c_LFO5_RANGE<4) led_keyboard |= 1 << table_led_key[9+curent_config.c_LFO5_RANGE];
        //else if ((curent_config.c_LFO5_RANGE==4)  &&  (g_led_blink>0)) led_keyboard |= 1 << table_led_key[9];
        break;
    case MENU_LFO6 :
        led_keyboard |= 1 << BIT_LED_MENU_LFO;
        if(curent_config.c_LFO6_WF<9) led_keyboard |= 1 << table_led_key[curent_config.c_LFO6_WF];
        else if ((curent_config.c_LFO6_WF<18)  &&  (g_led_blink>0))  led_keyboard |= 1 << table_led_key[curent_config.c_LFO6_WF-9];
        if(curent_config.c_LFO6_RANGE<4) led_keyboard |= 1 << table_led_key[9+curent_config.c_LFO6_RANGE];
        //else if ((curent_config.c_LFO6_RANGE==4)  &&  (g_led_blink>0)) led_keyboard |= 1 << table_led_key[9];
        break;
    case MENU_LFO7 :
        led_keyboard |= 1 << BIT_LED_MENU_LFO;
        led_keyboard |= 1 << table_led_key[9+curent_config.c_LFO7_RANGE];
    break;
    case MENU_EFFECTS :
        led_keyboard |= 1 << BIT_LED_MENU_EFFECTS;
        if(curent_config.c_EFFECT1_TYPE<7) led_keyboard |= 1 << table_led_key[curent_config.c_EFFECT1_TYPE];
        else if ( (curent_config.c_EFFECT1_TYPE<14) && (g_led_blink>0) ) led_keyboard |= 1 << table_led_key[curent_config.c_EFFECT1_TYPE - 7];
        if(curent_config.c_EFFECT2_TYPE<6) led_keyboard |= 1 << table_led_key[7 + curent_config.c_EFFECT2_TYPE];
        else if ( (curent_config.c_EFFECT2_TYPE<12) && (g_led_blink>0) )led_keyboard |= 1 << table_led_key[7 + curent_config.c_EFFECT2_TYPE-6];
    break;
    case MENU_MIDI:
        if (curent_config.c_MIDI_channel >= 0) {
            led_keyboard |= 1 << table_led_key[curent_config.c_MIDI_channel];
        } else {
            led_keyboard = ALL_KB_LEDS;
        }
    break;
    case MENU_LOAD:
        // light all key leds
        led_keyboard = ALL_KB_LEDS;
        if ( (g_last_load_save >= 0) && (g_last_load_save < 13) ) led_keyboard &= ~(1 << table_led_key[g_last_load_save]);
        led_keyboard |= 1<<table_led_key[13 + ((led_time++>>5)%7)];
    break;
    case MENU_SAVE:
    // light all key leds
        led_keyboard = ALL_KB_LEDS;
        if ( (g_last_load_save >= 0) && (g_last_load_save < 13) ) led_keyboard &= ~(1 << table_led_key[g_last_load_save]);
        led_keyboard |= 1<<table_led_key[19 - ((led_time++>>5)%7)];
    break;
    case MENU_LOAD_SAVE: // reset curent config
        if(g_affiche_version) {
				if(software_led_version & 1) led_keyboard += 1 << BIT_LED_MENU_KEY12;
				if(software_led_version & 2) led_keyboard += 1 << BIT_LED_MENU_KEY11;
				if(software_led_version & 4) led_keyboard += 1 << BIT_LED_MENU_KEY10;
				if(software_led_version & 8) led_keyboard += 1 << BIT_LED_MENU_KEY9;
				if(software_led_version & 16) led_keyboard += 1 << BIT_LED_MENU_KEY8;
				if(software_led_version & 32) led_keyboard += 1 << BIT_LED_MENU_KEY7;
				if(software_led_version & 64) led_keyboard += 1 << BIT_LED_MENU_KEY6;
				if(software_led_version & 128) led_keyboard += 1 << BIT_LED_MENU_KEY5;
				if(software_led_version & 256) led_keyboard += 1 << BIT_LED_MENU_KEY4;
				if(software_led_version & 512) led_keyboard += 1 << BIT_LED_MENU_KEY3;
		}
		else {
		        if (animation1_time > 0)	animation1_time--; else led_keyboard |= 1 << BIT_LED_MENU_KEY0;
			if (animation2_time > 0)	animation2_time--; else led_keyboard |= 1 << BIT_LED_MENU_KEY1;
			if (animation3_time > 0)	animation3_time--; else led_keyboard |= 1 << BIT_LED_MENU_KEY2;
			if (g_CV2KB) led_keyboard |= 1 << BIT_LED_MENU_KEY12;
		}
    break;
    }
    write_binary_led(led_keyboard);
}
