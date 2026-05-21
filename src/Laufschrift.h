#include <GameEvents.h>
#include <LEDDisplay.h>



inline int MAX(int a, int b) { return((a) > (b) ? a : b); }
inline int MIN(int a, int b) { return((a) < (b) ? a : b); }

typedef void(*func)();

void renderBlood() {
    int idle = 0;
    GameEventLoop events(10);
    while (true) {
        GameEvent event;
        if (!events.next(event) || event.type != GameEventType::Timer) {
            continue;
        }
        // game logic
        idle++;
        if (idle > 300) {
            return; // end the game for all
        }

        // blood render
        l8_left.showNumberDec(idle);
        RGB color;
        color.red = random(256);
        color.green = 0;
        color.blue = 0;
        display.setPixel(random(8), random(12), color);
        display.show();
    }
}

#include "SpaceShooter.h"
#include "Neo.h"
#include "SnakeGame.h"
#include "TetrisGame.h"


// reference implementation for an empty game
void dummygame() {
    // initialization
    int idle = 0;
    GameEventLoop events(30);

    // main loop
    while (true) {
        GameEvent event;
        if (!events.next(event)) {
            continue;
        }

        // game input
        if (event.type == GameEventType::Key) {
            for( const auto &key: event.keys) {
                if (/*key.first == 18 && */ key.second) {
                    // any key press
                    return; // get back to main
                }
            }
            continue;
        }

        if (event.type != GameEventType::Timer) {
            continue;
        }

        // game logic
        idle++;
        if (idle > 500) {
            return;
        }

        // game render
        l8_left.showNumberDec(idle);
        RGB color;
        color.red = random(256);
        color.green = random(256);
        color.blue = random(256);
        display.setPixel(random(8), random(12), color);
        display.show();
    }
}


class Laufschrift {
    LEDDisplay &display;
    const char *image[12] = {
    "  .....    .  .......    ......          -                                               yy       gggggggg                ",
    " .......  ..  ........  ........         --                                              yy       g gggg g      g .     g ",
    " ..       ..  ..    ..  ..               -                            ggggg          -   yy       gggggggg           y    ",
    " ..       ..  ..    ..  .......        .     g           rv           g   g          |-  yy       gggggggg     v  |       ",
    " ..       ..  ..    ..   .......       ..   ggg         w  w          g                           ggggg gg             g  ",
    " ..       ..  ..    ..        ..       .g   v..        .    g         g                           gggggggg        -       ",
    " ..       ..  ..    ..        ..       rg.  vrr        g    .         g   -                       gggggggg                ",
    " .......  ..  ........   .......       rg.. vrr       .      y        g                  yy       gg ggggg        w       ",
    "  .....   .   .......   .......        rg.y v..       v      -       gg                  yy       gggggggg       www      "};
    // CIDS Tetris:39 Pong:54 Snake:69 Flappybird:82
    const char *image2[3] = {
    " -    | ",
    "-      |",
    " -    | "};
    const struct{char* name; int start; func prog; } games[3] = {
        {"Tetris", 40, run_tetris},
        //{"Pong", 55, dummygame},
        {"Snake", 69, run_snake},
        //{"Flappybird", 85, dummygame},
        //{"Matrix", 99, run_neo},
        {"Spaceshooter", 111, spaceshooter},
    };
    int pos = 0;
    int pos2 = 0;
    int targetgame = -1;
    int idle = 0;
    int sz = strlen(image[0]);
    const int scrollduration = 4;
public:


    Laufschrift(LEDDisplay &display): display(display) {
        sz = strlen(image[0]);
    }

