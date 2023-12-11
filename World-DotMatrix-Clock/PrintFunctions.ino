extern bool reDraw;

//###################################
//        printMoon
//###################################
void printMoon(Timezone tz, time_t utc) {
  TimeChangeRule* tcr;  // pointer to the time change rule, use to get the TZ abbrev

  time_t t = tz.toLocal(utc, &tcr);

  switch (moonPhase(t)){
    case 0:
    #ifdef DEBUG
    Serial.println("Full Moon");
    #endif
    drawIcons(0, 8, 0);
    break;
    case 1:
    #ifdef DEBUG
    Serial.println("Waning Gibbous");
    #endif
    drawIcons(0, 8, 1);
    break;
    case 2:
    #ifdef DEBUG
    Serial.println("Third Quarter");
    #endif
    drawIcons(0, 8, 2);
    break;
    case 3:
    #ifdef DEBUG
    Serial.println("Waning Crescent");
    #endif
    drawIcons(0, 8, 3);
    break;
    case 4:
    #ifdef DEBUG
    Serial.println("New Moon");
    #endif
    drawIcons(0, 8, 4);
    break;
    case 5:
    #ifdef DEBUG
    Serial.println("Waxing Crescent");
    #endif
    drawIcons(0, 8, 5);
    break;
    case 6:
    #ifdef DEBUG
    Serial.println("First Quarter");
    #endif
    drawIcons(0, 8, 6);
    break;
    case 7:
    #ifdef DEBUG
    Serial.println("Waxing Gibbous");
    #endif
    drawIcons(0, 8, 7);
    break;
  }
}

int moonPhase(time_t now){
  // calculates the age of the moon phase(0 to 7)
  // there are eight stages, 0 is full moon and 4 is a new moon
  double jd = 0; // Julian Date
  int b= 0;
  jd = julianDate(year(now), month(now), day(now));
  //jd = julianDate(1972,1,1); // used to debug this is a new moon
  jd = int(jd - 2244116.75); // start at Jan 1 1972
  jd /= 29.53; // divide by the moon cycle
  b = jd;
  jd -= b; // leaves the fractional part of jd
  b = jd*8 +0.5;
  b = b & 7;

  return b;
}
double julianDate(int y, int m, int d){
  // convert a date to a Julian Date}
  int mm,yy;
  double k1, k2, k3;
  double j;

  yy = y- int((12-m)/10);
  mm = m+9;
  if(mm >= 12) {
    mm = mm-12;
  }
  k1 = 365.25 *(yy +4172);
  k2 = int((30.6001 * mm) + 0.5);
  k3 = int((((yy/100) + 4) * 0.75) -38);
  j = k1 +k2 + d + 59;
  j = j-k3; // j is the Julian date at 12h UT (Universal Time)

  return j;
}
//###################################
//        printDay
//###################################
void printDay(Timezone tz, time_t utc) {
  TimeChangeRule* tcr;  // pointer to the time change rule, use to get the TZ abbrev

  time_t t = tz.toLocal(utc, &tcr);

  String dotDay = String(day(t));

  if (day(t) >= 10) {
    draw3x7(startX, startY+17, dotDay[0]);
    draw3x7(startX+4, startY+17, dotDay[1]);
  } else {
    draw3x7(startX, startY+17, '0');
    draw3x7(startX+4, startY+17, dotDay[0]);
  }
}

