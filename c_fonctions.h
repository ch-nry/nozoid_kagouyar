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

void delay1_clear();

float table_CV2freq[269];

////////////////////////////////////////////////////
// from daisysp
/* one pole lpf
out is passed by reference, and must be retained between
calls to properly filter the signal
coeff can be calculated:
coeff = 1.0 / (time * sample_rate) ; where time is in seconds
i.e : f/fs
*/
inline void _fonepole(float &out, float in, float coeff) {
    out += coeff * (in - out);
}

/** efficient floating point min/max
c/o stephen mccaul
*/

inline float _fmax(float a, float b)
{
    float r;
#ifdef __arm__
    asm("vmaxnm.f32 %[d], %[n], %[m]" : [d] "=t"(r) : [n] "t"(a), [m] "t"(b) :);
#else
    r = (a > b) ? a : b;
#endif // __arm__
    return r;
}

inline float _fmin(float a, float b)
{
    float r;
#ifdef __arm__
    asm("vminnm.f32 %[d], %[n], %[m]" : [d] "=t"(r) : [n] "t"(a), [m] "t"(b) :);
#else
    r = (a < b) ? a : b;
#endif // __arm__
    return r;
}

// quick clamp
inline float _fclamp(float in, float min, float max) {
    return _fmin(_fmax(in, min), max);
}

inline float mix(float in1, float in2, float mix){ // mix from 0 to 1
  return(in1 + (in2-in1) * mix); // return in1 when mix=0, and in2 when mix= 1
}

inline uint32_t _rnd_ui() {
    uint32_t v = g_randomSeed_v;
    uint32_t u = g_randomSeed_u;

    v = 36969*(v & 65535) + (v >> 16);
    u = 18000*(u & 65535) + (u >> 16);

    g_randomSeed_v = v;
    g_randomSeed_u = u;

    return (v << 16) + (u & 65535);
}

inline int32_t _rnd_i() {
        return (int32_t)_rnd_ui();
}

inline float _rnd_f() { // from 0 to 1
        return _rnd_ui() * 2.3283064e-010f;
}

inline int _floor(float x) {
    return (int) x - (x < (int) x);
}

inline float wrap(float x) {
  return x - (int)x;
}

inline float _tanh(float x) {
  float x2 = x*x;
  return x * ( 27.f + x2 ) / ( 27.f + 9.f * x2 );
}
// a tester : x(abs(x*x*x)+16)/(16+4*abs(x*x*x)) : plus proche de X sur 0...0.5, et sature pour x=2

inline float _tanh_clip(float index){
    return _tanh(  _fclamp(index,-3.f, 3.f) );
}

inline float _cos(float index) { // index from 0 to 1 only
// 6 multiplications
  float x=index-0.5;
  float x2=x*x;

  return -0.99999944f + x2 * (19.73903275f + x2 * (-64.93054874f + x2 * (85.29509341f + x2 * (-58.90779707f + x2 * 21.27414825f))));
}

inline float _cos_positiv_loop(float index) { // positive index only
    return _cos(wrap(index));
}

inline float _cos_loop(float index) { //
    return _cos(index - _floor(index) );
}

inline float _sin(float index) { // index from 0 to 1 only
// 6 multiplication
  float x=index-0.5;
  float x2=x*x;

  return x * (-6.28308759f +x2*(41.33318714f + x2*(-81.39900318f + x2*(74.66885164f - x2*33.1532588f))));
}

inline float _sin_positiv_loop(float index) { // positive index only
    return _sin(wrap(index));
}

inline float _sin_loop(float index) { //
    return _sin(index - _floor(index) );
}
inline float sign(float in) {
	return ((in<0.f)?-1.f : 1.f);
}
void thomas(uint32_t i, float dt, float b) { // numero de l'attracteur a calculer
    float dx = _sin_positiv_loop(11.f + g_thomasY[i]) - b * g_thomasX[i]; // 11 : pour etre sur que le cos est positif
    float dy = _sin_positiv_loop(11.f + g_thomasZ[i]) - b * g_thomasY[i];
    float dz = _sin_positiv_loop(11.f + g_thomasX[i]) - b * g_thomasZ[i];
    g_thomasX[i] += dx * dt;
    g_thomasY[i] += dy * dt;
    g_thomasZ[i] += dz * dt;
}

