#ifndef FPGA_H
#define FPGA_H

#include "defines.h"
#include <Arduino.h>
#include <SPI.h>

SPIClass SPI_2(SPI2_MOSI,SPI2_MISO,SPI2_SCK);

enum OffsetRange {
    OFFSET_LOW,
    OFFSET_MID,
    OFFSET_HIGH
};

enum AmpRange {
    AMP_LOW,
    AMP_MID,
    AMP_HIGH
};

enum WaveformType : uint32_t {
    WAVE_SINE      = 0x60000,  // Flash address for sine
    WAVE_SQUARE    = 0x64000,  // Flash address for square
    WAVE_TRIANGLE  = 0x68000,
    WAVE_SAW       = 0x6B000,
    WAVE_CUSTOM1   = 0x6F000,
    WAVE_CUSTOM2   = 0x73000
};

static bool REGPOL = true;
static bool CSPOL = true;

static uint32_t FPGA_GetRequestedRegister();
static uint32_t FPGA_ReadFlashByte(uint32_t address);
static uint16_t FPGA_ReadFlashWord(uint32_t address);
static uint8_t FPGA_TestFlash();
static void FPGA_RequestRegister(uint16_t reg);
static void FPGA_EraseFlashPage(uint32_t address);
static void FPGA_WriteFlashByte(uint32_t Byte);
static void FPGA_WriteFlashWord(uint32_t Word);
static void FPGA_WaitReady();

uint32_t FPGA_ReadRegister(uint16_t reg);

void FPGA_Init();
void FPGA_WriteRegister(uint16_t reg, uint32_t value);
void FPGA_LoadWaveformFromFlash(uint32_t channel, uint32_t address);
void FPGA_GetResampledWaveform(WaveformType waveform, uint32_t *Samples);
void FPGA_EraseFlashWaveform(uint32_t address);
void FPGA_SetVoltage(uint32_t channel, uint32_t voltage);
void FPGA_SetFrequency(uint32_t channel, uint32_t frequency);
void FPGA_SetAmplitude(uint32_t channel, uint32_t amplitude);
void FPGA_SetOffset(uint32_t channel, OffsetRange offset);
void FPGA_SetPhase(uint32_t channel, uint32_t phase);
void FPGA_SetWaveform(uint32_t channel, WaveformType waveform);

void FPGA_Init(){
    // Initialize SPI for FPGA communication
    SPI_2.begin();

    pinMode(REGSEL, OUTPUT);
    pinMode(SPI2_NSS, OUTPUT);
    
    digitalWrite(REGSEL, REGPOL); // Default to data mode
    digitalWrite(SPI2_NSS, CSPOL); // Deselect FPGA

    uint8_t i;

    FPGA_WriteRegister(0x25,0x2FAF080);
    FPGA_WriteRegister(0x26,0x1DCD6500);
    FPGA_WriteRegister(0x28,0x2A05F200);
    FPGA_WriteRegister(0x27,0x1);
    FPGA_WriteRegister(0X2A,0xA931A000);
    FPGA_WriteRegister(0x29,0XE35F);
    FPGA_WriteRegister(0x24,0x10);//mod init
    FPGA_WriteRegister(0x06,0xC5);//relays init

    for(i=0; i<=15; i++)
    {
        FPGA_WriteRegister(0x10,i);
        FPGA_ReadRegister(0x1A);
    }

    for(i=0; i<=15; i++)
    {
        FPGA_WriteRegister(0x1B,i);
        FPGA_WriteRegister(0x19,1);
        FPGA_WriteRegister(0x19,0);
    }    
}



void FPGA_WriteRegister(uint16_t reg, uint32_t value){
    
    digitalWrite(REGSEL, !REGPOL); // Select FPGA register mode
    digitalWrite(SPI2_NSS, !CSPOL); // Select FPGA for SPI
    SPI_2.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    SPI_2.transfer16(reg);
    digitalWrite(REGSEL, REGPOL); // Deselect FPGA register mode
    SPI_2.transfer16((value >> 16) & 0xFFFF);
    SPI_2.transfer16(value & 0xFFFF);
    SPI_2.endTransaction();
    digitalWrite(SPI2_NSS, CSPOL); // Deselect FPGA
}

static void FPGA_RequestRegister(uint16_t reg){
    digitalWrite(REGSEL, !REGPOL); // Select FPGA register mode
    digitalWrite(SPI2_NSS, !CSPOL); // Select FPGA for SPI

    SPI_2.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    SPI_2.transfer16(reg | 0x8000); // Set MSB for read operation
    SPI_2.endTransaction();
    digitalWrite(SPI2_NSS, CSPOL); // Deselect FPGA register mode
    digitalWrite(REGSEL, REGPOL); // Deselect FPGA
}

