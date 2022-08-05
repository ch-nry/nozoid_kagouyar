//#if defined(_MSC_VER)
//#define FORCE_INLINE __forceinline /**< & */
//#elif defined(__clang__)
//#define FORCE_INLINE inline __attribute__((always_inline)) /**< & */
//#pragma clang diagnostic ignored "-Wduplicate-decl-specifier"
//#elif defined(__GNUC__)
//#define FORCE_INLINE inline __attribute__((always_inline)) /**< & */
//#else
//#error unknown compiler
//#endif

void UsbCallback(uint8_t* buf, uint32_t* len)
{
    for(size_t i = 0; i < *len; i++)
    {
    }
}


    // USB
    //hw.seed.usb_handle.Init(UsbHandle::FS_INTERNAL);
    //sprintf(buff, "hello;\n");
    //hw.seed.usb_handle.TransmitInternal((uint8_t*)buff, strlen(buff));
    //dsy_system_delay(500);
    //hw.seed.usb_handle.SetReceiveCallback(UsbCallback, UsbHandle::FS_INTERNAL);

        //sprintf(buff, "ping;\n");
        //hw.seed.usb_handle.TransmitInternal((uint8_t*)buff, strlen(buff));

// VCO macro : 
// les multiples IF sont plus lent
//#define modulation_VCO1(VCO_mod, VCO_MOD)                                           						\
//    modulation_value = VCO_mod;                                                     										\
//    modulation_value *= g_Modulation[curent_config.c_Modulation_Source[VCO_MOD]];   	\
//																																				\
// if(curent_config.c_Modulation_Type[VCO_MOD] & 0b100){ /* 4, 5 ou 6 */						\
//	 if(curent_config.c_Modulation_Type[VCO_MOD] & 0b10){ /* 6 */									\
//		VCO1_mod_PWM += modulation_value;                                           							\
//	} else { /* 4 ou 5*/																											\
//		if(curent_config.c_Modulation_Type[VCO_MOD] & 0b1){ /* 5 */									\
//			VCO1_clip -= modulation_value + VCO_mod + _fmax(0.f, VCO_mod-0.5f)*4.0f;   \
//		} else { /* 4 */																											\
//			VCO1_PM += modulation_value ;                                               							\
//		}																																	\
//	}																																		\
//} else { /* 0, 1, 2 ou 3*/																										\
//	if(curent_config.c_Modulation_Type[VCO_MOD] & 0b10){ /* 2 ou 3 */							\
//		if(curent_config.c_Modulation_Type[VCO_MOD] & 0b1){ /* 3 */									\
//			VCO1_AM -= VCO_mod - modulation_value;                                      					\
//		} else { /* 2 */																											\
//			VCO1_FM_lin += VCO_mod * modulation_value ;                                 					\
//		}																																	\
//	} else { /* 0 ou 1 */																										\
//		if(curent_config.c_Modulation_Type[VCO_MOD] & 0b1){ /* 1 */									\
//			VCO1_FM_Qtz += modulation_value ;                                           							\
//		} else { /* 0  */																											\
//			VCO1_FM_exp += modulation_value ;                                           							\
//		}																																	\
//	}																																		\
//}									

////////////////////////////////////
// VCF 

    // pas plus rapide avec des if
    /*
    if (curent_config.c_VCF1_TYPE >= 4){
		if (curent_config.c_VCF1_TYPE == 4){
			tmp = input1 - output1 - output1 + output2;
		} else { // == 5
			tmp = input1 - 4.f * (output1 + output3) + 6.f *  output2 + output4;
		}
	} else { // == 0, 1 ou 2
		if (curent_config.c_VCF1_TYPE == 2){
			tmp = output1 + output1 - output2 - output2;
		} else {// == 0 ou 1 
			if (curent_config.c_VCF1_TYPE == 0){
				tmp = output4;
			} else {// == 1 
				tmp = output2;
			}
		}
	} */
	
	
