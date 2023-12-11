unsigned int dot3x5[] PROGMEM{
  3, 31, 17, 31, 	// 0         0
	3, 18, 31, 16, 	// 1          1
	3, 18, 25, 22, 	// 2          2
	3, 17, 21, 31, 	// 3          3
	3, 15, 8, 28, 	// 4           4
	3, 23, 21, 29, 	// 5          5
	3, 31, 21, 29, 	// 6         6
	3, 1, 1, 31, 	// 7                7
	3, 31, 21, 31, 	// 8         8
	3, 23, 21, 31, 	// 9          9
	3, 0, 10, 0, 	// 10           :
	3, 30, 5, 30, 	// 11        A
	3, 31, 17, 27, 	// 12     C
	3, 31, 2, 31, 	// 13     M
	3, 31, 5, 7, 	// 14         P
	3, 0, 2, 0, 	// 15      	  °
	3, 0, 0, 0, 	// 16    space
};

unsigned int dot4x5[] PROGMEM {
  4, 31, 17, 17, 31, 	// 0     0
	4, 0, 2, 31, 0, 	// 1      1
	4, 29, 21, 21, 23, 	// 2      2
	4, 17, 21, 21, 31, 	// 3      3
	4, 15, 8, 30, 8, 	// 4      4
	4, 23, 21, 21, 29, 	// 5      5
	4, 31, 21, 21, 29, 	// 6     6
	4, 1, 1, 1, 31, 	// 7        7
	4, 31, 21, 21, 31, 	// 8     8
	4, 23, 21, 21, 31, 	// 9      9
	4, 0, 10, 0, 0, 	// 10     :
	4, 30, 5, 5, 30, 	// 11    A
	4, 14, 17, 17, 17, 	// 12    C
	4, 31, 2, 2, 31, 	// 13   M
	4, 31, 5, 5, 7, 	// 14   P
	4, 0, 6, 6, 0, 	// 15   °
	4, 0, 0, 0, 0, 	// 16  space
};

unsigned int dot3x7[] PROGMEM {
  3, 62, 65, 62, 	// 0   0
	3, 2, 127, 0, 	// 1 	    1
	3, 114, 73, 70, 	// 2   2
	3, 34, 73, 54, 	// 3 	  3
	3, 28, 18, 127, 	// 4 	  4
	3, 39, 69, 57, 	// 5 	  5
	3, 62, 73, 50, 	// 6   6
	3, 113, 9, 7, 	// 7 	  7
	3, 54, 73, 54, 	// 8   8
	3, 38, 73, 62, 	// 9 	  9
	3, 0, 20, 0, 	// 10 	     :
};

unsigned int dot4x7[] PROGMEM {
  4, 62, 65, 65, 62, 	// 0       0
	4, 0, 66, 127, 64, 	// 1        1
	4, 114, 73, 73, 70, 	// 2        2
	4, 33, 73, 77, 51, 	// 3        3
	4, 24, 20, 18, 127, 	// 4        4
	4, 39, 69, 69, 57, 	// 5        5
	4, 62, 73, 73, 49, 	// 6       6
	4, 97, 17, 9, 7, 	// 7          7
	4, 54, 73, 73, 54, 	// 8       8
	4, 70, 73, 73, 62, 	// 9        9
	4, 0, 20, 0, 0, 	// 10       :
	4, 126, 9, 9, 126, 	// 11      A
	4, 62, 65, 65, 34, 	// 12      C
	4, 127, 2, 2, 127, 	// 13     M
	4, 127, 9, 9, 6, 	// 14     P
	4, 0, 6, 6, 0, 	// 15     °
	4, 0, 0, 0, 0, 	// 16   space
};

