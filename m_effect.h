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

#define delay1_sizef 72000
#define delay1_sizei 144000

float g_effect1_phase;
float g_effect1_last_out = 0.f;
float g_effect1_param_filter = 0.f;
float g_effect1_param_filter2 = 0.f;
float g_vitesse = 0.f, g_old_sound_out = 0.f, g_last_sound_in = 0.;

//daisysp::DelayLine<float, 48000*4> DSY_SDRAM_BSS g_delay_effect1; // SDRAM
//daisysp::DelayLine<float, 10000>  g_delay_effect3;

/*union Delay_line {
	daisysp::DelayLine<int16_t, 72000*2>  g_delay_effect1;
	daisysp::DelayLine<float, 72000>  g_delay_effect3;
} delayline ;
*/

union delay_line {
	float delay1_float[delay1_sizef];
	int16_t delay1_int[delay1_sizei];
} g_delay1;

uint32_t g_delay1_pos; // position ds le buffer

inline void delay1_clear(){
	for (int i=0; i<delay1_sizef; i++) g_delay1.delay1_float[i] = 0.f;
	g_delay1_pos = 0;
}

inline void delay1_write_f(float in){
	int32_t l_delay1_pos = g_delay1_pos % delay1_sizef;
	g_delay1.delay1_float[l_delay1_pos] = in;
	l_delay1_pos += delay1_sizef - 1;
	l_delay1_pos %= delay1_sizef;
	g_delay1_pos = l_delay1_pos;
}

