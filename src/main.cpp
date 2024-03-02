#include <list>
#include <Arduino.h>
#include <AdaNeoDisplay.h>
#include <TetrisGame.h>
#include <TaskScheduler.h>
#include <TM1637Display.h>

// Module connection pins (Digital Pins)
#define CLK_LEFT_LED 27
#define DIO_LEFT_LED 26
#define CLK_RIGHT_LED 33
#define DIO_RIGHT_LED 32

TM1637Display l8_left(CLK_LEFT_LED, DIO_LEFT_LED);
TM1637Display l8_right(CLK_RIGHT_LED, DIO_RIGHT_LED);

struct Button
{
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool pressed;
};

Button button1 = {22, 0, false};

// variables to keep track of the timing of recent interrupts
unsigned long button_time = 0;
unsigned long last_button_time = 0;

void IRAM_ATTR isr()
{
  button_time = millis();
  if (button_time - last_button_time > 250)
  {
    button1.numberKeyPresses++;
    button1.pressed = true;
    last_button_time = button_time;
  }
}

// input pins
#define DISPLAY_PIN 14
#define L_JOYSTICK_PIN 22
#define R_JOYSTICK_PIN 23
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
#define KEY_UPDATE(pin, field)           \
  {                                      \
    bool pressed = digitalRead(pin);     \
    if (pressed != old_key_state.field)  \
    {                                    \
      current_key_state.field = pressed; \
    }                                    \
  }

#define PIXELS_X 8
#define PIXELS_Y 12

auto native_display = Adafruit_NeoPixel(PIXELS_X * PIXELS_Y, DISPLAY_PIN, NEO_GRB + NEO_KHZ800);
AdaNeoDisplay display = AdaNeoDisplay(native_display, PIXELS_X, PIXELS_Y);
TetrisGame tetris = TetrisGame(display, PIXELS_X, PIXELS_Y);

Keys old_key_state;
Keys current_key_state;

void tetrisThread();
void leftLEDThread();
Scheduler runner;
Task tetrisTask(1000, TASK_FOREVER, &tetrisThread);
Task lefLedTask(1000, TASK_FOREVER, &leftLEDThread);

void leftLEDThread()
{
  static int counter = 0;
  l8_left.showNumberDec(counter);
  counter++;
}

void tetrisThread()
{
  tetris.animate(current_key_state);
  display.show();
  old_key_state = current_key_state;
}

void initPins()
{
  for (auto pin : input_pins)
  {
    pinMode(pin, INPUT_PULLUP);
  }
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
    digitalWrite(BUTTON_LEFT_LED, 1);
    digitalWrite(BUTTON_RIGHT_LED, 1);
    /*
    l8_left.clear();
    l8_right.clear();
    l8_right.showNumberDec(1234);
    runner.addTask(lefLedTask);
    pinMode(button1.PIN, INPUT_PULLUP);
    attachInterrupt(button1.PIN, isr, FALLING);
 */
    Serial.begin(115200);
    Serial.write("Goo");
    runner.addTask(tetrisTask);
  }

  /**
   * updates keys and sets a new status if the input
   * is different from the known old state that was used
   * during the last animation sequence
   * thus: the anmiation task has to copy current_key_state
   *       to old_key_state after the animation
   */
  void readKeys()
  {
    KEY_UPDATE(L_JOYSTICK_PIN, left_joy);
    KEY_UPDATE(R_JOYSTICK_PIN, right_joy);
    KEY_UPDATE(U_JOYSTICK_PIN, up_joy);
    KEY_UPDATE(D_JOYSTICK_PIN, down_joy);
    KEY_UPDATE(L_PUSH_PIN, left_push);
    KEY_UPDATE(R_PUSH_PIN, right_push);
}

void loop()
{
  /* static int num_pressed = 0;
  if (button1.pressed)
  {
    num_pressed++;
    l8_right.showNumberDec(num_pressed);
    button1.pressed = false;
  }
  */
  // first: update the key status
//  readKeys();
  // the run all animations
  runner.execute();
}