//###################################
//        printTime
//###################################
// type 5x7=1 3x5=2
void printHour(uint8_t ampm, uint8_t hour, uint8_t x ,uint8_t y, uint8_t fontType) {
  String dotHour = String(hour);

  if (fontType == 1) {
    matrix.drawChar(x+9, y, ':', HIGH, LOW, 1);

    if (hour >= 10) {
      matrix.drawChar(x, y, dotHour[0], HIGH, LOW, 1);
      matrix.drawChar(x+5, y, dotHour[1], HIGH, LOW, 1);
    } else {
      matrix.drawChar(x, y, ' ', HIGH, LOW, 1);
      matrix.drawChar(x+5, y, dotHour[0], HIGH, LOW, 1);
    }

    if (ampm == 1) {  // AM
      matrix.drawRect(x+25, y, 3, 2, HIGH);
      matrix.drawRect(x+25+4, y, 3, 2, LOW);
    } else if (ampm == 2) {  // PM
      matrix.drawRect(x+25, y, 3, 2, LOW);
      matrix.drawRect(x+25+4, y, 3, 2, HIGH);
    }
  }
  else if (fontType == 2) {
    // draw4x5(x+9, y+3, 48+10); // 10    :
    //
    // if (hour >= 10) {
    //   draw4x5(x+1, y+3, dotHour[0]);
    //   draw4x5(x+5, y+3, dotHour[1]);
    // } else {
    //   draw4x5(x+1, y+3, 48+16); // 16    space
    //   draw4x5(x+5, y+3, dotHour[0]);
    // }

    draw4x7(x+9, y, 48+10); // 10    :

    if (hour >= 10) {
      draw4x7(x, y, dotHour[0]);
      draw4x7(x+5, y, dotHour[1]);
    } else {
      draw4x7(x, y, 48+16); // 16    space
      draw4x7(x+5, y, dotHour[0]);
    }
    // if (ampm == 1) {  // AM
    //   matrix.drawRect(x+2, y, 7, 2, HIGH);
    //   matrix.drawRect(x+12, y, 9, 2, LOW);
    // } else if (ampm == 2) {  // PM
    //   matrix.drawRect(x+2, y, 7, 2, LOW);
    //   matrix.drawRect(x+12, y, 9, 2, HIGH);
    // }
    // if (ampm == 1) {  // AM
    //   matrix.drawLine(x+11+5+2, y, 1, 3, HIGH);
    //   matrix.drawLine(x+11+5+2, y+4, 1, 3, LOW);
    // } else if (ampm == 2) {  // PM
    //   matrix.drawLine(x+11+5+2, y, 1, 3, LOW);
    //   matrix.drawLine(x+11+5+2, y+4, 1, 3, HIGH);
    // }
    if (ampm == 1) {  // AM
      matrix.drawRect(x+11+5+6, y, 2, 3, LOW);
      matrix.drawRect(x+11+5+6, y+4, 2, 3, HIGH);
    } else if (ampm == 2) {  // PM
      matrix.drawRect(x+11+5+6, y, 2, 3, HIGH);
      matrix.drawRect(x+11+5+6, y+4, 2, 3, LOW);
    }
  }
}

void printMinute(uint8_t minute, uint8_t x ,uint8_t y, uint8_t fontType) {
  String dotMin = String(minute);

  if (fontType == 1) {
    if (minute >= 10) {
      matrix.drawChar(x, y, dotMin[0], HIGH, LOW, 1);
      matrix.drawChar(x+6, y, dotMin[1], HIGH, LOW, 1);
    } else {
      matrix.drawChar(x, y, '0', HIGH, LOW, 1);
      matrix.drawChar(x+6, y, dotMin[0], HIGH, LOW, 1);
    }
  }
  else if (fontType == 2) {
    // if (minute >= 10) {
    //   draw4x5(x+11, y+3, dotMin[0]);
    //   draw4x5(x+11+5, y+3, dotMin[1]);
    // } else {
    //   draw4x5(x+11, y+3, '0');
    //   draw4x5(x+11+5, y+3, dotMin[0]);
    // }
    if (minute >= 10) {
      draw4x7(x+11, y, dotMin[0]);
      draw4x7(x+11+5, y, dotMin[1]);
    } else {
      draw4x7(x+11, y, '0');
      draw4x7(x+11+5, y, dotMin[0]);
    }
  }
}

void printSeconds(uint8_t seconds, uint8_t x ,uint8_t y, uint8_t fontType) {
  String dotSec = String(seconds);

  if (fontType == 1) {
    if (seconds >= 10) {
      draw3x5(x, y+3, dotSec[0]);
      draw3x5(x+4, y+3, dotSec[1]);
    } else {
      draw3x5(x, y+3, '0');
      draw3x5(x+4, y+3, dotSec[0]);
    }
  }
  else if (fontType == 2) {
  }
}

