#include <AdaNeoDisplay.h>
#include <TetrisGame.h>
#include <TaskScheduler.h>
#include <list>

// input pins
#define DISPLAY_PIN RX
#define L_JOYSTICK_PIN D0
#define R_JOYSTICK_PIN D1
#define U_JOYSTICK_PIN D2
#define D_JOYSTICK_PIN D5
#define L_PUSH_PIN D6
#define R_PUSH_PIN D7
#define S8D_LEFT_PIN D3
#define S8D_RIGHT_PIN D4
std::list<uint8_t> input_pins = {
  L_JOYSTICK_PIN,
  R_JOYSTICK_PIN,
  U_JOYSTICK_PIN,
  D_JOYSTICK_PIN,
  L_PUSH_PIN,
  R_PUSH_PIN
};
std::list<uint8_t> output_pins = {
    DISPLAY_PIN,
    S8D_LEFT_PIN,
    S8D_RIGHT_PIN
};
#define KEY_UPDATE(pin,field) {          \
  bool pressed = digitalRead(pin);       \
  if (pressed != old_key_state.field)    \
  {                                      \
    current_key_state.field = pressed;   \
  }                                      \
}

#define PIXELS_X 8
#define PIXELS_Y 12

auto native_display = Adafruit_NeoPixel(PIXELS_X*PIXELS_Y, DISPLAY_PIN, NEO_GRB + NEO_KHZ800);
AdaNeoDisplay display = AdaNeoDisplay( native_display, PIXELS_X, PIXELS_Y);
TetrisGame tetris = TetrisGame(display, PIXELS_X, PIXELS_Y);

Keys old_key_state;
Keys current_key_state;

void tetrisThread();
Scheduler runner;
Task tetrisTask(1000, TASK_FOREVER, &tetrisThread);

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
    pinMode(pin, INPUT);
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
  // first: update the key status
  readKeys();
  // the run all animations
  runner.execute();
}