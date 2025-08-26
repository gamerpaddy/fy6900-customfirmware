#ifndef PAGE_WAVE_H
#define PAGE_WAVE_H

#include <defines.h>
#include <TFT_eSPI.h>
#include "channel_manager.h"
#include <stdint.h>
#include <stddef.h>
#include "fpga.h"


static int selectedTile = 0;

void forceBeepOff();
void u64_to_str(uint64_t value, char *buf, size_t bufsize) {
    if (bufsize < 2) return; // must have space for at least '0' and '\0'

    if (value == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }

    char tmp[20];
    int i = 0;

    while (value > 0 && i < 20) {
        tmp[i++] = '0' + (value % 10);
        value /= 10;
    }

    if (i >= (int)bufsize) i = bufsize - 1; // safety

    int j;
    for (j = 0; j < i; j++) {
        buf[j] = tmp[i - j - 1];
    }
    buf[j] = '\0';
}




void waveSidebar(int selectedTile, TFT_eSPI& tft) {
    word colorActive = RGB565(65,28,28);
    word colorInactive = RGB565(65,65,65);
    byte textSize = 2; 

    tft.fillRect(260, 0, 60, 240, RGB565(20,20,20)); //sidebar bg

    tft.fillRect(265, 15, 50, 30, selectedTile == 1 ? colorActive : colorInactive);
    tft.setCursor(267, 25);
    tft.setTextSize(textSize);
    tft.setTextColor(RGB565(255,255,255));
    tft.println("FREQ");

    tft.fillRect(265, 60, 50, 30, selectedTile == 2 ? colorActive : colorInactive);
    tft.setCursor(267, 70);
    tft.setTextSize(textSize);
    tft.setTextColor(RGB565(255,255,255));
    tft.println("AMPL");

    tft.fillRect(265, 105, 50, 30, selectedTile == 3 ? colorActive : colorInactive);
    tft.setCursor(267, 115);
    tft.setTextSize(textSize);
    tft.setTextColor(RGB565(255,255,255));
    tft.println("OFFS");

    tft.fillRect(265, 150, 50, 30, selectedTile == 4 ? colorActive : colorInactive);
    tft.setCursor(267, 160);
    tft.setTextSize(textSize);
    tft.setTextColor(RGB565(255,255,255));
    tft.println("RIFA");

    tft.fillRect(265, 195, 50, 30, selectedTile == 5 ? colorActive : colorInactive);
    tft.setCursor(267, 205);
    tft.setTextSize(textSize);
    tft.setTextColor(RGB565(255,255,255));
    tft.println("PHAS");
}

void drawDelimiter(int selectedTile,uint64_t delta, TFT_eSPI& tft) {
    tft.fillRect(125, 31, 124, 2, TFT_BLACK);
    tft.fillRect(125, 79, 124, 2, TFT_BLACK);
    tft.fillRect(125, 79, 124, 2, TFT_BLACK);
    tft.fillRect(125, 127, 124, 2, TFT_BLACK);
    tft.fillRect(125, 175, 124, 2, TFT_BLACK);
    tft.fillRect(125, 223, 124, 2, TFT_BLACK);

    uint64_t power = 1;
    for (int i = 0; i < 11; i++) { // 1,10,100,...,10,000,000,000
        if (delta == power) {
            tft.fillRect(228 - i * 12, 31+(selectedTile-1)*48, 12, 2, RGB565(255,0,0)); // red
        } else {
            tft.fillRect(228 - i * 12, 31+(selectedTile-1)*48, 12, 2, TFT_BLACK); // off
        }
        power *= 10;
    }
}