#define  write_binary_led(data) hw.binary_led.Write_data(data, 24)

void init_table_CV2freq() {
  int i;
  double power;
  power = pow(2.0,1/12.0);
  for (i=0; i<268; i++) {
    table_CV2freq[i] = pow(power, i-(128+69)) * 440.;
  }
}

inline float CV2freq(float index) { // index from -128 to 139; 69 for 440Hz
  float f_index = index;
  f_index += 128.f;
  f_index = _fclamp(f_index, 0.f, 267.f );
  float index_entier = (int)(f_index);
  uint32_t i_index = (uint32_t)index_entier;
  float index_reste = f_index-index_entier;
  float inc1 = table_CV2freq[i_index];
  return inc1 * (1.f + .0577622650f * index_reste) ;
}

inline float CV2increment_lfo(uint32_t range, float cv) {
    switch (range) { // Frequence des LFO = SR/2
    case 0: // low
        return  CV2freq((92.f * cv) -115.f)*4.f*OneOverSR; // 92x -115 : (0.01 a 2)
    case 1: // med
        return  CV2freq((80.f * cv) -64.f)*4.f*OneOverSR; //  80x -64 (0.2 to 20)
    case 2: // high
        return  CV2freq((104.f * cv) -24.f)*4.f*OneOverSR; // 104x -24 (2 to 800)
    case 3: // sync midi
        return table_MIDI_fq[(int)(cv*10.999f)] * g_MIDI_LFO_increment;
    }
    return  0; // never used
}

inline float VCO_CV_range(uint32_t range, float cv) {
    switch (range) {
    case 0:
        return (127.f * cv) - 70.f;
    case 1:
        return (14.04f * cv) + 52.98f;
    case 2:
        return (100.f * cv);
    }
    return  0.f;
}

float LFO_compute_WF(float phase, uint32_t WF, float *last, uint32_t reset) {
    switch (WF) {
    case WF_AR:
        return 0.f; // not used
    case WF_sin:
        return _sin(phase);
    case WF_tri:
        return 1.f - 2.f*(fabs(2.f*wrap(phase+0.25f) - 1.f));
    case WF_square:
        return (phase < 0.5f)? 1.f:-1.f;
    case WF_ramp:
        return (phase + phase) - 1.f;
    case WF_saw:
        return 1.f - (phase + phase);
    case WF_spike:
        {
            float tmpf;
            tmpf = fabs(2.f*wrap(phase+0.275f) - 1.f);
            tmpf *= tmpf;
            tmpf *= tmpf;
            return 2.f*tmpf -1.f;
        }
    case WF_step:
        if ( reset ) {
            last[0] = (_rnd_f()-0.5f)*2.f;
        }
        return last[0];
    case WF_noise:
        if ( reset ) {
            last[1] = last[0];
            last[0] = (_rnd_f()-0.5f)*2.f;
        }
        return mix(last[1], last[0], phase*phase*(3.f-2.f*phase));
    }
    return(0.f);
}

float Interpolation_Curve(float phase, uint32_t WF, float *last_point) { // interpol through diferent points depending on the LFO waveform
    float out;

    switch (WF) {
    case WF_AR:
        return 0.f; // should never came here
    case WF_sin:
        out = 0.5f + _cos(phase*0.5f) * 0.5f;
        out *= last_point[1] - last_point[0];
        out += last_point[0];
        return out;
    case WF_tri:
        out = phase;
        out *= last_point[0] - last_point[1];
        out += last_point[1];
        return out;
    case WF_square:
        return (phase > 0.5f)? last_point[0]:last_point[1];
    case WF_ramp:
        return phase * last_point[0];
    case WF_saw:
        return (1.f-phase) * last_point[0];
    case WF_spike:
        out = phase;
        out *= phase;
        out *= out;
        out *= last_point[0] - last_point[1];
        out += last_point[1];
        return out;
    case WF_step:
        return last_point[0];
    case WF_noise: // interpolation cos avec 2 points
        return mix(last_point[1], last_point[0], phase*phase*(3.f-2.f*phase));
    }
    return(0.f); // not used
}

