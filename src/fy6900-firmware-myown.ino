#include "defines.h"
#include "Adafruit_Keypad.h"
#include <TFT_eSPI.h>
#include "SPI.h"
#include <Button.h>
#include "channel_manager.h"
#include "page_manager.h"
#include "page_system.h"
#include "fpga.h"




const byte ROWS = 4; // rows
const byte COLS = 4; // columns
char keys[ROWS][COLS] = {
    {'A', 'W', 'M', 'S'}, // BTN1 Wave Mod Sweep
    {'B', 'T', 'V', 'Y'}, // BTN2 Counter VCO System
    {'C', '1', 'K', '2'}, // BTN3 Ch1 OK Ch2
    {'D', 'E', '<', '>'}  // BTN4 BTN5 Left Right
};

byte rowPins[ROWS] = {KEY_R1, KEY_R2, KEY_R3, KEY_R4}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {KEY_C1, KEY_C2, KEY_C3, KEY_C4}; // connect to the column pinouts of the keypad
Adafruit_Keypad keypad = Adafruit_Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

Button pwrbtn(PWRBTN);
TFT_eSPI tft = TFT_eSPI();

// Page manager for UI navigation
PageManager pageManager;

bool powerState = true;

Channel channelManager = {1, CHANNEL_OFF, CHANNEL_OFF, 1000000UL, 1000000UL, 1000, 1000, 0, 0, 0, 0, WAVE_SINE, WAVE_SINE};

// Beep management for non-blocking operation
struct BeepState {
  bool isBeeping;
  unsigned long beepStartTime;
  const unsigned long beepDuration = 50; // 50ms beep duration
};

BeepState beepState = {false, 0, 50};


// Non-blocking beep function with forced timeout
void triggerBeep() {
  if (getSystemBeepEnabled()) {
    digitalWrite(BUZZER, HIGH);
    beepState.isBeeping = true;
    beepState.beepStartTime = millis();
  }
}

// Update beep state (call in main loop) - with safety timeout
void updateBeep() {
  if (beepState.isBeeping) {
    if (millis() - beepState.beepStartTime >= beepState.beepDuration) {
      digitalWrite(BUZZER, LOW);
      beepState.isBeeping = false;
    }
  }
}

// Emergency beep shutoff - call during long operations
void forceBeepOff() {
  if (beepState.isBeeping) {
    digitalWrite(BUZZER, LOW);
    beepState.isBeeping = false;
  }
}

// Channel functions (to be implemented later)
void channel1Function() {
  // Placeholder for channel 1 specific functionality
}

void channel2Function() {
  // Placeholder for channel 2 specific functionality
}

// Update LED states based on channel states
void updateChannelLEDs() {
  // LEDs use inverted logic: 0 = ON, 1 = OFF
  digitalWrite(CH1LED, (channelManager.ch1State == CHANNEL_ON) ? 0 : 1);
  digitalWrite(CH2LED, (channelManager.ch2State == CHANNEL_ON) ? 0 : 1);
}

// Handle channel key press logic
void handleChannelKey(int channel) {
  if (pageManager.getCurrentPage() != PAGE_WAVE) {
    // If not on WAVE page, switch to WAVE page and select the channel
    pageManager.switchToPage(PAGE_WAVE);
    channelManager.selectedChannel = channel;
  } else {
    // On WAVE page - handle channel logic
    if (channelManager.selectedChannel == channel) {
      // Same channel pressed - toggle on/off
      if (channel == 1) {
        channelManager.ch1State = (channelManager.ch1State == CHANNEL_OFF) ? CHANNEL_ON : CHANNEL_OFF;
        if (channelManager.ch1State == CHANNEL_ON) {
          channel1Function();
        }
      } else {
        channelManager.ch2State = (channelManager.ch2State == CHANNEL_OFF) ? CHANNEL_ON : CHANNEL_OFF;
        if (channelManager.ch2State == CHANNEL_ON) {
          channel2Function();
        }
      }
    } else {
      channelManager.selectedChannel = channel;
    }
  }
  updateChannelLEDs();
}

