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
| **Touch** | CST816 capacitive touch — I2C |
| **IMU** | QMI8658 (shares I2C bus) |
| **SD Card** | SDMMC 1-bit mode |
| **USB** | USB-C — CDC on boot (serial monitor) |

---

## Features supported

- [x] Display (LovyanGFX DMA backend — ST7789V2 @ 80 MHz SPI)
- [x] Capacitive touch screen (CST816)
- [x] BOOT button (GPIO 0) as ESC / wake-up
- [x] SD Card via SDMMC
- [x] Battery level (ADC GPIO 4)
- [x] Deep sleep / wake on button
- [x] CC1101 and NRF24 via SPI (pins configurable in Bruce menu)
- [x] IR TX / RX (pins configurable in Bruce menu)
- [x] PN532 RFID via I2C
- [x] USB HID (BadUSB)

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

| Signal   | GPIO |
|----------|------|
| CLK      | 41   |
| CMD      | 39   |
| DATA0    | 40   |

### Other

| Signal       | GPIO |
|--------------|------|
| BOOT button  | 0    |
| Battery ADC  | 4    |
| I2C SDA      | 47   |
| I2C SCL      | 48   |

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

The merged binary will be generated at:
```
Bruce-waveshare-esp32-s3-touch-lcd-19.bin
```

### Flash via esptool

```bash
esptool.py --chip esp32s3 --port COM_PORT write_flash 0x0 Bruce-waveshare-esp32-s3-touch-lcd-19.bin
```

Replace `COM_PORT` with your port (`COM3`, `/dev/ttyUSB0`, etc.).

### Flash via PlatformIO

Connect the board via USB-C, then:

```bash
pio run -e waveshare-esp32-s3-touch-lcd-19 --target upload
```

> **Note:** If the board is not detected, hold **BOOT** while connecting USB, then release.

---

## Key technical notes

### LovyanGFX backend
This port uses **LovyanGFX** instead of TFT_eSPI for the display driver.
LovyanGFX provides native DMA support over SPI, resulting in significantly
faster screen rendering compared to TFT_eSPI.

### Active-LOW backlight
The backlight circuit on this board is **active-LOW**: a low signal = backlight ON.
The PWM duty cycle is inverted in `_setBrightness()` accordingly
(duty 0 = full brightness, duty 255 = off).

### Color inversion
The ST7789V2 panel on this board works correctly in **INVOFF** state
(`TFT_INVERTION=0`). The `colorInverted` setting is forced to `false`
in `_post_setup_gpio()` to override any value stored in the config file.

---

## Precompiled binary

A ready-to-flash `.bin` is available in the
[Releases](https://github.com/Tercyus/Bruce/releases) section.

---

## Credits

| | |
|---|---|
| Original firmware | [BruceDevices/Bruce](https://github.com/BruceDevices/Bruce) |
| Waveshare port | [@Tercyus](https://github.com/Tercyus) |
| LovyanGFX | [lovyan03/LovyanGFX](https://github.com/lovyan03/LovyanGFX) |
| TouchLib | [mmMicky/TouchLib](https://github.com/mmMicky/TouchLib) |

---

## License

This project inherits the license from the original Bruce firmware.
See [LICENSE](../../LICENSE) in the root of the repository.
