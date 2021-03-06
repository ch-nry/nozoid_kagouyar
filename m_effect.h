float g_effect1_phase;
float g_effect1_last_out = 0.f;
float g_effect1_param_filter = 0.f;
float g_effect1_param_filter2 = 0.f;
float g_grain1_speed, g_grain2_speed;
float g_old_distance = 0.f, g_vitesse = 0.f, g_decole = 0.f, g_old_sound_out = 0.f;

daisysp::DelayLine<float, 48000*5> DSY_SDRAM_BSS g_delay_effect1; // SDRAM
daisysp::DelayLine<float, 10000>  g_delay_effect3; 	
			
inline float effect1(float sound_in) { //, float wet, float param1, float param2) {
	float wet = g_pot_audio[k_EFFECT1_wet] += g_pot_increment[k_EFFECT1_wet];
	float param1 = g_pot_audio[k_EFFECT1_p1] += g_pot_increment[k_EFFECT1_p1];
	float param2 = g_pot_audio[k_EFFECT1_p2] += g_pot_increment[k_EFFECT1_p2];

    float sound_out = 0.f;
    float tmp = 0;
    float effect1_phase;

    float param2_mod = param2 * g_Modulation[curent_config.c_Modulation_Source[EFFECT1_MOD]];
    float wetM = _fclamp(wet + param2_mod, 0.f, 1.f);
    float param1M = _fclamp(param1 + param2_mod, 0.f, 1.f);
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
        tmp = (239899.f * param1) + 100.f;
        g_delay_effect1.SetDelay(tmp);
        sound_out = g_delay_effect1.Read();
        g_delay_effect1.Write(_fclamp(sound_in + sound_out*param2, -3.f, 3.f));
        sound_out = mix(sound_in, sound_out, wet);
        return sound_out;
        //break;
    case 2 : // FREEZE : small delay (wet : feedback / param1 : time / param2 : time modulation) : flanger / chorus / doubler : OK
        param1M = _fclamp(param1 + param2_mod, 0.f, 1.f);
        tmp = param1M;// * param1M;
        tmp = tmp * 50000.f + 5.f; // 100ms max
        g_delay_effect1.SetDelay(tmp);
        sound_out = mix(sound_in , g_delay_effect1.Read(), wet);
        g_delay_effect1.Write(_fclamp(sound_out, -3.f, 3.f));
        return sound_out;
        //break;
    case 3 : // KS (wet :  attenuation (gain/filtre) , param1 : frequence, param2 : mod frequence) : OK
        tmp = 48000.f/CV2freq(param1M*127.f);
        tmp += 5.f;
        g_delay_effect1.SetDelay(tmp);
        tmp = g_delay_effect1.Read();
        _fonepole(g_effect1_last_out, tmp,_fmin(1.,CV2freq(wet*127.f)*(1.f/12000.f)));
        sound_out = sound_in - g_effect1_last_out;

        if(sound_out >  1.f) sound_out =  1.f + _tanh_clip(sound_out - 1.f); // soft clip with ID between -1 and 1;
        if(sound_out < -1.f) sound_out = -1.f + _tanh_clip(sound_out + 1.f); // between -2 and 2;
        g_delay_effect1.Write(_fclamp(sound_out, -3.f, 3.f));
        return sound_out;
        //break;
    case 4 : // chorus : ( WET + feedback, TIME, WET) : OK
		param1 *= 1200.f;// 1/8 du temps max du chorus, en echantillons 
        g_effect1_phase = wrap(g_effect1_phase + (0.005f/48000.f)); // LFO : vitesse de variation du temps du chorus
        effect1_phase = g_effect1_phase;
        sound_out = 0.f; //sound_in;
        g_delay_effect3.SetDelay(1.f + (_cos_positiv_loop(             effect1_phase*5.f)+1.f) * param1);
        sound_out  += g_delay_effect3.Read();
        g_delay_effect3.SetDelay(1.f + (_cos_positiv_loop(0.23f + effect1_phase*4.f)+3.f) * param1);
        sound_out -= g_delay_effect3.Read();
        g_delay_effect3.SetDelay(1.f + (_cos_positiv_loop(0.57f + effect1_phase*3.f)+5.f) * param1);
        sound_out += g_delay_effect3.Read();
        g_delay_effect3.SetDelay(1.f + (_cos_positiv_loop(0.71f + effect1_phase*7.f)+7.f) * param1);
        sound_out -= g_delay_effect3.Read();
        sound_out *= 0.33f;
        sound_out = sound_in + wetM*wetM*sound_out;
        g_delay_effect3.Write(_fclamp(sound_out, -3.f, 3.f));
        return sound_out;
    case 5 : // ring delay : WET : amplitude du feedback; param1 : temps de delay ; param2 : frequence du ring
        tmp = (24000.f * param1 * (param1+1.f)) + 50.f;
        g_delay_effect1.SetDelay(tmp);
        g_effect1_phase += OneOverSR + param2 * param2 * 400.f * OneOverSR;
        tmp = wrap(g_effect1_phase);
		g_effect1_phase = tmp;
		sound_out=(_cos(tmp));
		sound_out *= g_delay_effect1.Read();
		sound_out *= wet*wet;
		sound_out += sound_in;
		g_delay_effect1.Write(sound_out);
        return sound_out;
        //break;
	case 6 : //frottement : OK
	// D= distance(in, out)??; si D-vitesse<0 alors decole = 0 et vitesse = 0?? et out = in; si D> seuil alors decole = 1??; si decole == 1??alors vitesse += gain*D????; speedlim(vitesse)??; out += vitesse??;
		//g_old_sound_out += tmp  * 0.1 * wet * wet; // resiliance
		g_old_sound_out = sound_in - (1000.f*wetM/(999.f*wetM+1.f)) * (sound_in - g_old_sound_out );
		tmp = sound_in - g_old_sound_out;
		g_old_distance = tmp;
		if ( (abs(tmp)-abs(g_vitesse)) < 0 ){ // la sortie se raproche de l'entr??e : on reset tout, et on se colle
			g_decole = 0.f;
			g_vitesse = 0.f;
			g_old_sound_out = sound_in;
		}
		else if (abs(tmp) > (param1*0.5f)) { // trop de diference entre entr??e et sortie, on d??cole
			g_decole = 1.f;			
		}
		if ( g_decole == 1.f) { // si decoll?? : a quelle vitesse on retourne pour recoller
			g_vitesse += 0.1* wetM*wetM*tmp;
			g_old_sound_out += g_vitesse;
		}
		return _fclamp(g_old_sound_out, -3.f, 3.f);
    }
    return 0;
}

