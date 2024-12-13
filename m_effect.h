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

double g_effect1_phase;
double g_effect1_last_out = 0.;
double g_effect1_param_filter = 0.;
double g_effect1_param_filter2 = 0.;
double g_old_distance = 0., g_vitesse = 0., g_decole = 0., g_old_sound_out = 0.;

//daisysp::DelayLine<double, 48000*4> DSY_SDRAM_BSS g_delay_effect1; // SDRAM
//daisysp::DelayLine<double, 10000>  g_delay_effect3;

/*union Delay_line {
	daisysp::DelayLine<int16_t, 72000*2>  g_delay_effect1;
	daisysp::DelayLine<double, 72000>  g_delay_effect3;
} delayline ;
*/

union delay_line {
	double delay1_double[delay1_sizef];
	int16_t delay1_int[delay1_sizei];
} g_delay1;

uint32_t g_delay1_pos; // position ds le buffer

inline void delay1_clear(){
	for (int i=0; i<delay1_sizef; i++) g_delay1.delay1_double[i] = 0.;
	g_delay1_pos = 0;
}

inline void delay1_write_f(double in){
	int32_t l_delay1_pos = g_delay1_pos % delay1_sizef;
	g_delay1.delay1_double[l_delay1_pos] = in;
	l_delay1_pos += delay1_sizef - 1;
	l_delay1_pos %= delay1_sizef;
	g_delay1_pos = l_delay1_pos;
}

inline double delay1_read_f(double delay){
	int32_t delay_integral   = static_cast<int32_t>(delay);
	double   delay_fractional = delay - static_cast<double>(delay_integral);
	delay_integral += g_delay1_pos;
	const double a = g_delay1.delay1_double[(delay_integral) % delay1_sizef];
	const double b = g_delay1.delay1_double[(delay_integral + 1) % delay1_sizef];
	return a + (b - a) * delay_fractional;
}

inline void delay1_write_i(double  in){
	int32_t l_delay1_pos = g_delay1_pos; // % delay1_sizei// inutil car sizei > sizef
	g_delay1.delay1_int[l_delay1_pos] = f2s16(in/3.);
	l_delay1_pos += delay1_sizei - 1;
	l_delay1_pos %= delay1_sizei;
	g_delay1_pos = l_delay1_pos;
}

inline double delay1_read_i(double delay){
	int32_t delay_integral   = static_cast<int32_t>(delay);
	double   delay_fractional = delay - static_cast<double>(delay_integral);
	delay_integral += g_delay1_pos;
	const double a = s162f(g_delay1.delay1_int[(delay_integral) % delay1_sizei]);
	const double b = s162f(g_delay1.delay1_int[(delay_integral + 1) % delay1_sizei]);
	return 3.*(a + (b - a) * delay_fractional);
}


