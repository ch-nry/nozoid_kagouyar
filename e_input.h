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


#ifdef proto2
//version SSOP
// kb I2C data
#define BIT_KEY_SEL_VCO1  			3
#define BIT_KEY_SEL_VCO2  			7
#define BIT_KEY_SEL_VCO3  			4
#define BIT_KEY_SEL_VCF1  			5
#define BIT_KEY_SEL_ADSR  			6
#define BIT_KEY_SEL_LFO1  			17
#define BIT_KEY_SEL_LFO2  			19
#define BIT_KEY_SEL_LFO3  			20
#define BIT_KEY_SEL_LFO4  			10
#define BIT_KEY_SEL_LFO5  			11
#define BIT_KEY_SEL_LFO6  			15
#define BIT_KEY_SEL_LFO7  			21
#define BIT_KEY_VCO1_MOD1 		2
#define BIT_KEY_VCO1_MOD2 		27
#define BIT_KEY_VCO1_MOD3 		26
#define BIT_KEY_VCO2_MOD1 		1
#define BIT_KEY_VCO2_MOD2 		28
#define BIT_KEY_VCO2_MOD3 		31
#define BIT_KEY_VCO3_MOD1 		0
#define BIT_KEY_VCO3_MOD2 		30
#define BIT_KEY_VCO3_MOD3 		29
#define BIT_KEY_VCF1_MOD1 		9
#define BIT_KEY_VCF1_MOD2 		8
#define BIT_KEY_LFO1_MOD1 		16
#define BIT_KEY_LFO2_MOD1 		18
#define BIT_KEY_LFO3_MOD1 		23
#define BIT_KEY_EFF1      				22
#define BIT_KEY_EFF2_MOD  			13
#define BIT_KEY_EFF1_MOD  			12
#define BIT_KEY_VCF2_MOD  		14
#define BIT_KEY_LOAD      				24
#define BIT_KEY_SAVE      				25

// led I2C data
#define BIT_KEY_kb0      	3
#define BIT_KEY_kb1      	7
#define BIT_KEY_kb2      	4
#define BIT_KEY_kb3      	5
#define BIT_KEY_kb4      	6
#define BIT_KEY_kb5      	8
#define BIT_KEY_kb6      	9
#define BIT_KEY_kb7      	10
#define BIT_KEY_kb8      	11
#define BIT_KEY_kb9      	15
#define BIT_KEY_kb10     	12
#define BIT_KEY_kb11     	13
#define BIT_KEY_kb12     	14
#define BIT_KEY_SEL_MIDI 2
#define BIT_KEY_SEL_CV1  1
#define BIT_KEY_SEL_CV2  0
#endif

#ifndef proto2
//version QFN
// kb I2C data
#define BIT_KEY_SEL_VCO1  			3
#define BIT_KEY_SEL_VCO2  			4
#define BIT_KEY_SEL_VCO3  			5
#define BIT_KEY_SEL_VCF1  			6
#define BIT_KEY_SEL_ADSR  			7
#define BIT_KEY_SEL_LFO1  			17
#define BIT_KEY_SEL_LFO2  			19
#define BIT_KEY_SEL_LFO3  			21
#define BIT_KEY_SEL_LFO4  			10
#define BIT_KEY_SEL_LFO5  			11
#define BIT_KEY_SEL_LFO6  			12
#define BIT_KEY_SEL_LFO7  			22
#define BIT_KEY_VCO1_MOD1 		2
#define BIT_KEY_VCO1_MOD2 		27
#define BIT_KEY_VCO1_MOD3 		26
#define BIT_KEY_VCO2_MOD1 		1
#define BIT_KEY_VCO2_MOD2 		29
#define BIT_KEY_VCO2_MOD3 		28
#define BIT_KEY_VCO3_MOD1 		0
#define BIT_KEY_VCO3_MOD2 		31
#define BIT_KEY_VCO3_MOD3 		30
#define BIT_KEY_VCF1_MOD1 		9
#define BIT_KEY_VCF1_MOD2 		8
#define BIT_KEY_LFO1_MOD1 		16
#define BIT_KEY_LFO2_MOD1 		18
#define BIT_KEY_LFO3_MOD1 		20
#define BIT_KEY_EFF1      				23
#define BIT_KEY_EFF2_MOD  			14
#define BIT_KEY_EFF1_MOD  			13
#define BIT_KEY_VCF2_MOD  		15
#define BIT_KEY_LOAD      				24
#define BIT_KEY_SAVE      				25

// led I2C data
#define BIT_KEY_kb0      	3
#define BIT_KEY_kb1      	4
#define BIT_KEY_kb2      	5
#define BIT_KEY_kb3      	6
#define BIT_KEY_kb4      	7
#define BIT_KEY_kb5      	8
#define BIT_KEY_kb6      	9
#define BIT_KEY_kb7      	10
#define BIT_KEY_kb8      	11
#define BIT_KEY_kb9      	12
#define BIT_KEY_kb10     	13
#define BIT_KEY_kb11     	14
#define BIT_KEY_kb12     	15
#define BIT_KEY_SEL_MIDI 2
#define BIT_KEY_SEL_CV1  1
#define BIT_KEY_SEL_CV2  0
#endif

const int bit_key_I2C2[] = { BIT_KEY_kb0, BIT_KEY_kb1, BIT_KEY_kb2, BIT_KEY_kb3, BIT_KEY_kb4, BIT_KEY_kb5, BIT_KEY_kb6, BIT_KEY_kb7, BIT_KEY_kb8,
        BIT_KEY_kb9, BIT_KEY_kb10, BIT_KEY_kb11, BIT_KEY_kb12, BIT_KEY_SEL_MIDI, BIT_KEY_SEL_CV1, BIT_KEY_SEL_CV2 };

// timming :
//reception de la valeur d'1 capteur capacitif (8 key) = 100µs
// envoie des leds : 1.8ms

///////////////////////////////////////////////////////
// MIDI
///////////////////////////////////////////////////////

uint32_t g_MIDI_status, MIDI_data1, MIDI_data2, g_MIDI_state;

// pour la syncro :
uint32_t g_MIDI_QNote, g_MIDI_count;
// g_MIDI_count doit etre incrementé a la meme vitese que le calcule du phasor des LFO syncronisés sur le MIDI

// g_MIDI_state :
//    0 : no status received
//    1 : 1 status received, waiting for data1
//    2 : data 1 received, waiting for data2

// uint32_t g_last_key = 0; // pour les switch physique

///////////////////////////////////////////////////////
// Keyboard
///////////////////////////////////////////////////////
// valeur actuel
uint32_t g_switch1=0, g_switch2=0; // valeur des touches en cours de récupération