// for VCO band limited waveform

inline float Polyblep2(float increment, float phase) {
    float t = phase;
    if(t < increment) {
        t /= increment;
        return t * (2.f - t) - 1.0f; // -t² + 2t -1
    }
    else if(t - 1.f > - increment) {
        t = (t - 1.0f) / increment;
        return t * (t + 2.f) + 1.0f; // (t-1)(t+1)+1 = t²
    }
    else {
        return 0.0f;
    }
}

inline float Polyblep(float increment, float _phase)
{
    float phase = _phase;

    if (phase > 0.5) {
        phase = 1.f-phase;

        if (phase > 2.f * increment) return 0.0f;
        if (phase < increment) {
            phase /= increment;
            return 1.f + phase * ( (-4.f/3.f) + phase * phase * ((2.f/3.f) + phase * (-2.f/8.f)));
        }
        else {
            phase -= increment;
            phase /= increment;
            return (2.f/24.f) + phase * ((-2.f/6.f) + phase * ( (2.f/4.f) + phase * ((-2.f/6.f) + phase * (2.f/24.f))));
        }
    }
    else {
        if (phase > 2.f * increment) return 0.0f;
        if (phase < increment) {
            phase /= increment;
            return -1.f + phase * ( (4.f/3.f) + phase * phase * ((-2.f/3.f) + phase * (2.f/8.f)));
        }
        else {
            phase -= increment;
            phase /= increment;
            return (-2.f/24.f) + phase * ((2.f/6.f) + phase * ( (-2.f/4.f) + phase * ((2.f/6.f) + phase * (-2.f/24.f))));
        }
    }
}

inline float saw_bl(float phase, float increment) {
  return (phase+phase - 1.f - Polyblep(increment, phase));
}

inline float tri_bl(float phase, float increment, float &last_out) {
	float out;
	out = phase < 0.5 ? 1.0f : -1.0f;
	out += Polyblep2(increment, phase);
	out -= Polyblep2(increment, wrap(phase + 0.5f));
	// Leaky Integrator:
	// y[n] = A + x[n] + (1 - A) * y[n-1]
	out       = increment * out + (1.0f - increment) * last_out;
	last_out = out; 
    return 4.f*out;
}

void init_variables() {
    uint32_t i,j;
  	uint32_t tmp;

    for (i=0; i<modulation_source_last; i++) g_Modulation[i] = 0.;
    for (i=0; i<nb_thomas_attractor; i++) {
        g_thomasX[i] = 3.f * _rnd_f();
        g_thomasY[i] = 3.f * _rnd_f();
        g_thomasZ[i] = 3.f * _rnd_f();
        for (j=0; j<100; j++) thomas(i, 0.5, 0.3);
    }
    for (i=0; i<nb_voice; i++) { // pour les oscilateur logistic
        allvoice[i].v_VCO1_last[1] = _rnd_f();
    }

	do {tmp = hw.knobs_[k_CV1].Process_ch();} // on sort de l'initialisation, on attend d'avoir une valeur
	while (tmp == 0 );
	g_randomSeed_u = (tmp<<15)+tmp; // on compte sur le bruit de fond pour generer une seed aleatoire
	
	do {tmp = hw.knobs_[k_CV2].Process_ch();} // on sort de l'initialisation, on attend d'avoir une valeur
	while (tmp == 0 );
	g_randomSeed_v = (tmp<<15)+tmp; // idem
	    
    // mtof table initialisation in RAM
    init_table_CV2freq();

    // keyboard
    g_state_kb = 7; // force le recalcul des boutons etc
	
    //MIDI
    for (i=0; i<nb_potentiometer; i++) g_midi_parameter[i] = 0.;

    // Init des voies de polyphonie
    for (int i=0; i<nb_voice; i++) {
        allvoice[i].v_priority = i+1; // la voie 1 est la plus prioritaire apres l'initialisation
        allvoice[i].v_GATE_source = -1;
        allvoice[i].v_GATE = 0;
    }

    g_Modulation[NONE_OUT] = 0.f;
    g_Modulation_Phase[NONE_OUT] = 0.f;
    g_Modulation_Reset[NONE_OUT] = 0;
}

