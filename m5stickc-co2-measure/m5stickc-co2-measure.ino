#include <M5StickC.h>
#include <MHZ19_uart.h>

MHZ19_uart mhz19;
int before_led_status = HIGH;

void setup() {
  M5.begin();
  M5.Lcd.println("init...");
  
  M5.Lcd.setRotation(1);
  M5.Axp.ScreenBreath(10);
  M5.Lcd.fillScreen(BLACK);

  // for debug
  Serial.begin(115200);

  // setup led
  pinMode(GPIO_NUM_10, OUTPUT);
  digitalWrite(GPIO_NUM_10, HIGH);

  // co2 monitor init
  //mhz19.begin(0, 26);
  mhz19.begin(33, 32);
  mhz19.setAutoCalibration(false);

  delay(1000);
  M5.Lcd.fillScreen(BLACK);
}

void loop() {
  int current_co2_status = print_co2();
  print_battery_percentage();

  if (current_co2_status == RED) {
    blink_led();
  } else {
    clear_led();
  }

  delay(5000);
}

int print_co2() {
  char* label = "ppm";
  int co2 = mhz19.getPPM();
  int co2_status;

  Serial.printf("%dppm\n", co2);

  if (co2 == -1) {
    co2 = 0;
    label = "err";
    co2_status = RED;
  }
  else if (co2 < 800) {
    co2_status = BLUE;
  } else if (800 <= co2 && co2 <= 999) {
    co2_status = YELLOW;
  } else {
    co2_status = RED;
  }

  M5.Lcd.setTextColor(co2_status, BLACK);

  // co2 digit
  M5.Lcd.setCursor(20, 7);
  M5.Lcd.setTextFont(7);
  M5.Lcd.setTextSize(1);
  M5.Lcd.printf("%04d", co2);

  // label
  M5.Lcd.setCursor(110, 57);
  M5.Lcd.setTextFont(1);
  M5.Lcd.setTextSize(2);
  M5.Lcd.printf("%s\n", label);

  return co2_status;
}

void print_battery_percentage() {
  float vbat = M5.Axp.GetBatVoltage();
  int vlevel = ( vbat - 3.2 ) / 0.8 * 100;
  int battery_status = 0;
  
  if ( vlevel < 0 ) {
    vlevel = 0;
  }
  if ( 100 < vlevel ) {
    vlevel = 100;
  }

  if (vlevel > 30) {
    battery_status = BLUE;
  } else {
    battery_status = RED;
  }

  M5.Lcd.setTextColor(battery_status, BLACK);
  
  M5.Lcd.setCursor(10, 57);
  M5.Lcd.setTextFont(1);
  M5.Lcd.setTextSize(2);
  M5.Lcd.printf("%3d%%\n", vlevel);
}

void blink_led() {
  int s;
  
  if (before_led_status == HIGH) {
    s = LOW;
  } else {
    s = HIGH;
  }

  digitalWrite(GPIO_NUM_10, s);
  before_led_status = s;
}

void clear_led() {
  digitalWrite(GPIO_NUM_10, HIGH);
  before_led_status = HIGH;
}