///////////////////////////////////////////////////////
uint32_t g_menu_count = 0; // pour compter le temps d'apuis sur une touche (pour tester les appuis long)
volatile int32_t g_led_blink;


// sauvegarde des touches
uint32_t g_last_switch_keyboard_bit = 0; // toutes les touches, en binaire
//kb0..kb12
uint32_t g_save_switch_keyboard_bit = 0; // idem, mais pour les notes
int32_t g_last_switch_keyboard = -1; // numero de touche appuyé
int32_t g_last_switch_modulation = -1; // numero de la derniere touche de modulation apuyé
// VCO1 mod 1, mod2, mod3, VCO2 mod1....VCO3 mod3,  VCF1, mod 1 et 2, LFO 1 mod, LFO2 mod, LFO3 mod
int32_t g_last_switch_configuration = -1; // numero de la derniere touche de selection / configuration apuyé
// VCO1, VCO2, VCO3, VCF, LFO1, ... , LFO7,MIDI, CV1, CV2, EFFECT, LOAD, SAVE, CONFIG (== LOAD+SAVE)

// touches actuel : idem au dessus, pour pouvoir etre comparé.
uint32_t g_switch_keyboard_bit = 0;
int32_t g_switch_keyboard = -1;
int32_t  g_switch_modulation = -1;
int32_t  g_switch_configuration = -1;

volatile uint32_t g_change_any_key = 0;

uint32_t g_last_led_state = 0; // mode de fonctionement des leds
// 0 : affichage des valeurs de modulation et les touches du clavier
// 1 : affichage des sources de modulation et le clavier
// 2 : affichage des valeurs de modulation du menu de configuration
// 3 : aucun changement, on reste sur le mode precedent

uint32_t animation1_time = 0;
uint32_t animation2_time = 0;
uint32_t animation3_time = 0;
uint32_t g_affiche_version = 0;

///////////////////////////////////////////////////////
// analogue in
///////////////////////////////////////////////////////