void random_config() {	
    curent_config.c_VCO1_RANGE = 2;
    curent_config.c_VCO2_RANGE = 2;
    curent_config.c_VCO2_LINK = 0;
    curent_config.c_VCO3_RANGE = 2;
    curent_config.c_VCO3_LINK = 0;
    curent_config.c_VCF1_TYPE = 0;
    curent_config.c_VCF1_MOD1_TYPE = _rnd_ui()%2;
    curent_config.c_VCF1_MOD2_TYPE = _rnd_ui()%2;
    curent_config.c_VCF1_pitch_TRACK = 2;
    curent_config.c_ADSR_LOOP = 0;
    curent_config.c_VCA_TYPE = _rnd_ui()%2; // VCA or LowPasse_GATE
    curent_config.c_VCF2_TYPE = 0;

	curent_config.c_VCO1_WF = _rnd_ui()%9;
    curent_config.c_VCO2_WF =  _rnd_ui()%9;
    curent_config.c_VCO3_WF =  _rnd_ui()%9;
    
    curent_config.c_LFO1_WF = 1 + _rnd_ui()%8;
    curent_config.c_LFO2_WF = 1 + _rnd_ui()%8;
    curent_config.c_LFO3_WF = 1 + _rnd_ui()%8;
    curent_config.c_LFO4_WF = 1 + _rnd_ui()%8;
    curent_config.c_LFO5_WF = 1 + _rnd_ui()%8;
    curent_config.c_LFO6_WF = 1 + _rnd_ui()%8;

    curent_config.c_LFO1_RANGE = _rnd_ui()%3;
    curent_config.c_LFO2_RANGE = _rnd_ui()%3;
    curent_config.c_LFO3_RANGE = _rnd_ui()%3;
    curent_config.c_LFO4_RANGE = _rnd_ui()%3;
    curent_config.c_LFO5_RANGE = _rnd_ui()%3;
    curent_config.c_LFO6_RANGE = _rnd_ui()%3;
    curent_config.c_LFO7_RANGE = _rnd_ui()%3;

	curent_config.c_EFFECT1_TYPE = 7;
	delay1_clear();
    curent_config.c_EFFECT1_TYPE = _rnd_ui()%7; 
    curent_config.c_EFFECT2_TYPE = _rnd_ui()%6;

    for (uint32_t i=0; i<VCF1_MOD1; i++) { // pour tout les VCO
        curent_config.c_Modulation_Source[i] = _rnd_ui()%MIDI_modulation;
        curent_config.c_Modulation_Type[i] = _rnd_ui()%modulation_type_last;
    }

    for (uint32_t i=VCF1_MOD1; i<modulation_destination_last; i++) { // pour toutes les autres modulation, on ne module pas avec les VCO
        curent_config.c_Modulation_Source[i] = ADSR_OUT + _rnd_ui()%(MIDI_modulation-ADSR_OUT);
    }

    for (uint32_t i=LFO1_MOD; i<EFFECT1_MOD; i++) {
        curent_config.c_Modulation_Type[i] = _rnd_ui()%LFO_nb_algo;
    }
}

