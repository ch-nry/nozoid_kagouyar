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

#define ADSR_overshotA 1.2f
#define ADSR_overshotR -0.1f

inline float ADSR_time2filter(float time) {
//    coeff = 1.0 / (time * sample_rate) ; where time is in seconds
// time*SR = 1 : fastest attack ever
// time*SR = 1 000 000 : 20s
    float tmp;
    tmp = CV2freq(time*(128.f+36.f) -36.3763f); // from 1 to 12 000
    tmp *= 80.; // from 80 to 1 000 000
    tmp -= 79; // from 1 to 1 000 000
    return 1 / tmp;
}

//float ADSR(uint32_t j, float A, float D, float S, float R) {
float ADSR(uint32_t j) {
	//float A = g_pot_audio[k_ADSR_a];
	//float D = g_pot_audio[k_ADSR_d];
	float S = g_pot_audio[k_ADSR_s];
	//float R = g_pot_audio[k_ADSR_r];
	
    float tmp=0.;
    float ADSR_out = allvoice[j].v_ADSR_out;
    float ADSR_goal = 0.f;;
    uint32_t ADSR_LOOP = curent_config.c_ADSR_LOOP; // local variable for optimisation
    uint32_t ADSR_mode = allvoice[j].v_ADSR_mode; // local variable

    g_Modulation_Reset[ADSR_OUT] = 0;
    if(!allvoice[j].v_GATE) {
        ADSR_mode = Release; // pas de gate, on est dc en release
    } else {
        if ( ADSR_mode == Release )
        {
            ADSR_mode = Attack;
            g_Modulation_Reset[ADSR_OUT] = 1;
        }
        if ( (ADSR_LOOP == 1) && ( ADSR_out < S+0.01f ) ) {
            // AD loop : on est pres du S (a la fin du D) : on retrig le A
            ADSR_mode = Attack;
        }
        if (ADSR_LOOP == 2) {
            if ( ADSR_out == 0.f) { // ADSR loop : on est a 0 : on retrig une attack
                ADSR_mode = Attack;
                g_Modulation_Reset[ADSR_OUT] = 1;
            }
            else if ( ( ADSR_out < S+0.01f ) && (ADSR_mode == Decay) ) // ADSR loop : on est pres du S (a la fin du D) : on passe en mode pseudo R : (R mais avec un v_GATE)
                ADSR_mode = Release2;
        }
    }

    switch (ADSR_mode) {
    case Attack :
        tmp = g_pot_audio[k_ADSR_a];
        ADSR_goal = ADSR_overshotA;
        break;
    case Decay :
        tmp = g_pot_audio[k_ADSR_d];
        ADSR_goal = S;
        break;
    case Release :
    case Release2 :
        tmp = g_pot_audio[k_ADSR_r];
        ADSR_goal = ADSR_overshotR;
        break;
    }

    tmp = ADSR_time2filter(tmp);
    _fonepole(allvoice[j].v_ADSR_out, ADSR_goal, tmp ); // the core of the ADSR
    ADSR_out = allvoice[j].v_ADSR_out;

    if ( ADSR_out >= 1.f ) {
        ADSR_out = 1.f;
        ADSR_mode = Decay;
    } else
        ADSR_out = _fmax(0.f,ADSR_out);

    allvoice[j].v_ADSR_out = ADSR_out;
    g_Modulation[ADSR_OUT] = ADSR_out; // on met la bonne ADSR ds les modulations
    g_Modulation[ADSR_OUT+modulation_source_last] = -ADSR_out; // modulation négative
    allvoice[j].v_ADSR_mode = ADSR_mode; // save local variable
    return ADSR_out;
}

inline float VCA(uint32_t j, float sound) { // each voice gain
    if (curent_config.c_VCA_TYPE == 0) {
        return sound * allvoice[j].v_ADSR_out * allvoice[j].v_ADSR_out;
    } else {
        _fonepole(allvoice[j].v_LPG_last, sound, _fmin(1.,CV2freq(-80.f + allvoice[j].v_ADSR_out * 208.f)*(1.f/12000.f)));
        return allvoice[j].v_LPG_last;
    }
}