uint32_t g_analog_gate = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// prototype for led output fuction
inline void key_and_led();
void leds_mode_normal();
void leds_keyboard();
void leds_key_modulation(uint32_t my_modulation);
void leds_key_configuration(uint32_t my_menu_switch);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void get_keyboard() { // recupere les information des 6 capteurs capacitif, et les organise
    // les fonctions de communication sont bloquante (interrompu par la routine audio)
    // cette tache est séparé en diferentes fonction bloquante afin de pouvoir rendre la main a des fonction devant etre lancé plus frequement comme les receptions MIDI
    uint8_t kb_data[2];
    uint32_t ui_tmp;
    uint32_t l_switch1, l_switch2;
    kb_data[0] = 0;
    if (g_time < 0) return; // on execute la fonction seulement si c'est le bon moment
                            // g_time est incrémenté ds la boucle audio
    g_menu_count++;
    g_menu_count = (g_menu_count > 1000000)? 1000000:g_menu_count; // temps depuis le dernier changement pour les appuie long

    switch(g_state_kb) {
    case 0:
        key_and_led(); // update configuration and leds
        g_state_kb++;
        g_time = -3 * block_per_ms; // 3ms min entre les test des inputs capacitive
        g_change_any_key = 0;
        break;
    case 1:
        if(dsy_gpio_read(&Kb1_int) != 1)
        {	// si on as un changement a tester
			g_change_any_key = 1;
			if(i2c_kb.ReadDataAtAddress(136, 2, 1, kb_data, 2, 10) == I2CHandle::Result::OK)
			{
				ui_tmp = g_switch1;
				ui_tmp &= 0xFFFFFF00; // clear current bits
				ui_tmp += kb_data[0];
				g_switch1 = ui_tmp;
				//g_time = -1 * block_per_ms;
			} else {
				i2c_kb.Init(i2c_kb.GetConfig()); // reinit I2C driver, and do this test again
				g_time = -2 * block_per_ms;
			}
		}
		g_state_kb++;
		break;
    case 2 :
        if(dsy_gpio_read(&Kb1_int) != 1)
        {
			g_change_any_key = 1;
			if(i2c_kb.ReadDataAtAddress(138, 2, 1, kb_data, 2, 10) == I2CHandle::Result::OK)
			{
				ui_tmp = g_switch1;
				ui_tmp &= 0xFFFF00FF; // clear current bits
				ui_tmp += kb_data[0]<<8;
				g_switch1 = ui_tmp;
				//g_time = -1 * block_per_ms;
			} else {
				i2c_kb.Init(i2c_kb.GetConfig());
				g_time = -2 * block_per_ms;
			}
		}
		g_state_kb++;
		break;
    case 3 :
		if(dsy_gpio_read(&Kb1_int) != 1)
		{
			g_change_any_key = 1;
			if(i2c_kb.ReadDataAtAddress(140, 2, 1, kb_data, 2, 10) == I2CHandle::Result::OK)
			{
				ui_tmp = g_switch1;
				ui_tmp &= 0xFF00FFFF; // clear current bits
				ui_tmp += kb_data[0]<<16;
				g_switch1 = ui_tmp;
				//g_time = -1 * block_per_ms;
			} else {
				i2c_kb.Init(i2c_kb.GetConfig());
				g_time = -2 * block_per_ms;
			}
		}
		g_state_kb++;
		break;
    case 4 :
		if(dsy_gpio_read(&Kb1_int) != 1)
		{
			g_change_any_key = 1;
			if(i2c_kb.ReadDataAtAddress(142, 2, 1, kb_data, 2, 10) == I2CHandle::Result::OK)
			{
				ui_tmp = g_switch1;
				ui_tmp &= 0x00FFFFFF; // clear current bits
				ui_tmp += kb_data[0]<<24;
				g_switch1 = ui_tmp;
				//g_time = -1 * block_per_ms;
			} else {
				i2c_kb.Init(i2c_kb.GetConfig());
				g_time = -2 * block_per_ms;
			}
		}
		g_state_kb++;
		break;
    case 5 :
        if(dsy_gpio_read(&Kb2_int) != 1)
        {
			g_change_any_key = 1;
			if(i2c_led.ReadDataAtAddress(136, 2, 1, kb_data, 2, 10) == I2CHandle::Result::OK)
			{
				ui_tmp = g_switch2;
				ui_tmp &= 0xFFFFFF00; // clear current bits
				ui_tmp += kb_data[0];
				g_switch2 = ui_tmp;
				//g_time = -1 * block_per_ms;
			} else {
				i2c_led.Init(i2c_led.GetConfig());
				g_time = -2 * block_per_ms;
			}
		}
		g_state_kb++;
		break;
    case 6:
        if(dsy_gpio_read(&Kb2_int) != 1)
        {
			g_change_any_key = 1;
			if(i2c_led.ReadDataAtAddress(142, 2, 1, kb_data, 2, 10) == I2CHandle::Result::OK)
			{
				ui_tmp = g_switch2;
				ui_tmp &= 0xFFFF00FF; // clear current bits
				ui_tmp += kb_data[0]<<8;
				g_switch2 = ui_tmp;
				//g_time = -1 * block_per_ms;
			} else {
				i2c_led.Init(i2c_led.GetConfig());
				g_time = -2 * block_per_ms;
			}
		}
		g_state_kb++;
		break;
	case 7:
		int32_t switch_configuration, switch_modulation, switch_keyboard;
		uint32_t switch_keyboard_bit;
		int32_t i;
		if (g_change_any_key == 0) { g_state_kb++; break; }
		g_change_any_key = 0;

		l_switch1 = g_switch1;
		l_switch2 = g_switch2;

		switch_keyboard = -1;
		switch_keyboard_bit = 0;
		switch_modulation = -1;
		switch_configuration = -1;

		g_menu_count = 0;	// raz du  temps depuis le dernier changement

		if (l_switch1 & (1<<BIT_KEY_SEL_VCO1)) switch_configuration = MENU_VCO1;
		if (l_switch1 & (1<<BIT_KEY_SEL_VCO2)) switch_configuration = MENU_VCO2;
		if (l_switch1 & (1<<BIT_KEY_SEL_VCO3)) switch_configuration = MENU_VCO3;
		if (l_switch1 & (1<<BIT_KEY_SEL_VCF1)) switch_configuration = MENU_VCF1;
		if (l_switch1 & (1<<BIT_KEY_SEL_ADSR)) switch_configuration = MENU_ADSR;
		if (l_switch1 & (1<<BIT_KEY_SEL_LFO1)) switch_configuration = MENU_LFO1;
		if (l_switch1 & (1<<BIT_KEY_SEL_LFO2)) switch_configuration = MENU_LFO2;
		if (l_switch1 & (1<<BIT_KEY_SEL_LFO3)) switch_configuration = MENU_LFO3;
		if (l_switch1 & (1<<BIT_KEY_SEL_LFO4)) switch_configuration = MENU_LFO4;
		if (l_switch1 & (1<<BIT_KEY_SEL_LFO5)) switch_configuration = MENU_LFO5;
		if (l_switch1 & (1<<BIT_KEY_SEL_LFO6)) switch_configuration = MENU_LFO6;
		if (l_switch1 & (1<<BIT_KEY_SEL_LFO7)) switch_configuration = MENU_LFO7;
		if (l_switch1 & (1<<BIT_KEY_EFF1))     switch_configuration = MENU_EFFECTS;
		if (l_switch1 & (1<<BIT_KEY_LOAD))     switch_configuration = MENU_LOAD;
		if (l_switch1 & (1<<BIT_KEY_SAVE))     switch_configuration = MENU_SAVE;

		if ( (l_switch1 & (1<<BIT_KEY_SAVE))  && (l_switch1 & (1<<BIT_KEY_LOAD)) ) switch_configuration = MENU_LOAD_SAVE;

		if (l_switch2 & (1<<BIT_KEY_SEL_MIDI)) switch_configuration = MENU_MIDI;
		if (l_switch2 & (1<<BIT_KEY_SEL_CV1))  switch_configuration = MENU_CV1;
		if (l_switch2 & (1<<BIT_KEY_SEL_CV2))  switch_configuration = MENU_CV2;

		g_switch_configuration = switch_configuration;

		if (l_switch1 & (1<<BIT_KEY_VCO1_MOD1)) switch_modulation = VCO1_MOD1;
		if (l_switch1 & (1<<BIT_KEY_VCO1_MOD2)) switch_modulation = VCO1_MOD2;
		if (l_switch1 & (1<<BIT_KEY_VCO1_MOD3)) switch_modulation = VCO1_MOD3;
		if (l_switch1 & (1<<BIT_KEY_VCO2_MOD1)) switch_modulation = VCO2_MOD1;
		if (l_switch1 & (1<<BIT_KEY_VCO2_MOD2)) switch_modulation = VCO2_MOD2;
		if (l_switch1 & (1<<BIT_KEY_VCO2_MOD3)) switch_modulation = VCO2_MOD3;
		if (l_switch1 & (1<<BIT_KEY_VCO3_MOD1)) switch_modulation = VCO3_MOD1;
		if (l_switch1 & (1<<BIT_KEY_VCO3_MOD2)) switch_modulation = VCO3_MOD2;
		if (l_switch1 & (1<<BIT_KEY_VCO3_MOD3)) switch_modulation = VCO3_MOD3;
		if (l_switch1 & (1<<BIT_KEY_VCF1_MOD1)) switch_modulation = VCF1_MOD1;
		if (l_switch1 & (1<<BIT_KEY_VCF1_MOD2)) switch_modulation = VCF1_MOD2;
		if (l_switch1 & (1<<BIT_KEY_LFO1_MOD1)) switch_modulation = LFO1_MOD;
		if (l_switch1 & (1<<BIT_KEY_LFO2_MOD1)) switch_modulation = LFO2_MOD;
		if (l_switch1 & (1<<BIT_KEY_LFO3_MOD1)) switch_modulation = LFO3_MOD;
		if (l_switch1 & (1<<BIT_KEY_EFF1_MOD))  switch_modulation = EFFECT1_MOD;
		if (l_switch1 & (1<<BIT_KEY_EFF2_MOD))  switch_modulation = EFFECT2_MOD;
		if (l_switch1 & (1<<BIT_KEY_VCF2_MOD))  switch_modulation = VCF2_MOD1;
		g_switch_modulation = switch_modulation;

		for (i=0;i<13;i++) {
			if (l_switch2 & (1<<bit_key_I2C2[i])) {
				switch_keyboard_bit += (1<<i);
				switch_keyboard = i;
			}
		}
		g_switch_keyboard_bit = switch_keyboard_bit;
		g_switch_keyboard = switch_keyboard;
		g_state_kb++;
        g_time = 0;
        break;
    case 8:
        g_state_kb = 0; // retour au debut
        g_time = 0; // limitation de la vitesse d'update
        break;
    }
}

