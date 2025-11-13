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

#include "b_hardware.h"

// copy and adapted from daisy_pod
// 2020 cyrille henry

/*
 * -------- Daisy pinout --------
 *  1 : USB OTG                 //
 *  2 : UART3 RX                // power test
 *  3 : UART3 TX                // serial out test
 *  4 : I2C3                    // hw test input pin
 *  5 :                         // GATE IN
 *  6 : UART5 RX                // keyboard interuption 1
 *  7 : UART5 TX                // keyboard interuption 2
 *  8 : SPI1 (GPIO7)            // ADC MUX addresse 3
 *  9 : SPI1 (GPIO8)            // ADC MUX addresse 2
 * 10 : SPI1 (GPIO9)            // ADC MUX addresse 1
 * 11 : SPI1                    // test point 1
 * 12 : I2C1 UART4 RX           // I2C Led (400KHz) SCL + touch kb1 (clavier + CV1, CV2, MIDI)
 * 13 : I2C1 UART4 TX           // I2C Led (400KHz) SDA + touch kb1
 * 14 : I2C4 UART1 TX           // I2C keyboard (400KHz) SCL : kb2 : addresse 00, 01, 10, 11 : MOD, SEL, LOAD SAVE
 * 15 : I2C4 UART1 RX           // I2C keyboard (400KHz) SDA
 * 16 : AUDIO IN 1              // audio In
 * 17 : AUDIO IN 2              // audio in
 * 18 : AUDIO OUT 1             // audio out
 * 19 : AUDIO OUT 2             // audio out
 * 20 : AGRND                   // AGRND
 * 21 : 3.3V Ana                // 3.3V ana
 * 22 : ADC0                    // ADC IN1 : pb
 * 23 : ADC1                    // ADC IN2
 * 24 : ADC2                    // ADC IN3
 * 25 : ADC3                    // ADC IN4
 * 26 : ADC4                    // ADC IN5
 * 27 : ADC5                    // ADC IN6
 * 28 : ADC6                    // ADC IN1 bis
 * 29 : ADC7  / DAC OUT         // DAC Test out
 * 30 : ADC8  / DAC OUT         // DAC Test out
 * 31 : ADC9  / SAI2            // ADC CV1
 * 32 : ADC10 / SAI2            // ADC CV2
 * 33 : SAI2                    // digital led out / clock // shift register 595
 * 34 : SA12                    // digital led out / data
 * 35 : ADC11 / SAI2            // digital led out / latch
 * 36 : UART1 TX / USB          // MIDI out, not used
 * 37 : UART1 RX / USB          // MIDI in
 * 38 : 3.3V Digital            // 3.3V
 * 39 : VIN                     // V In
 * 40 : DGND                    // Gnd
 */

// 3 cartes :
// - top
//      : Led on the back
//      : hole for the knobs
//      : capacitive touch sensor
// - medimum
//      : knob and mux
// - bottom
//      : daisy
//      : connecteur :
//          power
//          MIDI in
//          AUDIO out
//          headphone out
//          headphone volume
//          CV1 in
//          CV2 in
//          GATE IN
//          on/off switch
//      : headphone ampli
//      : audio ampli
//      : CV and GATE In OPA
//      : MIDI opto
//      : allimentation (and filter)

#define KNOB_MUX1_PIN 21//15 // mux
#define KNOB_MUX2_PIN 18
#define KNOB_MUX3_PIN 17
#define KNOB_MUX4_PIN 16
#define KNOB_MUX5_PIN 20
#define KNOB_MUX6_PIN 19 // 6* 8 = 48 analogue input
#define CV1_PIN       25
#define CV2_PIN       24

#define PIN_MUX_SEL_0 9
#define PIN_MUX_SEL_1 8
#define PIN_MUX_SEL_2 7

#define PIN_LED_CLK   26 // SR595
#define PIN_LED_DATA  27
#define PIN_LED_LATCH 28

