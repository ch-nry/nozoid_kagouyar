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

#define delay1_sizef 72000 // attention : la reverb fait env 51k
#define delay1_sizei 144000
// mettre un multiple de 2 sur la taille des buffer

float g_effect1_phase;
float g_effect1_last_out = 0.f;
float g_effect1_param_filter = 0.f;
float g_effect1_param_filter2 = 0.f;
float g_vitesse = 0.f, g_old_sound_out = 0.f, g_last_sound_in = 0.f;
float g_effect1_f1 = 0.f, g_effect1_f2 = 0.f, g_effect1_f3 = 0.f, g_effect1_f4 = 0.f;
float g_effect1_f1old = 0.f, g_effect1_f2old = 0.f, g_effect1_f3old = 0.f, g_effect1_f4old = 0.f;

//daisysp::DelayLine<float, 48000*4> DSY_SDRAM_BSS g_delay_effect1; // SDRAM

union delay_line {
	float delay1_float[delay1_sizef];
	int16_t delay1_int[delay1_sizei];
	struct {
		float reverb1[4][8192];
		float reverb2[6][1024];
		float reverb3[3][4096];
	} reverb;
};

delay_line g_delay1;

uint32_t g_delay1_pos; // position ds le buffer
uint32_t g_delay2_pos;
uint32_t g_delay3_pos;
uint32_t g_delay1b_pos; // position ds le buffer

__attribute__((section(".dtcmram_bss"))) float  g_delay1b[16384];
inline void delay1b_write_f(float in){
	int32_t l_delay1b_pos = g_delay1b_pos % 16384;
	g_delay1b[l_delay1b_pos] = in;
	l_delay1b_pos += 16384 - 1;
	l_delay1b_pos %= 16384;
	g_delay1b_pos = l_delay1b_pos;
}
inline float delay1b_read_f(float delay){ // en sample
	int32_t delay_integral   = static_cast<int32_t>(delay);
	float const  delay_fractional = delay - static_cast<float>(delay_integral);
	delay_integral += g_delay1b_pos;
	const float a = g_delay1b[(delay_integral) % 16384];
	const float b = g_delay1b[(delay_integral + 1) % 16384];
	return a + (b - a) * delay_fractional;
}

inline float reverb1_read(uint32_t a, uint32_t i){
	return g_delay1.reverb.reverb1[a][(g_delay1_pos+i)%8192];
}
inline void reverb1_write(float a0, float a1, float a2, float a3){
	g_delay1_pos = (g_delay1_pos + 1)%8192;
	g_delay1.reverb.reverb1[0][g_delay1_pos] = a0;
	g_delay1.reverb.reverb1[1][g_delay1_pos] = a1;
	g_delay1.reverb.reverb1[2][g_delay1_pos] = a2;
	g_delay1.reverb.reverb1[3][g_delay1_pos] = a3;
}
inline float reverb2_read(uint32_t a, uint32_t i){
	return g_delay1.reverb.reverb2[a][(g_delay2_pos+i)%1024];
}
inline void reverb2_write(int i, float a0){
	g_delay2_pos = (g_delay2_pos + 1)%1024;
	g_delay1.reverb.reverb2[i][g_delay2_pos] = a0;
}
inline float reverb3_read(uint32_t a, uint32_t i){
	return g_delay1.reverb.reverb3[a][(g_delay3_pos+i)%4096];
}
inline void reverb3_write(int i, float a0){
	g_delay3_pos = (g_delay3_pos + 1)%4096;
	g_delay1.reverb.reverb3[i][g_delay3_pos] = a0;
}

