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

#define modulation_VCO1(VCO_mod, VCO_MOD)																	\
    modulation_value = VCO_mod; 																							\
    modulation_value *= g_Modulation[curent_config.c_Modulation_Source[VCO_MOD]];		\
    switch (curent_config.c_Modulation_Type[VCO_MOD]) {													\
    case MOD_FM_exp :																											\
        VCO1_FM_exp += modulation_value;																				\
        break;																																\
    case MOD_FM_Qtz :																											\
        VCO1_FM_Qtz += modulation_value;																				\
        break;																																\
    case MOD_FM_lin :																												\
        VCO1_FM_lin += VCO_mod * modulation_value;															\
        break;                                                                      \
    case MOD_AM :                                                                   \
        VCO1_AM -= VCO_mod - modulation_value;                                      \
        break;                                                                      \
    case MOD_PM :                                                                   \
        VCO1_PM += modulation_value ;                                               \
        break;                                                                      \
    case MOD_CLIP :                                                                 \
        VCO1_clip -= modulation_value + VCO_mod;    \
        break;                                                                      \
    case MOD_WF :                                                                   \
        VCO1_mod_PWM += modulation_value;                                           \
        break;                                                                      \
    }

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

    // Modulation
    modulation_VCO1(g_pot_audio[k_VCO1_mod1], VCO1_MOD1)
    modulation_VCO1(g_pot_audio[k_VCO1_mod2], VCO1_MOD2)
    modulation_VCO1(g_pot_audio[k_VCO1_mod3], VCO1_MOD3)

    float vco_pitch = frequency + 48.f * VCO1_FM_exp;
    vco_pitch += allvoice[j].v_pitch;
    vco_pitch += (int)((24.f*VCO1_FM_Qtz)+0.5f);

    VCO1_pitch(allvoice[j], vco_pitch); // sauve ou rapele la valeur de vco1 pour les pitch des VCO1 et 3 syncro sur le 1 (cf fonction.h)
    float freq = CV2freq(vco_pitch) + VCO1_FM_lin * 2000.f; // attention, la freq peux etre negative, ou nulle

    float increment = freq*OneOverSR;

	if (!isfinite(increment)) {  increment = 0.0f; } // Détecte NaN, +inf, -inf

	float VCO1_phase_local = wrap2(allvoice[j].v_VCO_phase[VCO1_Id] + increment);
    allvoice[j].v_VCO_phase[VCO1_Id] = VCO1_phase_local;

    float  out;

    g_Modulation[VCO1_SIN] = _cos(VCO1_phase_local); // g_Modulation sinus
    g_Modulation[VCO1_SQUARE] = (VCO1_phase_local > 0.5f)? 1.f : -1.f; // g_Modulation square
    g_Modulation[VCO1_TRI] = fabsf(4.f*VCO1_phase_local-2.f)-1.f;
    float const ramp = VCO1_phase_local + VCO1_phase_local - 1.f; // ramp (saw up)
    g_Modulation[VCO1_RAMP] = ramp;
    g_Modulation[VCO1_SAW] = -ramp; // saw down

	increment = fabsf(increment); // pour la FM, si increment est negatif cela pose des pb partout
	//increment = fmaxf(increment, 1e-6f); // Au lieu de 1e-10

    VCO1_PM *= 4.f;
    VCO1_phase_local += VCO1_PM;
    VCO1_phase_local = wrap2(VCO1_phase_local); // car on peux aller ds le negatif, ou aller au dela de 2 a cause des multiples modulations

    float PWM_local = _fclamp(PWM + VCO1_mod_PWM*0.5f, 0.f, 1.f);

	out = VCO_WF(curent_config.c_VCO1_WF, VCO1_phase_local, increment, PWM_local, allvoice[j].v_VCO_last[VCO1_Id]);

	if (curent_config.c_VCO1_WF >= 18) {
		allvoice[j].v_VCO_last[VCO1_Id][0] = _rnd_f();
		allvoice[j].v_VCO_last[VCO1_Id][1] = 0.f;
		allvoice[j].v_VCO_last[VCO1_Id][2] = 0.f;
		allvoice[j].v_VCO_last[VCO1_Id][3] = 0.f;
		allvoice[j].v_VCO_last[VCO1_Id][4] = 0.f;
		allvoice[j].v_VCO_last[VCO1_Id][5] = 0.f;
		allvoice[j].v_VCO_last[VCO1_Id][6] = 0.f;
		allvoice[j].v_VCO_last[VCO1_Id][7] = 0.f;
	    allvoice[j].v_VCO_phase[VCO1_Id] = 0.;
	    g_Modulation[VCO1_OUT] = 0.f;
	    g_Modulation[VCO1_SIN] = 0.f;
		g_Modulation[VCO1_SQUARE] = 0.f;
		g_Modulation[VCO1_TRI] = 0.f;
		g_Modulation[VCO1_RAMP] = 0.f;
		g_Modulation[VCO1_SAW] = 0.f;
	}

    out *= VCO1_AM;

    out = _fclamp2(out, -1.1f, VCO1_clip); // on peut etre en dessous du minimum, dc il faut faire attention a l'ordre des min/max

	allvoice[j].v_VCO_last[VCO1_Id][0] = _fclamp2(allvoice[j].v_VCO_last[VCO1_Id][0], -1.1f, 1.1f);
	allvoice[j].v_VCO_last[VCO1_Id][1] = _fclamp2(allvoice[j].v_VCO_last[VCO1_Id][1], -1.1f, 1.1f);
	allvoice[j].v_VCO_phase[VCO1_Id] = _fclamp2(allvoice[j].v_VCO_phase[VCO1_Id], 0.f, 1.f);

	if (!isfinite(out) || !isfinite(allvoice[j].v_VCO_last[VCO1_Id][0]) ||!isfinite(allvoice[j].v_VCO_last[VCO1_Id][1]) || !isfinite(allvoice[j].v_VCO_phase[VCO1_Id] ) )
	{  out = 0.0f; allvoice[j].v_VCO_last[VCO1_Id][0] = 0.0f; allvoice[j].v_VCO_last[VCO1_Id][1] = 0.0f; allvoice[j].v_VCO_phase[VCO1_Id] = 0.0f;  } // Détecte NaN, +inf, -inf

    g_Modulation[VCO1_OUT] = out;
    return out;
}
