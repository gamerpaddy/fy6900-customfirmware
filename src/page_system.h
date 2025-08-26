#ifndef PAGE_SYSTEM_H
#define PAGE_SYSTEM_H

#include <TFT_eSPI.h>
#include "channel_manager.h"

// Forward declaration for backlight update function
void updateBacklight();

// Global system settings
static int systemBacklight = 100;
static bool systemBeepEnabled = true;
static bool systemAutoSave = true;
static int systemSelectedItem = 1;

// Getter functions for system settings
int getSystemBacklight() { return systemBacklight; }
bool getSystemBeepEnabled() { return systemBeepEnabled; }
bool getSystemAutoSave() { return systemAutoSave; }

void drawSystemPage(TFT_eSPI& tft) {
    tft.fillRect(0, 0, 320, 240, TFT_BLACK);
    
    // Optimized drawing with batched operations
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 10);
    tft.print("SYSTEM");
    
    // System information - batched
    tft.setTextSize(1);
    tft.setTextColor(TFT_YELLOW);
    tft.setCursor(10, 40);
    tft.print("FY6900 Signal Generator v1.0");
    tft.setCursor(10, 55);
    tft.print("Hardware v2.1 STM32F103CB");
    
    // System settings - condensed
    tft.setTextColor(TFT_GREEN);
    tft.setCursor(10, 80);
    tft.print("Settings:");
    
    tft.setTextColor(TFT_CYAN);
    tft.setCursor(10, 100);
    tft.print("1.Backlight:");
    tft.print(systemBacklight);
    tft.print("% 2.Beep:");
    tft.print(systemBeepEnabled ? "ON" : "OFF");
    tft.setCursor(10, 115);
    tft.print("3.AutoSave:");
    tft.print(systemAutoSave ? "ON" : "OFF");
    tft.print(" 4.Lang:EN");
    tft.setCursor(10, 130);
    tft.print("5.Calibration 6.Factory Reset");
    
    // Test: Show selected channel (accessible globally)
    tft.setCursor(10, 150);
    tft.setTextColor(TFT_WHITE);
    tft.print("Selected Channel: CH");
    tft.print(getSelectedChannel());
}

void handleSystemPageInput(char key, TFT_eSPI& tft) {
    switch(key) {
        case '<':
            switch(systemSelectedItem) {
                case 1: // Backlight
                    systemBacklight = (systemBacklight > 10) ? systemBacklight - 10 : 10;
                    tft.fillRect(20, 130, 200, 15, TFT_BLACK);
                    tft.setCursor(20, 130);
                    tft.setTextColor(TFT_CYAN);
                    tft.print("1. Backlight: ");
                    tft.print(systemBacklight);
                    tft.println("%");
                    updateBacklight(); // Update actual backlight
                    break;
            }
            break;
        case '>':
            switch(systemSelectedItem) {
                case 1: // Backlight
                    systemBacklight = (systemBacklight < 100) ? systemBacklight + 10 : 100;
                    tft.fillRect(20, 130, 200, 15, TFT_BLACK);
                    tft.setCursor(20, 130);
                    tft.setTextColor(TFT_CYAN);
                    tft.print("1. Backlight: ");
                    tft.print(systemBacklight);
                    tft.println("%");
                    updateBacklight(); // Update actual backlight
                    break;
            }
            break;
        case 'K': // OK button
            switch(systemSelectedItem) {
                case 2: // Toggle beep
                    systemBeepEnabled = !systemBeepEnabled;
                    tft.fillRect(20, 145, 200, 15, TFT_BLACK);
                    tft.setCursor(20, 145);
                    tft.setTextColor(TFT_CYAN);
                    tft.print("2. Beep: ");
                    tft.println(systemBeepEnabled ? "ON" : "OFF");
                    break;
                case 3: // Toggle auto save
                    systemAutoSave = !systemAutoSave;
                    tft.fillRect(20, 160, 200, 15, TFT_BLACK);
                    tft.setCursor(20, 160);
                    tft.setTextColor(TFT_CYAN);
                    tft.print("3. Auto Save: ");
                    tft.println(systemAutoSave ? "ON" : "OFF");
                    break;
                case 5: // Calibration
                    tft.setCursor(10, 220);
                    tft.setTextColor(TFT_WHITE);
                    tft.println("Calibration started...");
                    break;
                case 6: // Factory reset
                    tft.setCursor(10, 220);
                    tft.setTextColor(TFT_RED);
                    tft.println("Factory reset confirmed!");
                    break;
            }
            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
            systemSelectedItem = key - '0';
            // Clear previous selection indicator
            tft.fillRect(10, 130, 10, 90, TFT_BLACK);
            // Show new selection
            tft.setCursor(10, 115 + (systemSelectedItem * 15));
            tft.setTextColor(TFT_RED);
            tft.println(">");
            break;
    }
}

#endif