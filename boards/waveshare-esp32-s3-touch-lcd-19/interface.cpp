#include "core/powerSave.h"
#include "core/utils.h"
#include <globals.h>
#include <interface.h>

// Touch IC: CST816 via I2C (pins defined in pins_arduino.h: SDA=47, SCL=48)
#define TOUCH_MODULES_CST_SELF
#include <TouchLib.h>
#include <Wire.h>
TouchLib touch(Wire, GROVE_SDA, GROVE_SCL, CTS820_SLAVE_ADDRESS, TOUCH_INT);

/***************************************************************************************
** Function name: _setup_gpio()
** Description:   Initial setup for the device
***************************************************************************************/
void _setup_gpio() {
    // SDMMC: set pins before sd_functions.cpp calls SD.begin()
#ifdef USE_SD_MMC
    SD.setPins(SDMMC_CLK, SDMMC_CMD, SDMMC_D0);
#endif

    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, LOW);

    // Touch CST816 init
    gpio_hold_dis((gpio_num_t)TOUCH_RST);
    pinMode(TOUCH_RST, OUTPUT);
    digitalWrite(TOUCH_RST, LOW);
    delay(50);
    digitalWrite(TOUCH_RST, HIGH);

    Wire.begin(GROVE_SDA, GROVE_SCL);
    if (!touch.init()) { Serial.println("Touch IC (CST816) not found"); }
    touch.setRotation(1);

    // BOOT button as ESC / wake-up
    pinMode(BTN_PIN, INPUT_PULLUP);

    // This panel requires color inversion ON for correct colors.
    // Set default here (before fromFile). fromFile() will override if user changed it.
    bruceConfig.colorInverted = true;

    // Default peripheral config (user can change via Bruce menu)
    bruceConfigPins.rfModule  = CC1101_SPI_MODULE;
    bruceConfigPins.rfidModule = PN532_I2C_MODULE;
    bruceConfigPins.irRx = RXLED;
    bruceConfigPins.irTx = TXLED;

    Serial.begin(115200);
}

/***************************************************************************************
** Function name: _post_setup_gpio()
** Description:   Second-stage GPIO setup (runs after TFT and SD init)
***************************************************************************************/
void _post_setup_gpio() {
    // Apply the inversion setting loaded from config (or the default set in _setup_gpio).
    // Do NOT force a fixed value here — that would override the user's saved preference.
    tft.invertDisplay(bruceConfig.colorInverted);

    // Backlight on this board is active-LOW: duty=0 (always LOW) = fully ON
    pinMode(TFT_BL, OUTPUT);
    ledcAttach(TFT_BL, TFT_BRIGHT_FREQ, TFT_BRIGHT_Bits);
    ledcWrite(TFT_BL, 0);
    previousMillis = millis();
}

/*********************************************************************
** Function: _setBrightness
** Set TFT backlight brightness (0-100)
**********************************************************************/
void _setBrightness(uint8_t brightval) {
    // Active-LOW backlight: duty=0 = fully ON, duty=255 = fully OFF
    if (brightval == 0) {
        ledcWrite(TFT_BL, 255); // fully OFF
    } else {
        int bl = MINBRIGHT + round(((255 - MINBRIGHT) * brightval / 100));
        ledcWrite(TFT_BL, 255 - bl); // invert: higher brightness → lower duty
    }
}

/*********************************************************************
** Function: InputHandler
** Handles NextPress, PrevPress, SelPress, EscPress, AnyKeyPress
** - Touch screen provides navigation (x/y → touchPoint)
** - BOOT button (GPIO 0) acts as ESC / back
**********************************************************************/
void InputHandler(void) {
    static long tm = 0;
    if (millis() - tm > 200 || LongPress) {
        // --- Touch ---
        if (touch.read()) {
            auto t = touch.getPoint(0);
            tm = millis();

            // Adjust coordinates for current rotation
            if (bruceConfigPins.rotation == 1) { t.y = (tftHeight + 20) - t.y; }
            if (bruceConfigPins.rotation == 3) { t.x = tftWidth - t.x; }
            if (bruceConfigPins.rotation == 0) {
                int tmp = t.x;
                t.x     = tftWidth - t.y;
                t.y     = tmp;
            }
            if (bruceConfigPins.rotation == 2) {
                int tmp = t.x;
                t.x     = t.y;
                t.y     = (tftHeight + 20) - tmp;
            }

            if (!wakeUpScreen()) AnyKeyPress = true;

            touchPoint.x       = t.x;
            touchPoint.y       = t.y;
            touchPoint.pressed = true;
            touchHeatMap(touchPoint);
        }

        // --- BOOT button (GPIO 0) → ESC / back ---
        if (digitalRead(BTN_PIN) == BTN_ACT) {
            tm = millis();
            if (!wakeUpScreen()) AnyKeyPress = true;
            else return;
            EscPress = true;
        }
    }
}

/*********************************************************************
** Function: powerOff
** Put device into deep sleep; BOOT button wakes it up
**********************************************************************/
void powerOff() {
    tft.fillScreen(bruceConfig.bgColor);
    ledcWrite(TFT_BL, 255); // active-LOW: 255 = fully OFF
    esp_sleep_enable_ext0_wakeup((gpio_num_t)BTN_PIN, BTN_ACT);
    esp_deep_sleep_start();
}

/*********************************************************************
** Function: checkReboot
** No hardware power-off button combo on this device
**********************************************************************/
void checkReboot() {}

/*********************************************************************
** Function: isCharging
** No charging IC accessible — always returns false
**********************************************************************/
bool isCharging() { return false; }
