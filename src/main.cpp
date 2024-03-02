#include <list>
#include <Arduino.h>
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


// input pins
#define DISPLAY_PIN 32
#define L_JOYSTICK_PIN 23
#define R_JOYSTICK_PIN 22
#define U_JOYSTICK_PIN 15
#define D_JOYSTICK_PIN 2
#define L_PUSH_PIN 4
#define R_PUSH_PIN 16
#define BUTTON_LEFT_LED 1
#define BUTTON_RIGHT_LED 17

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
TetrisGame tetris = TetrisGame(display, PIXELS_X, PIXELS_Y);
TM1637Display l8_left(CLK_LEFT_LED, DIO_LEFT_LED);
TM1637Display l8_right(CLK_RIGHT_LED, DIO_RIGHT_LED);
Keyboard keyboard = Keyboard();

void tetrisThread();
void leftLEDThread();
Scheduler runner;
//Task tetrisTask(1000, TASK_FOREVER, &tetrisThread);
Task leftLedTask(1000, TASK_FOREVER, &leftLEDThread);

void leftLEDThread()
{
  static int counter = 0;
  l8_left.showNumberDec(counter);
  counter++;
}

void tetrisThread()
{
  //
  //tetris.animate(current_key_state);
  //display.show();
  //old_key_state = current_key_state;
}

void initPins()
{
  for (auto pin : output_pins)
  {
    pinMode(pin, OUTPUT);
  }
}

void setup()
{
  display.start();
  randomSeed(analogRead(0)); // Seed the random number generator with an analog reading
  initPins();
  l8_left.clear();
  l8_left.setBrightness(0x0f);
  l8_left.showNumberDec(1234);
  l8_right.clear();
  l8_right.setBrightness(0x0f);
//  runner.addTask(tetrisTask);
  runner.addTask(leftLedTask);
  //  Serial.begin(115200);
  digitalWrite(BUTTON_LEFT_LED,1);
  digitalWrite(BUTTON_RIGHT_LED,1);
  for( const auto &key: input_pins)
  {
    keyboard.addKey( key, key);
  }
}

void loop()
{
  static int num_pressed = 0;
  const auto &pressed = keyboard.toggled();
  if (pressed.size())
  {
    num_pressed++;
    l8_right.showNumberDec(num_pressed);
  }
  for (auto x = 0; x < PIXELS_X; x++)
  {
    for (auto y = 0; y < PIXELS_Y; y++)
    {
      display.setPixel(x, y, {255, 255, 255});
//      display.show();
    }
  }
  display.setPixel(1, 1, {1, 255, 255});
  display.show();
  // the run all animations
  runner.execute();
}