void change_key(uint32_t current_key_bit) {
    int32_t pitch;

    for (int32_t i=0; i<13; i++) {
        if ( ((g_save_switch_keyboard_bit>>i) & 1) != ((current_key_bit>>i) & 1) ) { // on est sur une touche qui a changé
            pitch = i+12*curent_config.c_KEYBOARD_octave; //
            if(curent_config.c_KEYBOARD_toggle == 0) {  // mode normal
                if ( (current_key_bit>>i) & 1 ) { // appuie sur la touche : note on
                    add_voice(0, pitch);
                }
                else {
                    remove_voice(0, pitch); // on relache : note off
                }
            }
            else { // mode toggle
                if ( (current_key_bit>>i) & 1) { // appuie sur la touche : on toggle la note
                    if (test_voice(pitch))
                        remove_voice(0, pitch); // remove si la voie etait on
                    else
                        add_voice(0, pitch); // on l'ajoute sinon
                }
            }
        }
    }
    g_save_switch_keyboard_bit = current_key_bit;
}

void set_modulation (uint32_t switch_modulation, uint32_t new_mod, uint32_t change_time) {
	if(!change_time) {
		if (curent_config.c_Modulation_Source[switch_modulation] == new_mod)
			curent_config.c_Modulation_Source[switch_modulation] = NONE_OUT;
		else
			curent_config.c_Modulation_Source[switch_modulation] = new_mod;
	}
}

void set_modulation_negatif (uint32_t switch_modulation, uint32_t new_mod, uint32_t change_time) {
	if(change_time) {
		curent_config.c_Modulation_Source[switch_modulation] = new_mod + modulation_source_last;
	}
	else {
		if (curent_config.c_Modulation_Source[switch_modulation] == new_mod)
			curent_config.c_Modulation_Source[switch_modulation] = NONE_OUT;
		else
			curent_config.c_Modulation_Source[switch_modulation] = new_mod;
	}
}

 void set_modulation_special (uint32_t switch_modulation, uint32_t new_mod, uint32_t change_time) {
	if(!change_time) {
		uint32_t uint_tmp2 = curent_config.c_Modulation_Source[switch_modulation];
		uint32_t tmp = 0;
		if(uint_tmp2 < ADSR_OUT) { // on as une modulation issue d'un VCO
			tmp = uint_tmp2%6; // on cherche la formes d'ondes "speciales"
		}
		set_modulation( switch_modulation, new_mod + tmp, 0);
	}
 }

