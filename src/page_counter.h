#ifndef PAGE_COUNTER_H
#define PAGE_COUNTER_H

#include <TFT_eSPI.h>

void drawCounterPage(TFT_eSPI& tft) {
    tft.fillScreen(TFT_BLACK);
    
    // Page title
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.setCursor(10, 10);
    tft.println("COUNTER");
    
    // Current measurement
    tft.setTextSize(3);
    tft.setTextColor(TFT_GREEN);
    tft.setCursor(10, 40);
    tft.println("1.234567");
    tft.setTextSize(2);
    tft.setCursor(10, 75);
    tft.println("MHz");
    
    // Counter settings
    tft.setTextSize(1);
    tft.setTextColor(TFT_YELLOW);
    tft.setCursor(10, 105);
    tft.println("Gate Time: 1s");
    tft.setCursor(10, 120);
    tft.println("Coupling: DC");
    tft.setCursor(10, 135);
    tft.println("Trigger: AUTO");
    
    // Measurement modes
    tft.setTextColor(TFT_CYAN);
    tft.setCursor(10, 155);
    tft.println("Modes:");
    tft.setCursor(20, 170);
    tft.println("1. Frequency");
    tft.setCursor(20, 185);
    tft.println("2. Period");
    tft.setCursor(20, 200);
    tft.println("3. Pulse Width");
    
    // Current page indicator
    tft.setTextColor(TFT_RED);
    tft.setCursor(200, 220);
    tft.println("COUNT");
}

void handleCounterPageInput(char key, TFT_eSPI& tft) {
    static int gateTime = 1;
    static int mode = 1;
    static bool measuring = false;
    
    switch(key) {
        case 'K': // OK button - Start/Stop measurement
            measuring = !measuring;
            if (measuring) {
                // Simulate measurement update
                tft.fillRect(10, 40, 200, 40, TFT_BLACK);
                tft.setTextSize(3);
                tft.setTextColor(TFT_GREEN);
                tft.setCursor(10, 40);
                switch(mode) {
                    case 1:
                        tft.println("1.234567");
                        tft.setTextSize(2);
                        tft.setCursor(10, 75);
                        tft.println("MHz");
                        break;
                    case 2:
                        tft.println("810.3");
                        tft.setTextSize(2);
                        tft.setCursor(10, 75);
                        tft.println("ns");
                        break;
                    case 3:
                        tft.println("405.1");
                        tft.setTextSize(2);
                        tft.setCursor(10, 75);
                        tft.println("ns");
                        break;
                }
            }
            break;
        case '<':
            gateTime = (gateTime > 1) ? gateTime / 10 : 100;
            tft.fillRect(10, 105, 150, 15, TFT_BLACK);
            tft.setCursor(10, 105);
            tft.setTextColor(TFT_YELLOW);
            tft.print("Gate Time: ");
            if (gateTime >= 1) {
                tft.print(gateTime);
                tft.println("s");
            } else {
                tft.print(gateTime * 1000);
                tft.println("ms");
            }
            break;
        case '>':
            gateTime = (gateTime < 100) ? gateTime * 10 : 1;
            tft.fillRect(10, 105, 150, 15, TFT_BLACK);
            tft.setCursor(10, 105);
            tft.setTextColor(TFT_YELLOW);
            tft.print("Gate Time: ");
            if (gateTime >= 1) {
                tft.print(gateTime);
                tft.println("s");
            } else {
                tft.print(gateTime * 1000);
                tft.println("ms");
            }
            break;
        case '1':
        case '2':
        case '3':
            mode = key - '0';
            // Clear and redraw mode indicator
            tft.fillRect(155, 155, 100, 60, TFT_BLACK);
            tft.setCursor(155, 155 + (mode - 1) * 15);
            tft.setTextColor(TFT_RED);
            tft.println("<<");
            break;
    }
}

#endif