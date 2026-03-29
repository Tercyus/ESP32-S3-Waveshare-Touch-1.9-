# Bruce Firmware — Waveshare ESP32-S3 Touch 1.9"

Port of [Bruce Firmware](https://github.com/BruceDevices/Bruce) for the
**Waveshare ESP32-S3 Touch LCD 1.9"** board.

> **Port by:** [@Tercyus](https://github.com/Tercyus)
> **Original firmware:** [@BruceDevices](https://github.com/BruceDevices)

---

## Hardware

| | |
|---|---|
| **Board** | Waveshare ESP32-S3 Touch LCD 1.9" |
| **MCU** | ESP32-S3 (dual-core 240 MHz, 8 MB PSRAM) |
| **Flash** | 16 MB |
| **Display** | 1.9" ST7789V2 — 170×320 px, SPI |
| **Touch** | CST816 capacitive — I2C |
| **IMU** | QMI8658 (shares I2C bus) |
| **SD Card** | SDMMC 1-bit mode |
| **USB** | USB-C — CDC on boot (serial monitor) |

---

## Features supported

- [x] Display (LovyanGFX DMA — ST7789V2 @ 80 MHz SPI)
- [x] Capacitive touch screen (CST816)
- [x] BOOT button (GPIO 0) as ESC / wake-up
- [x] SD Card via SDMMC
- [x] Battery level (ADC GPIO 4)
- [x] Deep sleep / wake on button
- [x] CC1101 433/868/915 MHz via SPI (configurable pins)
- [x] NRF24L01 2.4 GHz via SPI (configurable pins)
- [x] PN532 RFID/NFC via I2C (Grove connector)
- [x] IR TX / IR RX (configurable pins)
- [x] USB HID — BadUSB
- [x] WiFi attacks (deauth, evil twin, captive portal)
- [x] Bluetooth tools

---

## Pin map

### Display (ST7789V2 — SPI3)

| Signal | GPIO |
|--------|------|
| MOSI   | 13   |
| SCLK   | 10   |
| CS     | 12   |
| DC     | 11   |
| RST    | 9    |
| BL     | 14 (active-LOW PWM) |

### Touch (CST816 — I2C)

| Signal | GPIO |
|--------|------|
| SDA    | 47   |
| SCL    | 48   |
| RST    | 17   |
| INT    | 21   |

### SD Card (SDMMC 1-bit)

| Signal | GPIO |
|--------|------|
| CLK    | 41   |
| CMD    | 39   |
| D0     | 40   |

### Other

| Signal      | GPIO |
|-------------|------|
| BOOT button | 0    |
| Battery ADC | 4    |
| I2C SDA     | 47   |
| I2C SCL     | 48   |

---

## Compatible modules

### Via Grove connector (I2C) — plug & play

| Module | Function |
|--------|----------|
| **PN532** | RFID/NFC — read/write Mifare cards, emulation |
| **SSD1306 / SH1106** | Secondary OLED display |

### Via SPI — free GPIO pins + Bruce Config Pins menu

CC1101 and NRF24 share the SPI bus (SCK/MOSI/MISO). Both can be connected simultaneously — Bruce activates one at a time.

#### CC1101 — RF 433/868/915 MHz

| Pin CC1101 | GPIO |
|------------|------|
| VCC        | 3.3V |
| GND        | GND  |
| SCK        | 1    |
| MOSI       | 2    |
| MISO       | 3    |
| CSN        | 5    |
| GDO0       | 6    |
| GDO2       | — (not connected) |

#### NRF24L01 — RF 2.4 GHz

| Pin NRF24 | GPIO |
|-----------|------|
| VCC       | 3.3V |
| GND       | GND  |
| SCK       | 1 (shared) |
| MOSI      | 2 (shared) |
| MISO      | 3 (shared) |
| CSN       | 7    |
| CE        | 8    |
| IRQ       | 15 (optional) |

> Assign pins in Bruce: **Settings → Config Pins → RF Module**

### Via single GPIO — one cable

| Module | Function | Suggested pin |
|--------|----------|---------------|
| **IR LED** (+ 100Ω resistor) | IR transmit — remote replay, TV-B-Gone | GPIO 16 |
| **IR receiver TSOP4838** | IR capture | GPIO 18 |

> Assign pins in Bruce: **Settings → Config Pins → IR TX / IR RX**

### Free GPIO pins available for modules

```
GPIO 1, 2, 3, 5, 6, 7, 8
GPIO 15, 16, 18, 19, 20
GPIO 38, 42, 43, 44, 45, 46
```

> **Avoid:** GPIO 0 (BOOT), 4 (BAT ADC), 9–14 (display),
> 17/21 (touch), 39–41 (SD), 47/48 (I2C)

### Recommended full setup (all modules simultaneously)

```
Grove port     → PN532   (RFID/NFC — I2C, plug & play)
GPIO 1,2,3,5,6 → CC1101  (SCK, MOSI, MISO, CSN, GDO0)
GPIO 1,2,3,7,8 → NRF24   (SCK, MOSI, MISO, CSN, CE)
GPIO 15        → NRF24   IRQ (optional)
GPIO 16        → IR LED  (IR transmit)
GPIO 18        → IR RX   (IR capture)
```

With this setup: **WiFi + BT + RFID + RF 433 MHz + RF 2.4 GHz + IR TX + IR RX + Touch + SD** — everything Bruce can do at once.

---

## Build & Flash

### Requirements

- [PlatformIO](https://platformio.org/) (CLI or VSCode extension)
- Python 3.x

### Build

```bash
git clone https://github.com/Tercyus/Bruce.git
cd Bruce
pio run -e waveshare-esp32-s3-touch-lcd-19
```

Output binary:
```
Bruce-waveshare-esp32-s3-touch-lcd-19.bin
```

### Flash via web (easiest)

1. Open [https://web.esptool.io](https://web.esptool.io) in Chrome/Edge
2. Click **Connect** → select your COM port
3. Click **Program** → select the `.bin` file → **Flash**

### Flash via esptool

```bash
esptool.py --chip esp32s3 --port COM_PORT write_flash 0x0 Bruce-waveshare-esp32-s3-touch-lcd-19.bin
```

### Flash via PlatformIO

```bash
pio run -e waveshare-esp32-s3-touch-lcd-19 --target upload
```

> If the board is not detected: hold **BOOT** while connecting USB, then release.

---

## Key technical notes

### LovyanGFX backend
Uses **LovyanGFX** instead of TFT_eSPI for native DMA over SPI,
resulting in significantly faster screen rendering.

### Active-LOW backlight
The backlight circuit is **active-LOW**: the PWM duty cycle is inverted
in `_setBrightness()` — duty 0 = full brightness, duty 255 = off.

### Color inversion
The ST7789V2 on this board works correctly in **INVOFF** state
(`TFT_INVERTION=0`). `colorInverted` is forced to `false` in
`_post_setup_gpio()` to override any stored config value.

### Simu
