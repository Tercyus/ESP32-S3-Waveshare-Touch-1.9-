# Bruce Firmware — ESP32 WROOM + TFT 1.8" + EC11

Port of [Bruce Firmware](https://github.com/BruceDevices/Bruce) for a custom build based on
**ESP32-WROOM-32U** with **1.8" ST7735S TFT** display and **EC11 rotary encoder**.

> **Port by:** [@Tercyus](https://github.com/Tercyus)
> **Original firmware:** [@BruceDevices](https://github.com/BruceDevices)

---

## Hardware

| | |
|---|---|
| **Board** | ESP32-WROOM-32U (38-pin DevKit) |
| **MCU** | ESP32 dual-core 240 MHz, 520 KB RAM |
| **Flash** | 4 MB |
| **Display** | 1.8" ST7735S — 128×160 px, SPI (VSPI) |
| **Input** | EC11 rotary encoder (CLK, DT, SW) |
| **SD Card** | SPI (optional, shares VSPI bus) |
| **USB** | Micro-USB / USB-C — UART serial |

---

## Features supported

- [x] Display (TFT_eSPI — ST7735S @ 27 MHz SPI)
- [x] EC11 rotary encoder navigation (prev / next / select)
- [x] Deep sleep / wake on encoder button
- [x] SD Card via SPI (GPIO 15 CS)
- [x] CC1101 433/868/915 MHz via SPI (configurable pins)
- [x] NRF24L01 2.4 GHz via SPI (configurable pins)
- [x] PN532 RFID/NFC via I2C (Grove SDA/SCL)
- [x] IR TX / IR RX (configurable pins — any free GPIO)
- [x] BadUSB via CH9329 (UART GPIO 1/3)
- [x] WiFi attacks (deauth, evil twin, captive portal)
- [x] Bluetooth tools

---

## Wiring

### Display ST7735S — VSPI

| Signal | GPIO |
|--------|------|
| SCL (SCLK) | 18 |
| SDA (MOSI) | 23 |
| MISO | 19 |
| CS | 17 |
| DC | 16 |
| RST | 4 |
| BL (backlight) | 26 |

> Backlight is **active-HIGH** (HIGH = ON). BL connected through a current-limiting resistor to 3.3 V.

### EC11 Rotary Encoder

| Signal | GPIO | Function |
|--------|------|----------|
| CLK (A) | 32 | Previous item |
| DT (B) | 33 | Next item |
| SW (button) | 25 | Select / ESC (long press) |

> SW uses internal pull-up. Connect the other SW leg to GND.

### SD Card — VSPI (optional)

| Signal | GPIO |
|--------|------|
| SCK | 18 (shared) |
| MOSI | 23 (shared) |
| MISO | 19 (shared) |
| CS | 15 |

### I2C (Grove)

| Signal | GPIO |
|--------|------|
| SDA | 21 |
| SCL | 22 |

### Serial / BadUSB CH9329

| Signal | GPIO |
|--------|------|
| TX | 1 |
| RX | 3 |

---

## Navigation

| Action | Result |
|--------|--------|
| Rotate clockwise | Previous item |
| Rotate counter-clockwise | Next item |
| Short press SW | Select |
| Long press SW | Back / ESC |

---

## Compatible modules

### Via I2C (GPIO 21/22) — plug & play

| Module | Function |
|--------|----------|
| **PN532** | RFID/NFC — read/write Mifare, emulation |
| **SSD1306 / SH1106** | Secondary OLED display |

### Via SPI — free GPIO pins + Bruce Config Pins menu

| Module | Function | Suggested pins |
|--------|----------|----------------|
| **CC1101** | RF 433/868/915 MHz — replay, analysis | CS=5, GDO0=34 |
| **NRF24L01** | RF 2.4 GHz — MouseJack | CS=5, CE=34 |

> Assign pins in Bruce: **Settings → Config Pins → RF Module**

### Via single GPIO — one cable

| Module | Function | Suggested pin |
|--------|----------|---------------|
| **IR LED** (+ 100Ω resistor) | IR transmit — remote replay | GPIO 27 |
| **IR receiver TSOP4838** | IR capture | GPIO 14 |

> Assign pins in Bruce: **Settings → Config Pins → IR TX / IR RX**

### Free GPIO pins available for modules

```
GPIO 5, 13, 14, 27, 34, 35, 36, 39
```

> **Avoid:** GPIO 1/3 (UART), 4 (TFT RST), 15 (SD CS), 16 (TFT DC),
> 17 (TFT CS), 18/19/23 (SPI bus), 21/22 (I2C), 25/32/33 (encoder), 26 (backlight)

---

## Build & Flash

### Requirements

- [PlatformIO](https://platformio.org/) (CLI or VSCode extension)
- Python 3.x

### Build

```bash
git clone https://github.com/Tercyus/Bruce.git
cd Bruce
pio run -e esp32-wroom-tft18-ec11
```

Output binary:
```
Bruce-esp32-wroom-tft18-ec11.bin
```

### Flash via web (easiest)

1. Open [https://web.esptool.io](https://web.esptool.io) in Chrome/Edge
2. Click **Connect** → select your COM port
3. Click **Program** → select the `.bin` file → offset `0x0` → **Flash**

> If the board is not detected: hold **BOOT** (IO0) while pressing **EN/RST**, then release BOOT.

### Flash via esptool

```bash
esptool.py --chip esp32 --port COM_PORT write_flash 0x0 Bruce-esp32-wroom-tft18-ec11.bin
```

### Flash via PlatformIO

```bash
pio run -e esp32-wroom-tft18-ec11 --target upload
```

---

## Key technical notes

### TFT_eSPI backend

Uses **TFT_eSPI** with `ST7735_GREENTAB` variant for correct pixel alignment on the 1.8" 128×160 panel. SPI frequency 27 MHz.

### Backlight

The backlight pin (GPIO 26) is **active-HIGH**. `analogWrite` PWM is used for brightness control. Brightness range is set by `MINBRIGHT=10` (minimum) to 254 (maximum).

### Encoder direction

Clockwise rotation → Previous item, counter-clockwise → Next item. If navigation feels reversed on your specific encoder, swap `ENCODER_INA` and `ENCODER_INB` in `pins_arduino.h`.

### No battery ADC

`ANALOG_BAT_PIN = -1` — no battery level indicator. The status bar shows a static icon.

---

## Precompiled binary

Ready-to-flash `.bin` available in the
[Releases](https://github.com/Tercyus/Bruce/releases) section.

---

## Credits

| | |
|---|---|
| Original firmware | [BruceDevices/Bruce](https://github.com/BruceDevices/Bruce) |
| Port | [@Tercyus](https://github.com/Tercyus) |
| TFT_eSPI | [Bodmer/TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) |
| RotaryEncoder | [mathertel/RotaryEncoder](https://github.com/mathertel/RotaryEncoder) |

---

https://github.com/user-attachments/assets/d93b7bbc-9356-4e83-baf2-26f5330090c0



## License

Inherits the license from the original Bruce firmware.
See [LICENSE](../../LICENSE) in the root of the repository.
