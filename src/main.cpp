#include <list>
#include <Arduino.h>
#include <AdaNeoDisplay.h>
#include <TetrisGame.h>
#include <TaskScheduler.h>
#include <TM1637Display.h>
#include <Keyboard.h>
#include <SteinScherePapier.h>
#include <Mandelbrot.h>
#include <Plasma.h>
#include <SnakeGame.h>

// Module connection pins (Digital Pins)
#define CLK_LEFT_LED 27
#define DIO_LEFT_LED 26
#define CLK_RIGHT_LED 25
#define DIO_RIGHT_LED 33

// input pins
#define DISPLAY_PIN 32
#define D_JOYSTICK_PIN 3
#define R_JOYSTICK_PIN 21
#define U_JOYSTICK_PIN 19
#define L_JOYSTICK_PIN 18
#define R_PUSH_PIN 22
#define L_PUSH_PIN 23
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
SteinScherePapier<PIXELS_X, PIXELS_Y> ssp(display);
Mandelbrot<PIXELS_X, PIXELS_Y> mandelbrot(display);
Plasma<PIXELS_X, PIXELS_Y> plasma(display);
SnakeGame<PIXELS_X, PIXELS_Y> snake(display);

#define ANIMATION snake

void sspThread();
void animationThread();
void leftLEDThread();
Scheduler runner;
Task animationTask(10, TASK_FOREVER, &animationThread);
Task leftLedTask(10, TASK_FOREVER, &leftLEDThread);

int counter = 0;

void animationThread()
{
  ANIMATION.paint();
}

void leftLEDThread()
{
  l8_left.showNumberDec(counter);
  counter++;
}

void blinkThread()
{
  static int lr = false;
  digitalWrite(BUTTON_LEFT_LED, lr ? 0 : 1);
  digitalWrite(BUTTON_RIGHT_LED, lr ? 1 : 0);
  lr = !lr;
}


void setup()
{
  display.start();
  randomSeed(analogRead(0)); // Seed the random number generator with an analog reading
  l8_left.clear();
  l8_left.setBrightness(0x0f);
  l8_left.showNumberDec(1234);
  l8_right.clear();
  l8_right.setBrightness(0x0f);
  runner.addTask(animationTask);
  runner.addTask(leftLedTask);
  leftLedTask.enable();
  animationTask.enable();
  for( const auto &key: input_pins)
  {
    keyboard.addKey( key, key);
  }
  display.clear();
  pinMode(BUTTON_LEFT_LED, OUTPUT);
  pinMode(BUTTON_RIGHT_LED, OUTPUT);
  digitalWrite(BUTTON_LEFT_LED, 0);
  digitalWrite(BUTTON_RIGHT_LED, 0);
  ANIMATION.init();
}

void loop()
{
  const auto &pressed = keyboard.toggled();
  if (pressed.size())
  {
    ANIMATION.init();
    counter=0;
  }
  // run all animations
  runner.execute();
}
