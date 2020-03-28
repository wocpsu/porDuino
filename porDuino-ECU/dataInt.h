#ifndef dataInt_h
#define dataInt_h
#include <avr/wdt.h> //Watchdog timer to allow software reset
///Accounting for different versions of harware
#define porDuinoVersion 2
  #if porDuinoVersion == 2
    #define relayClosed HIGH
    #define relayOpen LOW
  #else
    #define SDCARD true
    #define LCDScreen true
    #define relayClosed LOW
    #define relayOpen HIGH
  #endif
/////
unsigned long avgLoopTimeMs;
#include <EEPROM.h> //EEPROM Library for Auto Tune
#include <TFT_HX8357.h> // Hardware-specific library
#if LCDScreen
  ///////////////Initalize Screen
  TFT_HX8357 lcd = TFT_HX8357();       // Invoke custom library
  #define TFT_GREY 0x7BEF
#endif
////pinchange interrupts library
#include <PinChangeInterrupt.h> //////////////**********I DISABLED ALL OTHER PIN CHANGE INTERRUPTS EXCEPT PCINT_ENABLE_PCINT16 in settings file
#define PCINT_ENABLE_PCINT16
//#define ORANGE    0xFC00
//BT Stuff
boolean btLogSelButtonPushed;
boolean btLogDeleteButtonPushed;
boolean btLogPrintButtonPushed;
int myBTInt;
////////////////////Enable features
#define btButton 1
#define sirenEnable true
#define injStartCheckerEnable 1
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Pins Analog ins (A15 ARFLEFT, A14 AFRRIGHT, A13 BOOST, A12 OPEN, A11 MAF, A10 KNOCK, A9 THROTTLE, A8 RPM)
/////OLDDDD Pins Analog Outs (D4 INJECTOR LEFT, D13 INJECTOR RIGHT)
/////OLDDDD Pins Analog Outs (D11 INJECTOR LEFT, D12 INJECTOR RIGHT)
//Pins D10=LEDG,D9=LEDR,D8=LEDR
//Pin Definitions Analog Inputs
int AFRRightPin = A15;
int AFRLeftPin = A14;
int BoostPin = A13;
int MAFPin = A11;
int KnockPin = A10;
int ThrottlePin = A9;
int RPMPin = A8;
int RPMPinD = 62; ///Digital pin value for A8
unsigned long timeLastRPM;
//Pin Definitions Analog Outputs
int InjectorLeftPin = 11;
int InjectorRightPin = 12;
int InjectorRelayPin = 4;
int sirenPin = A0;
//Digitals
int LEDPinG = 10;
int LEDPinError = 9;
int LEDPinInjFiring= 8;
int buttonLogPin = 6;
int buttonFireInjPin = 5;
//int resetPin = 3; ///Pin that's hardwired to reset to reboot the arduino
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//AFR Stuff
float AFRLeft,AFRRight; 
float AFRLFiltered = 0;
float AFRRFiltered = 0;
#define AFRfilterTC 0.95
float AFRTC = 0.98; ///switches based off when inj test is running
#define AFRfilterDuringInjTestTC 0.7
//Boost Stuff
float BoostPSI;
float BoostFiltered;
float maxBoost;
#define BoostfilterTC 0.95
//MAF Stuff
#define MAFFilterTC 0.97
float MAFFiltered;
//Throttle Stuff
float ThrottleFiltered;
#define ThrottlefilterTC 0.96
//Knock Stuff
int knockCount;
float KnockVolts;
float knockThreshold = 4.0; //knock if less than this thresold
unsigned long knockCountStartTime = 15000; // Start knock count at 10 seconds;
boolean knockReset = true;
//RPM Stuff
float RPMFiltered;
volatile unsigned int toothCount =0;
float rpm;
#define teeth  58 //number of teeth on flywheel
#define RPMFilterTC 0.90
//Injector Stuff
boolean injectorsEnabled = false;
float injectorStartPWMVal = 30;
float injectorEndPWMVal = 87;
//float leftSidePWMult = 1.2; ///20% more for left side due to higher obsearved AFR
//float rightSidePWMult = 1.0;
float twoStepPWMVal = 10;
float injectorBoostOnLevel = 1;
float injectorOnThrottleLevel = 65;
float injectorOnMAFLevel = 145;
float injectorPulseWidth;
float injectorPulseWidthLeft;
float injectorPulseWidthRight;
float maxInjPulseWidth;
unsigned long injectorCallFrequency = 50;
unsigned long lastInjectorCallMillis;
boolean injectorArmed;
boolean autoInjFire;
boolean manualInjFire;
///Injector Start Test
String injStrtCheck = "PASSS";
boolean injStrLPass;
boolean injStrRPass;
boolean injStartChkCancelHighThrottle;
boolean injStartChkRanAlready;
boolean injectorTestRunning;
boolean injectorTestFireInjectors;
#define InjStrChkThrottleCancelThreshold 30 ////throtte threshold for canceling the start check
#define InjStrChkAFRLessThanThres 15 ///AFR must be below this threshold to do the start check
#define InjStrChkAFRGreaterThanThres 12.0 ///AFR must be greater than this threshold to do the start check
#define InjStrChkAFRDropThreshold 1.0 //How much AFR ratio must drop to consider injector check to be good
#define InjStrChkPulseTime 500 //How long to pulse injectors during start check
#define InjStrChkAddTime 2000 //Additional Time to look at AFRS after test has run
#define InjStrChkPW 6 //Pulse width percent for injector start check
#define InjStrCheckWaitTime 10000//Time to wait after ECU Startup before running the injector start check
#define InjTestMinMAF 2
#define InjTestMaxMAF 50
boolean injectorTestEnabled = false;
unsigned long InjLeftBelowThresholdTime;
unsigned long InjRightBelowThresholdTime;
unsigned long InjTestStartTime;
float AFRLAtTestStr;
float AFRRAtTestStr;
/////Push Button Stuff
long debounceDelay = 100;    // the debounce time; increase if the output flickers
///logging button
// Variables will change:
int logLedState = LOW;         // the current state of the output pin
int logButtonState;             // the current reading from the input pin
int lastLogButtonState = LOW;   // the previous reading from the input pin
long lastLogDebounceTime = 0;  // the last time the output pin was toggled
//Injector Firing Button
//int injLedState = LOW;         // the current state of the output pin
int injButtonState;             // the current reading from the input pin
int lastInjButtonState = LOW;   // the previous reading from the input pin
long lastInjDebounceTime = 0;  // the last time the output pin was toggled
///Screen init stuff
boolean screenMain = true;
boolean screenPlotAFR = false;
boolean screenPlotAFRBoost = false;
boolean screenPlotAFRInj = false;
//AutoTune Stuff
///Min max input checking
#define ATMinActiveAFR 9.5
#define ATMaxActiveAFR 18.0
#define ATMinActiveRPM 2000
#define ATMaxActiveBoost 9
boolean ATSensorError = false;
unsigned long ATSensorErrorState;
boolean ATAFRLeftError = false;
unsigned long ATAFRLeftErrorState;
boolean ATAFRRightError = false;
unsigned long ATAFRRightErrorState;
#define ATAFRTarget 12.0
#define ATGain 0.015 
#define ATMaxAdjustPercent 0.35 ///Max Fuel adjustment to left and right
#define ATMinRPM 3000
#define ATRPMSteps 500 //RPM Table Step Size
#define ATNumRPMBands 8
#define ATMinBoost 0
#define ATBoostSteps 1 
#define ATNumBoostBands 8
float ATMapRAM[128]; //Number RPM bands * number boost bands * 2 (for left and right side)
int ATSizeOfArray;
#define ATwriteFreq 10000 //write to EEPROM every 10 seconds
unsigned long ATLastWrite;
float ATnewMapValueL;
float ATnewMapValueR;
//Alarm Stuff
boolean ackKnowledgeAlarm;
boolean alarmLatched;
boolean AFRLLean;
boolean AFRRLean;
//#define AFRLeanAlarmThreshold 13
#define AFRLeanAlarmThresholdTD 1000 //Alarm time delay in ms
boolean firstCountLThisEvent =true;
boolean firstCountRThisEvent =true;
unsigned long AFRLeanLeftStartTime;
unsigned long AFRLeanRightStartTime;
unsigned int AFRLLeanCount;
unsigned int AFRRLeanCount;
String ATSensorErrorString = "";
String ATAFRLeftErrorString = "";
String ATAFRRightErrorString = "";
String sensorFailure = "Sensors Ok";

#endif