////////////////////////////////
// ADSR

/* // plus lent avec des if
	if(ADSR_mode <= 1) {
		if(ADSR_mode == 0) {
			tmp = g_pot_audio[k_ADSR_a];
			ADSR_goal = ADSR_overshotA;
		}
		else {
			tmp = g_pot_audio[k_ADSR_d];
			ADSR_goal = S;
		}
	}
	else {
		tmp = g_pot_audio[k_ADSR_r];
		ADSR_goal = ADSR_overshotR;
	}
*/

/////////////////////:
// effet audio

////////////////////////////////////////////////////////
/*
/*    case 4 : // chorus : ( WET + feedback, TIME, WET modulation) : OK
		//param1 *= 1200.f;// 1/8 du temps max du chorus, en echantillons 
        g_effect1_phase = wrap(g_effect1_phase + (0.2f*param1/48000.f)); // LFO : vitesse de variation du temps du chorus
        effect1_phase = g_effect1_phase;
        sound_out = 0.f; //sound_in;
        g_delay_effect3.SetDelay(1.f + (_cos_positiv_loop(             effect1_phase*5.f)+1.f) * 500.f);
        sound_out  += g_delay_effect3.Read();
        g_delay_effect3.SetDelay(1.f + (_cos_positiv_loop(0.23f + effect1_phase*4.f)+3.1f) * 500.f);
        sound_out -= g_delay_effect3.Read();
        g_delay_effect3.SetDelay(1.f + (_cos_positiv_loop(0.57f + effect1_phase*3.f)+5.2f) * 500.f);
        sound_out += g_delay_effect3.Read();
        g_delay_effect3.SetDelay(1.f + (_cos_positiv_loop(0.71f + effect1_phase*7.f)+7.3f) * 500.f);
        sound_out -= g_delay_effect3.Read();
        sound_out *= 0.25f;
        sound_out = sound_in + wetM*sound_out;
        g_delay_effect3.Write(_fclamp(sound_out, -3.f, 3.f)); // mettre une saturation plus propre
        return sound_out;
*/

    case 7 : // HARMO (-1, +1 v_pitch shift) : OK
        g_delay_effect3.Write(sound_in); // phase du grain 1
        effect1_phase = g_effect1_phase + 0.00020833333f; // 100ms pour 1 grain
        if (effect1_phase>1.f) {
            effect1_phase = effect1_phase-1.f;
            g_grain1_speed = 1.f - param1M;
        }
        g_effect1_phase = effect1_phase;

        // tmp = 1 : on va de 0 a 50
        // tmp = 0.5 : on va de 0 a 0
        // tmp = 0 : on va de 100 a 0
        // + tard : relation exponentiel plutot que lineaire... (13% d'erreur max)
        tmp = g_grain1_speed;
        tmp2 = 2.f * _fmin(0.f, tmp-0.5f);
        g_delay_effect3.SetDelay( 1.f + 48.f * (effect1_phase*(tmp-0.5f) * (1.f - tmp2) - tmp2 ) * 100.f );
        out  = g_delay_effect3.Read() * 0.5 * (1.f-_cos(effect1_phase));

        effect1_phase = (effect1_phase > 0.5f)? effect1_phase-0.5f : effect1_phase+0.5f;
        if(effect1_phase < 0.00020833333f) g_grain2_speed = 1.f - param1M;
        tmp = g_grain2_speed;
        tmp2 = 2.f * _fmin(0.f, tmp-0.5f);
        g_delay_effect3.SetDelay( 1.f + 48.f * (effect1_phase*(tmp-0.5f) * (1.f - tmp2) - tmp2 ) * 100.f );
        out  += g_delay_effect3.Read() * 0.5 * (1.f-_cos(effect1_phase));

        return mix(sound_in, out, wetM);
        //break;
    case 1 : // super Wave shaper
        tmp = wet*15.f;
        tmp2 = 0.1*tmp + sound_in*fabs(0.25*sound_in*tmp);
        tmp2 = mix(tmp2, _tanh_clip((0.5f*param2 + tmp2) * (1.f + (param1+0.1f) * param1 * 20.f)), _fmin(param1*10.f, 1.f));
        tmp2 = _sin_loop(tmp2+param2*0.25);
        tmp *= tmp;
        return (sound_in+tmp*tmp2)/(tmp+1.f);
        //return (sound_in + tmp*tmp*_sin_positiv_loop(1000.f + 0.1*tmp + sound_in*fabs(0.25*sound_in*tmp))) / (tmp*tmp + 1.f);

    case 0 : // DISTO : wet/type/wet modulation
        sound_out = _tanh_clip(sound_in * (1.f + (param1+0.1f) * param1 * 20.f));
        sound_out = mix(sound_in, sound_out, wetM);
        return sound_out;
      case 2 : // BITCRUSH : wet / bitcrush / downsample, sans modulation : OK
        param1M = 0.001f + param1*0.999f;
        sound_out = _floor(sound_in/param1M)*param1M;
        g_effect1_phase += CV2freq((1.f-param2)*127.f)*(1.f/12000.f);                      // calcul du phasor
        if (g_effect1_phase >=1.f) {
            g_effect1_phase = 0.f;
            g_effect1_last_out = sound_out;
            }
        return mix(sound_in, g_effect1_last_out, wet);
        //break;
        * 
    case 3 : //  DOUBLE ; modulation sur amplitude
        param1M = _fclamp(param1 + param2_mod, 0.f, 1.f);
        tmp = param1;// * param1M;
        tmp = tmp * 4800.f + 1.f; // 100ms max
        _fonepole(g_effect1_param_filter, tmp, data2audio_coef_filter);
        g_delay_effect1.SetDelay(g_effect1_param_filter);
        sound_out = sound_in + wetM * g_delay_effect1.Read();
        g_delay_effect1.Write(sound_in);
        return sound_out;
        //break;
    case 0: // disto, filtre lop, et modulation de la disto : OK
    // trouver un paramettre plus expressif sur le 2eme pot : WS en fold?, hysteresis?
        _fonepole(g_effect1_last_out, sound_in, CV2freq((1.f-param1)*127.f)*(1.f/12000.f));
        tmp = _fmax((wetM-0.1f),0.f)*20.f; // 0 to 18
        tmp2 = _fmin(wetM,0.1f)*10.f;
        sound_out = _tanh_clip(g_effect1_last_out * (tmp+1.5f));
        sound_out += sound_in - g_effect1_last_out;
        sound_out = mix(sound_in, sound_out, tmp2);
        return sound_out;
        //break;
    case 6 : // speedlim / hysteresys : OK, mais bof
        tmp = fabs(sound_in - g_effect1_last_out);
        tmp = (tmp >  wet*0.2f)? tmp-wet*0.2f: 0.f; // hysteresys
        tmp = _fmin(tmp, (1.f-param1M)*(1.f-param1M)*(1.f-param1M)); // speedlim
        if(g_effect1_last_out > sound_in) {
            g_effect1_last_out -= tmp;
        }
        else {
            g_effect1_last_out += tmp;
        }
        return g_effect1_last_out;
        //break;
    case 7 : // add bass : filtre (f=param1), add disto sur les basses (wet) et reforme le signal : OK mais 3 paramettres assez proche : a passer en effet2?
        _fonepole(g_effect1_last_out, sound_in, CV2freq(param1*100.f)*(1.f/48000.f));
        tmp = _tanh_clip(g_effect1_last_out* param1*5.f);
        sound_out = sound_in+mix(0.f,(tmp-g_effect1_last_out)*0.5f, wetM);
        return sound_out;
        //break;
    case 8 : // disto avec filtre pour disto seulement les basses, et mix avec l'original ???
        return 0.f;
        //break;
