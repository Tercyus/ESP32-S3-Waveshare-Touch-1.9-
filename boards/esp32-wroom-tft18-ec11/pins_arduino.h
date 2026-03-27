#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>

#ifndef DEVICE_NAME
#define DEVICE_NAME "ESP32 WROOM + TFT 1.8 + EC11"
#endif

// =========================
// SPI bus (VSPI - hardware)
// =========================
static const uint8_t SS   = 17;
static const uint8_t MOSI = 23;
static const uint8_t MISO = 19;
static const uint8_t SCK  = 18;

#define SPI_SCK_PIN  18
#define SPI_MOSI_PIN 23
#define SPI_MISO_PIN 19
#define SPI_SS_PIN   -1  // user-configurable for external modules

// =========================
// I2C
// =========================
#define GROVE_SDA 21
#define GROVE_SCL 22

static const uint8_t SDA = GROVE_SDA;
static const uint8_t SCL = GROVE_SCL;

// =========================
// Serial / GPS / BadUSB CH9329
// =========================
#define SERIAL_TX    1
#define SERIAL_RX    3
#define GPS_SERIAL_TX SERIAL_TX
#define GPS_SERIAL_RX SERIAL_RX
#define BAD_TX SERIAL_TX
#define BAD_RX SERIAL_RX

// =========================
// Display ST7735S (SPI)
// =========================
#define TFT_WIDTH   128
#define TFT_HEIGHT  160

#define TFT_SCLK  18
#define TFT_MOSI  23
#define TFT_MISO  19
#define TFT_CS    17
#define TFT_DC    16
#define TFT_RST   4
#define TFT_BL    26

// =========================
// EC11 Rotary Encoder
// =========================
#define ENCODER_INA  32   // CLK (A)
#define ENCODER_INB  33   // DT  (B)
#define ENCODER_KEY  25   // SW  (push button)

// =========================
// Navigation (via encoder)
// =========================
#define HAS_BTN    1
#define BTN_PIN    ENCODER_KEY
#define BTN_ACT    LOW
#define BTN_ALIAS  '"OK"'
#define SEL_BTN    ENCODER_KEY
#define UP_BTN     -1
#define DW_BTN     -1

// =========================
// SD Card (SPI, optional)
// Shares VSPI bus with display
// =========================
#define SDCARD_CS   15
#define SDCARD_SCK  SPI_SCK_PIN
#define SDCARD_MISO SPI_MISO_PIN
#define SDCARD_MOSI SPI_MOSI_PIN

// =========================
// External RF/RFID modules
// Pins user-configurable via Bruce menu
// =========================
#define CC1101_GDO0_PIN  -1
#define CC1101_SS_PIN    -1
#define CC1101_MOSI_PIN  SPI_MOSI_PIN
#define CC1101_SCK_PIN   SPI_SCK_PIN
#define CC1101_MISO_PIN  SPI_MISO_PIN

#define NRF24_CE_PIN     -1
#define NRF24_SS_PIN     -1
#define NRF24_MOSI_PIN   SPI_MOSI_PIN
#define NRF24_SCK_PIN    SPI_SCK_PIN
#define NRF24_MISO_PIN   SPI_MISO_PIN

// =========================
// IR (user-configurable)
// =========================
#define TXLED    -1
#define LED_ON   HIGH
#define LED_OFF  LOW

// =========================
// Battery (no ADC pin on this setup)
// =========================
#define ANALOG_BAT_PIN -1

// =========================
// Screen config
// =========================
#define HAS_SCREEN 1
#define ROTATION   1
#define MINBRIGHT  10

// Font sizes
#define FP 1
#define FM 1
#define FG 2

#endif /* Pins_Arduino_h */