void printTime(Timezone tz, time_t utc, uint8_t clockDisp, uint8_t fontType) {
  TimeChangeRule* tcr;  // pointer to the time change rule, use to get the TZ abbrev

  time_t t = tz.toLocal(utc, &tcr);
  //hour(t), minute(t), second(t), dayShortStr(weekday(t)), day(t), year(t)

  ////////////////////////////////
  // restart to reconnect and clean
  // if (hour(t) == 4 && minute(t) == 0 && second(t) == 0) {ESP.restart();}
  ////////////////////////////////

  // Print Hour
  if (zoneHour[clockDisp] != hour(t)) {  // checking to avoid re-drawing
    zoneHour[clockDisp] = hour(t);

    //cleanPanel(clockDisp);
    zoneYear[clockDisp] = year(t);
    zoneMonth[clockDisp] = month(t);
    zoneDay[clockDisp] = day(t);

    // Calculations for AM/PM
    zoneHourAMPM[clockDisp] = hour(t);
    if (zoneHourAMPM[clockDisp] >= 12) {
      zoneAMPM[clockDisp] = 2;  //PM
    } else {
      zoneAMPM[clockDisp] = 1;  // AM
    }
    if (zoneHourAMPM[clockDisp] > 12) {
      zoneHourAMPM[clockDisp] = zoneHourAMPM[clockDisp] - 12;
    }

    // Hour
    if (fontType == 1) {
      printHour(zoneAMPM[clockDisp], zoneHourAMPM[clockDisp], startX, startY +8*clockDisp, fontType);
    }
    else if (fontType == 2) {
      printHour(zoneAMPM[clockDisp], zoneHourAMPM[clockDisp], startX+8, startY +8*clockDisp, fontType);
    }

    if (clockDisp == 1) {printMoon(Clock[0], utc);}
    else if (clockDisp == 2) {printDay(Clock[0], utc);}

    reDraw = true;
    #ifdef DEBUG
    Serial.print("Draw Hour ");
    Serial.println(zoneHour[clockDisp]);
    #endif
  }
  // Print Minutes
  if (zoneMinute[clockDisp] != minute(t)) {  // checking to avoid re-drawing
    zoneMinute[clockDisp] = minute(t);
    if (fontType == 1) {
      printMinute(zoneMinute[clockDisp], startX + 13, startY +8*clockDisp, fontType);
    }
    else if (fontType == 2) {
      printMinute(zoneMinute[clockDisp], startX+9, startY +8*clockDisp, fontType);
    }

    reDraw = true;
    #ifdef DEBUG
    //Serial.print("Draw Minutes ");
    //Serial.println("zoneMinute[clockDisp]);
    #endif
  }

  // Print Seconds
  if (zoneSeconds[clockDisp] != second(t)) {  // checking to avoid re-drawing  
    zoneSeconds[clockDisp] = second(t);
    if (fontType == 1) {
      printSeconds(zoneSeconds[clockDisp], startX + 25, startY +8*clockDisp, fontType);
    }
    else if (fontType == 2) {
    }
    reDraw = true;
  }

/*
  #ifdef DEBUG
  Serial.println("Draw Seconds");
  #endif*/
}

//###################################
//        printTemp
//###################################
//#ifdef TEMPERATURE
// void printTemp(uint8_t clockDisp) {
//
//   float h = dht.readHumidity();
//   // Read temperature as Celsius (the default)
//   float t = dht.readTemperature();
//
//   // Check if any reads failed and exit early (to try again).
//   if (isnan(h) || isnan(t)) {
//     Serial.println(F("Failed to read from DHT sensor!"));
//     return;
//   }
//
//   // Compute heat index in Celsius (isFahreheit = false)
//   float hic = dht.computeHeatIndex(t, h, false);
//
//   // String dotTemp = t;
//   char dotTemp[4];
//   dtostrf(t, 3, 1, dotTemp);
//
//   Serial.println(dotTemp);
//   cleanPanel(clockDisp);
//
//   matrix.drawChar(startX + 1 + 32 * clockDisp, startY, dotTemp[0], HIGH, LOW, 1);
//   matrix.drawChar(startX + 7 + 32 * clockDisp, startY, dotTemp[1], HIGH, LOW, 1);
//   matrix.drawChar(startX + 12 + 32 * clockDisp, startY, dotTemp[2], HIGH, LOW, 1);
//   matrix.drawChar(startX + 18 + 32 * clockDisp, startY, dotTemp[3], HIGH, LOW, 1);
//   matrix.drawChar(startX + 25 + 32 * clockDisp, startY, 'C', HIGH, LOW, 1);
// }

// void cleanPanel(uint8_t clockDisp) {
//   matrix.fillRect(startX + 0 + 32 * clockDisp, startY, 32, 8, LOW);  // Clean only one screen
// }
//#endif


//###################################
//        printRiseSet
//###################################
// void printRiseSet(uint8_t clockDisp) {
//   double transit, sunrise, sunset;
//
//   // Calculate the times of sunrise, transit, and sunset, in hours (UTC)
//   calcSunriseSunset(zoneYear[clockDisp], zoneMonth[clockDisp], zoneDay[clockDisp], latitude, longitude, transit, sunrise, sunset);
//
//
//   String tape = String(sunrise, 4);
//   int spacer = 1;
//   int width = 5 + spacer;
//   int i = 0;
//
//
//   matrix.fillScreen(LOW);
//
//   int letter = i / width;
//   int x = (matrix.width() - 1) - i % width;
//   int y = (matrix.height() - 8) / 2;  // center the text vertically
//
//   while (x + width - spacer >= 0 && letter >= 0) {
//     if (letter < tape.length()) {
//       matrix.drawChar(x, y, tape[letter], HIGH, LOW, 1);
//     }
//     letter--;
//     x -= width;
//   }
//   matrix.write();  // Send bitmap to display
// }