// Function to update backlight based on system setting
void updateBacklight() {
  if (powerState) {
    int backlightValue = map(getSystemBacklight(), 0, 100, 0, 255);
    analogWrite(PA11, backlightValue);
  }
}

void setup()
{
  Serial2.begin(9600);


  pinMode(CH1LED, OUTPUT);
  pinMode(CH2LED, OUTPUT);
  pinMode(PWRLED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  pinMode(ENCA, INPUT_PULLUP);
  pinMode(ENCB, INPUT_PULLUP);


  keypad.begin();
  pwrbtn.begin();

  tft.init();
  tft.setRotation(3);
  Serial2.println("INIT");
  // Set initial backlight based on system setting
  int backlightValue = map(getSystemBacklight(), 0, 100, 0, 255);
  analogWrite(PA11, backlightValue);
  tft.fillScreen(TFT_BLACK);

  // Show startup message
  tft.setCursor(50, 100);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.println("FY6900");
  tft.setCursor(30, 130);
  tft.setTextSize(1);
  tft.println("Signal Generator");
  tft.setCursor(70, 150);
  tft.println("Starting...");

  //FPGA_WaitReady(); //hangs
  FPGA_TestFlash();
  FPGA_Init();

  delay(500);

  // Initialize UI with WAVE page
  pageManager.switchToPage(PAGE_WAVE);

  // Initialize channel LEDs (both OFF initially)
  updateChannelLEDs();
  digitalWrite(PWRLED, 1);
  
  // Set backlight based on power state and system setting
  if (powerState) {
    int backlightValue = map(getSystemBacklight(), 0, 100, 0, 255);
    analogWrite(PA11, backlightValue);
  } else {
    analogWrite(PA11, 0);
  }
}

// Encoder variables
long encoderPosition = 0;
int lastEncA = HIGH;
int lastEncB = HIGH;
bool encoderChanged = false;



void loop()
{

  if (pwrbtn.pressed())
  {
    powerState = !powerState;

    if (powerState)
    {
      digitalWrite(BUZZER, 1);
      delay(50);
      digitalWrite(BUZZER, 0);
      digitalWrite(PWRLED, 1); //PWRLED OFF
      
      // Set backlight based on system setting (convert percentage to PWM value)
      int backlightValue = map(getSystemBacklight(), 0, 100, 0, 255);
      analogWrite(PA11, backlightValue); //BL on with PWM control
    }
    else
    {
      digitalWrite(BUZZER, 1);
      delay(50);
      digitalWrite(BUZZER, 0);
      digitalWrite(PWRLED, 0); //PWRLED ON
      analogWrite(PA11, 0); //BL off
    }
  }

  if (powerState)
  {
    // Update non-blocking beep state
    updateBeep();
    
    pageManager.drawCurrentPage(tft);
    
    int encA = digitalRead(ENCA);
    int encB = digitalRead(ENCB);
    
    if (encA != lastEncA) {
      if (encA == LOW) {
        if (encB == HIGH) {
          encoderPosition++;
          encoderChanged = true;
        } else {
          encoderPosition--;
          encoderChanged = true;
        }
      }
      lastEncA = encA;
    }
    
    if (encoderChanged) {
      // No beep for encoder movements
      delay(10); // Debounce delay
      if (encoderPosition > 0) {
        pageManager.handlePageInput('+', tft);
      } else {
        pageManager.handlePageInput('-', tft);
      }
      encoderPosition = 0;
      encoderChanged = false;
    }
    
    keypad.tick();

    while (keypad.available())
    {
      keypadEvent e = keypad.read();
      char key = (char)e.bit.KEY;
      
      if (e.bit.EVENT == KEY_JUST_RELEASED)
      {
        triggerBeep();
        pageManager.handlePageInput(key, tft);
        delay(50);
      }
    }
    
  }
}
