/*
  graph.h - Library for flashing graphing on TFT_HX8357 code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef graph_h
#define graph_h
//#include <TFT_HX8357.h> // Hardware-specific library
#define LTBLUE    0xB6DF
#define LTTEAL    0xBF5F
#define LTGREEN   0xBFF7
#define LTCYAN    0xC7FF
#define LTRED     0xFD34
#define LTMAGENTA 0xFD5F
#define LTYELLOW  0xFFF8
#define LTORANGE  0xFE73
#define LTPINK    0xFDDF
#define LTPURPLE  0xCCFF
#define LTGREY    0xE71C

#define BLUE      0x001F
#define TEAL      0x0438
#define GREEN     0x07E0
#define CYAN      0x07FF
#define RED       0xF800
#define MAGENTA   0xF81F
#define YELLOW    0xFFE0
#define ORANGE    0xFC00
//#define PINK      0xF81F
#define PURPLE    0x8010
#define GREY      0xC618
#define WHITE     0xFFFF
#define BLACK     0x0000

#define DKBLUE    0x000D
#define DKTEAL    0x020C
#define DKGREEN   0x03E0
#define DKCYAN    0x03EF
#define DKRED     0x6000
#define DKMAGENTA 0x8008
#define DKYELLOW  0x8400
#define DKORANGE  0x8200
#define DKPINK    0x9009
#define DKPURPLE  0x4010
#define DKGREY    0x4A49
double ox = -999, oy = -999; // Force them to be off screen
boolean reInitGraph = true;
unsigned long lastTimePlotCalled;
unsigned long plotCallIter = 10;
unsigned long offsetTime;
float timeAt,dataToPlotY;
float maxPlotTime = 20.0;
float plotYMin = 9.0;
float plotYMax = 18.0;
boolean display1 = true;
boolean update1 = true;
boolean redraw = true;
boolean redraw1 = true;
boolean redraw2 = true;
boolean redraw3 = true;
//TFT_HX8357 tft = TFT_HX8357();       // Invoke custom library
#endif
