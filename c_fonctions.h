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

float table_CV2freq[269] ;
/* // un poile plus lent, mais 5% de flash en moins
static const float table_CV2freq[] = {
	0.00503, 0.00533, 0.00565, 0.00598, 0.00634, 0.00671, 0.00711, 0.00754, 0.00798, 0.00846, 0.00896, 0.00949, 0.01006, 0.01066,
	0.01129, 0.01196, 0.01267, 0.01343, 0.01423, 0.01507, 0.01597, 0.01692, 0.01792, 0.01899, 0.02012, 0.02132, 0.02258, 0.02393,
	0.02535, 0.02686, 0.02845, 0.03014, 0.03194, 0.03384, 0.03585, 0.03798, 0.04024, 0.04263, 0.04517, 0.04785, 0.05070, 0.05371,
	0.05690, 0.06029, 0.06387, 0.06767, 0.07170, 0.07596, 0.08048, 0.08526, 0.09033, 0.09570, 0.10139, 0.10742, 0.11381, 0.12058,
	0.12775, 0.13534, 0.14339, 0.15192, 0.16095, 0.17052, 0.18066, 0.19140, 0.20279, 0.21484, 0.22762, 0.24115, 0.25549, 0.27069,
	0.28678, 0.30383, 0.32190, 0.34104, 0.36132, 0.38281, 0.40557, 0.42969, 0.45524, 0.48231, 0.51099, 0.54137, 0.57356, 0.60767,
	0.64380, 0.68209, 0.72265, 0.76562, 0.81114, 0.85938, 0.91048, 0.96462, 1.02197, 1.08274, 1.14713, 1.21534, 1.28761, 1.36417,
	1.44529, 1.53123, 1.62228, 1.71875, 1.82095, 1.92923, 2.04395, 2.16549, 2.29426, 2.43068, 2.57522, 2.72835, 2.89058, 3.06246,
	3.24457, 3.43750, 3.64190, 3.85846, 4.08790, 4.33098, 4.58851, 4.86136, 5.15043, 5.45669, 5.78116, 6.12493, 6.48914, 6.87500,
	7.28381, 7.71693, 8.17580, 8.66196, 9.17702, 9.72272, 10.30086, 10.91338, 11.56233, 12.24986, 12.97827, 13.75000, 14.56762,
	15.43385, 16.35160, 17.32391, 18.35405, 19.44544, 20.60172, 21.82677, 23.12465, 24.49971, 25.95654, 27.50000, 29.13523, 30.86771,
	32.70320, 34.64783, 36.70810, 38.89087, 41.20345, 43.65353, 46.24930, 48.99943, 51.91309, 55.00000, 58.27047, 61.73541, 65.40639,
	69.29565, 73.41619, 77.78175, 82.40689, 87.30706, 92.49860, 97.99886, 103.82617, 110.00000, 116.54094, 123.47083, 130.81279,
	138.59131, 146.83238, 155.56349, 164.81378, 174.61412, 184.99721, 195.99771, 207.65234, 220.00000, 233.08188, 246.94165,
	261.62558, 277.18262, 293.66476, 311.12698, 329.62756, 349.22824, 369.99442, 391.99542, 415.30469, 440.00000, 466.16376,
	493.88330, 523.25116, 554.36523, 587.32953, 622.25397, 659.25513, 698.45648, 739.98883, 783.99084, 830.60938, 880.00000,
	932.32751, 987.76660, 1046.50232, 1108.73047, 1174.65906, 1244.50793, 1318.51025, 1396.91296, 1479.97766, 1567.98169,
	1661.21875, 1760.00000, 1864.65503, 1975.53320, 2093.00464, 2217.46094, 2349.31812, 2489.01587, 2637.02051, 2793.82593,
	2959.95532, 3135.96338, 3322.43750, 3520.00000, 3729.31006, 3951.06641, 4186.00928, 4434.92188, 4698.63623, 4978.03174,
	5274.04102, 5587.65186, 5919.91064, 6271.92676, 6644.87500, 7040.00000, 7458.62012, 7902.13281, 8372.01855, 8869.84375,
	9397.27246, 9956.06348, 10548.08203, 11175.30371, 11839.82129, 12543.85352, 13289.75000, 14080.00000, 14917.24023, 15804.26562,
	16744.03711, 17739.68750, 18794.54492, 19912.12695, 21096.16406, 22350.60742, 23679.64258, 25087.70703, 26579.50000
};
*/