    RGB getColor(char c) {
        RGB color;
        color.red = 0;
        color.green = 0;
        color.blue = 0;
        if (c == '.') {
            color.blue = 255;
        } else if (c == 'g') {
            color.green = 255;
        } else if (c == 'r') {
            color.red = 255;
        } else if (c == 'v') {
            color.red = 255;
            color.blue = 255;
        } else if (c == 'w') {
            color.red = 255;
            color.green = 255;
            color.blue = 255;
        } else if (c == 'y') {
            color.red = 255;
            color.green = 255;
        } else if (c == '+') {
            color.red = 255;
        } else if (c == '-') {
            color.red = 255;
            int brightness = (27+pos2) % 60;
            if (brightness > 30) {
                brightness = 64-brightness;
            }
            color = blendColor(color, brightness);
        } else if (c == '|') {
            color.red = 255;
            int brightness = pos2 % 60;
            if (brightness > 30) {
                brightness = 64-brightness;
            }
            color = blendColor(color, brightness);
        }
        return color;
    }

    RGB blendColor(RGB input, int brightness) {
        RGB color = input;
        color.red = min(max(((int)(color.red) * brightness) / 256, 0), 255);
        color.green = min(max(((int)(color.green) * brightness) / 256, 0), 255);
        color.blue = min(max(((int)(color.blue) * brightness) / 256, 0), 255);
        return color;
    }

    void playStartTransition() {
        int idle = 0;
        GameEventLoop events(10);

        for (int row = 11; row >= 0; row--) {
            for (int brightness = 200; brightness >= 0; brightness-= 100) {
                while (true) {
                    GameEvent event;
                    if (!events.next(event) || event.type != GameEventType::Timer) {
                        continue;
                    }

                    // game logic
                    idle++;

                    // game render
                    l8_left.showNumberDec(idle);
                    RGB color;
                    color.red = 255;
                    color.green = 255;
                    color.blue = 255;
                    for (int col = 0; col < 8; col++) {
                        display.setPixel(col, row, blendColor(color, brightness));
                    }
                    display.show();
                    break;
                }
            }
        }
    }

    void run() {
        // init all variables
        GameEventLoop events(30);
        events.reset();
        targetgame = -1;
        pos = 0;
        pos2 = 0;
        idle = 0;

        // enter main loop
        while (true) {
            GameEvent event;
            if (!events.next(event)) {
                continue;
            }

            if (event.type == GameEventType::Timer) {
                for (int i = 0; i < 9; i++) {
                    for (int j = 0; j < 8; j++) {
                        display.setPixel(j, 11-i, getColor(image[i][(pos/scrollduration + j) % sz]));
                    }
                }
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 8; j++) {
                        auto color = getColor(image2[i][j]);
                        display.setPixel(j, 2-i, color);
                    }
                }
                display.show();
                pos2++; // light animation
                //l8_left.showNumberDec(pos);

                // animation logic
                if (targetgame == -1) {
                    // scroll mode
                    pos = (pos + 1) % (scrollduration*sz);
                } else {
                    // select mode
                    if (pos < games[targetgame].start*scrollduration) {
                        pos += scrollduration;
                    }
                    if (pos > games[targetgame].start*scrollduration) {
                        pos -= scrollduration;
                    }
                }
                idle++;
                if (idle > 500 && targetgame != -1) {
                    idle = 0;
                    targetgame = -1;
                }
                continue;
            }

            if (event.type != GameEventType::Key) {
                continue;
            }

            // key press logic
            int ngames = sizeof(games) / sizeof(games[0]);
            for( const auto &key: event.keys) {
                l8_left.showNumberDec(key.first);
                l8_right.showNumberDec(key.second);
                if (key.first == L_JOYSTICK_PIN && key.second) {
                    if (targetgame == -1) {
                        targetgame = ngames - 1;
                    } else if (targetgame > 0) {
                        targetgame--;
                    }
                    idle = 0;
                }
                if (key.first == R_JOYSTICK_PIN && key.second) {
                    if (targetgame == -1) {
                        targetgame = 0;
                    } else if (targetgame < ngames - 1) {
                        targetgame++;
                    }
                    idle = 0;
                }
                if (targetgame != -1 && (key.first == R_PUSH_PIN || key.first == L_PUSH_PIN) && key.second) {
                    // blend-over animation
                    playStartTransition();
                    display.clear();
                    display.show();
                    keyboard.toggled(); // flush all keyboard events
                    // start game
                    games[targetgame].prog();
                    return; // restart main
                }
            }
            //l8_right.showNumberDec(events.size());
        }
    }
};