void waveFrequencyDisplay(int selectedTile, TFT_eSPI& tft) {
    // Clear frequency display area
    tft.fillRect(53, 3, 194, 28, TFT_BLACK);

    uint64_t frequency = getChannelFrequency(getSelectedChannel());
    
    word bgcolor = getSelectedChannel() == 1 ? RGB565(255,255,0) : RGB565(0,255,255);
    word bgcolor2 = getSelectedChannel() == 1 ? RGB565(100,100,0) : RGB565(0,100,100);
    word bgcolor3 = selectedTile == 1 ? bgcolor : RGB565(255,255,255);
 
    
    // Format and display frequency
    char freqStr[32];
    u64_to_str(frequency, freqStr, sizeof(freqStr));

    byte freq_strlen = strlen(freqStr); // right align
    tft.setCursor(240-freq_strlen*12, 10);
    tft.setTextSize(2);
    tft.setTextColor(bgcolor3);
    tft.print(freqStr);
    
    if(freq_strlen>0){
        tft.setTextSize(1);
        tft.setCursor(220, 1);
        tft.setTextColor(bgcolor);
        tft.print("mHz");
    }
    if(freq_strlen>3){  
        tft.setTextSize(1);
        tft.setCursor(184, 1);
        tft.setTextColor(bgcolor);
        tft.print("Hz");
        //tft.drawRoundRect(168, 0, 36, 31, 5, bgcolor2);
        tft.fillRect(202, 26, 2, 2, bgcolor3);
    }else{
        //tft.fillRect(169, 1, 34, 29, TFT_BLACK);
        tft.fillRect(202, 26, 2, 2, TFT_BLACK);
    }
    if(freq_strlen>6){
        tft.setTextSize(1);
        tft.setCursor(148, 1);
        tft.setTextColor(bgcolor);
        tft.print("kHz");
        tft.fillRect(166, 26, 2, 2, bgcolor3);
    }else{
        //tft.fillRect(169, 1, 34, 29, TFT_BLACK);
        tft.fillRect(166, 26, 2, 2, TFT_BLACK);
    }
    if(freq_strlen>9){
        tft.setTextSize(1);
        tft.setCursor(112, 1);
        tft.setTextColor(bgcolor);
        tft.print("MHz");
        tft.fillRect(130, 26, 2, 2, bgcolor3);
    }else{
        //tft.fillRect(169, 1, 34, 29, TFT_BLACK);
        tft.fillRect(130, 26, 2, 2, TFT_BLACK);
    }
    //drawDelimiter(selectedTile, 1000, tft);
}


void waveAmplitudeDisplay(int selectedTile, TFT_eSPI& tft) {
    tft.fillRect(126, 58, 123, 14, TFT_BLACK);

    uint32_t amplitude = getChannelAmplitude(getSelectedChannel()); // in mV
    word bgcolor = getSelectedChannel() == 1 ? RGB565(255,255,0) : RGB565(0,255,255);
    word bgcolor2 = selectedTile == 2 ? bgcolor : RGB565(255,255,255);

    char amplStr[16];
    sprintf(amplStr, "%lu", amplitude);  // always 3 decimal places

    byte ampl_len = strlen(amplStr); // right align
    tft.setCursor(206 - ampl_len * 12, 58);
    tft.setTextSize(2);
    tft.setTextColor(bgcolor2);
    tft.print(amplStr);
    tft.println("mV");
    

    //drawDelimiter(selectedTile, 1000, tft);

}

void waveOffsetDisplay(int selectedTile, TFT_eSPI& tft) {
    tft.fillRect(126, 106, 123, 14, TFT_BLACK);

    int32_t offset = getChannelOffset(getSelectedChannel());
    word bgcolor = getSelectedChannel() == 1 ? RGB565(255,255,0) : RGB565(0,255,255);
    word bgcolor2 = selectedTile == 3 ? bgcolor : RGB565(255,255,255);

    char offsetStr[16];
    sprintf(offsetStr, "%ld", offset);
    byte ampl_len = strlen(offsetStr); // right align
    tft.setCursor(206-ampl_len*12, 106);
    tft.setTextSize(2);
    tft.setTextColor(bgcolor2);
    tft.print(offsetStr);
    tft.println("mV");

    //drawDelimiter(selectedTile, 1000, tft);

}

void wavePhaseDisplay(int selectedTile, TFT_eSPI& tft) {
    tft.fillRect(126, 202, 123, 14, TFT_BLACK);

    int32_t phase = getChannelPhase(getSelectedChannel());
    word bgcolor = getSelectedChannel() == 1 ? RGB565(255,255,0) : RGB565(0,255,255);
    word bgcolor2 = selectedTile == 3 ? bgcolor : RGB565(255,255,255);

    char phaseStr[16];
    sprintf(phaseStr, "%ld", phase);
    byte ampl_len = strlen(phaseStr); // right align
    tft.setCursor(206-ampl_len*12, 202);
    tft.setTextSize(2);
    tft.setTextColor(bgcolor2);
    tft.print(phaseStr);
    tft.println("deg");

    //drawDelimiter(selectedTile, 1000, tft);

}