inline double effect1(double sound_in) { //, double wet, double param1, double param2) {
	double wet = g_pot_audio[k_EFFECT1_wet] += g_pot_increment[k_EFFECT1_wet];
	double param1 = _fclamp(g_pot_audio[k_EFFECT1_p1] += g_pot_increment[k_EFFECT1_p1], 0., 1.);
	double param2 = _fclamp(g_pot_audio[k_EFFECT1_p2] += g_pot_increment[k_EFFECT1_p2], 0., 1.);

    double sound_out = 0.;
    double tmp = 0.;
    double effect1_phase;

    double param2_mod = param2 * g_Modulation[curent_config.c_Modulation_Source[EFFECT1_MOD]];
    double wetM = _fclamp(wet + param2_mod, 0., 1.);
    double param1M = _fclamp(param1 + param2_mod, 0., 1.);
    switch(curent_config.c_EFFECT1_TYPE) {
    case 0 : // Wave shaper : OK
		// DRY
		// Waveform : a zero : on est sur une disto, a full sur un WS sauvage
		// DRY Modulation
		tmp = sound_in/(1. + 10.*param1)*150.*(wetM+1.)*wetM*wetM*wetM;
		tmp = (1.-param1)*(sound_in+tmp)/(1.+fabs(tmp));
		tmp += (1.-wetM)*param1*sound_in;
		tmp += wetM*param1*fabs(sound_in)*_sin_loop(5.*param1*sound_in*(1.+fabs(sound_in)));
		return tmp;
    case 1 : // ECHO : long delay (wet /  param1 : time / param2 : feedback) : OK
        tmp = ((delay1_sizei - 101.) * param1) + 100.;
        //delayline.g_delay_effect1.SetDelay(tmp);
        //sound_out = g_delay_effect1.Read();
        //delayline.g_delay_effect1.Write(_fclamp(sound_in + sound_out*param2, -3., 3.));
        sound_out = delay1_read_i(tmp);
        delay1_write_i(_fclamp(sound_in + sound_out*param2, -3., 3.));
        sound_out = mix(sound_in, sound_out, wet);
        return sound_out;
        //break;
    case 2 : // FREEZE : small delay (wet : feedback / param1 : time / param2 : time modulation) : flanger / chorus / doubler : OK
        param1M = _fclamp(param1 + param2_mod, 0., 1.);
        tmp = param1M * 50000. + 6.;
        //delayline.g_delay_effect1.SetDelay(tmp);
        //sound_out = mix(sound_in , g_delay_effect1.Read(), wet);
        //delayline.g_delay_effect1.Write(_fclamp(sound_out, -3., 3.));
        sound_out = mix(sound_in , delay1_read_f(tmp), wet);
		delay1_write_f(_fclamp(sound_out, -3., 3.));
        return sound_out;
        //break;

    case 3 : // KS (wet :  attenuation (gain/filtre) , param1 : frequence, param2 : mod frequence) : OK
        tmp = 48000./CV2req(param1M*127.);
        tmp += 5.;
        //g_delay_effect1.SetDelay(tmp);
        //tmp = g_delay_effect1.Read();
        tmp = delay1_read_f(tmp);
        _fonepole(g_effect1_last_out, tmp,_fmin(1.,CV2req(wet*127.)*(1./12000.)));
        sound_out = sound_in - g_effect1_last_out;

        if(sound_out >  1.) sound_out =  1. + _tanh_clip(sound_out - 1.); // soft clip with ID between -1 and 1;
        if(sound_out < -1.) sound_out = -1. + _tanh_clip(sound_out + 1.); // between -2 and 2;
        //g_delay_effect1.Write(_fclamp(sound_out, -3., 3.));
        delay1_write_f(_fclamp(sound_out, -3., 3.));
        return sound_out;
        //break;
    case 4 : // chorus : ( WET + feedback, TIME, WET modulation) : OK
		param1 *= 1200.;// 1/8 du temps max du chorus, en echantillons
        g_effect1_phase = wrap(g_effect1_phase + (0.005/48000.)); // LFO : vitesse de variation du temps du chorus
        effect1_phase = g_effect1_phase;
        sound_out = 0.; //sound_in;
        sound_out += delay1_read_f(1.634 + (fast_cos_loop(             effect1_phase*5.)+1.11) * param1);
        sound_out  -= delay1_read_f(5.285 + (fast_cos_loop(0.23 +  effect1_phase*4.)+2.31) * param1);
		sound_out += delay1_read_f(7.565 + (fast_cos_loop(0.57 + effect1_phase*3.)+3.22) * param1);
        sound_out  -= delay1_read_f(9.734 + (fast_cos_loop(0.71 + effect1_phase*7.)+4.13) * param1);
        sound_out *= 0.5;
        sound_out = sound_in + _tanh(wetM*sound_out);

        delay1_write_f(sound_out);
        return sound_out;
    case 5 : // ring delay : WET : amplitude du feedback; param1 : temps de delay ; param2 : frequence du ring
        //g_delay_effect1.SetDelay((24000. * param1 * (param1+1.)) + 50.);
		//sound_out = g_delay_effect1.Read();
		sound_out = delay1_read_f((24000. * param1 * (param1+1.)) + 50.);
        g_effect1_phase += OneOverSR + param2 * param2 * 400. * OneOverSR;
        tmp = wrap(g_effect1_phase);
		g_effect1_phase = tmp;
		sound_out *= _cos(tmp);
		sound_out *= wet*wet;
		sound_out += sound_in;
		//g_delay_effect1.Write(sound_out);
		delay1_write_f(sound_out);
        return sound_out;
        //break;
	case 6 : //frottement : WET; param1 : taille du frottement; param1 : modulation du wet
	// D= distance(in, out) ; si D-vitesse<0 alors decole = 0 et vitesse = 0  et out = in; si D> seuil alors decole = 1 ; si decole == 1 alors vitesse += gain*D  ; speedlim(vitesse) ; out += vitesse ;
		//g_old_sound_out += tmp  * 0.1 * wet * wet; // resiliance
		g_old_sound_out = sound_in - (1000.*wetM/(999.*wetM+1.)) * (sound_in - g_old_sound_out );
		tmp = sound_in - g_old_sound_out;
		g_old_distance = tmp;
		if ( (abs(tmp)-abs(g_vitesse)) < 0 ){ // la sortie se raproche de l'entrée : on reset tout, et on se colle
			g_decole = 0.;
			g_vitesse = 0.;
			g_old_sound_out = sound_in;
		}
		else if (abs(tmp) > (param1*0.5)) { // trop de diference entre entrée et sortie, on décole
			g_decole = 1.;
		}
		if ( g_decole == 1.) { // si decollé : a quelle vitesse on retourne pour recoller
			g_vitesse += 0.1* wetM*wetM*tmp;
			g_old_sound_out += g_vitesse;
		}
		return _fclamp(g_old_sound_out, -3., 3.);
	case 7 : //rien, utilisé lors du changement d'effet
		return sound_in;
    }
    return 0;
}

