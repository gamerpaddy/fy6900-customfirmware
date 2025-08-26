#ifndef PAGE_SWEEP_H
#define PAGE_SWEEP_H

#include <TFT_eSPI.h>

void drawSweepPage(TFT_eSPI& tft) {
    tft.fillScreen(TFT_BLACK);
    
    // Page title
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.setCursor(10, 10);
    tft.println("SWEEP");
    
    // Current sweep settings
    tft.setTextSize(1);
    tft.setTextColor(TFT_YELLOW);
    tft.setCursor(10, 40);
    tft.println("Mode: Linear");
    tft.setCursor(10, 55);
    tft.println("Start: 100 Hz");
    tft.setCursor(10, 70);
    tft.println("Stop:  10 kHz");
    tft.setCursor(10, 85);
    tft.println("Time:  1.0 s");
    tft.setCursor(10, 100);
    tft.println("Status: STOPPED");
    
    // Sweep modes
    tft.setTextColor(TFT_GREEN);
    tft.setCursor(10, 125);
    tft.println("Sweep Modes:");
    
    tft.setTextColor(TFT_CYAN);
    tft.setCursor(20, 145);
    tft.println("1. Linear");
    tft.setCursor(20, 160);
    tft.println("2. Logarithmic");
    tft.setCursor(20, 175);
    tft.println("3. Step");
    
    // Controls
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 195);
    tft.println("OK: Start/Stop");
    tft.setCursor(10, 210);
    tft.println("< >: Adjust values");
    
    // Current page indicator
    tft.setTextColor(TFT_RED);
    tft.setCursor(200, 210);
    tft.println("SWEEP");
}

void handleSweepPageInput(char key, TFT_eSPI& tft) {
    static bool sweepRunning = false;
    static int sweepMode = 1;
    static float startFreq = 100.0;
    static float stopFreq = 10000.0;
    static float sweepTime = 1.0;
    
    switch(key) {
        case 'K': // OK button - Start/Stop sweep
            sweepRunning = !sweepRunning;
            tft.fillRect(10, 100, 150, 15, TFT_BLACK);
            tft.setCursor(10, 100);
            tft.setTextColor(TFT_YELLOW);
            if (sweepRunning) {
                tft.println("Status: RUNNING");
            } else {
                tft.println("Status: STOPPED");
            }
            break;
        case '<':
            startFreq = (startFreq > 1) ? startFreq / 10 : 100000;
            tft.fillRect(10, 55, 150, 15, TFT_BLACK);
            tft.setCursor(10, 55);
            tft.setTextColor(TFT_YELLOW);
            tft.print("Start: ");
            if (startFreq >= 1000) {
                tft.print(startFreq / 1000);
                tft.println(" kHz");
            } else {
                tft.print(startFreq);
                tft.println(" Hz");
            }
            break;
        case '>':
            stopFreq = (stopFreq < 100000) ? stopFreq * 10 : 1;
            tft.fillRect(10, 70, 150, 15, TFT_BLACK);
            tft.setCursor(10, 70);
            tft.setTextColor(TFT_YELLOW);
            tft.print("Stop:  ");
            if (stopFreq >= 1000) {
                tft.print(stopFreq / 1000);
                tft.println(" kHz");
            } else {
                tft.print(stopFreq);
                tft.println(" Hz");
            }
            break;
        case '1':
        case '2':
        case '3':
            sweepMode = key - '0';
            tft.fillRect(10, 40, 150, 15, TFT_BLACK);
            tft.setCursor(10, 40);
            tft.setTextColor(TFT_YELLOW);
            switch(sweepMode) {
                case 1: tft.println("Mode: Linear"); break;
                case 2: tft.println("Mode: Logarithmic"); break;
                case 3: tft.println("Mode: Step"); break;
            }
            break;
    }
}

#endif