#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

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

/*
///////////////////////////////////////////////////
ceilf  arrondi vers le bas
floorf : arondi vers le haut
truncf : arondi vers zero
modf() = x-floor(x)

static inline float signf_fast(float x)
{
    return (x > 0.0f) - (x < 0.0f);
}

static inline float wrap01f_bits(float x)
{
    // Pas totalement exacte mais rapide pour |x| < 2^23
    return x - (float)((int)x);
}

*/
///////////////////////

// quick clamp
inline float _fclamp(float in, float min, float max) {
    return fminf(fmaxf(in, min), max);
}
inline float _fclamp2(float in, float min, float max) {
    return fmaxf(fminf(in, max), min);
}
// mix
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

static inline int _floor(float x) {
	return (int) x - (x < (int) x);
	//return (floorf(x));
}

static inline float wrap(float x) { // only for positive number, inversé pour les nombres negatifs
  return x - (int)x;
}

static inline float wrap2(float x) {
    x = x - floorf(x);
    return x;
}

inline float _tanh(float x) {
  float const x2 = x*x;
  return x * ( 27.f + x2 ) / ( 27.f + 9.f * x2 );
}

inline float _tanh_clip(float index){
    return _tanh(  _fclamp(index,-3.f, 3.f) );
}

inline float fast_cos(float index) { // index from 0 to 1 only
  const float x = fabsf(4.f*index-2.f)-1.f;
  return 2.f*x-x*fabsf(x);
}
/*
static const float cos_table[16] = {
  1.0f, 0.9239f, 0.7071f, 0.3827f, 0.0f, -0.3827f, -0.7071f, -0.9239f,
  -1.0f, -0.9239f, -0.7071f, -0.3827f, 0.0f, 0.3827f, 0.7071f, 0.9239f
};

inline float fast_cos3(float x) {
    x *= 15.0f;               // scale to table
    int i = (int)x;
    float t = x - i;
    float a = cos_table[i & 15];
    float b = cos_table[(i+1) & 15];
    return a + t * (b - a);
}

// Approximation rapide
static inline float fast_cos4(float x) {
    // x doit être dans [0,1)
    x -= (int)x; // wrap rapide
    float fx = x * 128.f;
    int i = (int)fx;
    float t = fx - (float)i;
    float a = cos_table2[i];
    float b = cos_table2[i + 1];
    return a + t * (b - a);
}
*/

inline float fast_cos_loop(float index) { //
    return fast_cos(wrap2(index) );
}

inline float _cos(float index) { // index from 0 to 1 only
// 6 multiplications
  float const x=index-0.5f;
  float const x2=x*x;
  return -0.99999944f + x2 * (19.73903275f + x2 * (-64.93054874f + x2 * (85.29509341f + x2 * (-58.90779707f + x2 * 21.27414825f))));
}

inline float _cos_loop(float index) { //
    return _cos(wrap2(index) );
}

inline float _sin(float index) { // index from 0 to 1 only
// 6 multiplication
  float const x=index-0.5f;
  float const x2=x*x;
  return x * (-6.28308759f +x2*(41.33318714f + x2*(-81.39900318f + x2*(74.66885164f - x2*33.1532588f))));
}

inline float _sin_positiv_loop(float index) { // positive index only
    return _sin(wrap(index));
}

inline float _sin_loop(float index) { //
    return _sin(wrap2(index) );
}
inline float sign(float in) {
	return ((in<0.f)?-1.f : 1.f);
}

void drunk_lfo(uint32_t i, float dt) { // numero de l'attracteur a calculer
    float tmp = g_drunk_lfo[i] + (_rnd_f()-0.5)*dt;
    tmp = wrap2(tmp/2.f)*2.f; // pour se mettre entre 0 et 2
    tmp = 1.f-fabsf(1.f-tmp);
    g_drunk_lfo[i] = tmp;
    }