static uint32_t FPGA_GetRequestedRegister(){
    uint32_t value = 0;

    digitalWrite(SPI2_NSS, !CSPOL); // Select FPGA for SPI
    SPI_2.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    value |= ((uint32_t)SPI_2.transfer16(0x0000)) << 16; // Read high 16 bits
    value |= (uint32_t)SPI_2.transfer16(0x0000);         // Read low 16 bits
    SPI_2.endTransaction();
    digitalWrite(SPI2_NSS, CSPOL); // Deselect FPGA

    return value;
}

uint32_t FPGA_ReadRegister(uint16_t reg){
    FPGA_RequestRegister(reg);
    return FPGA_GetRequestedRegister();
}

void FPGA_LoadWaveformFromFlash(uint32_t channel, uint32_t address){
    FPGA_WriteRegister(0x14,0x0); //clr flash write trigger
    FPGA_WriteRegister(0x16,0x0); //clr flash page erase trigger

    while(FPGA_ReadRegister(0x18));

    FPGA_WriteRegister(0x12,address); //set flash address
    FPGA_WriteRegister(0x11,channel);//set channel
    FPGA_WriteRegister(0x13,0x1);//Trigger flash read
    FPGA_WriteRegister(0x13,0x0);
    FPGA_WriteRegister(0x11,0x0);

    FPGA_RequestRegister(0x18);
    while(FPGA_GetRequestedRegister());
}

static uint32_t FPGA_ReadFlashByte(uint32_t address){
    FPGA_WriteRegister(0x12,address); //set flash address
    FPGA_WriteRegister(0x0F,0x1); //trigger read
    FPGA_WriteRegister(0x0F,0x0); //trigger read
    FPGA_RequestRegister(0x18);
    while(FPGA_GetRequestedRegister());
    FPGA_RequestRegister(0x0E);
    return FPGA_GetRequestedRegister();
}

static uint16_t FPGA_ReadFlashWord(uint32_t address){
    uint16_t value = 0;
    value |= FPGA_ReadFlashByte(address) << 8;
    value |= FPGA_ReadFlashByte(address+1);
    return value;
}

void FPGA_GetResampledWaveform(WaveformType waveform, uint32_t *Samples) {
    uint32_t i;

    // Clear flash triggers
    FPGA_WriteRegister(0x14, 0x0); // clr flash write trigger
    FPGA_WriteRegister(0x16, 0x0); // clr flash page trigger

    // Read 100 samples from flash
    uint32_t baseAddress = static_cast<uint32_t>(waveform);
    for (i = 0; i < 100; i++) {
        Samples[i] = FPGA_ReadFlashWord(baseAddress + i * 164);
    }
}


static void FPGA_EraseFlashPage(uint32_t address){
    FPGA_WriteRegister(0x12,address); //set flash address
    FPGA_WriteRegister(0x16,0x1); //trigger erase page
    FPGA_WriteRegister(0x16,0x0); //trigger erase
    FPGA_RequestRegister(0x18);
    while(FPGA_GetRequestedRegister());
}

void FPGA_EraseFlashWaveform(uint32_t address){
    FPGA_WriteRegister(0x14,0x0); // Clear flash write trigger
    FPGA_WriteRegister(0x16,0x0); // Clear flash page erase trigger
    FPGA_RequestRegister(0x18);
    while(FPGA_GetRequestedRegister());
    FPGA_EraseFlashPage(address);
    FPGA_EraseFlashPage(address+0x1000);
    FPGA_EraseFlashPage(address+0x2000);
    FPGA_EraseFlashPage(address+0x3000);
}

void FPGA_WriteFlashByte(uint32_t Byte){
    FPGA_WriteRegister(0x15,Byte&0xFF); //set data to write
    FPGA_WriteRegister(0x14,0x1); //trigger write
    FPGA_WriteRegister(0x14,0x0); //clear trigger
    FPGA_RequestRegister(0x18);
    while(FPGA_GetRequestedRegister());
}

void FPGA_WriteFlashWord(uint32_t Word){
    FPGA_WriteFlashByte((Word >> 8) & 0xFF);
    FPGA_WriteFlashByte(Word & 0xFF);
}


