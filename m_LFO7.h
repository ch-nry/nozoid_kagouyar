float LFO4_INC, LFO5_INC, LFO6_INC;
float LFO1_FQ, LFO2_FQ, LFO3_FQ;
float LFO1_INC, LFO2_INC, LFO3_INC;
float LFO1_MIX, LFO2_MIX, LFO3_MIX;
float LFO7_INC, LFO7_WF, LFO7_SYM;

inline void LFO7(float increment, float WF, float sym) {
    float phase, out, tmp, overflow_phase;

    phase = g_Modulation_Phase[LFO7_OUT] + increment;                               // calcul de la phase
    overflow_phase = (int)phase;
    phase -= overflow_phase;
    g_Modulation_Reset[LFO7_OUT] = overflow_phase;
    g_Modulation_Phase[LFO7_OUT] = phase;

    if (phase < sym) {                             // calcul du triangle
        out = phase / sym;
    } else {
        out = sym - phase;
        out /= 1.f-sym;
        out += 1.f;
    }

    float forme = WF * 3.f;                                 // on separe en 4 WF : tri / sin / tanh(sin) / square
    float forme1 = _fmin(forme,1.f);                        // tri a sinus
    float forme2 = _fclamp(forme-1.f,0.f,2.f)*0.5f;         // sinus a disto(sin)

    out = (out + out) -1.f;  // triangle
    out = mix(out, _sin_positiv_loop(2.f + out*0.25f), forme1);   // interpolation entre triangle et sinus
    out += forme2 * ( sym + sym - 1.f);                     // offset

    tmp = CV2freq((256.f*forme2)-127.f)*out;                // distortion coef a : f(y) = (x+ax)/(1+abs(ax))
    out = (out + tmp)/(1.f+fabs(tmp)); // distortion
    out = _fclamp(out, -1.f, 1.f);
    g_Modulation[LFO7_OUT] = out;
    g_Modulation[LFO7_OUT_FILTER] += 0.003 * (out - g_Modulation[LFO7_OUT_FILTER]);
}

void LFO()
{
    g_MIDI_count++; // pour le calcul de la frequence syncro midi

    LFO1(LFO1_FQ, LFO1_MIX, LFO1_INC);
    LFO2(LFO2_FQ, LFO2_MIX, LFO2_INC);
    LFO3(LFO3_FQ, LFO3_MIX, LFO3_INC);
    LFO4(LFO4_INC);
    LFO5(LFO5_INC);
    LFO6(LFO6_INC);
    LFO7(LFO7_INC, LFO7_WF, LFO7_SYM);
}
