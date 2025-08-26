#define BUZZER PA12
#define PWRLED PA15
#define CH1LED PB8
#define CH2LED PB9

#define PWRBTN PA10
#define ENCA PA9
#define ENCB PA8

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
#define REGSEL PB11
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

#define RGB565(r,g,b) ( ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3) )

#define USE_DMA_TO_TFT
#define COLOR_DEPTH 16
