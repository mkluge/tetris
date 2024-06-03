void spaceshooter() {
    // initialization
    int idle = 0;

    // main loop
    while (true) {
        int last_millis = millis();

        // game input
        auto events = keyboard.toggled();
        for( const auto &key: events) {
            if (/*key.first == 18 && */ key.second) {
                // any key press
                return; // get back to main
            }
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
        display.setPixel(random(8), random(12), color);
        display.show();

        // busy waiting loop until next frame
        while (millis() - last_millis < 30) {
            // busy spin loop until frame time is over
        }

    }
}