float interpol4(float mu, float y0, float y1, float y2, float y3) {
    float a1, a2, a3;
    // 4-point, 3rd-order Hermite (x-form)
    a1 = 0.5f * (y2 - y0);
    a2 = y0 - 2.5f * y1 + y2 + y2 - 0.5f * y3;
    a3 = 0.5f * (y3 - y0) + 1.5f * (y1 - y2);
    return ((a3 * mu + a2) * mu + a1) * mu + y1;
}

#define  write_binary_led(data) hw.binary_led.Write_data(data, 24)

void init_table_CV2freq() {
  int i;
  double const power = pow(2.0,1/12.0);
  for (i=0; i<269; i++) {
    table_CV2freq[i] = pow(power, i-(128+69)) * 440.;
  }
}

inline float CV2freq(float index) { // index from -128 to 139; 69 for 440Hz
  float const f_index = _fclamp(index+128.f, 0.f, 267.f );
  float const index_entier = (int)(f_index);
  uint32_t const i_index = (uint32_t)index_entier;
  float const index_reste = f_index-index_entier;
  float const inc1 = table_CV2freq[i_index];
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
        return 1.f - 2.f*(fabsf(2.f*wrap(phase+0.25f) - 1.f));
    case WF_square:
        return (phase < 0.5f)? 1.f:-1.f;
    case WF_ramp:
        return (phase + phase) - 1.f;
    case WF_saw:
        return 1.f - (phase + phase);
    case WF_spike:
        {
            float tmpf;
            tmpf = fabsf(2.f*wrap(phase+0.275f) - 1.f);
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
        out = 0.5f + _cos(phase*0.5) * 0.5;
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

/*
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
*/

inline float Polyblep2(const float increment, const float phase) {
    const float t = phase;
    if (t >= increment && t - 1.f <= -increment) {
        return 0.0f;
    }
    if (t < increment) {
        const float normalizedT = t / increment;
        return normalizedT * (2.f - normalizedT) - 1.0f;
    }
    const float normalizedT = (t - 1.0f) / increment;
    return normalizedT * (normalizedT + 2.f) + 1.0f;
}

/*
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
*/


inline float Polyblep(const float increment, const float phase)
{
    if (phase > 0.5) {
        const float invertedPhase = 1.f - phase;

        if (invertedPhase > 2.f * increment) {
            return 0.0f;
        }

        if (invertedPhase < increment) {
            const float t = invertedPhase / increment;
            const float t2 = t * t;
            return 1.f + t * (-4.f/3.f + t2 * (2.f/3.f - t * (2.f/8.f)));
        }

        const float t = (invertedPhase - increment) / increment;
        return (2.f/24.f) + t * (-2.f/6.f + t * (2.f/4.f + t * (-2.f/6.f + t * (2.f/24.f))));
    }

    if (phase > 2.f * increment) {
        return 0.0f;
    }

    if (phase < increment) {
        const float t = phase / increment;
        const float t2 = t * t;
        return -1.f + t * (4.f/3.f + t2 * (-2.f/3.f + t * (2.f/8.f)));
    }

    const float t = (phase - increment) / increment;
    return (-2.f/24.f) + t * (2.f/6.f + t * (-2.f/4.f + t * (2.f/6.f + t * (-2.f/24.f))));
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
    return 4.*out;
}


// --------------- potentiomettres -------------------
inline uint32_t get_pot(uint32_t i) {
    float tmpf;
    uint32_t j, index;
    int32_t raw_value, out, value_min, value_max, diff;

    raw_value = (int32_t)hw.knobs_[i].Process_ch();
    if ((raw_value > 0) && (raw_value < 65535) && (isfinite(raw_value))  ) { // raw_value = 0 si on n'as pas de nouvelle valeur
		// on vire les valeurs abherentes
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
            value_min = value_min>g_filter_moins[i][j]?value_min:g_filter_moins[i][j]; // on prend la plus haute valeur min
            value_max = value_max<g_filter_plus[i][j]?value_max:g_filter_plus[i][j]; // on prend la plus basse valeur max
        } // permet de virer les pics
		// ces 2 operations fonctionnent comme un filtre median : on vire les trop haute ou trop basse
		// on ne se deplace que si les n dernier echantillons sont soit d'un coté, soit de l'autre

        diff = (value_max > -value_min) ? // on regarde ou on se trouve par rapport a ces 2 valeurs
           ((value_max >= 64) ? value_max - 60 : value_max >> 4) :
           ((value_min <= -64) ? value_min + 60 : value_min >> 4);
			// pour se recentrer rapidement en cas de mouvement brusque, ou lentement en cas de mvt tres lent, tout en gardant un peu d'hysteresys
			// si on est trop loin : on saute directement, si on est proche, on se recentre doucement
			// ce filtre combine filtre median (FIR), low pass (IIR), hysteresys, tout en ayant une tres grande reactivité

		out += diff;

		g_pot16[i] = out;
        tmpf = (float) out;
        tmpf -= 250.f;
        tmpf = fmaxf(tmpf,0.f);
        tmpf *= 1.f/65000.f; // pour etre sur d'etre entre 0. et 1.
        tmpf +=  g_midi_parameter[i];
        tmpf= fminf(tmpf,1.f);
        g_knob[i] = tmpf;
        g_pot_increment[i] = (tmpf - g_pot_audio[i]) * coef_CV_to_audio_filter;
        return(1);
    }
    return(0);
    //hw.seed.SetLed(false);
    //hw.test_out(false);
}

void init_variables() {
    uint32_t i;
  	volatile uint32_t tmp;

    for (i=0; i<2.*modulation_source_last; i++) g_Modulation[i] = 0.;

	for (i=0; i<48; i++) {
		while (get_pot(i) == 0.); // initialisation des valeurs des potentiomettres
	}

	do {tmp = hw.knobs_[k_CV1].Process_ch();} // on sort de l'initialisation, on attend d'avoir une valeur
	while (tmp == 0.);
	g_randomSeed_u = (tmp<<15)+tmp; // on compte sur le bruit de fond pour generer une seed aleatoire

	do {tmp = hw.knobs_[k_CV2].Process_ch();} // on sort de l'initialisation, on attend d'avoir une valeur
	while (tmp == 0.);
	g_randomSeed_v = (tmp<<15)+tmp; // idem

    for (i=0; i<nb_drunk_attractor; i++) {
        g_drunk_lfo[i] = _rnd_f();
    }

    for (i=0; i<nb_voice; i++) {
        allvoice[i].v_VCO1_last[1] = _rnd_f();
        allvoice[i].v_VCO1_last[0] = 0.;
        allvoice[i].v_VCO1_phase= 0.;

        allvoice[i].v_VCO2_last[1] = _rnd_f();
        allvoice[i].v_VCO2_last[0] = 0.;
        allvoice[i].v_VCO2_phase= 0.;

        allvoice[i].v_VCO3_last[1] = _rnd_f();
        allvoice[i].v_VCO3_last[0] = 0.;
        allvoice[i].v_VCO3_phase= 0.;

        allvoice[i].v_ADSR_mode = Release;
    }

    // mtof table initialisation in RAM
    init_table_CV2freq();

    // keyboard
    g_state_kb = 7; // force le recalcul des boutons etc

    //MIDI
    for (i=0; i<nb_potentiometer; i++) {
		g_midi_parameter[i] = 0.;
	}
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

    for (uint32_t i=LFO1_MOD; i<=LFO3_MOD; i++) {
        curent_config.c_Modulation_Type[i] = _rnd_ui()%8;
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

    curent_config.c_EFFECT1_TYPE = 7;
    curent_config.c_EFFECT2_TYPE = 6;
    curent_config.c_VCF2_TYPE = 0;

    for (uint32_t i=0; i<modulation_destination_last; i++) { // pour tout les VCO
        curent_config.c_Modulation_Source[i] = NONE_OUT;
        curent_config.c_Modulation_Type[i] = 0; // FM_exp pour les VCO, LFO_mix pour les LFO
    }
 //    curent_config.c_Modulation_Source[VCF1_MOD1] = NONE_OUT;
 //    curent_config.c_Modulation_Source[VCF1_MOD2] = NONE_OUT;
 //    curent_config.c_Modulation_Source[VCF2_MOD1] = NONE_OUT;

    curent_config.c_Modulation_Source[LFO1_MOD] = LFO1_OUT;
    curent_config.c_Modulation_Source[LFO2_MOD] = LFO2_OUT;
    curent_config.c_Modulation_Source[LFO3_MOD] = LFO3_OUT;
 //   curent_config.c_Modulation_Type[LFO1_MOD] = LFO_Mix;
 //   curent_config.c_Modulation_Type[LFO2_MOD] = LFO_Mix;
 //   curent_config.c_Modulation_Type[LFO3_MOD] = LFO_Mix;
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
    curent_config.c_Modulation_Type[LFO2_MOD] = LFO_Mix;
    curent_config.c_Modulation_Type[LFO3_MOD] = LFO_Mix;

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
    curent_config.c_MIDI_channel = MIN(12, MAX(-1, tmp_config.c_MIDI_channel));
    curent_config.c_VCO1_WF = tmp_config.c_VCO1_WF%10;
    curent_config.c_VCO1_RANGE = tmp_config.c_VCO1_RANGE%3;
    curent_config.c_VCO2_WF = tmp_config.c_VCO2_WF%10;
    curent_config.c_VCO2_RANGE = tmp_config.c_VCO2_RANGE%3;
    curent_config.c_VCO3_WF = tmp_config.c_VCO3_WF%10;
    curent_config.c_VCO3_RANGE = tmp_config.c_VCO3_RANGE%3;
    curent_config.c_VCO2_LINK = tmp_config.c_VCO2_LINK%2;
    curent_config.c_VCO3_LINK = tmp_config.c_VCO3_LINK%2;
    curent_config.c_VCF1_TYPE = tmp_config.c_VCF1_TYPE%6;
    curent_config.c_VCF1_MOD1_TYPE = tmp_config.c_VCF1_MOD1_TYPE%2;
    curent_config.c_VCF1_MOD2_TYPE = tmp_config.c_VCF1_MOD2_TYPE%2;
    curent_config.c_VCF1_pitch_TRACK = tmp_config.c_VCF1_pitch_TRACK%3;
    curent_config.c_ADSR_LOOP = tmp_config.c_ADSR_LOOP%3;
    curent_config.c_KEYBOARD_octave = MIN(2, MAX(-3,tmp_config.c_KEYBOARD_octave));
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
    curent_config.c_EFFECT1_TYPE = tmp_config.c_EFFECT1_TYPE%8;
    curent_config.c_EFFECT2_TYPE = tmp_config.c_EFFECT2_TYPE%7;
    curent_config.c_VCF2_TYPE = tmp_config.c_VCF2_TYPE%4;
	for(i=0; i<modulation_destination_last; i++) curent_config.c_Modulation_Source[i] = tmp_config.c_Modulation_Source[i]%(2*modulation_source_last);
	for(i=0; i<10; i++) curent_config.c_Modulation_Type[i] = tmp_config.c_Modulation_Type[i]%(modulation_type_last);
	curent_config.c_Modulation_Type[LFO1_MOD] = tmp_config.c_Modulation_Type[LFO1_MOD]%(LFO_nb_algo);
	curent_config.c_Modulation_Type[LFO2_MOD] = tmp_config.c_Modulation_Type[LFO2_MOD]%(LFO_nb_algo);
	curent_config.c_Modulation_Type[LFO3_MOD] = tmp_config.c_Modulation_Type[LFO3_MOD]%(LFO_nb_algo);
	}
	if (slot == 14) { g_CV2KB = tmp_config.c_CV2KB != 0;} // on ne lit cette fct que pour la config sauvegardé lors de l'extinction
	return(1); //id valid

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