// --------------------------------------------------------------------------------------------
// EFFECT 2
// --------------------------------------------------------------------------------------------

double g_Effect2_filtre = 0.;
double g_effect2_sound_env = 0.;
double g_effect2_phase = 0.33;

daisysp::DelayLine<double, 48000> g_delay_effect2;

inline double effect2(double sound_in) { //, double param, double param1) {
	double param = g_pot_audio[k_EFFECT2_wet] += g_pot_increment[k_EFFECT2_wet];
	double param1 = g_pot_audio[k_EFFECT2_p1] += g_pot_increment[k_EFFECT2_p1];
    double sound_out;
    double tmp, tmp2;
	double effect2_phase;
    double wet = param + param1 * g_Modulation[curent_config.c_Modulation_Source[EFFECT2_MOD]];
    wet = _fclamp(wet, 0., 1.);

    switch(curent_config.c_EFFECT2_TYPE) {
    case 0: // disto : OK
        tmp = wet*(wet+1.);
        tmp = wet*wet;
        tmp = wet*wet*sound_in*150.;
        sound_out = (sound_in + tmp)/(1.+fabs(tmp));
        return sound_out;
     case 1: // WS : OK
        tmp = wet*15.;
        return (sound_in + tmp*tmp*_sin_positiv_loop(1000. + sound_in*fabs(0.25*sound_in*tmp))) / (tmp*tmp + 1.);
     case 2 : // BITCRUSH : OK
        wet = 0.01 + wet*0.99;
        sound_out = sound_in;
        //sound_out = sign(sound_out) * (1.5 - 1.5/(2.*fabs(sound_out) + 1.));
        sound_out = _floor(sound_out/wet)*wet;
        //sound_out = sign(sound_out) * (-0.5 - 0.75/(fabs(sound_out)-1.5)); // Pq ca ne marche pas???
        return mix(sound_in, sound_out, _fmin(1.,10.*wet));
    case 3: // auto doppler : on utilise le son comme temps de delay : OK
        g_delay_effect2.Write(sound_in);
        tmp = wet * 10000.;
        tmp *=  (1.+sound_in);
        _fonepole(g_Effect2_filtre, tmp, 0.001); // smooth le paramettre de temps et filtre le audio in
        g_delay_effect2.SetDelay((float)_fmax(1.,g_Effect2_filtre));
        sound_out = g_delay_effect2.Read();
        return sound_out;
    case 4 : // granular sub frequency generator : OK
        g_delay_effect2.Write(sound_in);
        effect2_phase = g_effect2_phase + 0.00020833333; // 100ms pour 1 grain
        if (effect2_phase>1.) {
            effect2_phase = effect2_phase-1.;
        }
        g_effect2_phase = effect2_phase;
        g_delay_effect2.SetDelay((float)(2400. * effect2_phase));
        sound_out  = g_delay_effect2.Read() * (1.-_cos(effect2_phase));

        effect2_phase = (effect2_phase > 0.5)? effect2_phase-0.5 : effect2_phase+0.5;
        g_delay_effect2.SetDelay((float)(2400.f * effect2_phase));
        sound_out  += g_delay_effect2.Read() *  (1.-_cos(effect2_phase));

        return mix(sound_in, sound_out*0.5, wet);
    case 5: // compresseur- attenuateur :
    //  qd pas de modulation, que faire avec param???
        tmp = fabs(sound_in);
        if (tmp > g_effect2_sound_env) {
            g_effect2_sound_env = mix(g_effect2_sound_env, tmp, 0.01); // temps de monté
        } else {
            g_effect2_sound_env = mix(g_effect2_sound_env, tmp, 0.001); // temps de descente
        }
        tmp = _fmax(g_effect2_sound_env, 0.1);                        // volume actuel
        tmp2 = tmp*param*param*param*20.;
        tmp2 = (tmp + tmp2) / (1. + fabs(tmp2));             // new volume
        sound_out = sound_in * tmp2/tmp;        // compress

        sound_out *= _fclamp(1. - param1 * (1.-g_Modulation[curent_config.c_Modulation_Source[EFFECT2_MOD]]), 0., 1.); // attenuation

        return sound_out;
	case 6 : //rien, utilisé lors du changement d'effet
		return sound_in;
    }
    return 0; //useless
}
