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

    float const forme = WF * 3.f;                                 // on separe en 4 WF : tri / sin / tanh(sin) / square
    float const forme1 = _fmin(forme,1.f);                        // tri a sinus
    float const forme2 = _fclamp(forme-1.f,0.f,2.f)*0.5f;         // sinus a disto(sin)

    out = (out + out) -1.f;  // triangle
    out = mix(out, _sin_positiv_loop(2.f + out*0.25f), forme1);   // interpolation entre triangle et sinus
    out += forme2 * ( sym + sym - 1.f);                     // offset

    tmp = CV2freq((256.f*forme2)-127.f)*out;                // distortion coef a : f(y) = (x+ax)/(1+abs(ax))
    out = (out + tmp)/(1.f+fabs(tmp)); // distortion
    out = _fclamp(out, -1.f, 1.f);
    g_Modulation[LFO7_OUT] = out;
    g_Modulation[LFO7_OUT + modulation_source_last] = -out;
    g_Modulation[LFO7_OUT_FILTER] += 0.003f * (out - g_Modulation[LFO7_OUT_FILTER]);
}

inline void LFO()
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