*/

/*
    case 3: // small delay (param : mix(in/out) / param1 : time) : on n'utilise pas wet
        tmp = 50.f + param1 * param1 * 48000.f;
        _fonepole(g_Effect2_filtre, tmp, data2audio_coef_filter); // smooth le paramettre de temps
        g_delay_effect2.SetDelay(g_Effect2_filtre);
        sound_out = sound_in + g_delay_effect2.Read();
        g_delay_effect2.Write(sound_out*param);
        return sound_out;
        //break;
    case 1 : // chorus
        g_delay_effect2.Write(sound_in);
        g_effect2_phase = wrap(g_effect2_phase + (0.005f/48000.f));
        g_delay_effect2.SetDelay(1.f + (_cos_positiv_loop(        g_effect2_phase*5.f)+1.f) * param1 * 200.f);
        sound_out  = g_delay_effect2.Read();
        g_delay_effect2.SetDelay(1.f + (_cos_positiv_loop(0.23f + g_effect2_phase*4.f)+1.f) * param1 * 200.f);
        sound_out += g_delay_effect2.Read();
        g_delay_effect2.SetDelay(1.f + (_cos_positiv_loop(0.57f  + g_effect2_phase*3.f)+1.f) * param1 * 200.f);
        sound_out += g_delay_effect2.Read();
        g_delay_effect2.SetDelay(1.f + (_cos_positiv_loop(0.71f + g_effect2_phase*7.f)+1.f) * param1 * 200.f);
        sound_out += g_delay_effect2.Read();
        sound_out *= 0.25f;
        return mix(sound_in, sound_out, param);
    case 2: // clip
        sound_out = sound_in * (1.f+5.f*wet);
        sound_out = _fclamp(sound_out, -1.f, 1.f);
        return sound_out;
    case 7: // disto : ne change pas grand chose
        tmp = wet*(wet+1.f);
        tmp = wet*wet;
        tmp = wet*wet*sound_in*150.f;
        sound_out = (sound_in + 0.3f*wet + tmp)/(1.f+fabs(tmp));
        return sound_out;
        //break;
    case 8: // tanh disto : OK
        tmp = _fmax((wet-0.1f),0.f)*20.f; // 0 to 18
        tmp2 = _fmin(wet,0.1f)*10.f;
        sound_out = _tanh_clip(sound_in * (tmp+1.5f));
        sound_out = mix(sound_in, sound_out, tmp2);
        return sound_out;
        //break;
    case 9: // tanh disto non symetrique : OK
        tmp = _fmax((wet-0.1f),0.f)*20.f; // 0 to 18
        tmp2 = _fmin(wet,0.1f)*10.f;
        sound_out = _tanh_clip(sound_in * (tmp+1.5f) + 0.7f * tmp2);
        sound_out = mix(sound_in, sound_out, tmp2);
        return sound_out;
        //break;
    case 10: // vca global???
        return sound_in;
    case 11 : delay chaos
    case 12 : WS : sin( b+(0.2+a)*in)
    * 
    *     case 5 : // add bass : filtre (f=param1), add disto sur les basses (wet) et reforme le signal
        _fonepole(g_Effect2_filtre, sound_in, 250.f/48000.f); // on garde les basses en dessous de 150Hz
        tmp = _tanh_clip(g_Effect2_filtre * wet*5.f); // on les distord
        sound_out = sound_in+mix(0.f,(tmp-g_Effect2_filtre), wet); // ajoute a l'original
        return sound_out;
        * 
*/

