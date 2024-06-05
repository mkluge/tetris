#include <list>
#include <Arduino.h>
#include <Basics.h>
#include <AdaNeoDisplay.h>
#include <TetrisGame.h>
#include <TaskScheduler.h>
#include <TM1637Display.h>
#include <Keyboard.h>

// Module connection pins (Digital Pins)
#define CLK_LEFT_LED 27
#define DIO_LEFT_LED 26
#define CLK_RIGHT_LED 25
#define DIO_RIGHT_LED 33

std::list<uint8_t> input_pins = {
    L_JOYSTICK_PIN,
    R_JOYSTICK_PIN,
    U_JOYSTICK_PIN,
    D_JOYSTICK_PIN,
    L_PUSH_PIN,
    R_PUSH_PIN,
};
std::list<uint8_t> output_pins = {
    DISPLAY_PIN,
    BUTTON_LEFT_LED,
    BUTTON_RIGHT_LED,
};

#define PIXELS_X 8
#define PIXELS_Y 12

auto native_display = Adafruit_NeoPixel(PIXELS_X * PIXELS_Y, DISPLAY_PIN, NEO_GRB + NEO_KHZ800);
AdaNeoDisplay display = AdaNeoDisplay(native_display, PIXELS_X, PIXELS_Y);
TM1637Display l8_left(CLK_LEFT_LED, DIO_LEFT_LED);
TM1637Display l8_right(CLK_RIGHT_LED, DIO_RIGHT_LED);
TetrisGame tetris = TetrisGame(display, PIXELS_X, PIXELS_Y, l8_right);
Keyboard keyboard = Keyboard();

int counter = 0;


void setup()
{
  display.start();
  randomSeed(analogRead(0)); // Seed the random number generator with an analog reading
  l8_left.clear();
  l8_left.setBrightness(0x0f);
  l8_left.showNumberDec(1234);
  l8_right.clear();
  l8_right.setBrightness(0x0f);
  for( const auto &key: input_pins)
  {
    keyboard.addKey( key, key);
  }
  display.clear();
  pinMode(BUTTON_LEFT_LED, OUTPUT);
  pinMode(BUTTON_RIGHT_LED, OUTPUT);
  digitalWrite(BUTTON_LEFT_LED, 0);
  digitalWrite(BUTTON_RIGHT_LED, 0);
  tetris.start();
  Serial.begin(115000);
}

void loop()
{
  // first handle keys, if any
  auto keys = keyboard.toggled();
  if( keys.size() )
  {
    tetris.onKey(keys);
  }
  // then call animation
  tetris.animate();
}