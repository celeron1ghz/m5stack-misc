#include <M5Atom.h>
#include <MHZ19_uart.h>

#define GREEN               0xFF0000      /*   0, 255,   0 */
#define YELLOW              0xFFFF00      /* 255, 255,   0 */
#define RED                 0x00FF00      /* 128,   0, 128 */

MHZ19_uart mhz19;

void setup() {
    M5.begin(true, false, true);
    Serial.begin(115200);
    
    mhz19.begin(32, 26);
    mhz19.setAutoCalibration(false);
}

void loop() {
    int co2 = mhz19.getPPM();
    Serial.println(co2);

    int cnt = co2 / 50;
    int color = 0;

    if (cnt > 24) {
      cnt = 24;
    }

    if (cnt >= 20) {
      // 50 * 20 = 1000ppm is red
      color = RED;
    } else if (cnt >= 15) {
      // 50 * 15 = 750ppm is yellow
      color = YELLOW;
    } else {
      // else green
      color = GREEN;
    }

    for (int i = 0; i <= 24; i++) {
        int c = i <= cnt ? color : 0;
        M5.dis.drawpix(i, c);
    }

    delay(5000);
    M5.update();
}