inline void delay1_clear(){
	//for (int i=0; i<delay1_sizef; i++) g_delay1.delay1_float[i] = 0.f;
	init_table_f_0(delay1_sizef, g_delay1.delay1_float);
	init_table_f_0(16384, g_delay1b);
	g_delay1_pos = 0;
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
}
inline void delay1_write_f(float in){
	int32_t l_delay1_pos = g_delay1_pos % delay1_sizef;
	g_delay1.delay1_float[l_delay1_pos] = in;
	l_delay1_pos += delay1_sizef - 1;
	l_delay1_pos %= delay1_sizef;
	g_delay1_pos = l_delay1_pos;
}
inline float delay1_read_f(float delay){ // en sample
	int32_t delay_integral   = static_cast<int32_t>(delay);
	float const  delay_fractional = delay - static_cast<float>(delay_integral);
	delay_integral += g_delay1_pos;
	const float a = g_delay1.delay1_float[(delay_integral) % delay1_sizef];
	const float b = g_delay1.delay1_float[(delay_integral + 1) % delay1_sizef];
	return a + (b - a) * delay_fractional;
}
inline float delay1_read_f_fixe(uint32_t delay){ // en sample
	const float a = g_delay1.delay1_float[(delay + g_delay1_pos) % delay1_sizef];
	return a;
}
inline void delay1_write_i(float  in){
	int32_t l_delay1_pos = g_delay1_pos; // % delay1_sizei// inutil car sizei > sizef
	g_delay1.delay1_int[l_delay1_pos] = f2s16(in/3.f);
	l_delay1_pos += delay1_sizei - 1;
	l_delay1_pos %= delay1_sizei;
	g_delay1_pos = l_delay1_pos;
}
inline float delay1_read_i(float delay){
	int32_t delay_integral   = static_cast<int32_t>(delay);
	float const  delay_fractional = delay - static_cast<float>(delay_integral);
	delay_integral += g_delay1_pos;
	const float a = s162f(g_delay1.delay1_int[(delay_integral) % delay1_sizei]);
	const float b = s162f(g_delay1.delay1_int[(delay_integral + 1) % delay1_sizei]);
	return 3.f*(a + (b - a) * delay_fractional);
}