float g_Effect2_filtre = 0.f;
float g_effect2_sound_env = 0.f;
float g_effect2_phase = 0.33f;

daisysp::DelayLine<float, 48000> g_delay_effect2;

inline float effect2(float sound_in) { //, float param, float param1) {
	float param = g_pot_audio[k_EFFECT2_wet] += g_pot_increment[k_EFFECT2_wet];
	float param1 = g_pot_audio[k_EFFECT2_p1] += g_pot_increment[k_EFFECT2_p1];
    float sound_out;
    float tmp, tmp2;
	float effect2_phase;
    float wet = param + param1 * g_Modulation[curent_config.c_Modulation_Source[EFFECT2_MOD]];
    wet = _fclamp(wet, 0., 1.);
    
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
        return mix(sound_in, sound_out, fmin(1.f,10.f*wet));
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
        effect2_phase = g_effect2_phase + 0.00020833333f; // 100ms pour 1 grain
        if (effect2_phase>1.f) {
            effect2_phase = effect2_phase-1.f;
        }
        g_effect2_phase = effect2_phase;
        g_delay_effect2.SetDelay(2400.f * effect2_phase);
        sound_out  = g_delay_effect2.Read() * (1.f-_cos(effect2_phase));

        effect2_phase = (effect2_phase > 0.5f)? effect2_phase-0.5f : effect2_phase+0.5f;
        g_delay_effect2.SetDelay(2400.f * effect2_phase);
        sound_out  += g_delay_effect2.Read() *  (1.f-_cos(effect2_phase));

        return mix(sound_in, sound_out*0.5, wet);        
    case 5: // compresseur- attenuateur :
        tmp = fabs(sound_in);
        if (tmp > g_effect2_sound_env) {
            g_effect2_sound_env = mix(g_effect2_sound_env, tmp, 0.01f); // temps de mont??
        } else {
            g_effect2_sound_env = mix(g_effect2_sound_env, tmp, 0.001f); // temps de descente
        }
        tmp = _fmax(g_effect2_sound_env, 0.1f);                        // volume actuel
        tmp2 = tmp*param*param*param*20.f;
        tmp2 = (tmp + tmp2) / (1.f + fabs(tmp2));             // new volume
        sound_out = sound_in * tmp2/tmp;        // compress
        
        sound_out *= _fclamp(1.f - param1 * (1.f-g_Modulation[curent_config.c_Modulation_Source[EFFECT2_MOD]]), 0.f, 1.f); // attenuation
        
        return sound_out;
    }
    return 0; //useless
}
