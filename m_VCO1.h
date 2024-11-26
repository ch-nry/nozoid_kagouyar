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

#define modulation_VCO1(VCO_mod, VCO_MOD)                                           \
    modulation_value = VCO_mod;                                                     \
    modulation_value *= g_Modulation[curent_config.c_Modulation_Source[VCO_MOD]];   \
																					\
    switch (curent_config.c_Modulation_Type[VCO_MOD]) {                             \
    case MOD_FM_exp :                                                               \
        VCO1_FM_exp += modulation_value ;                                           \
        break;                                                                      \
    case MOD_FM_Qtz :                                                               \
        VCO1_FM_Qtz += modulation_value ;                                           \
        break;                                                                      \
    case MOD_FM_lin :                                                               \
        VCO1_FM_lin += VCO_mod * modulation_value ;                                 \
        break;                                                                      \
    case MOD_AM :                                                                   \
        VCO1_AM -= VCO_mod - modulation_value;                                      \
        break;                                                                      \
    case MOD_PM :                                                                   \
        VCO1_PM += modulation_value ;                                               \
        break;                                                                      \
    case MOD_CLIP :                                                                 \
        VCO1_clip -= modulation_value + VCO_mod + _fmax(0.f, VCO_mod-0.5f)*4.0f;    \
        break;                                                                      \
    case MOD_WF :                                                                   \
        VCO1_mod_PWM += modulation_value;                                           \
        break;                                                                      \
    }

// used for multiple waveform
#define v_VCO1_filter1 v_VCO1_last[0]
#define v_VCO1_filter2 v_VCO1_last[1]