void empty_config() {	     
	curent_config.c_Version = memory_id;
	curent_config.c_MIDI_channel = -1; 

    curent_config.c_VCO1_WF = 0;
    curent_config.c_VCO1_RANGE = 2;

    curent_config.c_VCO2_WF = 0;
    curent_config.c_VCO2_RANGE = 2;
    curent_config.c_VCO2_LINK = 0;

    curent_config.c_VCO3_WF = 0;
    curent_config.c_VCO3_RANGE = 2;
    curent_config.c_VCO3_LINK = 0;

    curent_config.c_VCF1_TYPE = 0;
    curent_config.c_VCF1_MOD1_TYPE = 0;
    curent_config.c_VCF1_MOD2_TYPE = 0;
    curent_config.c_VCF1_pitch_TRACK = 2;

    curent_config.c_ADSR_LOOP = 0; // 0 : no loop, 1: AD loop, 2 : ADSR loop
    curent_config.c_KEYBOARD_octave = 0;
    curent_config.c_KEYBOARD_toggle = 0;

    curent_config.c_VCA_TYPE = 0; // VCA or LowPasse_GATE

    curent_config.c_LFO1_RANGE = 1;
    curent_config.c_LFO1_WF = WF_sin;

    curent_config.c_LFO2_RANGE = 1;
    curent_config.c_LFO2_WF = WF_sin;

    curent_config.c_LFO3_RANGE = 1;
    curent_config.c_LFO3_WF = WF_sin;

    // LFO4, 3 and 4
    curent_config.c_LFO4_RANGE = 1;
    curent_config.c_LFO4_WF = WF_sin;

    curent_config.c_LFO5_RANGE = 1;
    curent_config.c_LFO5_WF = WF_sin;

    curent_config.c_LFO6_RANGE = 1;
    curent_config.c_LFO6_WF = WF_sin;

    // LFO7
    curent_config.c_LFO7_RANGE = 1;

    curent_config.c_EFFECT1_TYPE = 0;
    curent_config.c_EFFECT2_TYPE = 0;
    curent_config.c_VCF2_TYPE = 0;

    for (uint32_t i=0; i<modulation_destination_last; i++) { // pour tout les VCO
        curent_config.c_Modulation_Source[i] = NONE_OUT;
        curent_config.c_Modulation_Type[i] = MOD_FM_exp;
    }
     curent_config.c_Modulation_Source[VCF1_MOD1] = NONE_OUT;
     curent_config.c_Modulation_Source[VCF1_MOD2] = NONE_OUT;
     curent_config.c_Modulation_Source[VCF2_MOD1] = NONE_OUT;

    for (uint32_t i=LFO1_MOD; i<EFFECT1_MOD; i++) {
        curent_config.c_Modulation_Type[i] = _rnd_ui()%LFO_nb_algo;
    }
    
    curent_config.c_Modulation_Source[LFO1_MOD] = LFO1_OUT;
    curent_config.c_Modulation_Source[LFO2_MOD] = LFO2_OUT;
    curent_config.c_Modulation_Source[LFO3_MOD] = LFO3_OUT;
    curent_config.c_Modulation_Type[LFO1_MOD] = LFO_Mix;
    curent_config.c_Modulation_Type[LFO2_MOD] = LFO_Mix;
    curent_config.c_Modulation_Type[LFO3_MOD] = LFO_Mix;
}

