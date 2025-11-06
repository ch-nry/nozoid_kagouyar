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

// g_variables : variables globales (static)
// k_knob : knob number
// v_variable : variable associé a une voie de polyphonie (ds la structure allvoice[])
// c_variable : variable de configuration qui est ds la structure sauvegardé en EPROM

// this code use an old and sligtly customized daisy lib.

#define major_version 1
#define minor_version  7
#define software_led_version (major_version << 8) + minor_version // pour l'affichage de version en mode debug

// Les 3 led MSB servent a indiquer la présence d'une calibration des CVs
// les 2 suivant sont la version majeur
//les 8 suivant la version mineur

#define block_size 24
#define block_per_ms 2.f // (48/block_size) // combien de bock audio par ms, pour calcul du temps
#define filter_order 4 // pour les pot et CV in

#define nb_voice 4

#ifndef SAMPLE_RATE
#define SAMPLE_RATE 48014.f
#endif
#define OneOverSR 1.f/SAMPLE_RATE

#define memory_id 0xABCDEF04 // identifiant unique pour verifier l'integrité des memoires

#define nb_potentiometer 48
#define nb_CV 50
#define coef_CV_to_audio_filter 0.004f // pot update every 0.5ms, so 24 samples. 0.04 comme coef permettrait un lisage parfait. 0.004 implique un lowpass de 0.1 * la valeur a chaque block audio
#define coef_audio_to_block block_size

//#define PI_F 3.1415927410125732421875f
//#define TWOPI_F (2.0f * PI_F)
//#define HALFPI_F (PI_F * 0.5f)

#define int2float 4.656612873e-010f
//#define uint2float 2.32830643e-010f

// ordre des potentiomettres
#define k_VCO1_fq       		0
#define k_VCO1_wfm      	3
#define k_VCO1_mod1     	5
#define k_VCO1_mod2     	10
#define k_VCO1_mod3     	9
#define k_VCO2_fq       		1
#define k_VCO2_wfm      	6
#define k_VCO2_mod1     	7
#define k_VCO2_mod2     	12
#define k_VCO2_mod3     	8
#define k_VCO3_fq       		2
#define k_VCO3_wfm      	4
#define k_VCO3_mod1     	15
#define k_VCO3_mod2     	14
#define k_VCO3_mod3     	11
#define k_MIX1          			27
#define k_MIX2          			31
#define k_MIX3          			30
#define k_VCF1_fq       		24
#define k_VCF1_q        		25
#define k_VCF1_mod1     	26
#define k_VCF1_mod2     	28
#define k_ADSR_a        		19
#define k_ADSR_d        		17
#define k_ADSR_s        		18
#define k_ADSR_r        		20
#define k_LFO1_fq       		23
#define k_LFO1_mod      	21
#define k_LFO4_fq       		16
#define k_LFO2_fq       		36
#define k_LFO2_mod      	33
#define k_LFO5_fq       		22
#define k_LFO3_fq       		38
#define k_LFO3_mod      	39
#define k_LFO6_fq       		34
#define k_LFO7_fq       		37
#define k_LFO7_wf       		35
#define k_LFO7_sym      		32
#define k_EFFECT1_wet   	41
#define k_EFFECT1_p1    	40
#define k_EFFECT1_p2    	43
#define k_EFFECT2_wet   	42
#define k_EFFECT2_p1    	45
#define k_VCF2_fq       		46
#define k_VCF2_mod      	47
#define k_GAIN          			44
#define k_CV1           			48
#define k_CV2           			49

const float table_MIDI_fq[] = {1.f/32.f, 1.f/16.f, 1.f/8.f, 0.25f, 0.5f, 1.f, 2.f, 4.f, 8.f, 16.f, 32.f};
const int table_step[] = {1, 2, 3, 4, 8, 16};
const uint32_t table_midi_order[] = {k_VCO1_fq,  k_VCO1_wfm, k_VCO1_mod1, k_VCO1_mod2, k_VCO1_mod3, k_VCO2_fq, k_VCO2_wfm,
	 k_VCO2_mod1, k_VCO2_mod2, k_VCO2_mod3, k_VCO3_fq, k_VCO3_wfm, k_VCO3_mod1, k_VCO3_mod2, k_VCO3_mod3, k_MIX1,
	 k_MIX2, k_MIX3, k_VCF1_fq, k_VCF1_q, k_VCF1_mod1, k_VCF1_mod2, k_ADSR_a, k_ADSR_d, k_ADSR_s, k_ADSR_r, k_LFO1_fq,
	 k_LFO1_mod, k_LFO4_fq, k_LFO2_fq, k_LFO2_mod, k_LFO5_fq, k_LFO3_fq, k_LFO3_mod, k_LFO6_fq, k_LFO7_fq, k_LFO7_wf,
	 k_LFO7_sym, k_EFFECT1_wet, k_EFFECT1_p1, k_EFFECT1_p2, k_EFFECT2_wet, k_EFFECT2_p1, k_VCF2_fq, k_VCF2_mod, k_GAIN};

