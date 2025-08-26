#ifndef PAGE_MOD_H
#define PAGE_MOD_H

#include <TFT_eSPI.h>

void drawModPage(TFT_eSPI& tft) {
    tft.fillRect(0, 0, 320, 240, TFT_BLACK);
    
    // Batch drawing for speed
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 10);
    tft.print("MODULATION");
    
    // Current settings - batched
    tft.setTextSize(1);
    tft.setTextColor(TFT_YELLOW);
    tft.setCursor(10, 40);
    tft.print("Type:AM Depth:50% Freq:1000Hz");
    
    // Modulation types - condensed
    tft.setTextColor(TFT_GREEN);
    tft.setCursor(10, 70);
    tft.print("Types:");
    tft.setTextColor(TFT_CYAN);
    tft.setCursor(10, 90);
    tft.print("1.AM 2.FM 3.PM 4.ASK 5.FSK");
    
    // Instructions
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 195);
    tft.print("< > to adjust");
}

void handleModPageInput(char key, TFT_eSPI& tft) {
    static int modType = 1;
    static int modDepth = 50;
    static int modFreq = 1000;
    
    switch(key) {
        case '<':
            modDepth = (modDepth > 0) ? modDepth - 10 : 100;
            break;
        case '>':
            modDepth = (modDepth < 100) ? modDepth + 10 : 0;
            break;
        case '1':
            modType = 1; // AM
            break;
        case '2':
            modType = 2; // FM
            break;
        case 'K': // OK button
            // Update display
            tft.fillRect(10, 40, 200, 45, TFT_BLACK);
            tft.setCursor(10, 40);
            tft.setTextColor(TFT_YELLOW);
            switch(modType) {
                case 1: tft.println("Type: AM"); break;
                case 2: tft.println("Type: FM"); break;
                case 3: tft.println("Type: PM"); break;
                case 4: tft.println("Type: ASK"); break;
                case 5: tft.println("Type: FSK"); break;
            }
            tft.setCursor(10, 55);
            tft.print("Depth: ");
            tft.print(modDepth);
            tft.println("%");
            tft.setCursor(10, 70);
            tft.print("Freq: ");
            tft.print(modFreq);
            tft.println(" Hz");
            break;
    }
}

#endif