#define PIN_TEST 10
#define PIN_GATE 4
#define PIN_KB1_INT 5
#define PIN_KB2_INT 6
#define PIN_HW_TEST 3
#define PIN_LOW_POWER 1

// prototype
inline float _fclamp(float, float, float);

using namespace daisy;

Kagouyar hw;

// test pin
dsy_gpio test_pin, gate_pin, Kb1_int, Kb2_int, HW_test, low_power_pin;

I2CHandle i2c_led, i2c_kb;

static constexpr I2CHandle::Config i2c_led_config
    = {I2CHandle::Config::Peripheral::I2C_1,
       {{DSY_GPIOB, 8}, {DSY_GPIOB, 9}},
       I2CHandle::Config::Speed::I2C_400KHZ}; //I2C_1MHZ};

static LedDriverPca9685<1, false>::DmaBuffer DMA_BUFFER_MEM_SECTION
    led_dma_buffer_a, led_dma_buffer_b;

static constexpr I2CHandle::Config i2c_kb_config
    = {I2CHandle::Config::Peripheral::I2C_4,
       {{DSY_GPIOB, 6}, {DSY_GPIOB, 7}},
       I2CHandle::Config::Speed::I2C_400KHZ};

void Kagouyar::Init()
{ // Initialize the hardware.

	SCB->CPACR |= (0xF << 20); // activer CP10 et CP11 (accès complet)
	FPU->FPCCR |= (FPU_FPCCR_ASPEN_Msk | FPU_FPCCR_LSPEN_Msk);

    seed.Configure();
    seed.Init(true); // true = c'est le boost mode des adc
    InitKnobs();
    SetAudioBlockSize(block_size);
    InitLeds();
    InitBinaryLeds();
    InitKeyboard();
    InitMidi();
    InitPin();
}

void Kagouyar::StartAudio(AudioHandle::InterleavingAudioCallback cb){seed.StartAudio(cb);}
void Kagouyar::StopAudio(){seed.StopAudio();}
void Kagouyar::SetAudioBlockSize(size_t size){seed.SetAudioBlockSize(size);}
void Kagouyar::StartAdc(){seed.adc.Start();}
inline void Kagouyar::test_out(bool out){dsy_gpio_write(&test_pin, out);};

void Kagouyar::InitKnobs()
{
    // Configure the ADC channels using the desired pin
    AdcChannelConfig adc_cfg[8];
    adc_cfg[0].InitMux(seed.GetPin(KNOB_MUX1_PIN), 8,
        seed.GetPin(PIN_MUX_SEL_0), seed.GetPin(PIN_MUX_SEL_1), seed.GetPin(PIN_MUX_SEL_2));
    adc_cfg[1].InitMux(seed.GetPin(KNOB_MUX2_PIN), 8,
        seed.GetPin(PIN_MUX_SEL_0), seed.GetPin(PIN_MUX_SEL_1), seed.GetPin(PIN_MUX_SEL_2));
    adc_cfg[2].InitMux(seed.GetPin(KNOB_MUX3_PIN), 8,
        seed.GetPin(PIN_MUX_SEL_0), seed.GetPin(PIN_MUX_SEL_1), seed.GetPin(PIN_MUX_SEL_2));
    adc_cfg[3].InitMux(seed.GetPin(KNOB_MUX4_PIN), 8,
        seed.GetPin(PIN_MUX_SEL_0), seed.GetPin(PIN_MUX_SEL_1), seed.GetPin(PIN_MUX_SEL_2));
    adc_cfg[4].InitMux(seed.GetPin(KNOB_MUX5_PIN), 8,
        seed.GetPin(PIN_MUX_SEL_0), seed.GetPin(PIN_MUX_SEL_1), seed.GetPin(PIN_MUX_SEL_2));
    adc_cfg[5].InitMux(seed.GetPin(KNOB_MUX6_PIN), 8,
        seed.GetPin(PIN_MUX_SEL_0), seed.GetPin(PIN_MUX_SEL_1), seed.GetPin(PIN_MUX_SEL_2));
    adc_cfg[6].InitSingle(seed.GetPin(CV1_PIN));
    adc_cfg[7].InitSingle(seed.GetPin(CV2_PIN));

    seed.adc.Init(adc_cfg, 8, daisy::AdcHandle::OVS_4);// Oversampling to 32x -> 200Hz update for all knob (en theory)
	// 8 -> on mesure 8 pot actualisé par ms, 166Hz en pratique pour l'update des pots
	// 4 -> 12 pot par ms, 250Hz d'actualisation

    for(int i = 0; i < 48; i++){ knobs_[i].Init(seed.adc.GetMuxPtr(i/8, i%8), seed.AudioCallbackRate()); }

    knobs_[48].Init(seed.adc.GetPtr(6), seed.AudioCallbackRate());
    knobs_[49].Init(seed.adc.GetPtr(7), seed.AudioCallbackRate());
}