void standard_config() {
    curent_config.c_Version = memory_id;
	curent_config.c_MIDI_channel = -1; 

    curent_config.c_VCO1_WF = 4;
    curent_config.c_VCO1_RANGE = 2;

    curent_config.c_VCO2_WF = 3;
    curent_config.c_VCO2_RANGE = 2;
    curent_config.c_VCO2_LINK = 0;

    curent_config.c_VCO3_WF = 0;
    curent_config.c_VCO3_RANGE = 2;
    curent_config.c_VCO3_LINK = 0;

    curent_config.c_VCF1_TYPE = 0;
    curent_config.c_VCF1_MOD1_TYPE = 0;
    curent_config.c_VCF1_MOD2_TYPE = 0;
    curent_config.c_VCF1_pitch_TRACK = 2;

    curent_config.c_ADSR_LOOP = 0; // 0 : no loop, 1: AD loop, 2 : ADSR loop
    curent_config.c_KEYBOARD_octave = 0;
    curent_config.c_KEYBOARD_toggle = 1;

    curent_config.c_VCA_TYPE = 0; // VCA or LowPassv_GATE

    curent_config.c_LFO1_RANGE = 0;
    curent_config.c_LFO1_WF = WF_sin;

    curent_config.c_LFO2_RANGE = 0;
    curent_config.c_LFO2_WF = WF_square;

    curent_config.c_LFO3_RANGE = 0;
    curent_config.c_LFO3_WF = WF_ramp;

    // LFO4, 3 and 4
    curent_config.c_LFO4_RANGE = 0;
    curent_config.c_LFO4_WF = WF_saw;

    curent_config.c_LFO5_RANGE = 0;
    curent_config.c_LFO5_WF = WF_spike;

    curent_config.c_LFO6_RANGE = 0;
    curent_config.c_LFO6_WF = WF_step;

    // LFO7
    curent_config.c_LFO7_RANGE = 2;

    //
    curent_config.c_EFFECT1_TYPE = 7;
	delay1_clear();
    curent_config.c_EFFECT1_TYPE = 0;
    curent_config.c_EFFECT2_TYPE = 0;
    curent_config.c_VCF2_TYPE = 0;

    curent_config.c_Modulation_Source[VCO1_MOD1] = LFO1_OUT;
    curent_config.c_Modulation_Source[VCO1_MOD2] = LFO2_OUT;
    curent_config.c_Modulation_Source[VCO1_MOD3] = LFO3_OUT;
    curent_config.c_Modulation_Type[VCO1_MOD1] = MOD_FM_exp;
    curent_config.c_Modulation_Type[VCO1_MOD2] = MOD_FM_exp;
    curent_config.c_Modulation_Type[VCO1_MOD3] = MOD_FM_exp;

    curent_config.c_Modulation_Source[VCO2_MOD1] = LFO1_OUT;
    curent_config.c_Modulation_Source[VCO2_MOD2] = LFO2_OUT;
    curent_config.c_Modulation_Source[VCO2_MOD3] = LFO3_OUT;
    curent_config.c_Modulation_Type[VCO2_MOD1] = MOD_FM_exp;
    curent_config.c_Modulation_Type[VCO2_MOD2] = MOD_FM_exp;
    curent_config.c_Modulation_Type[VCO2_MOD3] = MOD_FM_exp;

    curent_config.c_Modulation_Source[VCO3_MOD1] = LFO1_OUT;
    curent_config.c_Modulation_Source[VCO3_MOD2] = LFO2_OUT;
    curent_config.c_Modulation_Source[VCO3_MOD3] = LFO3_OUT;
    curent_config.c_Modulation_Type[VCO3_MOD1] = MOD_FM_exp;
    curent_config.c_Modulation_Type[VCO3_MOD2] = MOD_FM_exp;
    curent_config.c_Modulation_Type[VCO3_MOD3] = MOD_FM_exp;

    curent_config.c_Modulation_Source[VCF1_MOD1] = LFO1_OUT;
    curent_config.c_Modulation_Source[VCF1_MOD1] = LFO7_OUT;

    curent_config.c_Modulation_Source[LFO1_MOD] = LFO4_OUT;
    curent_config.c_Modulation_Source[LFO2_MOD] = LFO5_OUT;
    curent_config.c_Modulation_Source[LFO3_MOD] = LFO6_OUT;
    curent_config.c_Modulation_Type[LFO1_MOD] = LFO_Mix;
    curent_config.c_Modulation_Type[LFO2_MOD] = LFO_AM;
    curent_config.c_Modulation_Type[LFO3_MOD] = LFO_FM;

    curent_config.c_Modulation_Source[EFFECT1_MOD] = LFO4_OUT;
    curent_config.c_Modulation_Source[EFFECT2_MOD] = LFO5_OUT;
    curent_config.c_Modulation_Source[VCF2_MOD1] = LFO6_OUT;
}

void save_config(uint32_t slot) {
    uint32_t base = 0x90000000;
    base += slot*4096; // works only because sizeof(CONFIGURATION) < 4096
    hw.seed.qspi.Erase(base, base + sizeof(CONFIGURATION));
    hw.seed.qspi.Write(base, sizeof(CONFIGURATION), (uint8_t*)&curent_config);
}