inline float delay1_read_f(float delay){
	int32_t delay_integral   = static_cast<int32_t>(delay);
	float const  delay_fractional = delay - static_cast<float>(delay_integral);
	delay_integral += g_delay1_pos;
	const float a = g_delay1.delay1_float[(delay_integral) % delay1_sizef];
	const float b = g_delay1.delay1_float[(delay_integral + 1) % delay1_sizef];
	return a + (b - a) * delay_fractional;
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
		tmp = (1.f-param1)*(sound_in+tmp)/(1.f+fabs(tmp));
		tmp += (1.f-wetM)*param1*sound_in;
		tmp += wetM*param1*fabs(sound_in)*_sin_loop(5.f*param1*sound_in*(1.f+fabs(sound_in)));
		return tmp;
    case 1 : // ECHO : long delay (wet /  param1 : time / param2 : feedback) : OK
        tmp = ((delay1_sizei - 101.f) * param1) + 100.f;
        //delayline.g_delay_effect1.SetDelay(tmp);
        //sound_out = g_delay_effect1.Read();
        //delayline.g_delay_effect1.Write(_fclamp(sound_in + sound_out*param2, -3.f, 3.f));
        sound_out = delay1_read_i(tmp);
        delay1_write_i(_fclamp(sound_in + sound_out*param2, -3.f, 3.f));
        sound_out = mix(sound_in, sound_out, wet);
        return sound_out;
        //break;
    case 2 : // FREEZE : small delay (wet : feedback / param1 : time / param2 : time modulation) : flanger / chorus / doubler : OK
        tmp = param1M * 50000.f + 6.f;
        //delayline.g_delay_effect1.SetDelay(tmp);
        //sound_out = mix(sound_in , g_delay_effect1.Read(), wet);
        //delayline.g_delay_effect1.Write(_fclamp(sound_out, -3.f, 3.f));
        sound_out = mix(sound_in , delay1_read_f(tmp), wet);
		delay1_write_f(_fclamp(sound_out, -3.f, 3.f));
        return sound_out;
        //break;
    case 3 : // KS (wet :  attenuation (gain/filtre) , param1 : frequence, param2 : mod frequence) : OK
        tmp = 48000.f/CV2freq(param1M*127.f);
        tmp += 5.f;
        //g_delay_effect1.SetDelay(tmp);
        //tmp = g_delay_effect1.Read();
        tmp = delay1_read_f(tmp);
        _fonepole(g_effect1_last_out, tmp,_fmin(1.,CV2freq(wet*127.f)*(1.f/12000.f)));
        sound_out = sound_in - g_effect1_last_out;

        if(sound_out >  1.f) sound_out =  1.f + _tanh_clip(sound_out - 1.f); // soft clip with ID between -1 and 1;
        if(sound_out < -1.f) sound_out = -1.f + _tanh_clip(sound_out + 1.f); // between -2 and 2;

        //g_delay_effect1.Write(_fclamp(sound_out, -3.f, 3.f));
        delay1_write_f(_fclamp(sound_out, -3.f, 3.f));
        return sound_out;
        //break;
    case 4 : // chorus : ( WET + feedback, TIME, WET modulation) : OK
		param1 *= 2000.f; // param1 = 1/8 du temps max du chorus, en echantillons
		param1 += 100.f; // I.E : (6.73+1) * 2200 echantillons max
        g_effect1_phase = wrap(g_effect1_phase + (0.005f/48000.f)); // LFO : vitesse de variation du temps du chorus
        effect1_phase = g_effect1_phase;
        sound_out = 0.f; //sound_in;
        sound_out += delay1_read_f((fast_cos_loop(             effect1_phase*3.f)+1.11f) * param1);
        sound_out  -= delay1_read_f((fast_cos_loop(0.23f +  effect1_phase*4.f)+2.31f) * param1);
		sound_out += delay1_read_f((fast_cos_loop(0.57f + effect1_phase*5.f)+4.52f) * param1);
        sound_out  -= delay1_read_f((fast_cos_loop(0.71f + effect1_phase*7.f)+6.73f) * param1);
        sound_out *= 0.5;
        sound_out = sound_in + _tanh_clip(wetM*sound_out);

        delay1_write_f(sound_out);
        return sound_out;
    case 5 : // ring delay : WET : amplitude du feedback; param1 : temps de delay ; param2 : frequence du ring
        //g_delay_effect1.SetDelay((24000.f * param1 * (param1+1.f)) + 50.f);
		//sound_out = g_delay_effect1.Read();
		sound_out = delay1_read_f((24000.f * param1 * (param1+1.f)) + 50.f);
        g_effect1_phase += OneOverSR + param2 * param2 * 400.f * OneOverSR;
        tmp = wrap(g_effect1_phase);
		g_effect1_phase = tmp;
		sound_out *= _cos(tmp);
		sound_out *= wet*wet;
		sound_out += sound_in;
		//g_delay_effect1.Write(sound_out);
		delay1_write_f(sound_out);
        return sound_out;
        //break;
	case 6 : //frottement : WET; param1 : taille du seuil pour declancher un mouvement; param1 : inertie de la corde
		// le son reste bloqué tant que la diference avec l'entrée n'est pas sufisante.
		// Lorsque le seuil est atteins, le son se reinitialise a la position de l'entre,
		// et une inertie est calculé en fct de la vitersse actuel du son.
		// un paramettre control cette quantité d'inertie
		// c'est un bitcrusher assez sauvage, avec bcp d'instabilité qd on monte le paramettre
		sound_out = sound_in - g_old_sound_out;
		if( fabs(sound_out) > wet*wet ) {
			sound_out = sound_in;
			g_vitesse = sound_in - g_last_sound_in;
			g_last_sound_in = sound_in;
		} else {
			sound_out = g_old_sound_out + g_vitesse*param1M*0.1;
		}
		g_old_sound_out = sound_out;
		return sound_out;
	case 7 : //rien, utilisé lors du changement d'effet
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

daisysp::DelayLine<float, 48000> g_delay_effect2;

inline float effect2(float sound_in) { //, float param, float param1) {
	float const param = g_pot_audio[k_EFFECT2_wet] += g_pot_increment[k_EFFECT2_wet];
	float const param1 = g_pot_audio[k_EFFECT2_p1] += g_pot_increment[k_EFFECT2_p1];
    float sound_out;
    float tmp, tmp2;
	float effect2_phase;
    float wet = _fclamp(param + param1 * g_Modulation[curent_config.c_Modulation_Source[EFFECT2_MOD]], 0.f, 1.f);

    switch(curent_config.c_EFFECT2_TYPE) {
    case 0: // disto : OK
        tmp = wet*(wet+1.f);
        tmp = wet*wet;
        tmp = wet*wet*sound_in*150.f;
        sound_out = (sound_in + tmp)/(1.f+fabs(tmp));
        return sound_out;
     case 1: // WS : OK
        tmp = wet*15.f;
        return (sound_in + tmp*tmp*_sin_positiv_loop(1000.f + sound_in*fabs(0.25*sound_in*tmp))) / (tmp*tmp + 1.f);
     case 2 : // BITCRUSH : OK
        wet = 0.01f + wet*0.99f;
        sound_out = sound_in;
        //sound_out = sign(sound_out) * (1.5f - 1.5f/(2.f*fabs(sound_out) + 1.f));
        sound_out = _floor(sound_out/wet)*wet;
        //sound_out = sign(sound_out) * (-0.5f - 0.75f/(fabs(sound_out)-1.5f)); // Pq ca ne marche pas???
        return mix(sound_in, sound_out, _fmin(1.f,10.f*wet));
    case 3: // auto doppler : on utilise le son comme temps de delay : OK
        g_delay_effect2.Write(sound_in);
        tmp = wet * 10000.f;
        tmp *=  (1.f+sound_in);
        _fonepole(g_Effect2_filtre, tmp, 0.001f); // smooth le paramettre de temps et filtre le audio in
        g_delay_effect2.SetDelay(_fmax(1.f,g_Effect2_filtre));
        sound_out = g_delay_effect2.Read();
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
    case 5: // compresseur- attenuateur :
    //  qd pas de modulation, que faire avec param???
        tmp = fabs(sound_in);
        if (tmp > g_effect2_sound_env) {
            g_effect2_sound_env = mix(g_effect2_sound_env, tmp, 0.01f); // temps de monté rapide
        } else {
            g_effect2_sound_env = mix(g_effect2_sound_env, tmp, 0.001f); // temps de descente lent
        }
        tmp = _fmax(g_effect2_sound_env, 0.1f);                        // volume actuel
        tmp2 = tmp*param*param*param*20.f;
        tmp2 = (tmp + tmp2) / (1.f + fabs(tmp2));             // new volume
        sound_out = sound_in * tmp2/tmp;        // compress

        sound_out *= _fclamp(1.f - param1 * (1.f-g_Modulation[curent_config.c_Modulation_Source[EFFECT2_MOD]]), 0.f, 1.f); // attenuation

        return sound_out;
	case 6 : //rien, utilisé lors du changement d'effet
		return sound_in;
    }
    return 0; //useless
}