inline float effect1(float sound_in) { //, float wet, float param1, float param2) {
	float const wet = g_pot_audio[k_EFFECT1_wet] += g_pot_increment[k_EFFECT1_wet];
	float param1 = _fclamp(g_pot_audio[k_EFFECT1_p1] += g_pot_increment[k_EFFECT1_p1], 0.f, 1.f);
	float const param2 = _fclamp(g_pot_audio[k_EFFECT1_p2] += g_pot_increment[k_EFFECT1_p2], 0.f, 1.f);
	float a1, a2, a3, a4, b1, b2, b3, b4;

    float sound_out = 0.f;
    float tmp = 0.f;
    float effect1_phase;

    float const param2_mod = param2 * g_Modulation[curent_config.c_Modulation_Source[EFFECT1_MOD]];
    float const wetM = _fclamp(wet + param2_mod, 0.f, 1.f);
    float const param1M = _fclamp(param1 + param2_mod, 0.f, 1.f);
    switch(curent_config.c_EFFECT1_TYPE) {
    case 0 : // Wave shaper : OK
		// DRY
		// Waveform : a zero : on est sur une disto, a full sur un WS sauvage
		// DRY Modulation
		tmp = sound_in/(1.f + 10.f*param1)*150.f*(wetM+1.f)*wetM*wetM*wetM;
		tmp = (1.f-param1)*(sound_in+tmp)/(1.f+fabsf(tmp));
		tmp += (1.f-wetM)*param1*sound_in;
		tmp += wetM*param1*fabsf(sound_in)*_sin_loop(5.f*param1*sound_in*(1.f+fabsf(sound_in)));
		return tmp;
    case 1 : // ECHO : OK
		// long delay (wet /  param1 : time / param2 : feedback)
        tmp = ((delay1_sizei - 101.f) * param1) + 100.f;
        sound_out = delay1_read_i(tmp);
        delay1_write_i(_fclamp(sound_in + sound_out*param2, -3.f, 3.f));
        sound_out = mix(sound_in, sound_out, wet);
        return sound_out;
        //break;
    case 2 : // FREEZE : OK
		// small delay (wet : feedback / param1M : time) : flanger / chorus / doubler
        tmp = param1M * 50000.f + 6.f;
        sound_out = mix(sound_in , delay1_read_f(tmp), wet);
		delay1_write_f(_fclamp(sound_out, -3.f, 3.f));
        return sound_out;
        //break;
    case 3 : // KS : OK
		// (wet :  attenuation (gain/filtre) , param1 : frequence, param2 : mod frequence)
        tmp = 48000.f/CV2freq(param1M*127.f);
        tmp += 5.f;
        tmp = delay1_read_f(tmp);
        _fonepole(g_effect1_last_out, tmp,fminf(1.,CV2freq(wet*127.f)*(1.f/12000.f)));
        sound_out = sound_in - g_effect1_last_out;
        if(sound_out >  1.f) sound_out =  1.f + _tanh_clip(sound_out - 1.f); // soft clip with ID between -1 and 1;
        if(sound_out < -1.f) sound_out = -1.f + _tanh_clip(sound_out + 1.f); // between -2 and 2;
        delay1_write_f(_fclamp(sound_out, -3.f, 3.f));
        return sound_out;
        //break;
    case 4 : // chorus : OK
		// ( WET + feedback, TIME, WET modulation)
		/*
		param1 *= 2000.f; // param1 = 1/8 du temps max du chorus, en echantillons
		param1 += 100.f; // I.E : (6.73+1) * 2200 echantillons max
        g_effect1_phase = wrap(g_effect1_phase + (0.005f/48000.f)); // LFO : vitesse de variation du temps du chorus
        effect1_phase = g_effect1_phase;
        sound_out = 0.f; //sound_in;
        sound_out += delay1_read_f((tri_positif_loop(             effect1_phase*3.f)+1.11f) * param1);
        sound_out  -= delay1_read_f((tri_positif_loop(0.23f + effect1_phase*4.f)+2.31f) * param1);
		sound_out += delay1_read_f((tri_positif_loop(0.57f + effect1_phase*5.f)+3.52f) * param1);
        sound_out  -= delay1_read_f((tri_positif_loop(0.71f + effect1_phase*7.f)+5.73f) * param1);
        sound_out *= 0.5;
        sound_out = sound_in + _tanh_clip(wetM*sound_out);
        delay1_write_f(sound_out);
        return sound_out;*/
        {
			// 1. Précalculer les constantes
			float param_scaled = param1 * 2000.f + 100.f;
			float wet_scaled = wetM * 0.5f;

			// 2. Update phase
			g_effect1_phase = wrap(g_effect1_phase + (0.005f/48000.f));
			float phase = g_effect1_phase;

			// 3. Calculer phases modulées
			float p3 = phase * 3.f;
			float p4 = phase * 4.f;
			float p5 = phase * 5.f;
			float p7 = phase * 7.f;

			// 4. Calculer offsets
			float o1 = (tri_positif_loop(p3) + 1.11f) * param_scaled;
			float o2 = (tri_positif_loop(0.23f + p4) + 2.31f) * param_scaled;
			float o3 = (tri_positif_loop(0.57f + p5) + 3.52f) * param_scaled;
			float o4 = (tri_positif_loop(0.71f + p7) + 5.73f) * param_scaled;

			// 5. Lectures delay
			float s1 = delay1b_read_f(o1);
			float s2 = delay1b_read_f(o2);
			float s3 = delay1b_read_f(o3);
			float s4 = delay1b_read_f(o4);

			// 6. Combinaison
			float chorus = s1 - s2 + s3 - s4;

			// 7. Output
			sound_out = sound_in + _tanh_clip(wet_scaled * chorus);
			delay1b_write_f(sound_out);
			return sound_out;
		}
    case 5 : // ring delay : OK
		// WET : amplitude du feedback; param1 : temps de delay ; param2 : frequence du ring
		sound_out = delay1_read_f((24000.f * param1 * (param1+1.f)) + 50.f);
        g_effect1_phase += OneOverSR + param2 * param2 * 400.f * OneOverSR;
        tmp = wrap(g_effect1_phase);
		g_effect1_phase = tmp;
		sound_out *= _cos(tmp);
		sound_out *= wet*wet;
		sound_out += sound_in;
		delay1_write_f(sound_out);
        return sound_out;
	case 6 : //frottement OK
		// WET; param1 : taille du seuil pour declancher un mouvement; param1 : inertie de la corde
		// le son reste bloqué tant que la diference avec l'entrée n'est pas sufisante.
		// Lorsque le seuil est atteins, le son se reinitialise a la position de l'entre,
		// et une inertie est calculé en fct de la vitersse actuel du son.
		// un paramettre control cette quantité d'inertie
		// c'est un bitcrusher assez sauvage, avec bcp d'instabilité qd on monte le paramettre
		sound_out = sound_in - g_old_sound_out;
		if( fabsf(sound_out) > wet*wet ) {
			sound_out = sound_in;
			g_vitesse = sound_in - g_last_sound_in;
			g_last_sound_in = sound_in;
		} else {
			sound_out = g_old_sound_out + g_vitesse*param1M*0.1;
		}
		g_old_sound_out = sound_out;
		return sound_out;
	case 7: // WS2 : waveshaper simple : OK
		sound_out = _tanh(5.f * sound_in + param1M * fast_cos_loop(0.75 + param1M  * 4.f * sound_in));
		return mix(sound_in, sound_out*0.3f, wet);
	case 8: // ECHO 2 : disto ok
		tmp = delay1_read_f(10.f + 5000.f * param1M* param1M);
		sound_out = sound_in + tmp * wet;
		delay1_write_f( _fclamp(sound_out, -2.f, 2.f));
		return sound_out;
	case 9: // FREEZE 2 : filtre en peigne variable OK
		// algo from pd G07 :  4 delread, sans feedback a des temps diferent (30, 17, 11), et des amplitudes variable (random)
		delay1_write_f(sound_in);
		a1 = sound_in;
		a2 = delay1_read_f_fixe(1441);
		a3 = delay1_read_f_fixe(817);
		a4 = delay1_read_f_fixe(527);
		tmp = CV2increment_lfo(1, param1M)*0.5f;
		g_effect1_phase = wrap(g_effect1_phase + tmp);
		effect1_phase = g_effect1_phase;
		if(effect1_phase<tmp) { g_effect1_f1old = g_effect1_f1; g_effect1_f1 = _rnd_f()*2.f - 0.7f; }
		a1 *= mix(g_effect1_f1old, g_effect1_f1, effect1_phase);
		effect1_phase = wrap( effect1_phase + 0.25);
		if(effect1_phase<tmp) { g_effect1_f2old = g_effect1_f2; g_effect1_f2 = _rnd_f()*2.f - 0.7f; }
		a2 *= mix(g_effect1_f2old, g_effect1_f2, effect1_phase);
		effect1_phase = wrap( effect1_phase + 0.25);
		if(effect1_phase<tmp) { g_effect1_f3old = g_effect1_f3; g_effect1_f3 = _rnd_f()*2.f - 0.7f; }
		a3 *= mix(g_effect1_f3old, g_effect1_f3, effect1_phase);
		effect1_phase = wrap( effect1_phase + 0.25);
		if(effect1_phase<tmp) { g_effect1_f4old = g_effect1_f4; g_effect1_f4 = _rnd_f()*2.f - 0.7f; }
		a4 *= mix(g_effect1_f4old, g_effect1_f4, effect1_phase);
		sound_out = a1 + a2 + a3 + a4;
		return mix(sound_in, sound_out, wet);
	case 10: // STRING 2 : reverb OK
		// algorythm from miller puckette, in "Pure Data" example G28.
		_fonepole(g_effect1_param_filter, sound_in, 0.0003f); // low pass
		a1 = sound_in- g_effect1_param_filter; a2 = 0.f; b1 = a1 + a2; b2 = a1 - a2;
		reverb2_write(0, b2); b2 = reverb2_read(0, 261);
		a1 = b1; a2 = b2; b1 = a1 + a2; b2 = a1 - a2;
		reverb2_write(1, b2); b2 = reverb2_read(1, 406);
		a1 = b1; a2 = b2; b1 = a1 + a2; b2 = a1 - a2;
		reverb2_write(2, b2); b2 = reverb2_read(2, 645);
		a1 = b1; a2 = b2; b1 = a1 + a2; b2 = a1 - a2;
		reverb3_write(0, b2); b2 = reverb3_read(0, 1034);
		a1 = b1; a2 = b2; b1 = a1 + a2; b2 = a1 - a2;
		reverb3_write(1, b2); b2 = reverb3_read(1, 1651);
		a1 = b1; a2 = b2; b1 = a1 + a2; b2 = a1 - a2;
		reverb3_write(2, b2); b2 = reverb3_read(2, 2666);
		a1 = b1 + reverb1_read(0, 2880);
		a2 = b2 + reverb1_read(1, 3453);
		a3 = reverb1_read(2, 4164);
		a4 = reverb1_read(3, 4605);
		sound_out = a1;
		b1 = a1 + a2; b2 = a1 - a2; b3 = a3 + a4; b4 = a3 - a4;
		a1 = b1 + b3; a2 = b2 + b4; a3 = b1 - b3; a4= b2 - b4;
		tmp = 2*param1M - param1M * param1M;// courbe plus punchi au debut
		tmp *= 0.5f;
		a1 *= tmp; a2 *= tmp; a3 *= tmp; a4 *= tmp; // gain de la reverb
		a1 = _fclamp(a1, -30.f, 30.f); a2 = _fclamp(a2, -30.f, 30.f);  a3 = _fclamp(a3, -30.f, 30.f); a4 = _fclamp(a4, -30., 30.f);
		reverb1_write(a1, a2, a3, a4);
		sound_out = mix(sound_in, sound_out, wet);
		return sound_out;
	case 11: // CHORUS 2
		// all passe + feedback
		sound_out = delay1_read_f(5.f + 5000.f * param1M);
		tmp =  -0.9f*wet  * sound_out + sound_in;
		delay1_write_f(tmp);
		sound_out = sound_in + tmp * wet;
		return sound_out;
	case 12: // RING 2 : OK
		//WET : amplitude du ring; param1 : frequence du ring; param2 : modulation du wet
        g_effect1_phase +=  CV2freq(param1*127.f) * OneOverSR; // OneOverSR + param1 * param1 * 400.f * OneOverSR;
        tmp = wrap(g_effect1_phase);
		g_effect1_phase = tmp;
		sound_out = sound_in * _cos(tmp);
		sound_out = mix(sound_in, sound_out, wetM);
        return sound_out;
	case 13: // FRICTION 2 : disto avec hysteresys:
		_fonepole(g_effect1_param_filter, sound_in, 0.01f);
		sound_out = sound_in + 8.f * param1 * param1 * (sound_in - g_effect1_param_filter);
		sound_out = _tanh_clip( (1.f + 15.f*wetM*wetM) * sound_out);
		return mix(sound_in, sound_out*0.8f, fminf(1.f,4.f*wetM));
	case 14 : //rien, utilisé lors du changement d'effet
		g_effect1_phase = 0.;
		g_effect1_last_out = 0.f;
		g_effect1_param_filter = 0.f;
		g_effect1_param_filter2 = 0.f;
		g_vitesse = 0.f;
		g_old_sound_out = 0.f;
		g_last_sound_in = 0.;
		return sound_in;
    }
    return 0;
}