void waveMainContent(int selectedTile, TFT_eSPI& tft) {
    
    word bgcolor = getSelectedChannel() == 1 ? RGB565(255,255,0) : RGB565(0,255,255);
    word bgcolor2 = getSelectedChannel() == 1 ? RGB565(100,100,0) : RGB565(0,100,100);
    word bgcolor3 = selectedTile == 1 ? bgcolor : RGB565(255,255,255);
    tft.drawRoundRect(0, 0, 250, 240, 10, bgcolor);
    tft.drawRoundRect(0, 0, 50, 32, 10, bgcolor);
    tft.drawLine(0, 48, 250, 48, bgcolor);
    tft.drawLine(125, 96, 250, 96, bgcolor);
    tft.drawLine(125, 144, 250, 144, bgcolor);
    tft.drawLine(125, 192, 250, 192, bgcolor);
    tft.drawRect(0,48,125,144, bgcolor);

    tft.setCursor(10, 10);
    tft.setTextSize(2);
    tft.setTextColor(bgcolor);
    tft.fillRect(10, 10, 35, 15, TFT_BLACK);
    tft.println(getSelectedChannel() == 1 ? "CH1" : "CH2");
}





void changeValue(int tile, int64_t deltaValue, TFT_eSPI& tft) {
    int channel = getSelectedChannel();
    if (tile == 1) { // Frequency tile
        
        uint64_t currentFreq = getChannelFrequency(channel);
        
        if (deltaValue < 0) {
            uint64_t absDelta = (uint64_t)(-deltaValue); // convert safely
            if (absDelta > currentFreq) {
                currentFreq = 0;
            } else {
                currentFreq -= absDelta;
            }
        } else if( currentFreq+deltaValue >60000000000){
            currentFreq = 60000000000;
        } else {
            currentFreq += (uint64_t)deltaValue; // safe addition
        }

        // Set the new frequency
        setFrequency(channel, currentFreq);
        // Update display
        if(tile == 1){
            waveFrequencyDisplay(tile, tft);
        }
    }
    if(tile == 2){
        deltaValue = deltaValue / 1000;
        uint32_t amplitude = getChannelAmplitude(channel);
        if (deltaValue < 0) {
            uint64_t absDelta = (uint64_t)(-deltaValue); // convert safely
            if (absDelta > amplitude) {
                amplitude = 0;
            } else {
                amplitude -= absDelta;
            }
        } else if( amplitude+deltaValue >20000){
            amplitude = 20000;
        } else {
            amplitude += (uint64_t)deltaValue; // safe addition
        }
        setAmplitude(channel, amplitude);
        waveAmplitudeDisplay(tile, tft);
    }
    if(tile == 3){
        int32_t offset = getChannelOffset(channel);
        // Offset limits: -10000 to +10000 mV
        
        deltaValue = deltaValue / 1000;
        if (deltaValue < 0) {
            int32_t absDelta = -deltaValue; // safe since deltaValue < 0
            if (offset - absDelta < -10000) {
                offset = -10000;
            } else {
                offset -= absDelta;
            }
        } else if (offset + deltaValue > 10000) {
            offset = 10000;
        } else {
            offset += deltaValue;
        }

        setOffset(channel, offset);
        waveOffsetDisplay(tile, tft);
    }
    if(tile == 4){


    }
    if(tile == 5){
        deltaValue = deltaValue / 1000;
        int32_t phase = getChannelPhase(channel);
        // Phase limits: 0 to 360 degrees
        
        if (deltaValue < 0) {
            int32_t absDelta = -deltaValue; // safe since deltaValue < 0
            if (phase - absDelta < 0) {
                phase = 0;
            } else {
                phase -= absDelta;
            }
        } else if (phase + deltaValue > 360) {
            phase = 360;
        } else {
            phase += deltaValue;
        }

        setPhase(channel, phase);
        wavePhaseDisplay(tile, tft);
    }

}

void waveWaveformDisplay(TFT_eSPI& tft) {
    uint32_t Samples[100];

    int channel = getSelectedChannel();
    WaveformType waveform = getChannelWaveform(channel);

    //FPGA_GetResampledWaveform(waveform, Samples); // gets stuck...
 
    for (int i = 0; i < 100; i++) {
        tft.drawPixel(10+i, 160 - Samples[i] / 100, TFT_WHITE);
    }
    
    tft.setCursor(6, 52);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.println(waveformToString(waveform));
}

