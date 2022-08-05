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

// algorythm from :
// Vesa Välimäki and Antti Huovilainen
// Helsinki University of Technology
//Laboratory of Acoustics and Audio Signal Processing
//P.O. Box 3000, FI-02015 TKK, Espoo, Finland
// in article : "Oscillator and Filter
// Algorithms for Virtual Analog Synthesis †"
// CMJ302

inline float VCF1(uint32_t j, float fq, float input1) { //, float res, float mod1, float mod2) {
    float tmp = 0.f;
    float mod_fq = 0.f;
    float mod_q = 0.f;
	float res = g_pot_audio[k_VCF1_q] ;
	
    if(curent_config.c_VCF1_MOD1_TYPE == 0) 
        mod_fq += g_pot_audio[k_VCF1_mod1] * g_Modulation[curent_config.c_Modulation_Source[VCF1_MOD1]];
    else
        mod_q  += g_pot_audio[k_VCF1_mod1] * g_Modulation[curent_config.c_Modulation_Source[VCF1_MOD1]];
   
    if(curent_config.c_VCF1_MOD2_TYPE == 0) 
        mod_fq += g_pot_audio[k_VCF1_mod2] * g_Modulation[curent_config.c_Modulation_Source[VCF1_MOD2]];
    else 
        mod_q  += g_pot_audio[k_VCF1_mod2] * g_Modulation[curent_config.c_Modulation_Source[VCF1_MOD2]];

    float freq = fq+ (48.f*mod_fq) + (24.f*g_Modulation[MIDI_expression]);
    tmp = curent_config.c_VCF1_pitch_TRACK;
    freq += tmp * allvoice[j].v_pitch*0.5f;
    freq = _fclamp(freq, -128, 138);
    freq = CV2freq(freq);

    float omega = freq * (TWOPI_F/48000.f);
    float g = omega * (0.9892f + omega*(-0.4342f + omega*(0.1381f + omega * -0.0202f)));

    float Q = 4.f * _fclamp(res + mod_q, 0.f, 1.f);
    // resonance frequency compensation
    Q *= 1.0029f + omega*(0.0526f + omega * (-0.0926f  + omega*0.0218f));

    input1 *= 0.5; // limitation de l'amplitude d'entrée pour ne pas trop distordre le signal avant le filtre

	_fonepole(v_VCF1_input, input1, 0.25f);
	input1 = v_VCF1_input;
	
    float feedback = Q * ( allvoice[j].v_VCF1_last_output4 - (0.5f * input1) ); // feedback

    input1 -= feedback;
    input1 = _tanh(input1); // distortion

    float output1 = (input1 + 0.3f * allvoice[j].v_VCF1_last_input1)*(1.f/1.3f); // 4 * 6dB filter
    allvoice[j].v_VCF1_last_input1 = input1;
    tmp = allvoice[j].v_VCF1_last_output1;
    output1 -= tmp;
    output1 *= g;
    output1 += tmp;
    allvoice[j].v_VCF1_last_output1 = output1;

    float output2 = (output1 + 0.3f * allvoice[j].v_VCF1_last_input2)*(1.f/1.3f);
    allvoice[j].v_VCF1_last_input2 = output1;
    tmp = allvoice[j].v_VCF1_last_output2;
    output2 -= tmp;
    output2 *= g;
    output2 += tmp;
    allvoice[j].v_VCF1_last_output2 = output2;

    float output3 = (output2 + 0.3f * allvoice[j].v_VCF1_last_input3)*(1.f/1.3f);
    allvoice[j].v_VCF1_last_input3 = output2;
    tmp = allvoice[j].v_VCF1_last_output3;
    output3 -= tmp;
    output3 *= g;
    output3 += tmp;
    allvoice[j].v_VCF1_last_output3 = output3;

    float output4 = (output3 + 0.3f * allvoice[j].v_VCF1_last_input4)*(1.f/1.3f);
    allvoice[j].v_VCF1_last_input4 = output3;
    tmp = allvoice[j].v_VCF1_last_output4;
    output4 -= tmp;
    output4 *= g;
    output4 += tmp;
    allvoice[j].v_VCF1_last_output4 = output4;

    switch (curent_config.c_VCF1_TYPE) {
    case 0 :
        tmp = output4;
        break;
    case 1 :
        tmp = output2;
        break;
    case 2 :
        tmp = output1 + output1 - output2 - output2;
        break;
    case 3 :
        tmp = 4.f * (output2 - output3 - output3 + output4);
        break;
    case 4 :
        tmp = input1 - output1 - output1 + output2;
        break;
    case 5 :
        tmp = input1 - 4.f * (output1 + output3) + 6.f *  output2 + output4;
        break;
    }
 
    return 2.f*tmp; // facteur 2 pour compenser le gain d'entré
}


float g_VCF2_last_input1 = 0.f;
float g_VCF2_last_input2 = 0.f;
float g_VCF2_out = 0.f;

inline void VCF2(float &input) { 
	float fq = 127.f * ( g_pot_audio[k_VCF2_fq] += g_pot_increment[k_VCF2_fq]); 
	float mod1 =  g_pot_audio[k_VCF2_mod] += g_pot_increment[k_VCF2_mod]; 

    float filter_fq = mod1 * g_Modulation[curent_config.c_Modulation_Source[VCF2_MOD1]];
    filter_fq *= 48.f;
    filter_fq += fq;
    filter_fq = _fclamp(filter_fq, -127, 127.5);
    filter_fq = CV2freq(filter_fq)*(1.f/13000.f);

    _fonepole(g_VCF2_last_input1, input,              filter_fq);
    _fonepole(g_VCF2_last_input2, g_VCF2_last_input1, filter_fq);

    if(curent_config.c_VCF2_TYPE) {
        input = input - g_VCF2_last_input2;
    } else {
        input = g_VCF2_last_input2;
    }
}