inline float VCO1(uint32_t j, float frequency) {

    float VCO1_AM = 1.f;
    float VCO1_FM_lin = 0.f;
    float VCO1_FM_exp = 0.f;
    float VCO1_FM_Qtz = 0.f;
    float VCO1_PM = 0.f;
    float VCO1_mod_PWM = 0.f;
    float VCO1_clip = 1.1f;
    float modulation_value;
    float PWM = g_pot_audio[k_VCO1_wfm];
	float tmpf;

    // Modulation
    modulation_VCO1(g_pot_audio[k_VCO1_mod1], VCO1_MOD1)
    modulation_VCO1(g_pot_audio[k_VCO1_mod2], VCO1_MOD2)
    modulation_VCO1(g_pot_audio[k_VCO1_mod3], VCO1_MOD3)

    float vco_pitch = frequency + 48.f * VCO1_FM_exp;
    vco_pitch += allvoice[j].v_pitch;
    vco_pitch += (int)((12.f*VCO1_FM_Qtz)+0.5f);

    VCO1_pitch(allvoice[j], vco_pitch); // sauve ou rapele la valeur de vco1 pour les pitch des vco2 et 3 syncro sur le 1 (cf fonction.h)
    float freq = CV2freq(vco_pitch) + VCO1_FM_lin * 2000.f;// freq peux etre negative, ou nulle

    float increment = freq*OneOverSR;
    increment += (increment == 0) * 1e-10; // increment ne doit pas etre nul car on a plein de /increment plus tard.
    float phase2, tmp, out=0.f;

    float VCO1_phase_local = wrap2(allvoice[j].v_VCO1_phase + increment);
    allvoice[j].v_VCO1_phase = VCO1_phase_local;

    g_Modulation[VCO1_SIN] = _cos(VCO1_phase_local); // g_Modulation sinus
    g_Modulation[VCO1_SQUARE] = (VCO1_phase_local > 0.5f)? 1.f : -1.f; // g_Modulation square
    g_Modulation[VCO1_TRI] = fabs(4.f*VCO1_phase_local-2.f)-1.f;
    float ramp = VCO1_phase_local + VCO1_phase_local - 1.f; // ramp (saw up)
    g_Modulation[VCO1_RAMP] = ramp;
    g_Modulation[VCO1_SAW] = -ramp; // saw down

    VCO1_PM *= 4.f;
    VCO1_phase_local += VCO1_PM;
    //increment =  abs(_floor(VCO1_phase_local-old_phase)); // should we compute this increment after PM?
    VCO1_phase_local -= _floor(VCO1_phase_local); // car on peux aller ds le negatif, ou aller au dela de 2 a cause des multiples modulations
	VCO1_phase_local = _fclamp(VCO1_phase_local, 0.f,1.f); // inutil, mais au cas ou...

    float PWM_local = _fclamp(PWM + VCO1_mod_PWM, 0.f, 1.f);

    switch(curent_config.c_VCO1_WF) {
    case 0 : //sin
        phase2 = _sin(VCO1_phase_local);
        _fonepole(allvoice[j].v_VCO1_filter1, phase2, 6000.f*OneOverSR);
        phase2 = _cos_loop(VCO1_phase_local + allvoice[j].v_VCO1_filter1 * PWM_local * 0.4f);
        _fonepole(allvoice[j].v_VCO1_filter2, phase2, 0.5f);
		out = allvoice[j].v_VCO1_filter2;
        break;
    case 1 : //multi sin
        phase2 = _sin(VCO1_phase_local);
        _fonepole(allvoice[j].v_VCO1_filter1, phase2, 600.f*OneOverSR);
        out = _cos_loop((0.7f+3.5f*PWM_local) * allvoice[j].v_VCO1_filter1 + 0.33f );
        break;
    case 2 : // tri
        out = tri_bl(VCO1_phase_local, increment, allvoice[j].v_VCO1_filter1);
        tmpf = 1.f - 0.5f*(PWM_local*(1.f+_cos(VCO1_phase_local))); // TODO : virer le cos et mettre un truc plus legé
        out +=1.f;
        out *= tmpf * tmpf;
        out -= 1.f;
        break;
    case 3 :  // rectangle
        //phase2 = VCO1_phase_local + (1.f-PWM_local)*0.5f;
        //if (phase2 >= 1.f) phase2 -= 1.f;
        phase2 = wrap(VCO1_phase_local + (1.f-PWM_local)*0.5f);
        out = (saw_bl(VCO1_phase_local,increment) - saw_bl(phase2,increment));
        break;
    case 4 :  // double saw
        phase2 = wrap(VCO1_phase_local + PWM_local*0.5f);
        out = (saw_bl(VCO1_phase_local,increment) + saw_bl(phase2, increment) ) / (2.f-PWM_local) ;
        break;
    case 5 :  // noise filter
        tmp = 2.f*_rnd_f()-1.f;
        _fonepole(allvoice[j].v_VCO1_filter1, tmp, abs(_fmin(increment*15.f, 1.f)));
        tmp = CV2freq(60.f + PWM_local * 60.f)*(1.f/13000.f);
        _fonepole(allvoice[j].v_VCO1_filter2, allvoice[j].v_VCO1_filter1, tmp);
        out = 2.f * (allvoice[j].v_VCO1_filter2-allvoice[j].v_VCO1_filter1);
        break;
    case 6 :  // noise downsampled
        if ( (wrap(4.f*VCO1_phase_local)) < abs(4.f*increment) ) {
            phase2 = 2.f*_rnd_f() -1.f;
            allvoice[j].v_VCO1_filter1 = phase2;
        }
        tmp = CV2freq( 0.f + (1.f-PWM_local) * 180.f) * (1.f/27000.f);
        _fonepole(allvoice[j].v_VCO1_filter2, allvoice[j].v_VCO1_filter1, tmp);
        out = allvoice[j].v_VCO1_filter2;
        break;
    case 7 : // random + interpolation lineaire
        tmp = wrap(4.f*VCO1_phase_local); // 4 time * frequency
        if ( (tmp/4.f) < increment ) {
            allvoice[j].v_VCO1_last[0] = allvoice[j].v_VCO1_last[1];
            allvoice[j].v_VCO1_last[1] = 2.f*(_rnd_f() - 0.5f);
        }
        out = mix( allvoice[j].v_VCO1_last[0], allvoice[j].v_VCO1_last[1], tmp); // linear interpolation
        out *= mix(1.f,out*out,PWM_local);
        out *= mix(1.f,out*out,PWM_local);
        out *= mix(1.f,out*out,PWM_local);
        break;
    case 8 : // logistic oscillator, no interpolation
        if ( (wrap(4.f*VCO1_phase_local)) < 4.f*increment ) {
            tmp = allvoice[j].v_VCO1_last[1];
            if ((tmp <= 0)||(tmp>=1)) tmp = _rnd_f(); //on peut avoir des nb qui sont en dehors des paramettres de la logistic, en venant d'une autre forme d'onde
            allvoice[j].v_VCO1_last[0] = tmp;
            PWM_local +=  PWM_local*PWM_local;
            tmp *= (3.45f+0.51f*PWM_local) * (1.f-tmp);
            allvoice[j].v_VCO1_last[1] = tmp;
        }
        out = 2.f*(allvoice[j].v_VCO1_last[1]) -1.f;
        break;
	case 9:
	    out=0.f;
	    allvoice[j].v_VCO1_last[0] = 0;
	    allvoice[j].v_VCO1_last[1] = 0;
	    break;
    }
    out *= VCO1_AM;

    out = _fclamp(out, -1.1f, VCO1_clip);

    g_Modulation[VCO1_OUT] = out;
    return out;
}
