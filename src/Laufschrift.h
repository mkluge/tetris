#include <LEDDisplay.h>



inline int MAX(int a, int b) { return((a) > (b) ? a : b); }
inline int MIN(int a, int b) { return((a) < (b) ? a : b); }


class Laufschrift {
    LEDDisplay &display;
    const char *image[12] = {
    "  .....    .  .......    ......                                                            ",
    " .......  ..  ........  ........                                                           ",
    " ..       ..  ..    ..  ..                                                                 ",
    " ..       ..  ..    ..  .......                                                            ",
    " ..       ..  ..    ..   .......                                                           ",
    " ..       ..  ..    ..        ..                                                           ",
    " ..       ..  ..    ..        ..                                                           ",
    " .......  ..  ........   .......                                                           ",
    "  .....   .   .......   .......                                                            "};
    const char *image2[3] = {
    " -    | ",
    "-      |",
    " -    | "};
    int pos = 0;
    int pos2 = 0;
    int sz = strlen(image[0]);
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
                display.setPixel(j, 11-i, getColor(image[i][(pos/4 + j) % sz]));
            }
        }
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 8; j++) {
                auto color = getColor(image2[i][j]);
                display.setPixel(j, 2-i, color);
            }
        }
        display.show();
        l8_left.showNumberDec(pos);
        pos = (pos + 1) % (4*sz);
        pos2++;
    }
};