void drawWavePage(TFT_eSPI& tft) {
    // Clear only main content area, not sidebar
    tft.fillRect(0, 0, 260, 240, TFT_BLACK);

    setChannelWaveform(1, WAVE_SAW);
    setChannelWaveform(2, WAVE_SINE);

    waveSidebar(0, tft);
    waveMainContent(0, tft);
    waveFrequencyDisplay(0, tft);
    waveAmplitudeDisplay(0, tft);
    waveOffsetDisplay(0, tft);
    //waveRiseFallDisplay(0, tft);
    wavePhaseDisplay(0, tft);
    waveWaveformDisplay(tft);
    selectedTile = 0;    
}



void handleWavePageInput(char key, TFT_eSPI& tft) {
    // Handle specific key presses for wave selection
    static uint64_t delta = 1000;
    static int oldSelectedTile = 0;

    word colorActive = RGB565(65,28,28);
    word colorInactive = RGB565(65,65,65);

    switch(key) {
        case '-':
            changeValue(selectedTile, -delta, tft);
            break;
        case '+':
            changeValue(selectedTile, delta, tft);
            break;
        case '<':
            // Increase delta unit: mHz -> Hz -> kHz -> MHz
            if (delta < 10000000000) {
                delta*=10;
            }
            drawDelimiter(selectedTile, delta,tft);
            delay(100);
            break;
        case '>':
            // Decrease delta unit: MHz -> kHz -> Hz -> mHz
            if (delta > 1) {
                delta/=10;
            }
            drawDelimiter(selectedTile, delta,tft);
            delay(100);
            break;
        
        case 'K': // OK button

            break;
        case 'A':
            if (selectedTile != 1) {
                selectedTile = 1;
            }else{
                selectedTile = 0;
            }
            break;
        case 'B':
            if (selectedTile != 2) {
                selectedTile = 2;
            }else{
                selectedTile = 0;
            }
            break;
        case 'C':
            if (selectedTile != 3) {
                selectedTile = 3;
            }else{
                selectedTile = 0;
            }
            break;
        case 'D':
            if (selectedTile != 4) {
                selectedTile = 4;
            }else{
                selectedTile = 0;
            }
            break;
        case 'E':
            if (selectedTile != 5) {
                selectedTile = 5;
            }else{
                selectedTile = 0;
            }
            break;
        case '1':
            delay(20);
            forceBeepOff();
            drawWavePage(tft);
            selectedTile = 0;
            break;
        case '2':
            delay(20);
            forceBeepOff();
            drawWavePage(tft);
            selectedTile = 0;
            break;
    }
    if(selectedTile != oldSelectedTile){
        delay(20);
        forceBeepOff();
        
        waveSidebar(selectedTile, tft);
        waveMainContent(selectedTile, tft);

        if((oldSelectedTile == 1 && selectedTile != 1) || (selectedTile == 1 && oldSelectedTile != 1)){
            waveFrequencyDisplay(selectedTile, tft);
            drawDelimiter(selectedTile, delta, tft);
        }
        if((oldSelectedTile == 2 && selectedTile != 2) || (selectedTile == 2 && oldSelectedTile != 2)){
            waveAmplitudeDisplay(selectedTile, tft);
            drawDelimiter(selectedTile, delta, tft);
        }
        if((oldSelectedTile == 3 && selectedTile != 3) || (selectedTile == 3 && oldSelectedTile != 3)){
            waveOffsetDisplay(selectedTile, tft);
            drawDelimiter(selectedTile, delta, tft);
        }
        if((oldSelectedTile == 4 && selectedTile != 4) || (selectedTile == 4 && oldSelectedTile != 4)){
            //waveRiseFallDisplay(selectedTile, tft);
            drawDelimiter(selectedTile, delta, tft);
        }
        if((oldSelectedTile == 5 && selectedTile != 5) || (selectedTile == 5 && oldSelectedTile != 5)){
            wavePhaseDisplay(selectedTile, tft);
            drawDelimiter(selectedTile, delta, tft);
        }
        oldSelectedTile = selectedTile;
    }
}

#endif
