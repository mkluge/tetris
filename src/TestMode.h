#ifndef TESTMODE_H
#define TESTMODE_H

#include <Arduino.h>
#include <Basics.h>
#include <LEDDisplay.h>
#include <TM1637Display.h>
#include <list>

#pragma once

struct TestInput {
    int pin;
    int x;
    RGB color;
};

static const TestInput test_inputs[] = {
    {L_JOYSTICK_PIN, 0, TBLUE},
    {R_JOYSTICK_PIN, 1, TGREEN},
    {U_JOYSTICK_PIN, 2, TYELLOW},
    {D_JOYSTICK_PIN, 3, TRED},
    {L_PUSH_PIN, 4, TPINK},
    {R_PUSH_PIN, 5, TORANGE},
};

bool isAnyTestInputPressed()
{
    for (const auto &input : test_inputs) {
        if (!digitalRead(input.pin)) {
            return true;
        }
    }
    return false;
}

void drawTestProgress(LEDDisplay &display, const bool tested[], int active_index)
{
    display.clear();
    for (int i = 0; i < 6; i++) {
        RGB color = TRED;
        if (tested[i]) {
            color = TGREEN;
        }
        if (i == active_index) {
            color = test_inputs[i].color;
        }
        for (int y = 0; y < display.height(); y++) {
            display.setPixel(test_inputs[i].x, y, color);
        }
    }
    for (int y = 0; y < display.height(); y++) {
        display.setPixel(6, y, {32, 32, 32});
        display.setPixel(7, y, {32, 32, 32});
    }
    display.show();
}

void runDisplaySelfTest(LEDDisplay &display)
{
    const RGB colors[] = {TRED, TGREEN, TBLUE, TYELLOW, TPINK, {255, 255, 255}};
    for (const auto &color : colors) {
        for (int x = 0; x < display.width(); x++) {
            for (int y = 0; y < display.height(); y++) {
                display.setPixel(x, y, color);
            }
        }
        display.show();
        delay(350);
    }
    display.clear();
}

void flashTestInput(LEDDisplay &display, int index)
{
    for (int i = 0; i < 3; i++) {
        for (int y = 0; y < display.height(); y++) {
            display.setPixel(test_inputs[index].x, y, {255, 255, 255});
        }
        display.show();
        delay(90);
        for (int y = 0; y < display.height(); y++) {
            display.setPixel(test_inputs[index].x, y, test_inputs[index].color);
        }
        display.show();
        delay(90);
    }
}

void runHardwareTestMode(LEDDisplay &display, TM1637Display &left_digits, TM1637Display &right_digits)
{
    bool tested[6] = {false, false, false, false, false, false};
    int tested_count = 0;

    runDisplaySelfTest(display);
    left_digits.showNumberDec(8888);
    right_digits.showNumberDec(8888);
    delay(500);
    left_digits.clear();
    right_digits.showNumberDec(6);
    drawTestProgress(display, tested, -1);

    while (tested_count < 6) {
        for (int i = 0; i < 6; i++) {
            if (!tested[i] && !digitalRead(test_inputs[i].pin)) {
                tested[i] = true;
                tested_count++;
                left_digits.showNumberDec(test_inputs[i].pin);
                right_digits.showNumberDec(6 - tested_count);
                flashTestInput(display, i);
                drawTestProgress(display, tested, i);
                while (!digitalRead(test_inputs[i].pin)) {
                    delay(10);
                }
                delay(60);
            }
        }
        delay(10);
    }

    left_digits.showNumberDec(0);
    right_digits.showNumberDec(0);
    for (int flash = 0; flash < 6; flash++) {
        for (int x = 0; x < display.width(); x++) {
            for (int y = 0; y < display.height(); y++) {
                RGB color = {0, 0, 0};
                if (flash % 2) {
                    color = TGREEN;
                }
                display.setPixel(x, y, color);
            }
        }
        display.show();
        delay(180);
    }
    display.clear();
}

#endif
