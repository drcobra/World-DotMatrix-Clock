#include <SPI.h>
#include <Adafruit_GFX.h>  //http://adafruit.github.io/Adafruit-GFX-Library/html/class_adafruit___g_f_x.html
#include <Max72xxPanel.h>
//#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Timezone.h>
#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager

//#define DEBUG

const char startX = 0;
const char startY = 0;
const uint8_t zones = 3;

int pinCS = D6;  // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
int numberOfHorizontalDisplays = 12;
int numberOfVerticalDisplays = 1;
Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

unsigned int pixFont3[] PROGMEM{
  3, 124, 68, 124,  // 48 0
  3, 72, 124, 64,   // 49 1
  3, 72, 100, 88,   // 50 2
  3, 68, 84, 124,   // 51 3
  3, 60, 32, 112,   // 52 4
  3, 92, 84, 116,   // 53 5
  3, 124, 84, 116,  // 54 6
  3, 4, 4, 124,     // 55 7
  3, 124, 84, 124,  // 56 8
  3, 92, 84, 124,   // 57 9
  3, 120, 20, 120,  // 58 a
  3, 124, 20, 28,   // 59 p
  3, 124, 8, 124,   // 60 m
};

void drawCharSmall(int16_t x, int16_t y, unsigned char c) {
  unsigned int line;

  for (int i = 0; i < 3; i++) {
    line = pixFont3[(c - 48) * 4 + i + 1];
    for (int j = 0; j < 8; j++, line >>= 1) {
      if (line & 1)
        matrix.drawPixel(x + i, y + j, HIGH);
      else
        matrix.drawPixel(x + i, y + j, LOW);
    }
  }
}

WiFiUDP ntpUDP;
// You can specify the time server pool and the offset, (in seconds)
// additionaly you can specify the update interval (in milliseconds).
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 30 * 60 * 1000);

// European Time (Athens)
TimeChangeRule GRST = { "GRST", Last, Sun, Mar, 2, 180 };  // Summer Time
TimeChangeRule GRT = { "GRT ", Last, Sun, Oct, 3, 120 };   // Standard Time
Timezone GR(GRST, GRT);

// United Kingdom (London, Belfast)
TimeChangeRule BST = { "BST", Last, Sun, Mar, 1, 60 };  // British Summer Time
TimeChangeRule GMT = { "GMT", Last, Sun, Oct, 2, 0 };   // Standard Time
Timezone UK(BST, GMT);

// US Eastern Time Zone (New York, Detroit)
TimeChangeRule usEDT = { "EDT", Second, Sun, Mar, 2, -240 };  // Eastern Daylight Time = UTC - 4 hours
TimeChangeRule usEST = { "EST", First, Sun, Nov, 2, -300 };   // Eastern Standard Time = UTC - 5 hours
Timezone usET(usEDT, usEST);

uint8_t zoneHour[zones] = { 99, 99, 99 };
uint8_t zoneMinute[zones] = { 99, 99, 99 };
uint8_t zoneSeconds[zones] = { 99, 99, 99 };
uint8_t zoneHourAMPM[zones];
uint8_t zoneAMPM[zones];

