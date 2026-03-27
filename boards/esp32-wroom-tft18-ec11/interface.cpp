#include "core/powerSave.h"
#include <globals.h>
#include <interface.h>

// EC11 rotary encoder
#include <RotaryEncoder.h>
extern RotaryEncoder *encoder;
RotaryEncoder *encoder = nullptr;
IRAM_ATTR void checkPosition() { encoder->tick(); }

/***************************************************************************************
** Function name: _setup_gpio()
** Description:   initial setup for the device
***************************************************************************************/
void _setup_gpio() {
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH); // backlight ON (active-HIGH)

    pinMode(ENCODER_KEY, INPUT_PULLUP);
    encoder = new RotaryEncoder(ENCODER_INA, ENCODER_INB, RotaryEncoder::LatchMode::TWO03);
    attachInterrupt(digitalPinToInterrupt(ENCODER_INA), checkPosition, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_INB), checkPosition, CHANGE);
}

/***************************************************************************************
** Function name: _post_setup_gpio()
** Description:   second stage gpio setup
***************************************************************************************/
void _post_setup_gpio() {
    previousMillis = millis();
}

/***************************************************************************************
** Function name: getBattery()
** Description:   Returns the battery value from 1-100
***************************************************************************************/
int getBattery() { return 0; } // No battery ADC on this board (ANALOG_BAT_PIN = -1)

/*********************************************************************
** Function: _setBrightness
** set brightness value
**********************************************************************/
void _setBrightness(uint8_t brightval) {
    if (brightval == 0) {
        analogWrite(TFT_BL, 0);
    } else if (brightval > 99) {
        analogWrite(TFT_BL, 254);
    } else {
        int bl = MINBRIGHT + round(((255 - MINBRIGHT) * brightval / 100));
        analogWrite(TFT_BL, bl);
    }
}

/*********************************************************************
** Function: InputHandler
** Handles PrevPress, NextPress, SelPress, AnyKeyPress and EscPress
**********************************************************************/
void InputHandler(void) {
    static unsigned long tm  = millis(); // button debounce
    static unsigned long tm2 = millis(); // gap between encoder and select
    static int posDifference = 0;
    static int lastPos = 0;

    int newPos = encoder->getPosition();
    if (newPos != lastPos) {
        posDifference += (newPos - lastPos);
        lastPos = newPos;
    }

    bool sel = !BTN_ACT;
    if (millis() - tm > 200 || LongPress) {
        sel = digitalRead(SEL_BTN);
    }

    if (posDifference != 0 || sel == BTN_ACT) {
        if (!wakeUpScreen()) AnyKeyPress = true;
        else return;
    }

    if (posDifference > 0) {
        PrevPress = true;
        posDifference--;
        tm2 = millis();
    }
    if (posDifference < 0) {
        NextPress = true;
        posDifference++;
        tm2 = millis();
    }
    if (sel == BTN_ACT && millis() - tm2 > 200) {
        posDifference = 0;
        SelPress = true;
        tm = millis();
    }
}

/*********************************************************************
** Function: powerOff
** Turns off the device via deep sleep; wakes on encoder button press
**********************************************************************/
void powerOff() {
    tft.fillScreen(bruceConfig.bgColor);
    _setBrightness(0);
    esp_sleep_enable_ext0_wakeup((gpio_num_t)BTN_PIN, BTN_ACT);
    esp_deep_sleep_start();
}

/*********************************************************************
** Function: checkReboot
** Btn logic to restart or sleep the device
**********************************************************************/
void checkReboot() {}
