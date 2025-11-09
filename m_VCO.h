
// --------------- VCO -------------------
// diference entre VCO1 et les 2 autres :

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

// --------------- VCO -------------------
// forme d'onde

float VCO_WF(uint32_t VCO_WF, float VCO_phase, float increment, float PWM_local, float* v_VCO_last)
{
	float out = 0;
	float phase2, tmp, fa, fb;
	uint32_t ua;

    switch(VCO_WF) {
    case 0 : //sin
        phase2 = _sin(VCO_phase);
        _fonepole(v_VCO_last[0], phase2, 6000.f*OneOverSR);
        phase2 = _cos_loop(VCO_phase + v_VCO_last[0] * PWM_local * 0.4f);
        _fonepole(v_VCO_last[1], phase2, 0.5f);
		out = v_VCO_last[1];
        break;
    case 1 : //multi sin
        phase2 = _sin(VCO_phase);
        _fonepole(v_VCO_last[0], phase2, 600.f*OneOverSR);
        out = _cos_loop((0.7f+3.5f*PWM_local) * v_VCO_last[0] + 0.33f );
        break;
    case 2 : // tri
    	float tmpf;
        tmpf = tri_bl(VCO_phase, increment, v_VCO_last[0]);
        out = tmpf + fast_cos(VCO_phase);
        out = tmpf + 7.f * PWM_local * out;
        break;
    case 3 :  // rectangle
        phase2 = wrap(VCO_phase + (1.f-PWM_local)*0.5f);
        out = (saw_bl(VCO_phase,increment) - saw_bl(phase2,increment));
        break;
    case 4 :  // double saw
        phase2 = wrap(VCO_phase + PWM_local*0.5f);
        out = (saw_bl(VCO_phase,increment) + saw_bl(phase2, increment) ) / (2.f-PWM_local) ;
        break;
    case 5 :  // noise filter
        tmp = 2.f*_rnd_f()-1.f;
        _fonepole(v_VCO_last[0], tmp, abs(fminf(increment*15.f, 1.f)));
        tmp = CV2freq(60.f + PWM_local * 60.f)*(1.f/13000.f);
        _fonepole(v_VCO_last[1], v_VCO_last[0], tmp);
        out = 2.f * (v_VCO_last[1]-v_VCO_last[0]);
        break;
    case 6 :  // noise downsampled
        if ( (wrap(4.f*VCO_phase)) < abs(4.f*increment) ) {
            phase2 = 2.f*_rnd_f() -1.f;
            v_VCO_last[0] = phase2;
        }
        tmp = CV2freq( 0.f + (1.f-PWM_local) * 180.f) * (1.f/27000.f);
        _fonepole(v_VCO_last[1], v_VCO_last[0], tmp);
        out = v_VCO_last[1];
        break;
    case 7 : // random + interpolation lineaire
        tmp = wrap(4.f*VCO_phase); // 4 time * frequency
        if ( (tmp/4.f) < increment ) {
            v_VCO_last[0] = v_VCO_last[1];
            v_VCO_last[1] = 2.f*(_rnd_f() - 0.5f);
        }
        out = mix( v_VCO_last[0], v_VCO_last[1], tmp); // linear interpolation
        out *= mix(1.f,out*out,PWM_local);
        out *= mix(1.f,out*out,PWM_local);
        out *= mix(1.f,out*out,PWM_local);
        break;
    case 8 : // logistic oscillator, no interpolation
        if ( (wrap(4.f*VCO_phase)) < 4.f*increment ) {
            tmp = v_VCO_last[1];
            if ((tmp <= 0)||(tmp>=1)) tmp = _rnd_f(); //on peut avoir des nb qui sont en dehors des paramettres de la logistic, en venant d'une autre forme d'onde
            v_VCO_last[0] = tmp;
            PWM_local =  PWM_local*PWM_local;
            tmp *= (3.7f+0.5f*PWM_local) * (1.f-tmp);
            v_VCO_last[1] = tmp;
        }
        out = 2.f*(v_VCO_last[1]) -1.f;
        break;
	case 9 : // 0bis : atan(sin)
        phase2 = _sin(VCO_phase);
        _fonepole(v_VCO_last[0], phase2, 10000.f*OneOverSR);
        out = _tanh_clip ( PWM_local + phase2 * (1.f + 12.f*PWM_local*PWM_local));
        break;
    case 10 : // 1 bis : sin wrap phase
    	out = wrap2(VCO_phase + PWM_local * _sin_loop(VCO_phase * 1.5f));
		out = _tanh_clip(_sin_loop(out)*(1.f+3.f*PWM_local));
        break;
    case 11 : // 2 bis : TODO : tenter pd example E06 / F3
		fa = VCO_phase * (0.5f + PWM_local * 3.0f);
		fa = fa - floorf(fa);
		fa = fabsf(2.0f * fa - 1.0f);
		out = _sin_loop(fa);
        break;
    case 12 :  // 3 bis : squarenoise
        fa = wrap(2.f * VCO_phase);
		if ( fa < 2.f * increment) { v_VCO_last[0] = -1 * sign(v_VCO_last[0]) * mix(fabsf(v_VCO_last[0]), _rnd_f(), PWM_local); }
		out = v_VCO_last[0];
        break;
    case 13 : // 4 bis : sawnoise
		if ( VCO_phase < increment) {
			v_VCO_last[0] = mix(v_VCO_last[0], _rnd_f() * -1, PWM_local);
			v_VCO_last[1] =  mix(v_VCO_last[1], _rnd_f(), PWM_local);
		}
		out = v_VCO_last[0] + VCO_phase * ( v_VCO_last[1] - v_VCO_last[0]);
        break;
    case 14 : // 5 bis :
		fa = wrap(3.f * VCO_phase);
		if( fa < increment * 3.f) {
			v_VCO_last[0] = v_VCO_last[1];
			v_VCO_last[1] = v_VCO_last[2];
			v_VCO_last[2] = v_VCO_last[3];
			fb = (v_VCO_last[0]+1.f)*0.5f + PWM_local * PWM_local * 2.f *(_rnd_f()-0.5);
			fb = fabs(fb);
			fb = 1. - fabsf(fb-1.f);
			fb = fb * 2.f - 1.f;
			v_VCO_last[3]  = fb;
		}
		out = interpol4(fa, v_VCO_last[0], v_VCO_last[1], v_VCO_last[2], v_VCO_last[3]);
        break;
    case 15 : // 6 bis : quantizer la phase : floorf(phase * steps) / steps;
		phase2 = _floor(1./(0.3*PWM_local+0.001f));
        out = _sin_loop( _floor(VCO_phase * phase2)/phase2);
        break;
    case 16 : // 7 bis :  table rnd et boucler dessus :
		fa = 8.f * VCO_phase;
		ua = (int)fa;
		if( wrap(fa) < increment*8.f) { v_VCO_last[ua] = mix(v_VCO_last[ua], 2.f * _rnd_f() -1.f, PWM_local); }
		out = interpol4(wrap(fa), v_VCO_last[ua], v_VCO_last[(ua+1)%8], v_VCO_last[(ua+2)%8], v_VCO_last[(ua+3)%8]);
        break;
    case 17 : // 8 bis
		fa = (wrap(4.f*VCO_phase)) ;
        if ( fa < 4.f*increment ) {
            tmp = v_VCO_last[1];
            if ((tmp <= 0)||(tmp>=1)) tmp = _rnd_f(); //on peut avoir des nb qui sont en dehors des paramettres de la logistic, en venant d'une autre forme d'onde
            v_VCO_last[0] = tmp;
            PWM_local =  PWM_local*PWM_local;
            tmp *= (3.7f+0.5f*PWM_local) * (1.f-tmp);
            v_VCO_last[1] = tmp;

   			v_VCO_last[2] = v_VCO_last[3];
			v_VCO_last[3] = v_VCO_last[4];
			v_VCO_last[4] = v_VCO_last[5];
			v_VCO_last[5] = v_VCO_last[1];
        }
        out = interpol4(fa, v_VCO_last[2], v_VCO_last[3], v_VCO_last[4], v_VCO_last[5]);
        out = 2.f*out -1.f;
        break;
    }
    return out;
}