// Rounded HH:mm format
// char* hoursToString(double h, char* str) {
//   int m = int(round(h * 60));
//   int hr = (m / 60) % 24;
//   int mn = m % 60;
//
//   str[0] = (hr / 10) % 10 + '0';
//   str[1] = (hr % 10) + '0';
//   str[2] = ':';
//   str[3] = (mn / 10) % 10 + '0';
//   str[4] = (mn % 10) + '0';
//   str[5] = '\0';
//   return str;
// }

// void RiseSet(uint8_t clockDisp) {
//
//   // Location  52.951784309241816, -1.1407220625540686
//   double latitude = 52.951;
//   double longitude = -1.140;
//
//   double transit, sunrise, sunset;
//
//   // Calculate the times of sunrise, transit, and sunset, in hours (UTC)
//   calcSunriseSunset(zoneYear[clockDisp], zoneMonth[clockDisp], zoneDay[clockDisp], latitude, longitude, transit, sunrise, sunset);
//
//   // Get the approximate times (minimum program size) (iterations = 0)
//   //calcSunriseSunset(zoneYear[clockDisp], zoneMonth[clockDisp], dzoneDay[clockDisp], latitude, longitude, transit, sunrise, sunset, SUNRISESET_STD_ALTITUDE, 0);
//
//   // Print results
//   char str[6];
//   Serial.println(hoursToString(sunrise, str));
//   Serial.println(hoursToString(transit, str));
//   Serial.println(hoursToString(sunset, str));
// }

// // Rounded HH:mm format
// char* hoursToString(double h, char* str) {
//   int m = int(round(h * 60));
//   int hr = (m / 60) % 24;
//   int mn = m % 60;
//
//   str[0] = (hr / 10) % 10 + '0';
//   str[1] = (hr % 10) + '0';
//   str[2] = ':';
//   str[3] = (mn / 10) % 10 + '0';
//   str[4] = (mn % 10) + '0';
//   str[5] = '\0';
//   return str;
// }
//
// void RiseSet(uint8_t clockDisp) {
//
//   // Location  52.951784309241816, -1.1407220625540686
//   double latitude = 52.951;
//   double longitude = -1.140;
//
//   double transit, sunrise, sunset;
//
//   // Calculate the times of sunrise, transit, and sunset, in hours (UTC)
//   calcSunriseSunset(zoneYear[clockDisp], zoneMonth[clockDisp], zoneDay[clockDisp], latitude, longitude, transit, sunrise, sunset);
//
//   // Get the approximate times (minimum program size) (iterations = 0)
//   //calcSunriseSunset(zoneYear[clockDisp], zoneMonth[clockDisp], dzoneDay[clockDisp], latitude, longitude, transit, sunrise, sunset, SUNRISESET_STD_ALTITUDE, 0);
//
//   // Print results
//   char str[6];
//   Serial.println(hoursToString(sunrise, str));
//   Serial.println(hoursToString(transit, str));
//   Serial.println(hoursToString(sunset, str));
// }

// void printRiseSet(uint8_t clockDisp) {
//   double transit, sunrise, sunset;
//
//   // Calculate the times of sunrise, transit, and sunset, in hours (UTC)
//   calcSunriseSunset(zoneYear[clockDisp], zoneMonth[clockDisp], zoneDay[clockDisp], latitude, longitude, transit, sunrise, sunset);
//
//
//   String tape = String(sunrise, 4);
//   int spacer = 1;
//   int width = 5 + spacer;
//   int i = 0;
//
//
//   matrix.fillScreen(LOW);
//
//   int letter = i / width;
//   int x = (matrix.width() - 1) - i % width;
//   int y = (matrix.height() - 8) / 2;  // center the text vertically
//
//   while (x + width - spacer >= 0 && letter >= 0) {
//     if (letter < tape.length()) {
//       matrix.drawChar(x, y, tape[letter], HIGH, LOW, 1);
//     }
//     letter--;
//     x -= width;
//   }
//   matrix.write();  // Send bitmap to display
// }

//###################################
//        showIP
//###################################
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
    delay(10);
  }
}

//###################################
//        printSpin
//###################################
void printSpin() {
  int wait = 10;
  int inc = -2;

  for ( int times = 0; times < 2; times++ ) {

    for ( int x = 0; x < matrix.width() - 1; x++ ) {
      matrix.fillScreen(LOW);
      matrix.drawLine(x, 0, matrix.width() - 1 - x, matrix.height() - 1, HIGH);
      matrix.write(); // Send bitmap to display
      delay(wait);
    }

    for ( int y = 0; y < matrix.height() - 1; y++ ) {
      matrix.fillScreen(LOW);
      matrix.drawLine(matrix.width() - 1, y, 0, matrix.height() - 1 - y, HIGH);
      matrix.write(); // Send bitmap to display
      delay(wait);
    }

    wait = wait + inc;
    if ( wait == 0 ) inc = 2;
    if ( wait == 50 ) inc = -2;

    yield();
  }
}
