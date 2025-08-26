#ifndef PAGE_MANAGER_H
#define PAGE_MANAGER_H

#include <TFT_eSPI.h>
#include "channel_manager.h"
#include "page_wave.h"
#include "page_mod.h"
#include "page_sweep.h"
#include "page_counter.h"
#include "page_vco.h"
#include "page_system.h"

// Forward declarations for beep functions
void forceBeepOff();

// Page enumeration
enum PageType {
    PAGE_WAVE = 0,
    PAGE_MOD = 1,
    PAGE_SWEEP = 2,
    PAGE_COUNTER = 3,
    PAGE_VCO = 4,
    PAGE_SYSTEM = 5,
    PAGE_COUNT = 6
};

class PageManager {
private:
    PageType currentPage;
    bool pageChanged;
    
public:
    PageManager() : currentPage(PAGE_WAVE), pageChanged(true) {}
    
    // Get current page
    PageType getCurrentPage() { return currentPage; }
    
    // Check if page has changed since last draw
    bool hasPageChanged() { return pageChanged; }
    
    // Mark page as drawn
    void markPageDrawn() { pageChanged = false; }
    
    // Switch to a specific page
    void switchToPage(PageType page) {
        if (page != currentPage && page < PAGE_COUNT) {
            currentPage = page;
            pageChanged = true;
        }
    }
    
    // Force redraw of current page (useful after power on)
    void forceRedraw() {
        pageChanged = true;
    }
    
    // Handle navigation key presses
    bool handleNavigationKey(char key) {
        switch(key) {
            case 'W': // Wave page
                switchToPage(PAGE_WAVE);
                return true;
            case 'M': // Mod page
                switchToPage(PAGE_MOD);
                return true;
            case 'S': // Sweep page
                switchToPage(PAGE_SWEEP);
                return true;
            case 'T': // Counter page
                switchToPage(PAGE_COUNTER);
                return true;
            case 'V': // VCO page
                switchToPage(PAGE_VCO);
                return true;
            case 'Y': // System page
                switchToPage(PAGE_SYSTEM);
                return true;
            default:
                return false; // Key not handled by navigation
        }
    }
    
    // Draw the current page with optimizations
    void drawCurrentPage(TFT_eSPI& tft) {
        if (!pageChanged) return; // Only redraw if page changed
        
        // Force beep off before drawing to prevent long beeps
        delay(50);
        forceBeepOff();
        
        switch(currentPage) {
            case PAGE_WAVE:
                drawWavePage(tft);
                break;
            case PAGE_MOD:
                drawModPage(tft);
                break;
            case PAGE_SWEEP:
                drawSweepPage(tft);
                break;
            case PAGE_COUNTER:
                drawCounterPage(tft);
                break;
            case PAGE_VCO:
                drawVCOPage(tft);
                break;
            case PAGE_SYSTEM:
                drawSystemPage(tft);
                break;
        }
        markPageDrawn();
    }
    
    // Handle input for the current page
    void handlePageInput(char key, TFT_eSPI& tft) {
        // First check if it's a navigation key
        if (handleNavigationKey(key)) {
            return; // Navigation handled, don't pass to page
        }
        
        // Check for channel keys (1 and 2) - these work globally
        if (key == '1') {
            handleChannelKey(1);
        }
        if (key == '2') {
            handleChannelKey(2);
        }
                
        // Pass input to current page handler
        switch(currentPage) {
            case PAGE_WAVE:
                handleWavePageInput(key, tft);
                break;
            case PAGE_MOD:
                handleModPageInput(key, tft);
                break;
            case PAGE_SWEEP:
                handleSweepPageInput(key, tft);
                break;
            case PAGE_COUNTER:
                handleCounterPageInput(key, tft);
                break;
            case PAGE_VCO:
                handleVCOPageInput(key, tft);
                break;
            case PAGE_SYSTEM:
                handleSystemPageInput(key, tft);
                break;
        }
    }
    
    // Get page name as string
    const char* getPageName() {
        switch(currentPage) {
            case PAGE_WAVE: return "WAVE";
            case PAGE_MOD: return "MOD";
            case PAGE_SWEEP: return "SWEEP";
            case PAGE_COUNTER: return "COUNTER";
            case PAGE_VCO: return "VCO";
            case PAGE_SYSTEM: return "SYSTEM";
            default: return "UNKNOWN";
        }
    }
};

#endif