// --------------------------------------------------------------------------------------------
// EFFECT 2
// --------------------------------------------------------------------------------------------

float g_Effect2_filtre = 0.f;
float g_effect2_sound_env = 0.f;
float g_effect2_phase = 0.33f;


daisysp::DelayLine<float, 32768> g_delay_effect2;
daisysp::DelayLine<float, 16384>  g_delay_effect2b;
//__attribute__((section(".dtcmram_bss"))) daisysp::DelayLine<float, 16384>  g_delay_effect2b;

inline float effect2(float sound_in) { //, float param, float param1) {
	float const param = g_pot_audio[k_EFFECT2_wet] += g_pot_increment[k_EFFECT2_wet];
	float const param1 = g_pot_audio[k_EFFECT2_p1] += g_pot_increment[k_EFFECT2_p1];
    float sound_out;
    float tmp, tmp2, tmp3;
	float effect2_phase;
    float wet = _fclamp(param + param1 * g_Modulation[curent_config.c_Modulation_Source[EFFECT2_MOD]], 0.f, 1.f);

    switch(curent_config.c_EFFECT2_TYPE) {
    case 0: // disto : OK
        tmp = wet*(wet+1.f);
        tmp = wet*wet;
        tmp = wet*wet*sound_in*150.f;
        sound_out = (sound_in + tmp)/(1.f+2.f*fabsf(tmp));
        return sound_out;
     case 1: // WS : OK
        tmp = wet*15.f;
        return (sound_in + tmp*tmp*_sin_positiv_loop(1000.f + sound_in*fabsf(0.25*sound_in*tmp))) / (tmp*tmp + 1.f);
     case 2 : // BITCRUSH : OK
        wet = 0.01f + wet*0.99f;
        sound_out = sound_in;
        sound_out = floorf(sound_out/wet)*wet;
        return mix(sound_in, sound_out, fminf(1.f,10.f*wet));
    case 3: // auto doppler : on utilise le son comme temps de delay : OK
        g_delay_effect2.Write(sound_in);
        tmp = wet * 10000.f * (1.f+sound_in);
        _fonepole(g_Effect2_filtre, tmp, 0.001f); // smooth le paramettre de temps et filtre le audio in
        sound_out = g_delay_effect2.ReadHermite(fmaxf(1.f,g_Effect2_filtre));
        return sound_out;
    case 4 : // granular sub frequency generator : OK
        g_delay_effect2.Write(sound_in);
        effect2_phase = wrap(g_effect2_phase + 0.00020833333f); // 100ms pour 1 grain
        g_effect2_phase = effect2_phase;
        g_delay_effect2.SetDelay(2400.f * effect2_phase);
        sound_out  = g_delay_effect2.Read() * (1.f-_cos(effect2_phase));
        effect2_phase = (effect2_phase > 0.5f)? effect2_phase-0.5f : effect2_phase+0.5f;
        g_delay_effect2.SetDelay(2400.f * effect2_phase);
        sound_out  += g_delay_effect2.Read() *  (1.f-_cos(effect2_phase));
        return mix(sound_in, sound_out*0.5, wet);
    case 5: // compresseur- attenuateur : ok
        tmp = fabsf(sound_in);
        tmp = fminf(tmp, 3.f); // on ne devrait pas avoir de son plus fort que ca.
        if (tmp > g_effect2_sound_env) { g_effect2_sound_env = mix(g_effect2_sound_env, tmp, 0.01f); } // temps de monté rapide
        else { g_effect2_sound_env = mix(g_effect2_sound_env, tmp, 0.001f); } // temps de descente lent
        tmp = fmaxf(g_effect2_sound_env, 0.1f); // volume actuel
        tmp2 = tmp*param*param*param*20.f;
        tmp2 = (tmp + tmp2) / (1.f + fabsf(tmp2)); // new volume
        sound_out = sound_in * tmp2/tmp;        // compress
        sound_out *= _fclamp(1.f - param1 * (1.f-g_Modulation[curent_config.c_Modulation_Source[EFFECT2_MOD]]), 0.f, 1.f); // attenuation
        return sound_out;
	case 6: // DIST 2 : ok : plus soupe que la disto 1
		sound_out = _tanh_clip( (15.f*wet*wet+1) * sound_in);
		sound_out = mix(sound_in, sound_out*0.5f, fminf(1.f, wet*10.f) );
		return sound_out;
	case 7: // WS 2 : delay : OK : delay infini, mais qui peux s'effacer pour laisser la place au nvx sond in
		g_delay_effect2.SetDelay(5.f + 10000.f * param1);
		tmp = sound_in - g_delay_effect2.Read();
		tmp = _tanh_clip( tmp);
		tmp *= _tanh(3.f*param); // NL sur le wet, car interessant seulement a haut nvx de wet
		g_delay_effect2.Write(tmp);
		return sound_in + tmp;
	case 8: // BITCRUSH 2 : downsampler : ok
	    g_effect2_phase +=  CV2freq(125.f - wet*100.f) * OneOverSR;
        tmp = wrap(g_effect2_phase);
		if ( tmp < g_effect2_phase ) { // reset de phase
			g_Effect2_filtre = sound_in;
		}
		g_effect2_phase = tmp;
		sound_out = mix(sound_in, g_Effect2_filtre, (fminf(1.f,10.f*wet)));
		return sound_out;
	case 9: // doepler 2: ok
        g_delay_effect2.Write(sound_in);
        tmp = (wet * 5000.f) * (2.f - _tanh_clip(3.f*sound_in));
        _fonepole(g_Effect2_filtre, tmp, 0.0003f); // smooth le paramettre de temps et filtre le audio in
        sound_out = g_delay_effect2.ReadHermite(fmaxf(1.f,g_Effect2_filtre));
        return sound_out;
	case 10: // sub2 : delay a resonnance metalique, non lineaire bizare
		tmp2 = param1 * param1;
		tmp = g_delay_effect2.Read(tmp2 * 5500.f);
		tmp2 = g_delay_effect2b.Read(tmp2 * 5550.f + 50.f);
		sound_out = _tanh(param * (tmp - tmp2)) ;
		_fonepole(g_Effect2_filtre, sound_out, 0.0001f); // low pass pour virer le CC
		tmp3 = _tanh(3.f * param); // petite courbe
		g_delay_effect2.Write(sound_in + (sound_out-g_Effect2_filtre) * tmp3);
		g_delay_effect2b.Write(sound_in - (sound_out-g_Effect2_filtre) * tmp3);
		sound_out = tmp + tmp2;
		return sound_out;
	case 11: // compress 2: binarizator : sign * envelope
	tmp = fabsf(sound_in);
		if ( tmp > g_effect2_phase ) _fonepole(g_effect2_phase, tmp, 0.3);
		else  _fonepole(g_effect2_phase, tmp, 0.01);
		return mix(sound_in, g_effect2_phase * sign(sound_in), wet);
	case 12 : //rien, utilisé lors du changement d'effet
		g_effect2_sound_env = 0.;
		g_Effect2_filtre = 0.f;
		g_effect2_phase = 0.33f;
		return sound_in;
    }
    return 0; //useless
}
