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

#define VCF1_FILTER_SCALE (1.f/1.3f)
#define VCF1_FILTER_FEEDBACK 0.3f
#define VCF1_Q_SCALE 4.5f
#define VCF1_FREQ_CLAMP_MIN -128.f
#define VCF1_FREQ_CLAMP_MAX 132.f

inline float compute_filter_stage(float input, float last_input, float last_output, float g) {
    const float output = (input + VCF1_FILTER_FEEDBACK * last_input) * VCF1_FILTER_SCALE;
    return output + g * (output - last_output);
}

inline float VCF1(const uint32_t j, const float fq, float input1) {
    // Modulation calculation
    const float mod1_value = g_pot_audio[k_VCF1_mod1] * g_Modulation[curent_config.c_Modulation_Source[VCF1_MOD1]];
    const float mod2_value = g_pot_audio[k_VCF1_mod2] * g_Modulation[curent_config.c_Modulation_Source[VCF1_MOD2]];
    
    const float mod_fq = (curent_config.c_VCF1_MOD1_TYPE == 0 ? mod1_value : 0.f) +
                        (curent_config.c_VCF1_MOD2_TYPE == 0 ? mod2_value : 0.f);
    
    const float mod_q = (curent_config.c_VCF1_MOD1_TYPE != 0 ? mod1_value : 0.f) +
                       (curent_config.c_VCF1_MOD2_TYPE != 0 ? mod2_value : 0.f);

    // Frequency calculation
    const float pitch_offset = curent_config.c_VCF1_pitch_TRACK * 0.5f * (allvoice[j].v_pitch - 12.f);
    const float freq = CV2freq(_fclamp(
        fq + (48.f * mod_fq) + (24.f * g_Modulation[MIDI_expression]) + pitch_offset,
        VCF1_FREQ_CLAMP_MIN,
        VCF1_FREQ_CLAMP_MAX
    ));

    // Filter coefficients
    const float omega = freq * (TWOPI_F/48000.f);
    const float g = omega * (0.9892f + omega * (-0.4342f + omega * (0.1381f + omega * -0.0202f)));
    
    // Resonance calculation with frequency compensation
    const float res = g_pot_audio[k_VCF1_q];
    const float freq_comp = 1.0029f + omega * (0.0526f + omega * (-0.0926f + omega * 0.0218f));
    const float Q = VCF1_Q_SCALE * _fclamp(res + mod_q, 0.f, 1.f) * freq_comp * 1.01f;

    // Input preprocessing
    _fonepole(allvoice[j].v_VCF1_filter, input1, 12000.f * OneOverSR);
    input1 = allvoice[j].v_VCF1_filter;
    input1 = _tanh_clip(input1 - Q * (allvoice[j].v_VCF1_last_output4 - (0.5f * input1)));

    // Four cascaded one-pole filters
    const float stage1 = compute_filter_stage(input1, allvoice[j].v_VCF1_last_input1, 
                                            allvoice[j].v_VCF1_last_output1, g);
    const float stage2 = compute_filter_stage(stage1, allvoice[j].v_VCF1_last_input2, 
                                            allvoice[j].v_VCF1_last_output2, g);
    const float stage3 = compute_filter_stage(stage2, allvoice[j].v_VCF1_last_input3, 
                                            allvoice[j].v_VCF1_last_output3, g);
    const float stage4 = compute_filter_stage(stage3, allvoice[j].v_VCF1_last_input4, 
                                            allvoice[j].v_VCF1_last_output4, g);

    // Update filter states
    allvoice[j].v_VCF1_last_input1 = input1;
    allvoice[j].v_VCF1_last_output1 = stage1;
    allvoice[j].v_VCF1_last_input2 = stage1;
    allvoice[j].v_VCF1_last_output2 = stage2;
    allvoice[j].v_VCF1_last_input3 = stage2;
    allvoice[j].v_VCF1_last_output3 = stage3;
    allvoice[j].v_VCF1_last_input4 = stage3;
    allvoice[j].v_VCF1_last_output4 = stage4;

    // Calculate intermediate values for filter types
    const float output1 = (input1 + VCF1_FILTER_FEEDBACK * allvoice[j].v_VCF1_last_input1) * VCF1_FILTER_SCALE;
    const float output2 = (stage1 + VCF1_FILTER_FEEDBACK * allvoice[j].v_VCF1_last_input2) * VCF1_FILTER_SCALE;
    const float output3 = (stage2 + VCF1_FILTER_FEEDBACK * allvoice[j].v_VCF1_last_input3) * VCF1_FILTER_SCALE;
    const float output4 = (stage3 + VCF1_FILTER_FEEDBACK * allvoice[j].v_VCF1_last_input4) * VCF1_FILTER_SCALE;

    // Filter type selection
    switch (curent_config.c_VCF1_TYPE) {
        case 0:
            return stage4;
        case 1:
            return stage2;
        case 2:
            return (output1 + output1 - output2 - output2);
        case 3:
            return 4.f * (output2 - output3 - output3 + output4);
        case 4:
            return (input1 - output1 - output1 + output2);
        case 5:
            return (input1 - 4.f * (output1 + output3) + 6.f * output2 + output4);
        default:
            return 0.f;
    }
}

float g_VCF2_last_input1 = 0.f;
float g_VCF2_last_input2 = 0.f;
float g_VCF2_out = 0.f;

inline void VCF2(float &input) {
    if (curent_config.c_VCF2_TYPE == 2) {
        return;
    }

    const float fq = 127.f * (g_pot_audio[k_VCF2_fq] += g_pot_increment[k_VCF2_fq]);
    const float mod1 = g_pot_audio[k_VCF2_mod] += g_pot_increment[k_VCF2_mod];
    const float mod_source = g_Modulation[curent_config.c_Modulation_Source[VCF2_MOD1]];
    
    const float filter_fq = CV2freq(
        _fclamp(mod1 * mod_source * 48.f + fq, -127.f, 127.5f)
    ) * (1.f/13000.f);

    _fonepole(g_VCF2_last_input1, input, filter_fq);
    _fonepole(g_VCF2_last_input2, g_VCF2_last_input1, filter_fq);

    input = curent_config.c_VCF2_TYPE ? (input - g_VCF2_last_input2) : g_VCF2_last_input2;
}