// potentiomettres:
volatile float g_pot_increment[nb_CV] __attribute__((aligned(4))) = {0.0f}; // utilisé pour le filtre IIR en audio
volatile float g_pot_audio[nb_CV] __attribute__((aligned(4))) = {0.0f}; // valeur des pots mais filtré en audio
volatile int32_t g_pot16[nb_CV] __attribute__((aligned(4))) = {0}; // filtre en 16 bit
volatile uint32_t g_filter_index[nb_CV] __attribute__((aligned(4))) = {0};
int32_t g_filter_moins[nb_CV][filter_order] = {0};
int32_t g_filter_plus[nb_CV][filter_order] = {0};

// MIDI
volatile float g_midi_parameter[nb_CV] = {0.0f};
volatile float g_knob[nb_CV] = {0.0f};
volatile float g_MIDI_LFO_increment = 0.0f;
volatile float g_MIDI_led_time = 0.0f;

// Random
volatile int32_t g_randomSeed_v;
volatile int32_t g_randomSeed_u;

// chaos :
#define nb_drunk_attractor 3*16 // 16 for LFO 1 to 3
volatile float g_drunk_lfo[nb_drunk_attractor];

// affichage de la sauvegarde des memoires sur le clavier
int32_t g_last_load_save  = -1;
// affichage des animations
uint32_t led_time = 0;

// sheduller :
volatile int32_t g_time = 0;

// MIDI
uint32_t g_MIDI_exprssion_LSB = 0, g_MIDI_MODWHEEL_LSB = 0, g_RNPN_value_MSB = 0, g_RNPN_value_LSB = 0, g_RNPN_addresse_MSB = 0, g_RNPN_addresse_LSB = 0;
float g_MIDI_pitchWHEEL = 0.f,  g_MIDI_MODWHEEL = 0.f;

//clavier
uint32_t g_state_kb = 7; // machine a etat de la fonction de reception du clavier
// 1er etat : on recalcule les valeur global du clavier

// calibration CV
float g_CV1_offset, g_CV2_offset, g_CV1_gain;

// fonctions speciale
uint32_t g_CV2KB = 0;

// g_Modulation
enum MOD_OUT {
    VCO1_OUT, VCO1_SIN, VCO1_TRI, VCO1_SQUARE, VCO1_RAMP, VCO1_SAW,
    VCO2_OUT, VCO2_SIN, VCO2_TRI, VCO2_SQUARE, VCO2_RAMP, VCO2_SAW,
    VCO3_OUT, VCO3_SIN, VCO3_TRI, VCO3_SQUARE, VCO3_RAMP, VCO3_SAW,
    ADSR_OUT,
    LFO1_OUT, LFO2_OUT, LFO3_OUT,
    LFO4_OUT, LFO5_OUT, LFO6_OUT,
    LFO7_OUT,
    MIDI_modulation, CV1_OUT, CV2_OUT, NONE_OUT, MIDI_expression,
    LFO1_OUT_FILTER, LFO2_OUT_FILTER, LFO3_OUT_FILTER,
    LFO4_OUT_FILTER, LFO5_OUT_FILTER, LFO6_OUT_FILTER,
	LFO7_OUT_FILTER,
    modulation_source_last
};

enum MOD_DESTINATION {
    VCO1_MOD1, VCO1_MOD2, VCO1_MOD3,
    VCO2_MOD1, VCO2_MOD2, VCO2_MOD3,
    VCO3_MOD1, VCO3_MOD2, VCO3_MOD3,
    VCF1_MOD1, VCF1_MOD2,
    LFO1_MOD, LFO2_MOD, LFO3_MOD,
    EFFECT1_MOD, EFFECT2_MOD,
    VCF2_MOD1,
    modulation_destination_last
};

enum drunk_attractor {
    drunk_LFO1, drunk_LFO2, drunk_LFO3
};

enum VCO_list {
	VCO1_Id, VCO2_Id, VCO3_Id
};

enum MOD_TYPE_VCO { MOD_FM_exp, MOD_FM_Qtz, MOD_FM_lin, MOD_AM, MOD_PM, MOD_CLIP, MOD_WF, modulation_type_last }; // VCO modulation
enum ADSR_MOD { Attack, Decay, Release, Release2};
enum LFO_WF { WF_AR, WF_sin, WF_tri, WF_square, WF_ramp, WF_saw, WF_spike, WF_step, WF_noise, WF_AR2, WF_sin2, WF_tri2, WF_square2, WF_ramp2, WF_saw2, WF_spike2, WF_step2, WF_noise2 };
enum MOD_TYPE_LFO { LFO_Mix, LFO_AM, LFO_FM, LFO_PM, LFO_CLIP, LFO_Fold, LFO_Xor, LFO_FILTER, LFO_RESET, LFO_SYNC, LFO_GATE, LFO_TH, LFO_RndLoop, LFO_nb_algo}; // LFO modulation
enum MENU_switch {MENU_VCO1, MENU_VCO2, MENU_VCO3, MENU_VCF1, MENU_ADSR, MENU_LFO1, MENU_LFO2, MENU_LFO3,
        MENU_LFO4, MENU_LFO5, MENU_LFO6, MENU_LFO7, MENU_MIDI, MENU_CV1, MENU_CV2, MENU_EFFECTS, MENU_LOAD, MENU_SAVE, MENU_LOAD_SAVE, MENU_OCTmoins, MENU_OCTplus };