void printTime(Timezone tz, time_t utc, uint8_t fnPanel) {
  TimeChangeRule *tcr;  // pointer to the time change rule, use to get the TZ abbrev

  time_t t = tz.toLocal(utc, &tcr);
  //hour(t), minute(t), second(t), dayShortStr(weekday(t)), day(t), year(t)

  // Print Hour
  if (zoneHour[fnPanel] != hour(t)) {  // checking to avoid re-drawing
    zoneHour[fnPanel] = hour(t);

#ifdef DEBUG
    Serial.println("Draw Hour");
#endif

    // Calculations for AM/PM
    zoneHourAMPM[fnPanel] = hour(t);
    if (zoneHourAMPM[fnPanel] >= 12) {
      zoneAMPM[fnPanel] = 2;  //PM
    } else {
      zoneAMPM[fnPanel] = 1;  // AM
    }
    if (zoneHourAMPM[fnPanel] > 12) {
      zoneHourAMPM[fnPanel] = zoneHourAMPM[fnPanel] - 12;
    }

    String dotHour = String(zoneHourAMPM[fnPanel]);
    if (zoneHourAMPM[fnPanel] >= 10) {
      matrix.drawChar(startX + 32 * fnPanel, startY, dotHour[0], HIGH, LOW, 1);
      matrix.drawChar(startX + 5 + 32 * fnPanel, startY, dotHour[1], HIGH, LOW, 1);
    } else {
      matrix.drawChar(startX + 32 * fnPanel, startY, ' ', HIGH, LOW, 1);
      matrix.drawChar(startX + 5 + 32 * fnPanel, startY, dotHour[0], HIGH, LOW, 1);
    }
  }

  // Print Minutes
  if (zoneMinute[fnPanel] != minute(t)) {  // checking to avoid re-drawing
    zoneMinute[fnPanel] = minute(t);

#ifdef DEBUG
    Serial.println("Draw Minutes");
#endif

    String dotMin = String(zoneMinute[fnPanel]);
    if (zoneMinute[fnPanel] >= 10) {
      matrix.drawChar(startX + 13 + 32 * fnPanel, startY, dotMin[0], HIGH, LOW, 1);
      matrix.drawChar(startX + 19 + 32 * fnPanel, startY, dotMin[1], HIGH, LOW, 1);
    } else {
      matrix.drawChar(startX + 13 + 32 * fnPanel, startY, '0', HIGH, LOW, 1);
      matrix.drawChar(startX + 19 + 32 * fnPanel, startY, dotMin[0], HIGH, LOW, 1);
    }
  }

  // Print Seconds
  zoneSeconds[fnPanel] = second(t);

#ifdef DEBUG
  Serial.println("Draw Seconds");
#endif

  String dotSec = String(zoneSeconds[fnPanel]);
  if (zoneSeconds[fnPanel] >= 10) {
    drawCharSmall(startX + 25 + 32 * fnPanel, startY + 1, dotSec[0]);
    drawCharSmall(startX + 29 + 32 * fnPanel, startY + 1, dotSec[1]);
  } else {
    drawCharSmall(startX + 25 + 32 * fnPanel, startY + 1, '0');
    drawCharSmall(startX + 29 + 32 * fnPanel, startY + 1, dotSec[0]);
  }

  if (zoneAMPM[fnPanel] == 1) {  // AM
    matrix.drawRect(startX + 25 + 32 * fnPanel, startY, 3, 2, HIGH);
    matrix.drawRect(startX + 25 + 32 * fnPanel + 4, startY, 3, 2, LOW);
  } else if (zoneAMPM[fnPanel] == 2) {  // PM
    matrix.drawRect(startX + 25 + 32 * fnPanel, startY, 3, 2, LOW);
    matrix.drawRect(startX + 25 + 32 * fnPanel + 4, startY, 3, 2, HIGH);
  }
}

void showIP() {
  String tape = WiFi.localIP().toString();
  int spacer = 1;
  int width = 5 + spacer;
  for (int i = 0; i < width * tape.length() + matrix.width() - 1 - spacer; i++) {

    matrix.fillScreen(LOW);

    int letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2;  // center the text vertically

    while (x + width - spacer >= 0 && letter >= 0) {
      if (letter < tape.length()) {
        matrix.drawChar(x, y, tape[letter], HIGH, LOW, 1);
      }
      letter--;
      x -= width;
    }
    matrix.write();  // Send bitmap to display
    delay(50);
  }
}

void setup() {
  matrix.setIntensity(2);  // Use a value between 0 and 15 for brightness
  for (int i = 0; i < numberOfHorizontalDisplays; i++) {
    matrix.setRotation(i, 1);
  }

#ifdef DEBUG
  Serial.begin(115200);
#endif

  WiFiManager wm;

  // reset settings - wipe stored credentials for testing
  // these are stored by the esp library
  // wm.resetSettings();

  bool res;
  res = wm.autoConnect("WorldClock");

  if (!res) {
#ifdef DEBUG
    Serial.println("Failed to connect");
#endif
  } else {
//if you get here you have connected to the WiFi
#ifdef DEBUG
    Serial.println("connected...yeey :)");
#endif
  }

  yield();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
#ifdef DEBUG
    Serial.print(".");
#endif
  }

  showIP();

  timeClient.begin();
  if (timeClient.update()) {
#ifdef DEBUG
    Serial.print("Adjust local clock");
#endif
    unsigned long epoch = timeClient.getEpochTime();
    setTime(epoch);
  } else {
#ifdef DEBUG
    Serial.print("NTP Update not WORK!!");
#endif
  }

  yield();

  matrix.drawChar(startX + 9, startY, ':', HIGH, LOW, 1);
  matrix.drawChar(startX + 9 + 32, startY, ':', HIGH, LOW, 1);
  matrix.drawChar(startX + 9 + 64, startY, ':', HIGH, LOW, 1);
}

void loop() {
  
  yield();
  time_t utc = now();
  printTime(GR, utc, 0);
  printTime(UK, utc, 1);
  printTime(usET, utc, 2);
  matrix.write();

  yield();
  delay(1000);

}