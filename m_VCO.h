
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
	uint32_t ua, ub;

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
	case 9 : //atan(sin)
        phase2 = _sin(VCO_phase);
        _fonepole(v_VCO_last[0], phase2, 6000.f*OneOverSR);
        out = _tanh_clip ( phase2 * (1.f + 12.f*PWM_local*PWM_local));
        break;
    case 10 :
    	out = wrap2(VCO_phase + PWM_local * _sin_loop(VCO_phase * 1.5f));
		out = _tanh_clip(_sin_loop(out)*(1.f+3.f*PWM_local));
        break;
    case 11 : // sin(clip(phase*gain));
		out = _sin_loop(_fclamp(VCO_phase * (1.f + 3.f * PWM_local), 0.f, 1.f));
        break;
    case 12 :  // sin(mix(phase, sin(phase), param);
        out = _sin_loop(mix(VCO_phase, _sin(VCO_phase),PWM_local));
        break;
    case 13 : //fb    y = sin(phase + feedback * prevY); prevY = y;
        out = _sin_loop(VCO_phase + 0.6 * PWM_local * v_VCO_last[0]);
        v_VCO_last[0] = out;
        break;
    case 14 : // quantizer la phase : floorf(phase * steps) / steps;
		phase2 = _floor(1./(0.3*PWM_local+0.001f));
        out = _sin_loop( _floor(VCO_phase * phase2)/phase2);
        break;
    case 15 : //
		fa = VCO_phase * (0.5f + PWM_local * 3.0f);
		fa = fa - floorf(fa);
		fa = fabsf(2.0f * fa - 1.0f);
		out = _sin_loop(fa);
        break;
    case 16 : // table rnd et boucler dessus : TODO : mvt moins brusque
		fa = 7.f * VCO_phase;
		fb = v_VCO_last[0];
		v_VCO_last[0] = fa;
		ua = (int)fa;
		ub = (int)fb;
		if( int(ua !=ub) && (PWM_local >_rnd_f()*1000.f ) ) { v_VCO_last[ua] = 2.f * _rnd_f() -1.f; }
		out = v_VCO_last[ua];
        break;
    case 17 : // sin noise : ok
		fa = wrap(4.f * VCO_phase);
		if( fa < increment * 4.f) {
			v_VCO_last[0] = v_VCO_last[1];
			v_VCO_last[1] = v_VCO_last[2];
			v_VCO_last[2] = v_VCO_last[3];
			fb = v_VCO_last[0] + PWM_local * PWM_local * 2.f * (_rnd_f() -0.5f);
			fb = 2.f * wrap2(fb/2.f);
			fb = 1. - fabsf(fb-1.f);
			v_VCO_last[3]  = -1.f * sign(v_VCO_last[1]) * fb;
		}
		out = interpol4(fa, v_VCO_last[0], v_VCO_last[1], v_VCO_last[2], v_VCO_last[3]);
        break;
    case 18 : // squarenoise
        fa = wrap(2.f * VCO_phase);
		if ( fa < 2.f * increment) {
			fb = v_VCO_last[0];
			v_VCO_last[0] = -v_VCO_last[1];
			fb += PWM_local * PWM_local * 2.f * (_rnd_f() -0.5f);
			fb = 2.f * wrap2(fb/2.f);
			fb = 1. - fabsf(fb-1.f);
			v_VCO_last[1]  = -fabs(fb);
		}
		out = v_VCO_last[0];// + fa * ( v_VCO_last[1] - v_VCO_last[0]);
        break;
    case 19 : // sawnoise
		if ( VCO_phase < increment) {
			v_VCO_last[0] = mix(v_VCO_last[0], _rnd_f() * -1, PWM_local);
			v_VCO_last[1] =  mix(v_VCO_last[1], _rnd_f(), PWM_local);
		}
		out = v_VCO_last[0] + VCO_phase * ( v_VCO_last[1] - v_VCO_last[0]);
        break;
    case 20 : //
		fa = wrap(4.f*VCO_phase);
		if( fa < increment*4.f) {
			v_VCO_last[0] = v_VCO_last[1];
			v_VCO_last[1] = v_VCO_last[2];
			v_VCO_last[2] = v_VCO_last[3];
			fb = v_VCO_last[0] + PWM_local * PWM_local * 2.f * (_rnd_f() -0.5f);
			fb = 2.f * wrap2(fb/2.f);
			fb = 1. - fabsf(fb-1.f);
			v_VCO_last[3]  = fb;
		}
		out = interpol4(fa, v_VCO_last[0], v_VCO_last[1], v_VCO_last[2], v_VCO_last[3]);
		out = 2.f * out - 1.f;
        break;
    }
    return out;
}