struct CONFIGURATION
{
    // Memory structure c_Version
    uint32_t c_Version = memory_id;
    int32_t c_MIDI_channel = 0; // -1 pour omni

	// CV calibration
	float c_CV1_offset = 0.f;
	float c_CV2_offset = 0.f;
	float c_CV1_gain = 1.f;

    // VCO
    uint32_t c_VCO1_WF ;
    uint32_t c_VCO1_RANGE ;

    uint32_t c_VCO2_WF;
    uint32_t c_VCO2_RANGE ;
    uint32_t c_VCO2_LINK;

    uint32_t c_VCO3_WF;
    uint32_t c_VCO3_RANGE;
    uint32_t c_VCO3_LINK;

    // VCF
    uint32_t c_VCF1_TYPE ;
    uint32_t c_VCF1_MOD1_TYPE;
    uint32_t c_VCF1_MOD2_TYPE;
    uint32_t c_VCF1_pitch_TRACK;

    // ADSR // keyboard
    uint32_t c_ADSR_LOOP; // 0 : no loop, 1: AD loop, 2 : ADSR loop
    int32_t  c_KEYBOARD_octave;
    uint32_t c_KEYBOARD_toggle;

    // VCA
    uint32_t c_VCA_TYPE; // VCA or LowPassv_GATE

    // LFO1, 2 and 3
    uint32_t c_LFO1_RANGE;
    uint32_t c_LFO1_WF;

    uint32_t c_LFO2_RANGE;
    uint32_t c_LFO2_WF;

    uint32_t c_LFO3_RANGE;
    uint32_t c_LFO3_WF;

    // LFO4, 3 and 4
    uint32_t c_LFO4_RANGE;
    uint32_t c_LFO4_WF;

    uint32_t c_LFO5_RANGE;
    uint32_t c_LFO5_WF;

    uint32_t c_LFO6_RANGE;
    uint32_t c_LFO6_WF;

    // LFO7
    uint32_t c_LFO7_RANGE;

    // EFFECTS
    uint32_t c_EFFECT1_TYPE;
    uint32_t c_EFFECT2_TYPE;
    int32_t c_VCF2_TYPE;

    // modulation
    uint32_t c_Modulation_Source[modulation_destination_last]; // source des g_Modulation des diferents potentiomettres
    uint32_t c_Modulation_Type[modulation_destination_last];    // type de g_Modulation pour chaques g_Modulation des VCO et des LFO

    //fonction special
	uint32_t c_CV2KB;
};

// configuration actuel
volatile CONFIGURATION curent_config; // configuration actuel

// modulations, LFO
volatile float g_Modulation[2*modulation_source_last]; // valeur des diferentes g_Modulation
volatile uint32_t g_Modulation_Reset[modulation_source_last]; // bool pour marquer un reset de phase, utilisé pour les lfo 1 2 3 ds certain cas.
volatile float g_Modulation_Phase[modulation_source_last]; // valeur des phases des diferentes g_Modulation

enum GATE_source {gate_KB_int, gate_MIDI, gate_5V};

struct voice
{
// general
    int32_t  v_priority;
        // 1 : prochaine voie a utiliser
        // 2 : la voie suivante, etc
        // -1 : on vient de l'utiliser
        // -2 : celle qui a ete utilisé juste avant, etc
    uint32_t v_GATE_source; // 0 = Kb int, 1 = MIDI, 2 = v_GATE logic
    volatile float v_pitch;
    volatile uint32_t v_GATE; // GATE on/off
    //float  v_velocity;

    // VCO*
    volatile float v_VCO_phase[3] = {0};
    float v_VCO_last[3][8] = {0};
    volatile float v_VCO1_pitch = 0.f;

// MIX
// Low Pass v_GATE
    float v_LPG_last = 0.f;
// VCF1
	float v_VCF1_filter = 0.f;
    volatile float v_VCF1_last_input1  = 0.f;
    volatile float v_VCF1_last_input2  = 0.f;
    volatile float v_VCF1_last_input3  = 0.f;
    volatile float v_VCF1_last_input4  = 0.f;
    volatile float v_VCF1_last_output1 = 0.f;
    volatile float v_VCF1_last_output2 = 0.f;
    volatile float v_VCF1_last_output3 = 0.f;
    volatile float v_VCF1_last_output4 = 0.f;
// ADSR
    volatile uint32_t v_ADSR_mode = Release;
    float v_ADSR_out = 0.f;
};

voice allvoice[4]; // declaration des memoires des voies de polyphonie