int keyboard_all() { // gere le clavier : change les configs si besion and return le mode d'affichage des leds :
    // 0 : mode normal
    // 1 : mode modulation
    // 2 : mode configuration
    // 3 : aucun changement, on reste sur le dernier mode

    uint32_t change_keyboard, change_modulation, change_configuration, change_time;
    uint32_t pressed_keyboard, pressed_modulation, pressed_configuration;
    uint32_t uint_tmp, uint_tmp2;
    int32_t switch_modulation = g_switch_modulation;
    int32_t switch_configuration = g_switch_configuration;
    uint32_t switch_keyboard_bit = g_switch_keyboard_bit;
    int32_t switch_keyboard = g_switch_keyboard;
    uint32_t l_last_switch_keyboard_bit;
    uint32_t j;

    change_keyboard = (g_last_switch_keyboard_bit != switch_keyboard_bit);
    change_modulation = (g_last_switch_modulation != switch_modulation);
    change_configuration = (g_last_switch_configuration != switch_configuration);
    change_time = ((g_menu_count > 1000) && (g_menu_count < 1000000)); // pour tester les appuie long
    if (change_time) g_menu_count=1000000; // on ne traite qu'une fois le "change_time"

    g_last_switch_modulation = switch_modulation;
    g_last_switch_configuration = switch_configuration;
    g_last_switch_keyboard = switch_keyboard;
    l_last_switch_keyboard_bit = g_last_switch_keyboard_bit;
    g_last_switch_keyboard_bit = switch_keyboard_bit;

    // si il y a un changement : a quel niveau?
    pressed_keyboard = (switch_keyboard>=0);
    pressed_modulation = (switch_modulation>=0);
    pressed_configuration = (switch_configuration>=0);

    if (!change_keyboard && !change_modulation && !change_configuration && !change_time)  return 3; // si rien n'a changé, on ne fait rien de neuf
    g_affiche_version = 0;

   if (pressed_modulation && pressed_configuration ) { // changement de source de modulation
        switch (switch_configuration) {
           case MENU_VCO1:
				set_modulation_special(switch_modulation, VCO1_OUT, change_time);
            break;
            case MENU_VCO2:
				set_modulation_special(switch_modulation, VCO2_OUT, change_time);
            break;
            case MENU_VCO3:
				set_modulation_special(switch_modulation, VCO3_OUT, change_time);
            break;
            case MENU_VCF1:
                // ce n'est pas une modulation...
            break;
            case MENU_ADSR:
				set_modulation_negatif(switch_modulation, ADSR_OUT, change_time);
            break;
            case MENU_LFO1:
				set_modulation_negatif(switch_modulation, LFO1_OUT, change_time);
            break;
            case MENU_LFO2:
				set_modulation_negatif(switch_modulation, LFO2_OUT, change_time);
            break;
            case MENU_LFO3:
				set_modulation_negatif(switch_modulation, LFO3_OUT, change_time);
            break;
            case MENU_LFO4:
				set_modulation_negatif(switch_modulation, LFO4_OUT, change_time);
            break;
            case MENU_LFO5:
				set_modulation_negatif(switch_modulation, LFO5_OUT, change_time);
            break;
            case MENU_LFO6:
				set_modulation_negatif(switch_modulation, LFO6_OUT, change_time);
            break;
            case MENU_LFO7:
				set_modulation_negatif(switch_modulation, LFO7_OUT, change_time);
            break;
            case MENU_MIDI:
            	set_modulation_negatif(switch_modulation, MIDI_modulation, change_time);
            break;
            case MENU_CV1:
				set_modulation_negatif(switch_modulation, CV1_OUT, change_time);
            break;
            case MENU_CV2:
				set_modulation_negatif(switch_modulation, CV2_OUT, change_time);
            break;
        }
        return 1;
    }

    if (pressed_configuration && pressed_keyboard) { // changement de paramettre de configuration
        switch(switch_configuration){
        case MENU_VCO1 :
            if (switch_keyboard <= 8) {
            	curent_config.c_VCO1_WF = 18;
            	for (j=0;j<nb_voice;j++) {
					allvoice[j].v_VCO_last[VCO1_Id][0] = _rnd_f();
					allvoice[j].v_VCO_last[VCO1_Id][1] = 0.f;
					allvoice[j].v_VCO_last[VCO1_Id][2] = 0.f;
					allvoice[j].v_VCO_last[VCO1_Id][3] = 0.f;
					allvoice[j].v_VCO_last[VCO1_Id][4] = 0.f;
					allvoice[j].v_VCO_last[VCO1_Id][5] = 0.f;
					allvoice[j].v_VCO_last[VCO1_Id][6] = 0.f;
					allvoice[j].v_VCO_last[VCO1_Id][7] = 0.f;
					allvoice[j].v_VCO_phase[VCO1_Id] = 0.f;
				}
				g_Modulation[VCO1_OUT] = 0.0f;
				g_Modulation[VCO1_SIN] = 0.f;
				g_Modulation[VCO1_SQUARE] = 0.f;
				g_Modulation[VCO1_TRI] = 0.f;
				g_Modulation[VCO1_RAMP] = 0.f;
				g_Modulation[VCO1_SAW] = 0.f;
				if(curent_config.c_VCO1_WF == (uint32_t)switch_keyboard + 9 * change_time) curent_config.c_VCO1_WF=18;
                else curent_config.c_VCO1_WF = switch_keyboard + 9 * change_time;
			}
            else {
                switch (switch_keyboard-9) {
                case 0 :
                    break;
                case 1 :
                    curent_config.c_VCO1_RANGE = 0;
                    break;
                case 2 :
                    curent_config.c_VCO1_RANGE = 1;
                    break;
                case 3 :
                    curent_config.c_VCO1_RANGE = 2;
                    break;

                default :
                    break;
                }
            }
        break;
        case MENU_VCO2 :
            if (switch_keyboard <= 8) {
				curent_config.c_VCO2_WF = 18;
            	for (j=0;j<nb_voice;j++) {
					allvoice[j].v_VCO_last[VCO2_Id][0] = _rnd_f();
					allvoice[j].v_VCO_last[VCO2_Id][1] = 0.f;
					allvoice[j].v_VCO_last[VCO2_Id][2] = 0.f;
					allvoice[j].v_VCO_last[VCO2_Id][3] = 0.f;
					allvoice[j].v_VCO_last[VCO2_Id][4] = 0.f;
					allvoice[j].v_VCO_last[VCO2_Id][5] = 0.f;
					allvoice[j].v_VCO_last[VCO2_Id][6] = 0.f;
					allvoice[j].v_VCO_last[VCO2_Id][7] = 0.f;
					allvoice[j].v_VCO_phase[VCO2_Id] = 0.f;
				}
				g_Modulation[VCO2_OUT] = 0.0f;
				g_Modulation[VCO2_SIN] = 0.f;
				g_Modulation[VCO2_SQUARE] = 0.f;
				g_Modulation[VCO2_TRI] = 0.f;
				g_Modulation[VCO2_RAMP] = 0.f;
				g_Modulation[VCO2_SAW] = 0.f;
				if(curent_config.c_VCO2_WF == (uint32_t)switch_keyboard+ 9 * change_time) curent_config.c_VCO2_WF=18;
                else curent_config.c_VCO2_WF = switch_keyboard+ 9 * change_time;
			}
            else {
                switch (switch_keyboard-9) {
                case 0 :
                    curent_config.c_VCO2_LINK = (curent_config.c_VCO2_LINK == 0);
                    break;
                case 1 :
                    curent_config.c_VCO2_RANGE = 0;
                    break;
                case 2 :
                    curent_config.c_VCO2_RANGE = 1;
                    break;
                case 3 :
                    curent_config.c_VCO2_RANGE = 2;
                    break;
                default :
                    break;
                }
            }
        break;
        case MENU_VCO3 :
            if (switch_keyboard <= 8) {
            	curent_config.c_VCO3_WF = 18;
            	for (j=0;j<nb_voice;j++) {
					allvoice[j].v_VCO_last[VCO3_Id][0] = _rnd_f();
					allvoice[j].v_VCO_last[VCO3_Id][1] = 0.f;
					allvoice[j].v_VCO_last[VCO3_Id][2] = 0.f;
					allvoice[j].v_VCO_last[VCO3_Id][3] = 0.f;
					allvoice[j].v_VCO_last[VCO3_Id][4] = 0.f;
					allvoice[j].v_VCO_last[VCO3_Id][5] = 0.f;
					allvoice[j].v_VCO_last[VCO3_Id][6] = 0.f;
					allvoice[j].v_VCO_last[VCO3_Id][7] = 0.f;
					allvoice[j].v_VCO_phase[VCO3_Id] = 0.f;
				}
				g_Modulation[VCO3_OUT] = 0.0f;
				g_Modulation[VCO3_SIN] = 0.f;
				g_Modulation[VCO3_SQUARE] = 0.f;
				g_Modulation[VCO3_TRI] = 0.f;
				g_Modulation[VCO3_RAMP] = 0.f;
				g_Modulation[VCO3_SAW] = 0.f;
 				if(curent_config.c_VCO3_WF == (uint32_t)switch_keyboard+ 9 * change_time) curent_config.c_VCO3_WF=18;
                else curent_config.c_VCO3_WF = switch_keyboard+ 9 * change_time;
			}
            else {
                switch (switch_keyboard-9) {
                case 0 :
                    curent_config.c_VCO3_LINK = (curent_config.c_VCO3_LINK == 0);
                    break;
                case 1 :
                    curent_config.c_VCO3_RANGE = 0;
                    break;
                case 2 :
                    curent_config.c_VCO3_RANGE = 1;
                    break;
                case 3 :
                    curent_config.c_VCO3_RANGE = 2;
                    break;
                default :
                    break;
                }
            }
        break;
        case MENU_VCF1 :
            if (switch_keyboard < 6) {
                curent_config.c_VCF1_TYPE = switch_keyboard;
            } else {
				if (switch_keyboard <= 8)
					curent_config.c_VCF1_pitch_TRACK = switch_keyboard - 6;
				else if ((switch_keyboard >= 11) && (l_last_switch_keyboard_bit < switch_keyboard_bit )) { // seulement si on press une touche (car on test l'appuie simultané de 2 touches)
					if ( ((switch_keyboard_bit>>11) & 0b11) == 0b11) { // kb11 et kb12 : les 2 derniere touches du clavier
						curent_config.c_VCF2_TYPE = 3;
						}
					else {
						if(curent_config.c_VCF2_TYPE == switch_keyboard - 11+1)
							curent_config.c_VCF2_TYPE = 0;
						else
							curent_config.c_VCF2_TYPE = switch_keyboard - 11+1;
						}
					}
			}
			break;
        case MENU_ADSR :
            switch (switch_keyboard) {
            case 0:
                curent_config.c_ADSR_LOOP = 0;
                break;
            case 1:
                curent_config.c_ADSR_LOOP = 1;
                break;
            case 2:
                curent_config.c_ADSR_LOOP = 2;
                break;
            case 3:
                curent_config.c_VCA_TYPE = 0;
                break;
            case 4:
                curent_config.c_VCA_TYPE = 1;
                break;
            case 5:
                curent_config.c_KEYBOARD_octave = -3;
                break;
            case 6:
                curent_config.c_KEYBOARD_octave = -2;
                break;
            case 7:
                curent_config.c_KEYBOARD_octave = -1;
                break;
            case 8:
                curent_config.c_KEYBOARD_octave = 0;
                break;
            case 9:
                curent_config.c_KEYBOARD_octave = 1;
                break;
            case 10:
                curent_config.c_KEYBOARD_octave = 2;
                break;
            case 11:
                curent_config.c_KEYBOARD_toggle = 0;
                break;
            case 12:
                curent_config.c_KEYBOARD_toggle = 1;
                break;
            }
        break;
        case MENU_LFO1 :
            if (switch_keyboard < 9) curent_config.c_LFO1_WF = switch_keyboard + 9 * change_time;
            else {
                switch_keyboard -= 9;
                curent_config.c_LFO1_RANGE = switch_keyboard;
            }
            break;
        case MENU_LFO2 :
            if (switch_keyboard < 9) curent_config.c_LFO2_WF = switch_keyboard + 9 * change_time;
            else {
                switch_keyboard -= 9;
                curent_config.c_LFO2_RANGE = switch_keyboard;
            }
            break;
        case MENU_LFO3 :
            if (switch_keyboard < 9) curent_config.c_LFO3_WF = switch_keyboard + 9 * change_time;
            else {
                switch_keyboard -= 9;
                curent_config.c_LFO3_RANGE = switch_keyboard;
            }
            break;
        case MENU_LFO4 :
            if (switch_keyboard > 0 ){
                if (switch_keyboard < 9) curent_config.c_LFO4_WF = switch_keyboard + 9 * change_time;
                else {
                    switch_keyboard -= 9;
                    if(curent_config.c_LFO4_RANGE == switch_keyboard) curent_config.c_LFO4_RANGE = 4;
                    else curent_config.c_LFO4_RANGE = switch_keyboard;
                }
            }
            break;
        case MENU_LFO5 :
            if (switch_keyboard > 0 ){
                if (switch_keyboard < 9) curent_config.c_LFO5_WF = switch_keyboard + 9 * change_time;
                else {
                    switch_keyboard -= 9;
                    if(curent_config.c_LFO5_RANGE = switch_keyboard) curent_config.c_LFO5_RANGE = 4;
                    else curent_config.c_LFO5_RANGE = switch_keyboard;
                 }
            }
            break;
        case MENU_LFO6 :
            if (switch_keyboard > 0 ){
                if (switch_keyboard < 9) curent_config.c_LFO6_WF = switch_keyboard + 9 * change_time;
                else {
                    switch_keyboard -= 9;
                    if(curent_config.c_LFO6_RANGE = switch_keyboard) curent_config.c_LFO6_RANGE = 4;
                    else curent_config.c_LFO6_RANGE = switch_keyboard;
                 }
            }
            break;
        case MENU_LFO7 :
            if(switch_keyboard >= 9) {
                switch_keyboard -= 9;
                curent_config.c_LFO7_RANGE = switch_keyboard;
            }
        case MENU_MIDI :
            if(curent_config.c_MIDI_channel == switch_keyboard) { // si on rapuie sur le cannal deja selectionné
                curent_config.c_MIDI_channel = -1;
            } else {
                curent_config.c_MIDI_channel = switch_keyboard;
            }
            break;
        case MENU_CV1 :
            break;
        case MENU_CV2 :
            break;
        case MENU_EFFECTS :
            if(switch_keyboard < 7) {
				if (curent_config.c_EFFECT1_TYPE != (uint32_t)switch_keyboard + 7 * change_time) {
					curent_config.c_EFFECT1_TYPE = 14;
					delay1_clear();
					g_effect1_phase = 0.f;
					g_effect1_last_out = 0.f;
					g_effect1_param_filter = 0.f;
					g_effect1_param_filter2 = 0.f;
					g_vitesse = 0.f;
					g_old_sound_out = 0.f;
					g_last_sound_in = 0.f;
					g_effect1_f1 = 0.f;
					g_effect1_f2 = 0.f;
					g_effect1_f3 = 0.f;
					g_effect1_f4 = 0.f;
					g_effect1_f1old = 0.f;
					g_effect1_f2old = 0.f;
					g_effect1_f3old = 0.f;
					g_effect1_f4old = 0.f;
					curent_config.c_EFFECT1_TYPE = switch_keyboard + 7 * change_time;
					}
				else curent_config.c_EFFECT1_TYPE = 14;
			}
            else {
                switch_keyboard -= 7;
				if (curent_config.c_EFFECT2_TYPE != (uint32_t)switch_keyboard + 6 * change_time) {
					curent_config.c_EFFECT2_TYPE = 12;
					g_Effect2_filtre = 0.f;
					g_effect2_sound_env = 0.f;
					g_effect2_phase = 0.33f;
					g_delay_effect2.Reset();
					g_delay_effect2b.Reset();
					curent_config.c_EFFECT2_TYPE = switch_keyboard + 6 * change_time;
					}
				else curent_config.c_EFFECT2_TYPE = 12;
            }
            break;
        case MENU_LOAD :
			load_config(switch_keyboard);
            g_last_load_save = switch_keyboard; // affichage
            break;
        case MENU_SAVE :
			save_config(switch_keyboard);
			g_last_load_save = switch_keyboard;
            break;
         case MENU_LOAD_SAVE :
			switch (switch_keyboard) {
				case 0: // load empty config
					empty_config();
					animation1_time = 30;
				break;
				case 1: // load standard config
					standard_config();
					animation2_time = 30;
				break;
				case 2: // load random config
					random_config();
					animation3_time = 30;
				break;
				case 3:
					g_affiche_version = 1;
				break;
				case 4:
				break;
				case 5:
				break;
				case 6:
				break;
				case 7:
				break;
				case 8:
				break;
				case 9:
				break;
				case 10:
				break;
				case 11:
				break;
				case 12:
					g_CV2KB = !g_CV2KB; // on toggle le bit
				break;
			}
			break;
        }
        return 2;
    }

	if (change_time) return 3; // si change_time, alors il n'y a qu'un cas qui nous interesse
	// si change_time on reste ssi modulation et configuration sont aussi appuyé
	// cad on sort si change_time et si (modulation et configuration) ne sont pas appuyé simultanement

	if( pressed_keyboard && change_modulation)
		return 3; // on appuie sur une touche de modulation alors que le clavier est appuyé : le clavier a la priorité : on joue des notes
	if( pressed_keyboard && change_configuration)
		return 3; // on appuie sur une touche de configuration alors que le clavier est appuyé : le clavier a la priorité : on joue des notes
	if( pressed_configuration && change_modulation)
		return 3; // on appuie sur une touche de modulation alors qu'une touche de configuration est appuyé : on ne fait rien : priorité a la configuration

    if (!pressed_keyboard && !pressed_modulation && !pressed_configuration) { // on a relaché toutes les touches
        g_last_load_save = -1; // pour l'affichage des derniere sauvegarde
        if (change_keyboard == 0) return 0 ; // Ce n'est pas le clavier, dc on a relaché une touche de configuration ou de modulation : on n'as rien a faire
        change_key(switch_keyboard_bit); // else : on a relaché une touche du clavier : on gere ca ailleur
        return 0 ;
    }
    if ( (pressed_keyboard + pressed_modulation + pressed_configuration) == 1 ) { // une seule touche : on a juste a afficher les leds, a moins que ce ne soit le clavier
        if (pressed_modulation) return 1;
        if (pressed_configuration) {
			if (change_configuration) { // action seulement a l'appuie de la touche
				if (g_CV2KB)
				{// octave + et - sur les touches CV1 et 2
					if (switch_configuration == MENU_CV1)   curent_config.c_KEYBOARD_octave = MIN(curent_config.c_KEYBOARD_octave+1, 2);
					if (switch_configuration == MENU_CV2)   curent_config.c_KEYBOARD_octave = MAX(curent_config.c_KEYBOARD_octave-1, -3);
				}
				if (switch_configuration == MENU_LOAD)   led_time = 0;
				if (switch_configuration == MENU_SAVE)   led_time = 0;
				if (switch_configuration == MENU_LOAD_SAVE) { animation1_time = 0; animation2_time = 0; animation3_time = 0; }
			}
			return 2; // on reste ds le menu tant que la touche est appuyé
		}

        // on as donc une touche du clavier seul
        if (change_keyboard == 0) return 0; // seul une touche de clavier est apuyé, mais c'est une touche de configuration ou de selection qui a changé : c'est un reste de la configuration que l'on viens de relacher : on ne la gere pas
        // defaut : c'est une touche du clavier qui a ete appuyé
        change_key(switch_keyboard_bit); // le clavier a changé, on gere ca ailleur
        return 0;
    }
    // 2 touches sont appuyés, il y a forcement une configuration qq part a changer
    if (pressed_modulation && pressed_keyboard) { // changement de paramettre de modulation
        switch(switch_modulation){
        case VCO1_MOD1 :
        case VCO1_MOD2 :
        case VCO1_MOD3 :
        case VCO2_MOD1 :
        case VCO2_MOD2 :
        case VCO2_MOD3 :
        case VCO3_MOD1 :
        case VCO3_MOD2 :
        case VCO3_MOD3 :
            if (switch_keyboard < 7) { // modulation type
                curent_config.c_Modulation_Type[switch_modulation] = switch_keyboard;
            } else { // alternate modulation in case of VCO_MOD
                uint_tmp = switch_keyboard - 7;
                uint_tmp2 = curent_config.c_Modulation_Source[switch_modulation]; // source de modulation actuel
                if(uint_tmp2 < LFO1_OUT) { // on as une modulation issue d'un VCO
                    uint_tmp2 -= uint_tmp2%6; // on enleve les formes d'ondes "speciales"
                    uint_tmp2 += uint_tmp; // on ajoute la config du clavier
                    curent_config.c_Modulation_Source[switch_modulation] = uint_tmp2;
                }
            }
            break;
        case VCF1_MOD1 :
            if (switch_keyboard == 9)
                curent_config.c_VCF1_MOD1_TYPE = 0;
            else  if (switch_keyboard == 10)
                curent_config.c_VCF1_MOD1_TYPE = 1;
            break;
        case VCF1_MOD2 :
            if (switch_keyboard == 9)
                curent_config.c_VCF1_MOD2_TYPE = 0;
            else  if (switch_keyboard == 10)
                curent_config.c_VCF1_MOD2_TYPE = 1;
            break;
        case LFO1_MOD :
            curent_config.c_Modulation_Type[LFO1_MOD] = switch_keyboard;
            break;
        case LFO2_MOD :
            curent_config.c_Modulation_Type[LFO2_MOD] = switch_keyboard;
            break;
        case LFO3_MOD :
            curent_config.c_Modulation_Type[LFO3_MOD] = switch_keyboard;
            break;
        }
        return 1;
    }
    return 0; // on ne devrait jamais arriver ici
}