/////////////////////
// algo LFO


       //~ case LFO_RND : // moyenne et amplitude du bruit
            //~ {
                //~ uint32_t WF1_reset;
                //~ float l_mix_factor;
                //~ if (source_addresse != LFO1_OUT) { //automodulation
                    //~ WF1_reset = g_Modulation_Reset[source_addresse];
                    //~ l_mix_factor = mix_factor;
                //~ } else {
                    //~ if (g_LFO1_reset == false) {
                        //~ if (mix_factor > 0.51f) {
                            //~ WF1_reset = true;
                            //~ g_LFO1_reset = true;
                        //~ } else {
                            //~ WF1_reset = false;
                        //~ }
                    //~ } else {
                        //~ WF1_reset = false;
                        //~ if (mix_factor < 0.49f) {
                            //~ g_LFO1_reset = false;
                        //~ }
                    //~ }
                    //~ l_mix_factor = 1.f;
                //~ }
                //~ if (WF1_reset) {
                    //~ g_Modulation_Phase[LFO1_OUT] -= 1.f;
                    //~ g_LFO1_noise[1] = g_LFO1_noise[0];
                    //~ g_LFO1_noise[0] = (2.f*_rnd_f())-1.f;
                    //~ g_Modulation_Reset[LFO1_OUT] = 1;
                //~ } else
                    //~ g_Modulation_Reset[LFO1_OUT] = 0;

                //~ float tmp = Interpolation_Curve(g_Modulation_Phase[source_addresse], curent_config.c_LFO1_WF, g_LFO1_noise);
                //~ tmp = tmp * l_mix_factor;
                //~ tmp += fq + fq - 1.f;
                //~ modulation = _fclamp(tmp,-1.f,1.f);
                //~ g_Modulation_Phase[LFO1_OUT] = g_Modulation_Phase[source_addresse];
            //~ }
            //~ break;
       //~ case LFO_Chaos : // fq2 = frequence, mix_factor = variation speed
            //~ // remplacé par drunk
            //~ thomas(thomas_LFO1, 4.f*increment, 1.5f-(0.25*(mix_factor+1.f)*(WF1_1+1.f)*1.3f));
            //~ modulation = g_thomasX[thomas_LFO1] * 0.5f;
            //~ break;