static uint8_t FPGA_TestFlash(){
    FPGA_LoadWaveformFromFlash(0, 0xB0000);
    FPGA_LoadWaveformFromFlash(0, 0xB4000);
    FPGA_LoadWaveformFromFlash(0, 0xB8000);
    FPGA_LoadWaveformFromFlash(0, 0xBC000);

    return (FPGA_ReadFlashByte(0x0F0000) == 0 && (FPGA_ReadFlashByte(0x0F0004) == 4));
}

static void FPGA_WaitReady() 
{
    pinMode(FPGARDY, INPUT);
    uint16_t TimeON = 0;
    uint16_t TimeOFF = 0;
        
    // Wait for 100ms continuous ON State of FPGA_Ready signal: FPGA code loading launched
    while (TimeON <100)
    {		
        TimeON = digitalRead(FPGARDY) ? (++TimeON):0;
        delay(1);
    }
        
    // Wait for 5 ms continuous OFF State of FPGA_Ready signal: FPGA loaded and ready
    while (TimeOFF < 5)
    {		
        TimeOFF = digitalRead(FPGARDY) ? 0:(++TimeOFF);
        delay(1);
    }
}


void FPGA_SetVoltage(uint32_t channel, uint32_t voltage){
    uint32_t mapped_voltage = map(voltage, 2500, 5000, 0x150, 0xE65);
    if(channel == 1){
        FPGA_WriteRegister(0x2D, mapped_voltage);
    }else if(channel == 2){
        FPGA_WriteRegister(0x2E, mapped_voltage);
    }
}

void FPGA_SetPhase(uint32_t channel, float phaseDeg) {
    // Clamp input to 0..360 degrees
    if (phaseDeg < 0.0f) phaseDeg = 0.0f;
    if (phaseDeg > 360.0f) phaseDeg = 360.0f;

    // Convert phase (deg) to FPGA code
    uint32_t phaseDW = (uint32_t)((1.0f - (phaseDeg / 360.0f)) * 1048575.0f);

    // Send to correct channel register
    if (channel == 1) {
        FPGA_WriteRegister(0x08, phaseDW);
    } else if (channel == 2) {
        FPGA_WriteRegister(0x09, phaseDW);
    }
}

void FPGA_SetOffset(uint32_t channel, float offsetVolts, OffsetRange range) {
    float scale = 1.0f;

    // Select scaling based on range
    switch (range) {
        case OFFSET_LOW:  scale = 0.62f; break;
        case OFFSET_MID:  scale = 6.21f; break;
        case OFFSET_HIGH: scale = 24.0f; break;
    }

    // Compute offset DAC word
    int32_t offsetDW = 2047 + (int32_t)((4094.0f / scale) * offsetVolts);

    // Clamp to valid DAC range [0..4095]
    if (offsetDW < 0) offsetDW = 0;
    if (offsetDW > 4095) offsetDW = 4095;

    // Write to correct channel register
    if (channel == 1) {
        FPGA_WriteRegister(0x2B, offsetDW);
    } else if (channel == 2) {
        FPGA_WriteRegister(0x2C, offsetDW);
    }
}

void FPGA_SetAmplitude(uint32_t channel, float amplitudeVolts, AmpRange range) {
    float scale = 1.0f;

    switch (range) {
        case AMP_LOW:  scale = 0.5f; break;
        case AMP_MID:  scale = 5.0f; break;
        case AMP_HIGH: scale = 20.0f; break;
    }

    // Compute amplitude DAC word
    int32_t ampDW = (int32_t)((amplitudeVolts * 3685.0f) / scale);

    // Clamp to valid range for DAC (0..3685 for this design)
    if (ampDW < 0) ampDW = 0;
    if (ampDW > 3685) ampDW = 3685;

    // Write to channel
    if (channel == 1) {
        FPGA_WriteRegister(0x2D, ampDW);
    } else if (channel == 2) {
        FPGA_WriteRegister(0x2E, ampDW);
    }
}

void FPGA_SetDutyCycle(uint32_t channel, float dutyPercent) {
    // Clamp input 0..100%
    if (dutyPercent < 0.0f) dutyPercent = 0.0f;
    if (dutyPercent > 100.0f) dutyPercent = 100.0f;

    // Convert to FPGA code
    uint32_t dutyDW = (uint32_t)((dutyPercent / 100.0f) * 0x1FFFF);

    // Write to the correct channel
    if (channel == 1) {
        FPGA_WriteRegister(0x2F, dutyDW);
    } else if (channel == 2) {
        FPGA_WriteRegister(0x30, dutyDW);
    }
}

void FPGA_SetWaveform(uint32_t channel, WaveformType waveform) {
    FPGA_LoadWaveformFromFlash(channel, static_cast<uint32_t>(waveform));
}


#endif;