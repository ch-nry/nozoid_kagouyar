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

#define modulation_VCO1(VCO_mod, VCO_MOD)                                                      \
    {                                                                                          \
        const float mod_source = g_Modulation[curent_config.c_Modulation_Source[VCO_MOD]];    \
        const float mod_value = VCO_mod * mod_source;                                         \
        switch (curent_config.c_Modulation_Type[VCO_MOD]) {                                   \
        case MOD_FM_exp:                                                                       \
            VCO1_FM_exp += mod_value;                                                         \
            break;                                                                             \
        case MOD_FM_Qtz:                                                                       \
            VCO1_FM_Qtz += mod_value;                                                         \
            break;                                                                             \
        case MOD_FM_lin:                                                                       \
            VCO1_FM_lin += VCO_mod * mod_value;                                              \
            break;                                                                             \
        case MOD_AM:                                                                           \
            VCO1_AM -= VCO_mod - mod_value;                                                   \
            break;                                                                             \
        case MOD_PM:                                                                           \
            VCO1_PM += mod_value;                                                             \
            break;                                                                             \
        case MOD_CLIP:                                                                         \
            VCO1_clip -= mod_value + VCO_mod;                                                 \
            break;                                                                             \
        case MOD_WF:                                                                           \
            VCO1_mod_PWM += mod_value;                                                        \
            break;                                                                             \
        }                                                                                      \
    }

// used for multiple waveform
#define v_VCO1_filter1 v_VCO1_last[0]
#define v_VCO1_filter2 v_VCO1_last[1]

