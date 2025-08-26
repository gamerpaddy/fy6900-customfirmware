#ifndef PAGE_VCO_H
#define PAGE_VCO_H


#include <TFT_eSPI.h>

void drawVCOPage(TFT_eSPI& tft) {
    tft.fillScreen(TFT_BLACK);
    
    // Page title
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.setCursor(10, 10);
    tft.println("VCO CONTROL");
    
 
    
    // Controls
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 210);
    tft.println("Encoder: Adjust | OK: Toggle");
    
    // Current page indicator
    tft.setTextColor(TFT_RED);
    tft.setCursor(250, 210);
    tft.println("VCO");
}

void handleVCOPageInput(char key, TFT_eSPI& tft) {
    static bool vcoEnabled = true;
    static float centerFreq = 1.0; // MHz
    static float deviation = 100.0; // kHz
    static float inputVoltage = 2.5; // V
    static int selectedParam = 1;
    
    switch(key) {
        case 'K': // OK button - Toggle VCO enable
            vcoEnabled = !vcoEnabled;
            tft.fillRect(10, 40, 150, 15, TFT_BLACK);
            tft.setCursor(10, 40);
            tft.setTextColor(TFT_YELLOW);
            if (vcoEnabled) {
                tft.println("Status: ENABLED");
            } else {
                tft.println("Status: DISABLED");
            }
            break;
        case '-':
            switch(selectedParam) {
                case 1: // Center frequency
                    centerFreq = (centerFreq > 0.1) ? centerFreq - 0.1 : 10.0;
                    tft.fillRect(10, 55, 200, 15, TFT_BLACK);
                    tft.setCursor(10, 55);
                    tft.setTextColor(TFT_YELLOW);
                    tft.print("Center Freq: ");
                    tft.print(centerFreq, 1);
                    tft.println(" MHz");
                    break;
                case 2: // Deviation
                    deviation = (deviation > 10) ? deviation - 10 : 1000;
                    tft.fillRect(10, 70, 200, 15, TFT_BLACK);
                    tft.setCursor(10, 70);
                    tft.setTextColor(TFT_YELLOW);
                    tft.print("Deviation: +/- ");
                    tft.print(deviation, 0);
                    tft.println(" kHz");
                    break;
            }
            break;
        case '+':
            switch(selectedParam) {
                case 1: // Center frequency
                    centerFreq = (centerFreq < 10.0) ? centerFreq + 0.1 : 0.1;
                    tft.fillRect(10, 55, 200, 15, TFT_BLACK);
                    tft.setCursor(10, 55);
                    tft.setTextColor(TFT_YELLOW);
                    tft.print("Center Freq: ");
                    tft.print(centerFreq, 1);
                    tft.println(" MHz");
                    break;
                case 2: // Deviation
                    deviation = (deviation < 1000) ? deviation + 10 : 10;
                    tft.fillRect(10, 70, 200, 15, TFT_BLACK);
                    tft.setCursor(10, 70);
                    tft.setTextColor(TFT_YELLOW);
                    tft.print("Deviation: +/- ");
                    tft.print(deviation, 0);
                    tft.println(" kHz");
                    break;
            }
            break;
        case '1':
        case '2':
        case '3':
        case '4':
            selectedParam = key - '0';
            // Clear previous selection indicator
            tft.fillRect(10, 145, 10, 60, TFT_BLACK);
            // Show new selection
            tft.setCursor(10, 130 + (selectedParam * 15));
            tft.setTextColor(TFT_RED);
            tft.println(">");
            break;
    }
}

#endif