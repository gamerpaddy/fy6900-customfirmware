#include "Adafruit_Keypad.h"
#include <HardwareSerial.h>

#define USER_SETUP_LOADED
#include "User_Setup.h"
#include "SPI.h"
#include "TFT_eSPI.h"

#include <Button.h>

#define BUZZER PA12
#define PWRLED PA15
#define CH1LED PB8
#define CH2LED PB9

#define PWRBTN PA10
#define ENCA PA8
#define ENCB PA9

#define LCD_BL PA11
#define LCD_RES PC13
#define LCD_SCL PA5
#define LCD_DC PB10
#define LCD_SDA PA7
#define LCD_CS PA4

#define UART_TX PA2
#define UART_RX PA3

#define VCO_IN PA0
#define FPGARDY PA1
#define SPI2_NSS PB12
#define SPI2_SCK PB13
#define SPI2_MISO PB14
#define SPI2_MOSI PB15

#define KEY_R1 PB0
#define KEY_R2 PB1
#define KEY_R3 PB2
#define KEY_R4 PB3

#define KEY_C1 PB4
#define KEY_C2 PB5
#define KEY_C3 PB6
#define KEY_C4 PB7

// F1 PB0 PB4
// F2 PB1 PB4
// F3 PB2 PB4
// F4 PB3 PB4

// F5 PB2 PB5
// MOD PB0 PB5
// SYS PB1 PB5
// WAVE PB3 PB5

// SWEEP PB0 PB6
// SYNC PB2 PB6
// MEAS PB3 PB6

// CH1 PB0 PB7
// CH2 PB1 PB7
// LEFT PB2 PB7
// RIGHT PB3 PB7

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

HardwareSerial Serial2(UART_RX, UART_TX); // USB UART on the back

boolean powerState = false;

void setup()
{
  Serial2.begin(9600);

  pinMode(CH1LED, OUTPUT);
  pinMode(CH2LED, OUTPUT);
  pinMode(PWRLED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  pinMode(ENCA, INPUT_PULLUP);
  pinMode(ENCB, INPUT_PULLUP);

  pinMode(LCD_BL, OUTPUT);

  keypad.begin();
  pwrbtn.begin();

  tft.init();
  tft.setRotation(3);
  Serial2.println("LCD INIT");
  tft.fillScreen(TFT_BLACK);

  tft.fillScreen(TFT_BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(TFT_YELLOW);
  tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(TFT_RED);
  tft.setTextSize(3);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(5);
  tft.println("Groop");
  tft.setTextSize(2);
  tft.println("I implore thee,");
  // tft.setTextColor(TFT_GREEN,TFT_BLACK);
  tft.setTextSize(1);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");

  delay(1500);

  digitalWrite(PWRLED, 0);
  digitalWrite(CH1LED, 1);
  digitalWrite(CH2LED, 1);

  delay(500);
  Serial2.println("GO");
}

long position = -999;
bool t = false;
void loop()
{

  if (pwrbtn.pressed())
  {
    powerState = !powerState;

    if (powerState)
    {
      Serial2.println("TURN ON");
      digitalWrite(BUZZER, 1);
      delay(50);
      digitalWrite(BUZZER, 0);
      digitalWrite(PWRLED, 1);
      digitalWrite(LCD_BL, 1);

      // tftPrintTest();
    }
    else
    {
      digitalWrite(PWRLED, 0);
      digitalWrite(LCD_BL, 0);
    }
  }

  if (powerState)
  {
    keypad.tick();

    while (keypad.available())
    {
      keypadEvent e = keypad.read();
      Serial2.print((char)e.bit.KEY);
      if (e.bit.EVENT == KEY_JUST_PRESSED)
        Serial2.println(" pressed");
      else if (e.bit.EVENT == KEY_JUST_RELEASED)
        Serial2.println(" released");
    }
    digitalWrite(CH1LED, digitalRead(ENCA));
    digitalWrite(CH2LED, digitalRead(ENCB));
  }
}
