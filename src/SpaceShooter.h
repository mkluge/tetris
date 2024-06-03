#include <vector>
#include <utility>

void spaceshooter() {
    // initialization
    int idle = 0;
    const int starn = 17;
    int starx[starn]; // x
    int stary[starn]; // y
    int starb[starn]; // brightness
    int stars[starn]; // parallax speed
    for (int i = 0; i < starn; i++) {
        starx[i] = random(8);
        starb[i] = random(30);
        stars[i] = random(90) + 30;
        stary[i] = random(12 * stars[i]);
    }

    std::vector<std::pair<int, int>> shots;
    int reload = 0;
    int shipx = 4;


    // main loop
    while (true) {
        int last_millis = millis();

        // game input
        auto events = keyboard.toggled();
        for( const auto &key: events) {
            idle = 0;
            if ((key.first == 22 || key.first == 23) && key.second) {
                // shoot
                if (reload == 0) {
                    reload = 3; // idle to reload
                    shots.push_back(std::pair<int, int>(shipx, 1));
                }
            }
            if (key.first == 18 && key.second && shipx > 1) {
                // move left
                shipx--;
            }
            if (key.first == 21 && key.second && shipx < 6) {
                // move right
                shipx++;
            }
        }

        // game logic
        idle++;
        if (idle > 800) {
            return; // return on idle
        }
        if (reload) {
            reload--;
        }
        for (int i = 0; i < starn; i++) {
            stary[i]--;
            if (stary[i] < 0) {
                stary[i] = 12*stars[i];
            }
        }
        for (auto shot = shots.begin(); shot != shots.end(); shot) {
            (*shot) = std::pair<int, int>(shot->first, shot->second+1);
            // TODO: check collision against enemy
            if (shot->second > 11) {
                // shot has left the screen
                shot = shots.erase(shot);
            } else {
                shot++;
            }
        }

        // game render
        l8_left.showNumberDec(idle);
        display.clear();
        // render stars
        RGB color;
        for (int i = 0; i < starn; i++) {
            color.red = starb[i];
            color.green = starb[i];
            color.blue = starb[i];
            display.setPixel(starx[i], MAX(0, MIN(11, stary[i] / stars[i])), color);
        }
        color.red = 255;
        color.green = 255;
        color.blue = 255;
        // render ship
        display.setPixel(shipx-1, 0, color);
        display.setPixel(shipx, 0, color);
        display.setPixel(shipx+1, 0, color);
        display.setPixel(shipx, 1, color);
        // render shots
        for (auto &shot: shots) {
            color.red = 255;
            color.green = 0;
            color.blue = 0;
            display.setPixel(shot.first, shot.second, color);
        }
        display.show();

        // busy waiting loop until next frame
        while (millis() - last_millis < 30) {
            // busy spin loop until frame time is over
        }

    }
}