////////////////////////////////
// audio waveform

/*   case 8 : // logistic oscillator,with linear interpolation between point
        phase2 = wrap(4.f*VCO1_phase_local);
        if ( phase2 < 4.f*increment ) {
            tmp = allvoice[j].v_VCO1_last[1];
            allvoice[j].v_VCO1_last[0] = tmp;
            tmp *= (3.4f+0.6f*PWM_local) * (1.f-tmp);
            allvoice[j].v_VCO1_last[1] = tmp;
        }
        out = 2.f*(mix( allvoice[j].v_VCO1_last[0], allvoice[j].v_VCO1_last[1], phase2)) -1.f; // linear interpolation
        break;
*/

//////////////////////////////
//fonction exponentiel
/*
// exp from 0 to 1 -> 1 to 2
// y(x) = 0.00189440x^5 + 0.00894068x^4 + 0.05587631x^3 + 0.24013186x^2 + 0.69315673x^1 + 0.99999977
inline float CV2freq_slow(float index) { // index from -128 to 139; 69 for 440Hz
    float midi = index + 144.f;
    midi *= 1.f/12.f;
    uint32_t i_midi = midi;
    midi -= i_midi;
    i_midi = -i_midi + 23;
    i_midi = 1 << i_midi;
    midi = 0.99999977f + midi*(0.69315673f + midi * (0.24013186f + midi * (0.05587631f + midi * (0.00894068f + midi * 0.00189440f))));
    midi *= 16744.f / (float)i_midi;
    return midi;
}
*/
/////////////////////////////////////
/*
float interpol4(float mu, float y0, float y1, float y2, float y3) {
    float a1, a2, a3;
    // 4-point, 3rd-order Hermite (x-form)
    a1 = 0.5f * (y2 - y0);
    a2 = y0 - 2.5f * y1 + y2 + y2 - 0.5f * y3;
    a3 = 0.5f * (y3 - y0) + 1.5f * (y1 - y2);
    return ((a3 * mu + a2) * mu + a1) * mu + y1;
}
*/
