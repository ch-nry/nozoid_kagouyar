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

uint32_t g_shaser;
int32_t g_test_mode;
volatile  uint32_t g_led_time; // pour l'annimation des leds, variable temporaire

const uint32_t table_led_shaser[] = { LED_VCO1, LED_VCO2, LED_VCO3, LED_ADSR, \
    LED_LFO1, LED_LFO2, LED_LFO3, LED_LFO7, LED_LFO4, LED_LFO5, LED_LFO6, LED_GAIN, \
    LED_MIDI, LED_CV1, LED_CV2};

void g_time_wait(uint32_t time) {
	g_led_time = 0;
	while(g_led_time < time) {}
}

static void AudioCallbackTest(AudioHandle::InterleavingInputBuffer  in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t                                size)
{
    g_time++;
    g_led_time++;

    float increment1 = 1000.f*OneOverSR;
    float increment2 = 440.f*OneOverSR;

    //g_delay_effect1.SetDelay(100.f);

    for(size_t i = 0; i < size; ) {
        allvoice[0].v_VCO1_phase += increment1;
        if(allvoice[0].v_VCO1_phase > 1.f) allvoice[0].v_VCO1_phase -= 1.f;
        //g_delay_effect1.Write(_cos(allvoice[0].v_VCO1_phase));
        //out[i++] = g_delay_effect1.Read(); // on lit a travers un delay, pour tester aussi la memoire embarqué du daisy
		out[i++] =_cos(allvoice[0].v_VCO1_phase);
		
        allvoice[0].v_VCO2_phase += increment2;
        if(allvoice[0].v_VCO2_phase > 1.f) allvoice[0].v_VCO2_phase -= 1.f;
        out[i++] = _cos(allvoice[0].v_VCO2_phase);
    }
}

void set_all_led(float L1, float L2, float L3, float L4, float L5) {
	hw.led_driver_.SetLed_ch(LED_ADSR, L1);
	hw.led_driver_.SetLed_ch(LED_LFO1, L2);
	hw.led_driver_.SetLed_ch(LED_LFO2, L3);
	hw.led_driver_.SetLed_ch(LED_LFO3, L4);
	hw.led_driver_.SetLed_ch(LED_LFO7, L5);
	hw.led_driver_.SetLed_ch(LED_VCO1, 0.f);
	hw.led_driver_.SetLed_ch(LED_VCO2, 0.f);
	hw.led_driver_.SetLed_ch(LED_VCO3, 0.f);
	hw.led_driver_.SetLed_ch(LED_LFO4, 0.f);
	hw.led_driver_.SetLed_ch(LED_LFO5, 0.f);
	hw.led_driver_.SetLed_ch(LED_LFO6, 0.f);
	hw.led_driver_.SetLed_ch(LED_GAIN, 0.f);
	hw.led_driver_.SetLed_ch(LED_MIDI, 0.f);
	hw.led_driver_.SetLed_ch(LED_CV1, 0.f);
	hw.led_driver_.SetLed_ch(LED_CV2, 0.f);
	
	hw.led_driver_.SwapBuffersAndTransmit();
}


