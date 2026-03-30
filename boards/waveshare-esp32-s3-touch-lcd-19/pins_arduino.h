#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>

#ifndef DEVICE_NAME
#define DEVICE_NAME "Waveshare ESP32-S3 Touch 1.9"
#endif

// =========================
// External SPI Bus (CC1101, NRF24, etc.)
// =========================
#define SPI_SCK_PIN  1
#define SPI_MOSI_PIN 2
#define SPI_MISO_PIN 3
#define SPI_SS_PIN   -1

static const uint8_t SS   = 5;
static const uint8_t MOSI = 2;
static const uint8_t MISO = 3;
static const uint8_t SCK  = 1;

// =========================
// I2C (Touch CST816 + IMU QMI8658 share this bus)
// =========================
#define GROVE_SDA 47
#define GROVE_SCL 48

static const uint8_t SDA = GROVE_SDA;
static const uint8_t SCL = GROVE_SCL;

// =========================
// Serial / GPS (free pins — NOT the I2C/touch pins)
// =========================
#define SERIAL_TX     19
#define SERIAL_RX     20
#define GPS_SERIAL_TX SERIAL_TX
#define GPS_SERIAL_RX SERIAL_RX

// =========================
// CC1101 RF module
// =========================
#define CC1101_GDO0_PIN  6
#define CC1101_SS_PIN    5
#define CC1101_MOSI_PIN  SPI_MOSI_PIN
#define CC1101_SCK_PIN   SPI_SCK_PIN
#define CC1101_MISO_PIN  SPI_MISO_PIN

// =========================
// NRF24L01 RF module
// =========================
#define NRF24_CE_PIN    8
#define NRF24_SS_PIN    7
#define NRF24_MOSI_PIN  SPI_MOSI_PIN
#define NRF24_SCK_PIN   SPI_SCK_PIN
#define NRF24_MISO_PIN  SPI_MISO_PIN

// =========================
// Display ST7789V2 (SPI) — pins shared by TFT_eSPI and LovyanGFX
// =========================
#define TFT_WIDTH  170
#define TFT_HEIGHT 320

#define TFT_MOSI 13
#define TFT_SCLK 10
#define TFT_CS   12
#define TFT_DC   11
#define TFT_RST  9
#define TFT_BL   14

// =========================
// Touch CST816 (I2C)
// =========================
#define TOUCH_SDA  GROVE_SDA  // GPIO 47
#define TOUCH_SCL  GROVE_SCL  // GPIO 48
#define TOUCH_RST  17
#define TOUCH_INT  21

// =========================
// SD Card - SDMMC 1-bit mode
// =========================
#define SDMMC_CLK 41
#define SDMMC_CMD 39
#define SDMMC_D0  40

// SPI SD disabled (board uses SDMMC only)
#define SDCARD_CS   -1
#define SDCARD_SCK  -1
#define SDCARD_MISO -1
#define SDCARD_MOSI -1

// =========================
// Display / Screen config
// =========================
#define HAS_SCREEN
#define ROTATION 1
#define MINBRIGHT (uint8_t)1

// Font Sizes
#define FP 1
#define FM 2
#define FG 3

// =========================
// Touch
// =========================
#define HAS_TOUCH

// =========================
// Buttons (only BOOT button = GPIO 0)
// Used as ESC / wake-up
// =========================
#define BTN_ALIAS "\"Boot\""
#define HAS_BTN   1
#define BTN_PIN   0
#define BTN_ACT   LOW

// =========================
// IR
// =========================
#define TXLED   16   // IR transmit LED
#define RXLED   18   // IR receiver
#define LED_ON  HIGH
#define LED_OFF LOW

// =========================
// Battery (ADC on GPIO 4)
// =========================
#define ANALOG_BAT_PIN 4

// =========================
// USB CDC on boot
// =========================
#define USB_as_HID 1

#endif /* Pins_Arduino_h */