void key_and_led(){ // test keyboard and update leds
    uint32_t led_mode = keyboard_all();
    if (led_mode == 3) led_mode = g_last_led_state;
    else g_last_led_state = led_mode;

    switch (led_mode) {
    case 0 :
        leds_mode_normal();
        leds_keyboard();
        break;
    case 1 :
        leds_key_modulation(g_switch_modulation);
        break;
    case 2 :
        leds_key_configuration(g_switch_configuration);
        leds_mode_normal();
        break;
    }
}

///////////////////////////////////////////////////////
// MIDI input
///////////////////////////////////////////////////////
void midi_in(uint32_t MIDI_data) {
    uint32_t uint_tmp;
    float tmpf;

  MIDI_data &= 0xFF; // on tronque au cas ou (???)

    if ((MIDI_data & 0xF0) == 0xF0) { // real time messages
        g_MIDI_led_time = 0.3f; // very small blink

        if (MIDI_data == 0xF8) { // real time clock
            if( ++g_MIDI_QNote == 24) { // on as eu 24 Quarter Note
                g_MIDI_LFO_increment = 0.25f/(float)g_MIDI_count;
                g_MIDI_QNote = 0;
                g_MIDI_count = 0;
             }
        }
        else if (MIDI_data == 0xFA)  { // play : reset phase LFO syncronisés
            if(curent_config.c_LFO1_RANGE == 3) { g_Modulation_Phase[LFO1_OUT] = 0; g_Modulation_Reset[LFO1_OUT] = 1; }
            if(curent_config.c_LFO2_RANGE == 3) { g_Modulation_Phase[LFO2_OUT] = 0; g_Modulation_Reset[LFO2_OUT] = 1; }
            if(curent_config.c_LFO3_RANGE == 3) { g_Modulation_Phase[LFO3_OUT] = 0; g_Modulation_Reset[LFO3_OUT] = 1; }
            if(curent_config.c_LFO4_RANGE == 3) { g_Modulation_Phase[LFO4_OUT] = 0; g_Modulation_Reset[LFO4_OUT] = 1; }
            if(curent_config.c_LFO5_RANGE == 3) { g_Modulation_Phase[LFO5_OUT] = 0; g_Modulation_Reset[LFO5_OUT] = 1; }
            if(curent_config.c_LFO6_RANGE == 3) { g_Modulation_Phase[LFO6_OUT] = 0; g_Modulation_Reset[LFO6_OUT] = 1; }
            if(curent_config.c_LFO7_RANGE == 3) { g_Modulation_Phase[LFO7_OUT] = 0; g_Modulation_Reset[LFO7_OUT] = 1; }
            g_MIDI_QNote = 0;
            g_MIDI_count = 0;
        }
    }
    else {
        g_MIDI_led_time = 1.f; // small blink

        if (MIDI_data & 0x80) { // test for status
            if (curent_config.c_MIDI_channel == -1) { // omni
                g_MIDI_status = MIDI_data & 0xF0;
                g_MIDI_state = 1; // on a un midi status
            } else { // on doit tester le channel
                if ( (MIDI_data & 0x0F) == (uint32_t)curent_config.c_MIDI_channel ) { // c'est le bon channel
                    g_MIDI_status = MIDI_data & 0xF0;
                    g_MIDI_state = 1; // on a un midi status
                } else { // ce n'est pas le bon channel
                    g_MIDI_state = 0; // on attend un status sur le bon channel
                }
            }
        }
        else { // data
            switch (g_MIDI_state) {
            case 0 :
                // data without any status : discard
                break;
            case 1:
                MIDI_data1 = MIDI_data;
                g_MIDI_state = 2;
                if ( g_MIDI_status == 0b11000000)  // progam change
                {
                    load_config((MIDI_data1<13)?MIDI_data1:13);
                }
                break;
            case 2:
                MIDI_data2 = MIDI_data;
                g_MIDI_state = 1; // go back waiting for data 1 (running status)

                // we have a full midi message
                if ( ( g_MIDI_status == 0x90) &&  (MIDI_data2 != 0) ) // note on
                {
                    add_voice(1, MIDI_data1-60);//, (float)MIDI_data2/127.);
                    //g_Modulation[MIDI_vel] = MIDI_data2;
                }
                if ( ( (g_MIDI_status == 0x90) &&  (MIDI_data2 == 0) ) || ( g_MIDI_status == 0x80 ) )
                 // note off
                {
                    remove_voice(1, MIDI_data1-60);
                }

                if ( g_MIDI_status == 0b11100000) // v_pitch bend
                {
                    g_MIDI_pitchWHEEL = 5.f * (-1.f + (float)(MIDI_data1 + (MIDI_data2 << 7)) * 0.00012207f); // pitch bend = +/- 5 1/2 tone
                }
                if ( g_MIDI_status == 0b10110000) // CC
                {
                    if (MIDI_data1 == 01) { // mod wheel
                        tmpf = (float)((MIDI_data2 << 7)+ g_MIDI_MODWHEEL_LSB);
                        tmpf = ( tmpf * 0.00006103515f); // 14 bit de 0 a 1
                        g_Modulation[MIDI_modulation] = tmpf;
                        g_Modulation[MIDI_modulation + modulation_source_last] = -tmpf;
                    }
                    if (MIDI_data1 == 11) {
						tmpf = (float)((MIDI_data2 << 7) + g_MIDI_exprssion_LSB) * 0.00006103515f;  // positif seulement
						g_Modulation[MIDI_expression] = tmpf;
						}
                    if (MIDI_data1 == 33) { g_MIDI_MODWHEEL_LSB = MIDI_data2; }  // mod wheel LSB
                    if (MIDI_data1 == 99) { g_RNPN_addresse_MSB = MIDI_data2; }
                    if (MIDI_data1 == 98) { g_RNPN_addresse_LSB = MIDI_data2; }
                    if (MIDI_data1 == 6 ) {
						g_RNPN_value_MSB = MIDI_data2;
						uint_tmp = g_RNPN_addresse_LSB + (g_RNPN_addresse_MSB << 7);
                        if (uint_tmp < nb_potentiometer) {
							uint_tmp = table_midi_order[uint_tmp];
                            g_midi_parameter[uint_tmp] = (g_RNPN_value_MSB << 7) * 0.00006103515f;
                        }
					}
                    if (MIDI_data1 == 38) {
						g_RNPN_value_LSB = MIDI_data2;
                        uint_tmp = g_RNPN_addresse_LSB + (g_RNPN_addresse_MSB << 7);
                        if (uint_tmp < nb_potentiometer) {
							uint_tmp = table_midi_order[uint_tmp];
                            g_midi_parameter[uint_tmp] = ( (g_RNPN_value_MSB << 7) + g_RNPN_value_LSB) * 0.00006103515f;
                        }
                    }
                    if (MIDI_data1 == 43) { g_MIDI_exprssion_LSB = MIDI_data2; }
                    // autre CC si necessaire
                }
                break;
            }
        }
    }
}

