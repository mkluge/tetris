#include <LEDDisplay.h>



inline int MAX(int a, int b) { return((a) > (b) ? a : b); }
inline int MIN(int a, int b) { return((a) < (b) ? a : b); }


class Laufschrift {
    LEDDisplay &display;
    const char *image[12] = {
    "  .....    .  .......    ......          -                                               ..      ",
    " .......  ..  ........  ........         --                                              ..      ",
    " ..       ..  ..    ..  ..               -                            .....          -   ..      ",
    " ..       ..  ..    ..  .......        .     .           ..           .   .          --  ..      ",
    " ..       ..  ..    ..   .......       ..   ...         .  .          .                          ",
    " ..       ..  ..    ..        ..       ..   ...        .    .         .                          ",
    " ..       ..  ..    ..        ..       ...  ...        .    .         .   -                      ",
    " .......  ..  ........   .......       .... ...       .      .        .                  ..      ",
    "  .....   .   .......   .......        .... ...       .      -       ..                  ..      "};
    // CIDS Tetris:39 Pong:54 Snake:69 Flappybird:82
    const char *image2[3] = {
    " -    | ",
    "-      |",
    " -    | "};
    const struct{char* name; int start; } games[4] = {
        {"Tetris", 40},
        {"Pong", 55},
        {"Snake", 69},
        {"Flappybird", 85},
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

    void run() {
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

        // key press logic
        auto events = keyboard.toggled();
        int ngames = sizeof(games) / sizeof(games[0]);
        for( const auto &key: events) {
            l8_left.showNumberDec(key.first);
            l8_right.showNumberDec(key.second);
            if (key.first == 18 && key.second) {
                if (targetgame == -1) {
                    targetgame = ngames - 1;
                } else if (targetgame > 0) {
                    targetgame--;
                }
                idle = 0;
            }
            if (key.first == 21 && key.second) {
                if (targetgame == -1) {
                    targetgame = 0;
                } else if (targetgame < ngames - 1) {
                    targetgame++;
                }
                idle = 0;
            }
        }
        //l8_right.showNumberDec(events.size());
    }
};
