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

#define ADSR_overshotA 1.2
#define ADSR_overshotR -0.1

inline double ADSR_time2ilter(double time) {
//    coeff = 1.0 / (time * sample_rate) ; where time is in seconds
// time*SR = 1 : fastest attack ever
// time*SR = 1 000 000 : 20s
    double tmp;
    tmp = CV2req(time*(128.+36.) -36.3763); // from 1 to 12 000
    tmp *= 80.; // from 80 to 1 000 000
    tmp -= 79; // from 1 to 1 000 000
    return 1 / tmp;
}

//double ADSR(uint32_t j, double A, double D, double S, double R) {
double ADSR(uint32_t j) {
	//double A = g_pot_audio[k_ADSR_a];
	//double D = g_pot_audio[k_ADSR_d];
	double S = g_pot_audio[k_ADSR_s];
	//double R = g_pot_audio[k_ADSR_r];

    double tmp=0.;
    double ADSR_out = allvoice[j].v_ADSR_out;
    double ADSR_goal = 0.;
    uint32_t ADSR_LOOP = curent_config.c_ADSR_LOOP; // local variable for optimisation
    uint32_t ADSR_mode = allvoice[j].v_ADSR_mode; // local variable

    g_Modulation_Reset[ADSR_OUT] = 0;
    switch (allvoice[j].v_GATE ) {
		case 0: // pas de gate
			ADSR_mode = Release; // on est dc en release
		break;
		case 1 : // un gate vient d'arriver
			allvoice[j].v_GATE = 2; // pour la suite, le gate ne sera pas nvx
			ADSR_mode = Attack; // on relance l'attaque qq soit l'etat ds lequel on etait precedement
			g_Modulation_Reset[ADSR_OUT] = 1;
		// break; pas de break, les tests suivant sont valable tout le temps
		case 2 : // un gate est present, mais ce n'est pas nvx
			if ( (ADSR_LOOP == 1) && ( ADSR_out < (S+0.01) ) ) {
			// AD loop : on est pres du S (a la fin du D) : on retrig le A
			// il se peut qu'on retrig l'attaque si on est deja en attaque : c'est pas grave
			// si on est en release, on ne peut pas arriver la
				ADSR_mode = Attack;
			}
			if (ADSR_LOOP == 2) {
				if ( ADSR_out == 0.) { // ADSR loop : on est a 0 : on retrig une attack
					ADSR_mode = Attack;
					//g_Modulation_Reset[ADSR_OUT] = 1;
				}
				else if ( ( ADSR_out < (S+0.01) ) && (ADSR_mode == Decay) ) // ADSR loop : on est pres du S (a la fin du D) : on passe en mode pseudo R : (R mais avec un v_GATE)
				ADSR_mode = Release2;
			}
		break;
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

    tmp = ADSR_time2ilter(tmp);
    _fonepole(allvoice[j].v_ADSR_out, ADSR_goal, tmp ); // the core of the ADSR
    ADSR_out = allvoice[j].v_ADSR_out;

    if ( ADSR_out >= 1. ) {
        ADSR_out = 1.;
        ADSR_mode = Decay;
    } else
        ADSR_out = _fmax(0.,ADSR_out);

    allvoice[j].v_ADSR_out = ADSR_out;
    g_Modulation[ADSR_OUT] = ADSR_out; // on met la bonne ADSR ds les modulations
    g_Modulation[ADSR_OUT+modulation_source_last] = -ADSR_out; // modulation négative
    allvoice[j].v_ADSR_mode = ADSR_mode; // save local variable
    return ADSR_out;
}

inline double VCA(uint32_t j, double sound) { // each voice gain
    if (curent_config.c_VCA_TYPE == 0) {
        return sound * allvoice[j].v_ADSR_out * allvoice[j].v_ADSR_out * allvoice[j].amplitude;
;
    } else {
        _fonepole(allvoice[j].v_LPG_last, sound, _fmin(1.,CV2req(-80. + allvoice[j].v_ADSR_out * 208.)*(1./12000.)));
        return allvoice[j].v_LPG_last * allvoice[j].amplitude;
    }
}