int load_config(uint32_t slot)
{
	CONFIGURATION tmp_config; 
	
    memcpy(&tmp_config, reinterpret_cast<void*>(0x90000000 + (slot * 4096)), sizeof(CONFIGURATION));
    if (tmp_config.c_Version != memory_id) {
		standard_config(); // la structure memoire n'est pas bonne, la memoire est vide, on charge une config par defaut
		if (slot == 13) { // uniquement utilisé pour la calibration des CV
			curent_config.c_CV1_offset = 0.f;
			curent_config.c_CV2_offset = 0.f;
			curent_config.c_CV1_gain = 1.f;
		}
		return(0); //id not valid
	}
	else { // la memoire est valide, mais on met les elements un par un afin de rester ds le bon range, 
		//pour d'etre sur qu'on a des valeurs correct meme si il y a eu des erreurs de lecture/ecriture
	uint32_t i;
	if (slot == 13) { // uniquement utilisé pour la calibration des CV
		curent_config.c_CV1_offset = tmp_config.c_CV1_offset;
		curent_config.c_CV2_offset = tmp_config.c_CV2_offset;
		curent_config.c_CV1_gain = tmp_config.c_CV1_gain;
	}
    curent_config.c_MIDI_channel = _fmin(12, _fmax(-1, tmp_config.c_MIDI_channel));
    curent_config.c_VCO1_WF = tmp_config.c_VCO1_WF%9;
    curent_config.c_VCO1_RANGE = tmp_config.c_VCO1_RANGE%3;
    curent_config.c_VCO2_WF = tmp_config.c_VCO1_WF%9;
    curent_config.c_VCO2_RANGE = tmp_config.c_VCO1_RANGE%3;
    curent_config.c_VCO3_WF = tmp_config.c_VCO1_WF%9;
    curent_config.c_VCO3_RANGE = tmp_config.c_VCO1_RANGE%3;
    curent_config.c_VCO2_LINK = tmp_config.c_VCO2_LINK%2;
    curent_config.c_VCO3_LINK = tmp_config.c_VCO3_LINK%2;
    curent_config.c_VCF1_TYPE = tmp_config.c_VCF1_TYPE%6;
    curent_config.c_VCF1_MOD1_TYPE = tmp_config.c_VCF1_MOD1_TYPE%2;
    curent_config.c_VCF1_MOD2_TYPE = tmp_config.c_VCF1_MOD2_TYPE%2;
    curent_config.c_VCF1_pitch_TRACK = tmp_config.c_VCF1_pitch_TRACK%3;
    curent_config.c_ADSR_LOOP = tmp_config.c_ADSR_LOOP%3;
    curent_config.c_KEYBOARD_octave = _fmin(2, _fmax(-3,tmp_config.c_KEYBOARD_octave));
    curent_config.c_KEYBOARD_toggle = tmp_config.c_KEYBOARD_toggle%2;
    curent_config.c_VCA_TYPE = tmp_config.c_VCA_TYPE%2;
    curent_config.c_LFO1_RANGE = tmp_config.c_LFO1_RANGE%4;
    curent_config.c_LFO2_RANGE = tmp_config.c_LFO2_RANGE%4;
    curent_config.c_LFO3_RANGE = tmp_config.c_LFO3_RANGE%4;
    curent_config.c_LFO4_RANGE = tmp_config.c_LFO4_RANGE%4;
    curent_config.c_LFO5_RANGE = tmp_config.c_LFO5_RANGE%4;
    curent_config.c_LFO6_RANGE = tmp_config.c_LFO6_RANGE%4;
    curent_config.c_LFO7_RANGE = tmp_config.c_LFO7_RANGE%4;
    curent_config.c_LFO1_WF = tmp_config.c_LFO1_WF%9;
    curent_config.c_LFO2_WF = tmp_config.c_LFO2_WF%9;
    curent_config.c_LFO3_WF = tmp_config.c_LFO3_WF%9;
    curent_config.c_LFO4_WF = ((tmp_config.c_LFO4_WF-1)%8)+1;
    curent_config.c_LFO5_WF = ((tmp_config.c_LFO5_WF-1)%8)+1;
    curent_config.c_LFO6_WF = ((tmp_config.c_LFO6_WF-1)%8)+1;
    curent_config.c_EFFECT1_TYPE = 7;
	delay1_clear();
    curent_config.c_EFFECT1_TYPE = tmp_config.c_EFFECT1_TYPE%7;
    curent_config.c_EFFECT2_TYPE = tmp_config.c_EFFECT2_TYPE%6;
    curent_config.c_VCF2_TYPE = tmp_config.c_VCF2_TYPE%2;
	for(i=0; i<modulation_destination_last; i++) curent_config.c_Modulation_Source[i] = tmp_config.c_Modulation_Source[i]%(2*modulation_source_last);
	for(i=0; i<10; i++) curent_config.c_Modulation_Type[i] = tmp_config.c_Modulation_Type[i]%(modulation_type_last);
	curent_config.c_Modulation_Type[LFO1_MOD] = tmp_config.c_Modulation_Type[LFO1_MOD]%(LFO_nb_algo);
	curent_config.c_Modulation_Type[LFO2_MOD] = tmp_config.c_Modulation_Type[LFO2_MOD]%(LFO_nb_algo);
	curent_config.c_Modulation_Type[LFO3_MOD] = tmp_config.c_Modulation_Type[LFO3_MOD]%(LFO_nb_algo);
	}
	return(1); //id not valid

}