void test() {
    uint32_t tmp;
    float tmpf;
    uint32_t led_keyboard = 0;

    get_keyboard();
    
    for( int i=0; i<50; i++)
        get_pot(i);  // get potentiometters value and filter them

    if (g_state_kb>7) g_state_kb=1; // pour deconnecter la gestion des leds de get_keyboard()
    //if(g_time<2) return; // pour ne pas effectuer la boucle trop souvent
    //g_time=-2;
   
   	//g_switch_keyboard_bit = 0;
	//g_switch_keyboard = -1;
	//g_switch_configuration = -1; 
	//g_switch_modulation = -1;
	
   if ( (g_switch_configuration == 17) && (g_switch_keyboard == 0) ) {// SAVE + KB0
   		g_CV1_offset = 1.f-g_knob[k_CV1]; // 0V
		for (uint32_t i=0; i<16; i++) // reset toutes les led analogique
			hw.led_driver_.SetLed_ch(i, 0.f);
		for (uint32_t i=0; i<5; i++) {
			hw.led_driver_.SetLed_ch(LED_CV1, 0.f);
			hw.led_driver_.SwapBuffersAndTransmit();
			g_time_wait(30);
			hw.led_driver_.SetLed_ch(LED_CV1, 0.5f);
			hw.led_driver_.SwapBuffersAndTransmit();
			g_time_wait(30);
		}
		return;
   }
   if ( (g_switch_configuration == 17) && (g_switch_keyboard == 1) ) {// SAVE + KB1
		tmpf = 1.f-g_knob[k_CV1];  // 1V
		tmpf -= g_CV1_offset; // on doit avoir 1V de diference (par definition de la procedure de calibration)
											// cad on doit avoir 1/10 car l'amplitude du cv in est de -5 a 5V
		tmpf *= 10.f; // doit etre a 1
		g_CV1_gain = 1.f/tmpf;	
			
		for (uint32_t i=0; i<16; i++) // reset toutes les led analogique
			hw.led_driver_.SetLed_ch(i, 0.f);
		for (uint32_t i=0; i<5; i++) {
			hw.led_driver_.SetLed_ch(LED_CV1, 1.f);
			hw.led_driver_.SwapBuffersAndTransmit();
			g_time_wait(30);
			hw.led_driver_.SetLed_ch(LED_CV1, 0.5f);
			hw.led_driver_.SwapBuffersAndTransmit();
			g_time_wait(30);
		}
		return;
   }
   
   if ( (g_switch_configuration == 17) && (g_switch_keyboard == 2) ) {// SAVE + KB2
		g_CV1_offset = 0.5f-g_knob[k_CV1];
		g_CV2_offset = 0.5f-g_knob[k_CV2];		
		curent_config.c_CV1_offset = g_CV1_offset;
		curent_config.c_CV2_offset = g_CV2_offset;
		curent_config.c_CV1_gain = g_CV1_gain;
		save_config(13);

		for (uint32_t i=0; i<16; i++) // reset toutes les led analogique
			hw.led_driver_.SetLed_ch(i, 0.f);
		for (uint32_t i=0; i<15; i++) {
			hw.led_driver_.SetLed_ch(LED_CV1, 0.f);
			hw.led_driver_.SetLed_ch(LED_CV2, 0.f);
			hw.led_driver_.SwapBuffersAndTransmit();
			g_time_wait(30);
			hw.led_driver_.SetLed_ch(LED_CV1, 0.5f);
			hw.led_driver_.SetLed_ch(LED_CV2, 0.5f);
			hw.led_driver_.SwapBuffersAndTransmit();
			g_time_wait(30);
		}
		return;
   }
   
   if (g_switch_configuration >= 0) {
		for (uint32_t i=0; i<16; i++) // reset toutes les led analogique
			hw.led_driver_.SetLed_ch(i, 0.f);
	   switch(g_switch_configuration) {
		   case 0: // VCO1
			  led_keyboard = 1 << BIT_LED_MENU_KEY0;
			  led_keyboard += 1 << BIT_LED_MENU_VCO;
			  write_binary_led(led_keyboard);
		   break;
		   case 1: // VCO2
		      led_keyboard = 1 << BIT_LED_MENU_KEY1;
			  led_keyboard += 1 << BIT_LED_MENU_VCO;
			  write_binary_led(led_keyboard);
		   break;
		   case 2: // VCO3
		      led_keyboard = 1 << BIT_LED_MENU_KEY2;
			  led_keyboard += 1 << BIT_LED_MENU_VCO;
			  write_binary_led(led_keyboard);
		   break;
		   case 3: // VCF1
		      led_keyboard = 1 << BIT_LED_MENU_KEY3;
			  led_keyboard += 1 << BIT_LED_MENU_VCO;
			  write_binary_led(led_keyboard);
		   break;		   
		   case 4: // ADSR
		      led_keyboard = 1 << BIT_LED_MENU_KEY4;
			  led_keyboard += 1 << BIT_LED_MENU_VCO;
			  write_binary_led(led_keyboard);
		   break;
		   case 5: // LFO1
		      led_keyboard = 1 << BIT_LED_MENU_KEY5;
			  led_keyboard += 1 << BIT_LED_MENU_VCO;
			  write_binary_led(led_keyboard);
		   break;
		   case 6: // LFO2
		      led_keyboard = 1 << BIT_LED_MENU_KEY6;
			  led_keyboard += 1 << BIT_LED_MENU_VCO;
			  write_binary_led(led_keyboard);
		   break;
		   case 7: // LFO3
		      led_keyboard = 1 << BIT_LED_MENU_KEY7;
			  led_keyboard += 1 << BIT_LED_MENU_VCO;
			  write_binary_led(led_keyboard);
		   break;
		   case 8: // LFO4
		      led_keyboard = 1 << BIT_LED_MENU_KEY5;
			  led_keyboard += 1 << BIT_LED_MENU_VCF;
			  write_binary_led(led_keyboard);
		   break;
		   case 9: // LFO5
		      led_keyboard = 1 << BIT_LED_MENU_KEY6;
			  led_keyboard += 1 << BIT_LED_MENU_VCF;
			  write_binary_led(led_keyboard);
		   break;
		   case 10: // LFO6
		      led_keyboard = 1 << BIT_LED_MENU_KEY7;
			  led_keyboard += 1 << BIT_LED_MENU_VCF;
			  write_binary_led(led_keyboard);
		   break;
		   case 11: // LFO7
		      led_keyboard = 1 << BIT_LED_MENU_KEY8;
			  led_keyboard += 1 << BIT_LED_MENU_VCO;
			  write_binary_led(led_keyboard);
		   break;
		   case 12: // MIDI
				led_keyboard = 1 << BIT_LED_MENU_KEY0;
				led_keyboard += 1 << BIT_LED_MENU_LFO;
				write_binary_led(led_keyboard);
				// MIDI RX
				if(hw.midi.Readable()) { // MIDI in
					hw.midi.Rx(); // RX midi
					hw.led_driver_.SetLed_ch(LED_MIDI, 1.f);
				}
		   break;
		   case 13: // CV1
				led_keyboard = 1 << BIT_LED_MENU_KEY0;
				led_keyboard += 1 << BIT_LED_MENU_LFO_MOD;
				write_binary_led(led_keyboard);		
				// GATE, CV1, CV2 test
				hw.led_driver_.SetLed_ch(LED_CV1, 1.f-g_knob[k_CV1]);
				hw.led_driver_.SetLed_ch(LED_CV2, 1.f-g_knob[k_CV2]);
				if (dsy_gpio_read(&gate_pin)) 
					hw.led_driver_.SetLed_ch(LED_MIDI, 1.f);
		   break;
		   case 14: // CV2
		        led_keyboard = 1 << BIT_LED_MENU_KEY0;
			    led_keyboard += 1 << BIT_LED_MENU_EFFECTS;
			    write_binary_led(led_keyboard);
				// GATE, CV1, CV2 test		  
				hw.led_driver_.SetLed_ch(LED_CV1, 1.f-g_knob[k_CV1]);
				hw.led_driver_.SetLed_ch(LED_CV2, 1.f-g_knob[k_CV2]);
				if (dsy_gpio_read(&gate_pin)) 
					hw.led_driver_.SetLed_ch(LED_MIDI, 1.f);
		   break;
		   case 15: // EFFECT2
		      led_keyboard = 1 << BIT_LED_MENU_KEY9;
		      led_keyboard += 1 << BIT_LED_MENU_KEY10;
			  led_keyboard += 1 << BIT_LED_MENU_VCO;
			  write_binary_led(led_keyboard);
		   break;
		   case 16: // SAVE
		      led_keyboard = 1 << BIT_LED_MENU_KEY1;
			  led_keyboard += 1 << BIT_LED_MENU_EFFECTS;
			  write_binary_led(led_keyboard);
		   break;
		   case 17: // LOAD
		      led_keyboard = 1 << BIT_LED_MENU_KEY1;
			  led_keyboard += 1 << BIT_LED_MENU_LFO_MOD;
			  write_binary_led(led_keyboard);
		   break;
		   case 18: // LOAD+SAVE : numero de version
				tmp = software_led_version;
				if(tmp & 1) led_keyboard += 1 << BIT_LED_MENU_KEY12;
				tmp >>= 1;
				if(tmp & 1) led_keyboard += 1 << BIT_LED_MENU_KEY11;
				tmp >>= 1;
				if(tmp & 1) led_keyboard += 1 << BIT_LED_MENU_KEY10;
				tmp >>= 1;
				if(tmp & 1) led_keyboard += 1 << BIT_LED_MENU_KEY9;
				tmp >>= 1;
				if(tmp & 1) led_keyboard += 1 << BIT_LED_MENU_KEY8;
				tmp >>= 1;
				if(tmp & 1) led_keyboard += 1 << BIT_LED_MENU_KEY7;
				tmp >>= 1;
				if(tmp & 1) led_keyboard += 1 << BIT_LED_MENU_KEY6;
				tmp >>= 1;
				if(tmp & 1) led_keyboard += 1 << BIT_LED_MENU_KEY5;
				tmp >>= 1;
				if(tmp & 1) led_keyboard += 1 << BIT_LED_MENU_KEY4;
				tmp >>= 1;
				if(tmp & 1) led_keyboard += 1 << BIT_LED_MENU_KEY3;
			
				if(g_CV1_offset != 0.f) led_keyboard += 1 << BIT_LED_MENU_KEY0;
				if(g_CV1_gain != 1.f)   led_keyboard += 1 << BIT_LED_MENU_KEY1;
				if(g_CV2_offset != 0.f) led_keyboard += 1 << BIT_LED_MENU_KEY2;

				write_binary_led(led_keyboard);
		   break;		   
		   }
		hw.led_driver_.SwapBuffersAndTransmit();
		return;
	}
	
    if (g_switch_modulation >= 0) {
		set_all_led(0.f, 0.f, 0.f, 0.f, 0.f);

	   switch(g_switch_modulation) {
		   case 0: // VCO1 MOD1
		      led_keyboard = 1 << BIT_LED_MENU_KEY0;
			  led_keyboard += 1 << BIT_LED_MENU_VCO_MOD;
			  write_binary_led(led_keyboard);
		   break;		   
		   case 1: // VCO1 MOD2
		      led_keyboard = 1 << BIT_LED_MENU_KEY0;
			  led_keyboard += 1 << BIT_LED_MENU_VCF;
			  write_binary_led(led_keyboard);
		   break;
		   case 2: // VCO1 MOD3
		      led_keyboard = 1 << BIT_LED_MENU_KEY0;
			  led_keyboard += 1 << BIT_LED_MENU_ADSR;
			  write_binary_led(led_keyboard);
		   break;	
		   case 3: // VCO2 MOD1
		      led_keyboard = 1 << BIT_LED_MENU_KEY1;
			  led_keyboard += 1 << BIT_LED_MENU_VCO_MOD;
			  write_binary_led(led_keyboard);
		   break;		   
		   case 4: // VCO2 MOD2
		      led_keyboard = 1 << BIT_LED_MENU_KEY1;
			  led_keyboard += 1 << BIT_LED_MENU_VCF;
			  write_binary_led(led_keyboard);
		   break;
		   case 5: // VCO2 MOD3
		      led_keyboard = 1 << BIT_LED_MENU_KEY1;
			  led_keyboard += 1 << BIT_LED_MENU_ADSR;
			  write_binary_led(led_keyboard);
		   break;	
		   case 6: // VCO3 MOD1
		      led_keyboard = 1 << BIT_LED_MENU_KEY2;
			  led_keyboard += 1 << BIT_LED_MENU_VCO_MOD;
			  write_binary_led(led_keyboard);
		   break;		   
		   case 7: // VCO3 MOD2
		      led_keyboard = 1 << BIT_LED_MENU_KEY2;
			  led_keyboard += 1 << BIT_LED_MENU_VCF;
			  write_binary_led(led_keyboard);
		   break;
		   case 8: // VCO3 MOD3
		      led_keyboard = 1 << BIT_LED_MENU_KEY2;
			  led_keyboard += 1 << BIT_LED_MENU_ADSR;
			  write_binary_led(led_keyboard);
		   break;	
		   case 9: // VCF MOD1
		      led_keyboard = 1 << BIT_LED_MENU_KEY3;
			  led_keyboard += 1 << BIT_LED_MENU_VCO_MOD;
			  write_binary_led(led_keyboard);
		   break;		   
		   case 10: // VCF MOD2
		      led_keyboard = 1 << BIT_LED_MENU_KEY3;
			  led_keyboard += 1 << BIT_LED_MENU_VCF;
			  write_binary_led(led_keyboard);
		   break;
		   case 11: // LFO1 MOD
		      led_keyboard = 1 << BIT_LED_MENU_KEY5;
			  led_keyboard += 1 << BIT_LED_MENU_VCO_MOD;
			  write_binary_led(led_keyboard);
		   break;
		   case 12: // LFO2 MOD
		      led_keyboard = 1 << BIT_LED_MENU_KEY6;
			  led_keyboard += 1 << BIT_LED_MENU_VCO_MOD;
			  write_binary_led(led_keyboard);
		   break;
		   case 13: // LFO3 MOD
		      led_keyboard = 1 << BIT_LED_MENU_KEY7;
			  led_keyboard += 1 << BIT_LED_MENU_VCO_MOD;
			  write_binary_led(led_keyboard);
		   break;
		   case 14: // EFF1 MOD1
		      led_keyboard = 1 << BIT_LED_MENU_KEY9;
			  led_keyboard += 1 << BIT_LED_MENU_VCO_MOD;
			  write_binary_led(led_keyboard);
		   break;
		   case 15: // EFF2 MOD1
		      led_keyboard = 1 << BIT_LED_MENU_KEY10;
			  led_keyboard += 1 << BIT_LED_MENU_VCO_MOD;
			  write_binary_led(led_keyboard);
		   break;
		   case 16: // VCF2 MOD
		      led_keyboard = 1 << BIT_LED_MENU_KEY11;
			  led_keyboard += 1 << BIT_LED_MENU_VCO_MOD;
			  write_binary_led(led_keyboard);
		   break;
		}
		return;
	}

	if ( (g_switch_modulation == -1) && (g_switch_configuration == -1) ) {
		switch(g_switch_keyboard+1) { 
		case 0 :  // chaser leds : si aucune touche du clavier n'est appuyé
			if ( g_led_time > 300 ) {
				for (uint32_t i=0; i<16; i++) // reset toutes les led analogique
					hw.led_driver_.SetLed_ch(i, 0.f);
				led_keyboard = 0; // reset les leds numérique
				
				if (g_shaser < 15) { 
					hw.led_driver_.SetLed_ch(table_led_shaser[g_shaser], 1.f); // set la led analogique si besion
				} else {
					led_keyboard = 1 << table_led_key[g_shaser-15]; // set les leds numérique
				}
				write_binary_led(led_keyboard);
				hw.led_driver_.SwapBuffersAndTransmit();

				g_led_time = 0;
				g_shaser++; // position de la led actuel
				if (g_shaser >= BIT_LAST + 15)
					g_shaser = 0;
			}
			break;
		case 1 : // touche 0 : test fader VCO1
			// affichage du menu actuel
			led_keyboard = 1 << table_led_key[0];
			write_binary_led(led_keyboard);
			set_all_led(g_knob[k_VCO1_fq], g_knob[k_VCO1_wfm], g_knob[k_VCO1_mod1], g_knob[k_VCO1_mod2], g_knob[k_VCO1_mod3]);
			break;
		  case 2 : // touche 1 : test fader VCO2
			led_keyboard = 1 << table_led_key[1];
			write_binary_led(led_keyboard);
			set_all_led(g_knob[k_VCO2_fq], g_knob[k_VCO2_wfm], g_knob[k_VCO2_mod1], g_knob[k_VCO2_mod2], g_knob[k_VCO2_mod3]);
			break;
		 case 3 : // touche 2 : test fader VCO3
			led_keyboard = 1 << table_led_key[2];
			write_binary_led(led_keyboard);
			set_all_led(g_knob[k_VCO3_fq], g_knob[k_VCO3_wfm], g_knob[k_VCO3_mod1], g_knob[k_VCO3_mod2], g_knob[k_VCO3_mod3]);
			break;
		 case 4 : // touche 3 : test fader MIX
			led_keyboard = 1 << table_led_key[3];
			write_binary_led(led_keyboard);
			set_all_led(g_knob[k_MIX1], g_knob[k_MIX2], g_knob[k_MIX3], 0.f, 0.f);
			break;
		 case 5 : // touche 4 : test fader VCF1
			led_keyboard = 1 << table_led_key[4];
			write_binary_led(led_keyboard);
			set_all_led(g_knob[k_VCF1_fq], g_knob[k_VCF1_q], g_knob[k_VCF1_mod1], g_knob[k_VCF1_mod2], 0.f);
			break;         
		 case 6 : // touche 5 : test fader ADSR
			led_keyboard = 1 << table_led_key[5];
			write_binary_led(led_keyboard);
			set_all_led(g_knob[k_ADSR_a], g_knob[k_ADSR_d], g_knob[k_ADSR_s], g_knob[k_ADSR_r], 0.f);
			break;
		 case 7 : // touche 6 : test fader LFO1
			led_keyboard = 1 << table_led_key[6];
			write_binary_led(led_keyboard);
			set_all_led(g_knob[k_LFO1_fq], g_knob[k_LFO1_mod], g_knob[k_LFO4_fq], 0.f, 0.f);
			break;
		 case 8 : // touche 7 : test fader LFO2
			led_keyboard = 1 << table_led_key[7];
			write_binary_led(led_keyboard);
			set_all_led(g_knob[k_LFO2_fq], g_knob[k_LFO2_mod], g_knob[k_LFO5_fq], 0.f, 0.f);
			break;
		 case 9 : // touche 8 : test fader LFO3
			led_keyboard = 1 << table_led_key[8];
			write_binary_led(led_keyboard);
			set_all_led(g_knob[k_LFO3_fq], g_knob[k_LFO3_mod], g_knob[k_LFO6_fq], 0.f, 0.f);
			break;
		 case 10 : // touche 9 : test fader LFO7
			led_keyboard = 1 << table_led_key[9];
			write_binary_led(led_keyboard);
			set_all_led(g_knob[k_LFO7_fq], g_knob[k_LFO7_wf], g_knob[k_LFO7_sym], 0.f, 0.f);
			break;
		 case 11 : // touche 10 : test fader EFFECT1
			led_keyboard = 1 << table_led_key[10];
			write_binary_led(led_keyboard);
			set_all_led(g_knob[k_EFFECT1_wet], g_knob[k_EFFECT1_p1], g_knob[k_EFFECT1_p2], 0.f, 0.f);
			break;
		 case 12 : // touche 11 : test fader EFFECT2
			led_keyboard = 1 << table_led_key[11];
			write_binary_led(led_keyboard);
			set_all_led(g_knob[k_EFFECT2_wet], g_knob[k_EFFECT2_p1], 0.f, 0.f, 0.f);
			break;
		 case 13 : // touche 12 : test fader VCF2
			led_keyboard = 1 << table_led_key[12];
			write_binary_led(led_keyboard);
			set_all_led(g_knob[k_VCF2_fq], g_knob[k_VCF2_mod], g_knob[k_GAIN], 1.f-g_knob[k_CV1], 1.f-g_knob[k_CV2]);
			break;
		}
		return;
	}
}
