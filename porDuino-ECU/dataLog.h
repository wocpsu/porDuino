#ifndef dataLog_h
#define dataLog_h

#if SDCARD
#include <SPI.h>
#include <SD.h>
File myFile;
#endif
boolean SDCardInit = false;
const int chipSelect = 53;
String logHeader = "Time,AFRL,AFRR,Boost,MAF,THROT,RPM,INJPW,INJPL,INJPWR,KnockCnt";
String fileName;
unsigned long logFrequencymS = 100;
unsigned long lastLogTimemS;
unsigned long mainPrintFreq = 100;
unsigned long lastMainPrint;
unsigned long logStartMillis;
boolean logging = false;


#endif
