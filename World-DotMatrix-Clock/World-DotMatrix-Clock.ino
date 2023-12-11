#include <SPI.h>
#include <Adafruit_GFX.h>  //http://adafruit.github.io/Adafruit-GFX-Library/html/class_adafruit___g_f_x.html
#include <Max72xxPanel.h>
//#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Timezone.h>
#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager

//#include <SolarCalculator.h>

void printTime(Timezone tz, time_t utc, uint8_t fnPanel, uint8_t fontType);
//void printMoon(Timezone tz, time_t utc);
//void printDay(Timezone tz, time_t utc);
//void printRiseSet(uint8_t fnPanel);

void showIP();

//#define DEBUG
//#define TEMPERATURE

//###################################
//        Temperature
//###################################
#ifdef TEMPERATURE
#include <DHT.h>

void printTemp(uint8_t fnPanel);

// Tempeture sensor use
#define DHTPIN 2  // D4 Digital pin connected to the DHT sensor
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// current temperature & humidity, updated in loop()
float t = 0.0;
float h = 0.0;
unsigned long previousMillis = 0;  // will store last time DHT was updated
// Updates DHT readings every 10 seconds
const long interval = 10000;
#endif
//###################################
//        Sunrise
//###################################
// GPS Location for Sun Rise/Set 52.951784309241816, -1.1407220625540686
const double latitude = 52.951;
const double longitude = -1.140;

//###################################
//        Dot Matrix
//###################################
// Clock use
const uint8_t startX = 0;
const uint8_t startY = 0;
const uint8_t zones = 3;

int pinCS = D6;  // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
int numberOfHorizontalDisplays = 4;
int numberOfVerticalDisplays = 3;
Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

//###################################
//        WiFi
//###################################
WiFiUDP ntpUDP;
// You can specify the time server pool and the offset, (in seconds)
// additionaly you can specify the update interval (in milliseconds).
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 2 * 60 * 60 * 1000); // Every 2 hours
//NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 5 * 60 * 1000);

WiFiManager wm;
//###################################
//        NTP Tome TZ
//###################################
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

//###################################
//###################################
//###################################
const Timezone Clock[3] = { UK, GR, usET };  //UK
//const Timezone Clock[3] = { GR, UK, usET };  //Greece
//const Timezone Clock[3] = { usET, GR, UK }; //America
//###################################
//###################################
//###################################
//###################################
//        Clocks Variables
//###################################
uint16_t zoneYear[zones] = { 99, 99, 99 };
uint8_t zoneMonth[zones] = { 99, 99, 99 };
uint8_t zoneDay[zones] = { 99, 99, 99 };
uint8_t zoneHour[zones] = { 99, 99, 99 };
uint8_t zoneMinute[zones] = { 99, 99, 99 };
uint8_t zoneSeconds[zones] = { 99, 99, 99 };
uint8_t zoneHourAMPM[zones];
uint8_t zoneAMPM[zones];

bool reDraw = false;

void cleanPanel(uint8_t fnPanel) {
  matrix.fillRect(startX + 0 + 32 * fnPanel, startY, 32, 8, LOW);  // Clean only one screen
}

void setup() {
  yield();
  delay(1000);
  matrix.setIntensity(0);  // Use a value between 0 and 15 for brightness
  yield();
  delay(10);
  for (int i = 0; i < numberOfHorizontalDisplays*numberOfVerticalDisplays; i++) {
    matrix.setRotation(i, 1);
    delay(10);
  }
  matrix.setPosition(0, 0, 0);
  matrix.setPosition(1, 1, 0);
  matrix.setPosition(2, 2, 0);
  matrix.setPosition(3, 3, 0);
  matrix.setPosition(4, 0, 1);
  matrix.setPosition(5, 1, 1);
  matrix.setPosition(6, 2, 1);
  matrix.setPosition(7, 3, 1);
  matrix.setPosition(8, 0, 2);
  matrix.setPosition(9, 1, 2);
  matrix.setPosition(10, 2, 2);
  matrix.setPosition(11, 3, 2);

  yield();
  delay(10);

  #ifdef DEBUG
  Serial.begin(115200);
  yield();
  delay(10);
  #endif

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
  yield();

  timeClient.begin();
  if (timeClient.update()) {
    #ifdef DEBUG
    Serial.println("Adjust local clock");
    #endif
    unsigned long epoch = timeClient.getEpochTime();
    setTime(epoch);
  } else {
    #ifdef DEBUG
    Serial.println("NTP Update not WORK!!");
    #endif
  }

  yield();
  printSpin();

  #ifdef TEMPERATURE
  dht.begin();
  #endif

  matrix.fillScreen(LOW);
}


uint8_t rotateDisplay = 0;
const uint8_t PanelInfo = 2;

void loop() {
  if (WiFi.status() != WL_CONNECTED) { // Check if the device is not connected to the WLAN
    wm.setConfigPortalTimeout(0); // Disable the configuration portal
    while (WiFi.status() != WL_CONNECTED) { // Continue attempting to connect until a successful
      #ifdef DEBUG
      Serial.print("WiFi Trying to reconnect");
      #endif    
      delay(5000);
      WiFi.begin(); // Try to connect to the WLAN again
      delay(2000);
    }
  }

  yield();
  time_t utc = now();
  printTime(Clock[0], utc, 0, 1);
  printTime(Clock[1], utc, 1, 2);
  printTime(Clock[2], utc, 2, 2);

  // if (rotateDisplay < 10) {
  //   printTime(usET, utc, 2);
  // } else if (rotateDisplay == 10) {
  //   printTemp(PanelInfo);
  // }

  // Serial.println(rotateDisplay);
  if (reDraw == true) {
    matrix.write();
    reDraw = false;

    if ( hour(utc) % 2 == 0 && minute(utc) == 10 && second(utc) == 0) { // NTP update every 2 hours
      timeClient.forceUpdate();
    }
  }
  
  yield();
  delay(10);
  //delay(1000);

  // rotateDisplay++;
  // if (rotateDisplay > 20) {
  //   rotateDisplay = 0;
  //   zoneHour[PanelInfo] = 99;
  //   zoneMinute[PanelInfo] = 99;
  // }
}