// --------------- VCO -------------------
// on le met ici pour pouvoir avoir des fonctions diferentes pour chaques VCO (le reste du code est un copié-collé a la compilation)
inline void VCO1_pitch(voice &myvoice, float &pitch) {
    pitch += g_MIDI_pitchWHEEL;
    myvoice.v_VCO1_pitch = pitch;
}
inline void VCO2_pitch(voice &myvoice, float &pitch) {
    if(curent_config.c_VCO2_LINK) pitch += myvoice.v_VCO1_pitch -(60.f + myvoice.v_pitch); else pitch +=  g_MIDI_pitchWHEEL;
}
inline void VCO3_pitch(voice &myvoice, float &pitch) {
    if(curent_config.c_VCO3_LINK) pitch += myvoice.v_VCO1_pitch -(60.f + myvoice.v_pitch); else pitch +=  g_MIDI_pitchWHEEL;
}

inline void get_pot(uint32_t i) {
    uint32_t raw_value;
    float tmpf;
    uint32_t out, j, index;
    int32_t value_min, value_max, diff;

    raw_value = hw.knobs_[i].Process_ch();
    if (raw_value) { // raw_value = 0 si on n'as pas de nouvelle valeur
		//hw.test_out(true);
		//hw.seed.SetLed(true);
        index = ++g_filter_index[i];
        index = (index >= filter_order)? 0:index;
        g_filter_index[i] = index;
        out = g_pot16[i];
        diff = raw_value - out;
        if (diff > 0) {
            g_filter_moins[i][index] = 0;
            g_filter_plus[i][index]  = diff;
        } else {
            g_filter_moins[i][index] = diff;
            g_filter_plus[i][index]  = 0;
        }
        value_min = g_filter_moins[i][0];
        value_max = g_filter_plus[i][0];
        for (j=1; j < filter_order; j++) {
            value_min = value_min>g_filter_moins[i][j]?value_min:g_filter_moins[i][j];
            value_max = value_max<g_filter_plus[i][j]?value_max:g_filter_plus[i][j];
        }
        if (value_max > -value_min) {
            diff = (value_max>=64)? value_max-60:value_max>>4;
        } else {
            diff = (value_min<=-64)? value_min+60:value_min>>4;
        }
        out += diff;
        g_pot16[i] = out;
        tmpf = (float) out;
        tmpf -= 150.f;
        tmpf = _fmax(tmpf,0.f); 
        tmpf *= 1.f/65300.f; // pour etre sur d'etre entre 0. et 1.
        tmpf +=  g_midi_parameter[i];
        tmpf= _fmin(tmpf,1.f);
        g_knob[i] = tmpf;
        g_pot_increment[i] = (tmpf - g_pot_audio[i]) * coef_CV_to_audio_filter;
    }
    //hw.seed.SetLed(false);
    //hw.test_out(false);
}