///////////////////////////////////////////////////////
// analogue input
///////////////////////////////////////////////////////

inline void compute_CV_in() {
	// les valeurs sont deja recupérer par get_pot()
    uint32_t analog_gate;
	float tmpf;
	tmpf = ((0.5f -  g_knob[k_CV1]) - g_CV1_offset) * 2.f ;
    g_Modulation[CV1_OUT] =  tmpf;
    g_Modulation[CV1_OUT+modulation_source_last] =  -tmpf;
    tmpf = ((0.5f - g_knob[k_CV2]) - g_CV2_offset) * 2.f;
    g_Modulation[CV2_OUT] = tmpf;
    g_Modulation[CV2_OUT+modulation_source_last] = -tmpf;

    analog_gate = dsy_gpio_read(&gate_pin);
    if (g_analog_gate > analog_gate) remove_voice(2, 0);
    if (g_analog_gate < analog_gate) add_voice(2, 0);
    g_analog_gate = analog_gate;
}

///////////////////////////////////////////////////////
// serial input
///////////////////////////////////////////////////////

inline void get_midi() {
	if(hw.midi.Readable()) // MIDI in
		midi_in(hw.midi.Rx()); // analyse les datas midi qu'on reçoit
	if(!hw.midi.RxActive()) { // In case of UART Error, (particularly overrun error), UART disables itself.
		hw.midi.FlushRx(); // Flush the buff,
		hw.midi.StartRx(); // and restart.
	}
}