unsigned int dotIcons[] PROGMEM {
  8, 60, 126, 255, 255, 255, 255, 126, 60, 	// 7 Full Moon
  8, 60, 126, 255, 255, 189, 129, 66, 60, 	// 5 Waning Gibbous
  8, 60, 126, 255, 255, 129, 129, 66, 60, 	// 4 Third Quarter
  8, 60, 126, 255, 195, 129, 129, 66, 60, 	// 3 Waning Crescent
  8, 60, 66, 129, 129, 129, 129, 66, 60, 	// 1 New Moon
  8, 60, 66, 129, 129, 195, 255, 126, 60, 	// 11 Waxing Crescent
  8, 60, 66, 129, 129, 255, 255, 126, 60, 	// 10 First Quarter
  8, 60, 66, 129, 189, 255, 255, 126, 60, 	// 9 Waxing Gibbous
  8, 34, 180, 72, 47, 72, 180, 34, 0, 	// 16    Sun Rise
  8, 34, 52, 72, 143, 72, 52, 34, 0, 	// 17   Sun Set

};

void draw3x5(int16_t x, int16_t y, unsigned char c) {
  unsigned int line;

  for (int i = 0; i < 3; i++) {
    line = dot3x5[(c - 48) * 4 + i + 1];
    for (int j = 0; j < 5; j++, line >>= 1) {  // Because font hight j=3
      if (line & 1)
        matrix.drawPixel(x + i, y + j, HIGH);
      else
        matrix.drawPixel(x + i, y + j, LOW);
    }
  }
}

void draw4x5(int16_t x, int16_t y, unsigned char c) {
  unsigned int line;

  for (int i = 0; i < 4; i++) {
    line = dot4x5[(c - 48) * 5 + i + 1];
    for (int j = 0; j < 5; j++, line >>= 1) {  // Because font hight j=5
      if (line & 1)
      matrix.drawPixel(x + i, y + j, HIGH);
      else
      matrix.drawPixel(x + i, y + j, LOW);
    }
  }
}

void draw3x7(int16_t x, int16_t y, unsigned char c) {
  unsigned int line;

  for (int i = 0; i < 3; i++) {
    line = dot3x7[(c - 48) * 4 + i + 1];
    for (int j = 0; j < 7; j++, line >>= 1) {  // Because font hight j=5
      if (line & 1)
      matrix.drawPixel(x + i, y + j, HIGH);
      else
      matrix.drawPixel(x + i, y + j, LOW);
    }
  }
}

void draw4x7(int16_t x, int16_t y, unsigned char c) {
  unsigned int line;

  for (int i = 0; i < 4; i++) {
    line = dot4x7[(c - 48) * 5 + i + 1];
    for (int j = 0; j < 7; j++, line >>= 1) {  // Because font hight j=5
      if (line & 1)
      matrix.drawPixel(x + i, y + j, HIGH);
      else
      matrix.drawPixel(x + i, y + j, LOW);
    }
  }
}

void drawIcons(int16_t x, int16_t y, unsigned char c) {
  unsigned int line;

  for (int i = 0; i < 8; i++) {
    line = dotIcons[c * 9 + i + 1];
    for (int j = 0; j < 8; j++, line >>= 1) {  // Because font hight j=5
      if (line & 1)
      matrix.drawPixel(x + i, y + j, HIGH);
      else
      matrix.drawPixel(x + i, y + j, LOW);
    }
  }
}

// unsigned int dot3x5OLD[] PROGMEM{
//   3, 248, 136, 248,  // 48   0
//   3, 144, 248, 128,  // 49    1
//   3, 144, 200, 176,  // 50    2
//   3, 136, 168, 248,  // 51    3
//   3, 120, 64, 224,   // 52    4
//   3, 184, 168, 232,  // 53    5
//   3, 248, 168, 232,  // 54   6
//   3, 8, 8, 248,      // 55      7
//   3, 248, 168, 248,  // 56   8
//   3, 184, 168, 248,  // 57    9
//   3, 240, 40, 240,   // 58 A
//   3, 248, 40, 56,    // 59 P
//   3, 248, 16, 248,   // 60 M
// };

// void draw3x5(int16_t x, int16_t y, unsigned char c) {
//   unsigned int line;

//   for (int i = 0; i < 3; i++) {
//     line = dot3x5[(c - 48) * 4 + i + 1];
//     for (int j = 0; j < 8; j++, line >>= 1) {  // Because font hight j=3
//       if (line & 1)
//         matrix.drawPixel(x + i, y + j, HIGH);
//       else
//         matrix.drawPixel(x + i, y + j, LOW);
//     }
//   }
// }
