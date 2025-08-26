
#define ILI9341_DRIVER       // Generic driver for common displays


#define TFT_RGB_ORDER TFT_BGR  // Colour order Red-Green-Blue

#define CGRAM_OFFSET

#define TFT_WIDTH  240 // ST7789 240 x 240 and 240 x 320
#define TFT_HEIGHT 280 // ST7789 240 x 320

#define TFT_BL   PA11            // LED back-light control pin
#define TFT_BACKLIGHT_ON HIGH  // Level to turn ON back-light (HIGH or LOW)

#define TFT_MOSI  PA7  // Automatically assigned with ESP8266 if not defined
#define TFT_SCLK  PA5  // Automatically assigned with ESP8266 if not defined
#define TFT_CS    PA4  // Chip select control pin D8
#define TFT_DC    PB10  // Data Command control pin
#define TFT_RST   PC13  // Reset pin (could connect to NodeMCU RST, see next line)


#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
//#define LOAD_FONT8N // Font 8. Alternative to Font 8 above, slightly narrower, so 3 digits fit a 160 pixel TFT
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

// Comment out the #define below to stop the SPIFFS filing system and smooth font code being loaded
// this will save ~20kbytes of FLASH
//#define SMOOTH_FONT

#define SPI_FREQUENCY  80000000