void Kagouyar::InitLeds()
{
    uint8_t   addr[1] = {0x00};
    i2c_led.Init(i2c_led_config);
    led_driver_.Init(i2c_led, addr, led_dma_buffer_a, led_dma_buffer_b);
    ClearLeds();
    UpdateLeds();
}

void Kagouyar::InitBinaryLeds(){ //595 binary led
    dsy_gpio_pin bin_led_pin[3];
    bin_led_pin[0] = seed.GetPin(PIN_LED_LATCH);
    bin_led_pin[1] = seed.GetPin(PIN_LED_CLK);
    bin_led_pin[2] = seed.GetPin(PIN_LED_DATA);

    hw.binary_led.Init(bin_led_pin, 3); //(dsy_gpio_pin *pin_cfg, size_t num_daisy_chained)
}

void Kagouyar::InitKeyboard(){
    i2c_kb.Init(i2c_kb_config);
}

inline void Kagouyar::ClearLeds()
{
    for(int i =0; i < 16; i++) {
        led_driver_.SetLed_ch(i, (uint8_t)0); // from 0 to 255, include a curve
    }
}

inline void Kagouyar::UpdateLeds()
{
    led_driver_.SwapBuffersAndTransmit();
}

void Kagouyar::InitMidi()
{
	MidiUartTransport::Config midi_config;
	midi_config.periph = UartHandler::Config::Peripheral::USART_1;
	midi_config.rx     = {DSY_GPIOB, 15};
    midi_config.tx     = {DSY_GPIOB, 14};

    midi.Init(midi_config);
}

inline void Kagouyar::InitPin()
{
    test_pin.pin = seed.GetPin(PIN_TEST);
    test_pin.mode = DSY_GPIO_MODE_OUTPUT_PP;
    test_pin.pull = DSY_GPIO_NOPULL;
    dsy_gpio_init(&test_pin);

    gate_pin.pin = seed.GetPin(PIN_GATE);
    gate_pin.mode = DSY_GPIO_MODE_INPUT;
    gate_pin.pull = DSY_GPIO_NOPULL;
    dsy_gpio_init(&gate_pin);

    Kb1_int.pin = seed.GetPin(PIN_KB1_INT);
    Kb1_int.mode = DSY_GPIO_MODE_INPUT;
    Kb1_int.pull = DSY_GPIO_NOPULL;
    dsy_gpio_init(&Kb1_int);

    Kb2_int.pin = seed.GetPin(PIN_KB2_INT);
    Kb2_int.mode = DSY_GPIO_MODE_INPUT;
    Kb2_int.pull = DSY_GPIO_NOPULL;
    dsy_gpio_init(&Kb2_int);

    HW_test.pin = seed.GetPin(PIN_HW_TEST);
    HW_test.mode = DSY_GPIO_MODE_INPUT;
    HW_test.pull = DSY_GPIO_PULLUP;
    dsy_gpio_init(&HW_test);

    low_power_pin.pin = seed.GetPin(PIN_LOW_POWER);
    low_power_pin.mode = DSY_GPIO_MODE_INPUT;
    low_power_pin.pull = DSY_GPIO_PULLUP;
    dsy_gpio_init(&low_power_pin);
}