inline float VCO1(const uint32_t j, const float frequency) {
    float VCO1_AM = 1.f;
    float VCO1_FM_lin = 0.f;
    float VCO1_FM_exp = 0.f;
    float VCO1_FM_Qtz = 0.f;
    float VCO1_PM = 0.f;
    float VCO1_mod_PWM = 0.f;
    float VCO1_clip = 1.1f;

    const float PWM = g_pot_audio[k_VCO1_wfm];

    // Modulation
    modulation_VCO1(g_pot_audio[k_VCO1_mod1], VCO1_MOD1)
    modulation_VCO1(g_pot_audio[k_VCO1_mod2], VCO1_MOD2)
    modulation_VCO1(g_pot_audio[k_VCO1_mod3], VCO1_MOD3)

    // Calcul du pitch
    float vco_pitch = frequency + 48.f * VCO1_FM_exp + allvoice[j].v_pitch + (int)((12.f*VCO1_FM_Qtz)+0.5f);
    VCO1_pitch(allvoice[j], vco_pitch);

    // Calcul de la fréquence et de l'incrément
    const float freq = CV2freq(vco_pitch) + VCO1_FM_lin * 2000.f;
    float increment = freq * OneOverSR;
    increment = (increment == 0) ? 1e-10f : increment;
    increment = fabs(increment);

    // Phase et formes d'onde de base
    const float VCO1_phase_local = wrap2(allvoice[j].v_VCO1_phase + increment);
    allvoice[j].v_VCO1_phase = VCO1_phase_local;

    const float phase4 = 4.f * VCO1_phase_local;
    const float phase2 = 2.f * VCO1_phase_local;
    const float ramp = phase2 - 1.f;

    // Modulations de base
    g_Modulation[VCO1_SIN] = _cos(VCO1_phase_local);
    g_Modulation[VCO1_SQUARE] = (VCO1_phase_local > 0.5f) ? 1.f : -1.f;
    g_Modulation[VCO1_TRI] = fabs(phase4-2.f)-1.f;
    g_Modulation[VCO1_RAMP] = ramp;
    g_Modulation[VCO1_SAW] = -ramp;

    // Phase finale avec PM
    float final_phase = VCO1_phase_local + VCO1_PM * 4.f ;
    final_phase -= _floor(final_phase);
    final_phase = _fclamp(final_phase, 0.f, 1.f);

    // PWM local
    const float PWM_local = _fclamp(PWM + VCO1_mod_PWM, 0.f, 1.f);
    const float PWM2 = PWM_local * PWM_local;

    float out = 0.f;

    // Calcul de la forme d'onde
    switch (curent_config.c_VCO1_WF) {
    case 0: // sin
        {
            const float phase_sin = _sin(final_phase);
            _fonepole(allvoice[j].v_VCO1_filter1, phase_sin, 6000.f*OneOverSR);
            const float phase_cos = _cos_loop(final_phase + allvoice[j].v_VCO1_filter1 * PWM_local * 0.4f);
            _fonepole(allvoice[j].v_VCO1_filter2, phase_cos, 0.5f);
            out = allvoice[j].v_VCO1_filter2;
        }
        break;

    case 1: // multi sin
        {
            const float phase_sin = _sin(final_phase);
            _fonepole(allvoice[j].v_VCO1_filter1, phase_sin, 600.f*OneOverSR);
            out = _cos_loop((0.7f+3.5f*PWM_local) * allvoice[j].v_VCO1_filter1 + 0.33f);
        }
        break;

    case 2: // tri
        {
            const float cos_phase = fast_cos(final_phase);
            const float tmpf = 1.f - 0.5f*(PWM2*(1.f+cos_phase));
            const float tmpf2 = tmpf * tmpf;
            out = tri_bl(final_phase, increment, allvoice[j].v_VCO1_filter1);
            out = ((out + 1.f) * tmpf2 * tmpf2) - 1.f;
        }
        break;

    case 3: // rectangle
        {
            const float phase_shifted = wrap(final_phase + (1.f-PWM_local)*0.5f);
            out = saw_bl(final_phase,increment) - saw_bl(phase_shifted,increment);
        }
        break;

    case 4: // double saw
        {
            const float phase_shifted = wrap(final_phase + PWM_local*0.5f);
            out = (saw_bl(final_phase,increment) + saw_bl(phase_shifted, increment)) / (2.f-PWM_local);
        }
        break;

    case 5: // noise filter
        {
            const float rnd = 2.f*_rnd_f()-1.f;
            _fonepole(allvoice[j].v_VCO1_filter1, rnd, abs(_fmin(increment*15.f, 1.f)));
            const float filter_freq = CV2freq(60.f + PWM_local * 60.f)*(1.f/13000.f);
            _fonepole(allvoice[j].v_VCO1_filter2, allvoice[j].v_VCO1_filter1, filter_freq);
            out = 2.f * (allvoice[j].v_VCO1_filter2-allvoice[j].v_VCO1_filter1);
        }
        break;

    case 6: // noise downsampled
        {
            const float phase_wrapped = wrap(phase4);
            if (phase_wrapped < abs(4.f*increment)) {
                const float rnd = 2.f*_rnd_f() -1.f;
                allvoice[j].v_VCO1_filter1 = rnd;
            }
            const float filter_freq = CV2freq(0.f + (1.f-PWM_local) * 180.f) * (1.f/27000.f);
            _fonepole(allvoice[j].v_VCO1_filter2, allvoice[j].v_VCO1_filter1, filter_freq);
            out = allvoice[j].v_VCO1_filter2;
        }
        break;

    case 7: // random + interpolation lineaire
        {
            const float phase_wrapped = wrap(phase4);
            if ((phase_wrapped/4.f) < increment) {
                allvoice[j].v_VCO1_last[0] = allvoice[j].v_VCO1_last[1];
                allvoice[j].v_VCO1_last[1] = 2.f*(_rnd_f() - 0.5f);
            }
            out = mix(allvoice[j].v_VCO1_last[0], allvoice[j].v_VCO1_last[1], phase_wrapped);
            const float out2 = out * out;
            out *= mix(1.f, out2, PWM_local);
            out *= mix(1.f, out2, PWM_local);
            out *= mix(1.f, out2, PWM_local);
        }
        break;

    case 8: // logistic oscillator
        {
            if (wrap(phase4) < 4.f*increment) {
                float tmp = allvoice[j].v_VCO1_last[1];
                if ((tmp <= 0) || (tmp >= 1)) {
                    tmp = _rnd_f();
                }
                allvoice[j].v_VCO1_last[0] = tmp;
                const float pwm_squared = PWM_local + PWM_local*PWM_local;
                tmp *= (3.45f + 0.51f*pwm_squared) * (1.f-tmp);
                allvoice[j].v_VCO1_last[1] = tmp;
            }
            out = 2.f * allvoice[j].v_VCO1_last[1] - 1.f;
        }
        break;

    case 9:
        out = 0.f;
        allvoice[j].v_VCO1_last[0] = 0.f;
        allvoice[j].v_VCO1_last[1] = 0.f;
        allvoice[j].v_VCO1_phase = 0.f;
        break;
    }

    out *= VCO1_AM;
    out = _fclamp2(out, -1.1f, VCO1_clip);
    g_Modulation[VCO1_OUT] = out;
    return out;
}
