#include <vector>
#include <utility>

class enemy_t {
    public:
    int x;
    int y;
    int type;
    int state;
    enemy_t(int x, int y): x(x), y(y), state(0) {}
    void step() {
        switch (state) {
            case 0:
                x++;
                state++;
                break;
            case 1:
                y--;
                state++;
                break;
            case 2:
                x--;
                state++;
                break;
            case 3:
                y--;
                state = 0;
                break;

        }
    }
};

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

    int level = 1;
    int score = 0;
    std::vector<enemy_t> enemies;
    enemies.push_back(enemy_t(2, 10));

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
            bool shotEnd = shot->second > 11;
            // check collision against enemy
            for (auto it = enemies.begin(); it != enemies.end(); it) {
                if (it->x == shot->first && it->y == shot->second) {
                    it = enemies.erase(it);
                    shotEnd++;
                    score+=50;
                } else {
                    it++;
                }
            }
            if (shotEnd) {
                // shot has left the screen
                shot = shots.erase(shot);
            } else {
                shot++;
            }
        }
        if (random(5) == 0) {
            for (auto it = enemies.begin(); it != enemies.end(); it) {
                it->step();
                if (it->y < 0) {
                    // lose animation if it->x == shipx+-1
                    if (it->x >= shipx - 1 && it->x <= shipx+1) {
                        // you lose
                        renderBlood();
                        return;
                    }
                    it = enemies.erase(it);
                } else {
                    it++;
                }
            }
        }

        if (enemies.size() == 0) {
            // level up sequence
            level++;
            for (int i = 0; i < level; i++) {
                enemies.push_back(enemy_t(random(8), random(5)+7));
            }
        }

        // game render
        l8_left.showNumberDec(level);
        l8_right.showNumberDec(score);
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
        // render enemies
        for (auto &enemy: enemies) {
            color.red = 0;
            color.green = 200;
            color.blue = 0;
            display.setPixel(enemy.x, enemy.y, color);
        }
        display.show();

        // busy waiting loop until next frame
        while (millis() - last_millis < 30) {
            // busy spin loop until frame time is over
        }

    }
}