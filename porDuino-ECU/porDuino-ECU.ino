///William O. Curry IV Piggyback ECU GitHub Source Control
#include "dataInt.h"
#include "graph.h"
#include "dataInt.h"
#include "dataLog.h"
void setup()
{
  Serial.begin(38400);
  Serial3.begin(38400);
  pinMode(buttonLogPin, INPUT_PULLUP);
  pinMode(buttonFireInjPin, INPUT_PULLUP);
  pinMode(LEDPinG, OUTPUT);
  pinMode(LEDPinError, OUTPUT);
  pinMode(LEDPinInjFiring, OUTPUT);
  pinMode(InjectorLeftPin,OUTPUT);
  pinMode(InjectorRightPin,OUTPUT);
  pinMode(InjectorRelayPin,OUTPUT);
  ///Open Relay so no power flows through injectors
  digitalWrite(InjectorRelayPin,relayOpen);
  digitalWrite(LEDPinError,HIGH);
  digitalWrite(LEDPinG,HIGH);
  digitalWrite(LEDPinInjFiring,HIGH);
  #if sirenEnable
  analogWrite(sirenPin,129);
  #endif
  delay(2000);
  analogWrite(sirenPin,0);
  //TCCR1B = TCCR1B & B11111000 | B00000100;  // for for PWM frequency of 122.55 Hz
   TCCR1B = TCCR1B & B11111000 | B00000101;   // for  PWM frequency of 30.64 Hz
//////////////LCD and OBD Init
#if LCDScreen
// Setup the LCD
  lcd.init();
  lcd.setRotation(1);  
  lcd.fillScreen(TFT_BLACK);
  lcd.setTextColor(TFT_RED,TFT_BLACK);
  lcd.drawString("Bill's PORSCHE ECU Display", 70, 150, 4);
  lcd.setTextColor(ORANGE,TFT_BLACK);
#endif
/////////////SD Card Init  
      #if SDCARD
        #if LCDScreen
          lcd.drawString("Initializing SD card...", 70, 200, 4);
        #endif
      Serial.print("Initializing SD card...");
      delay(500);
      for(int i = 0;i<=5;i++) {
            if (!SD.begin(chipSelect)) {
              SDCardInit = false;
            }    
            else
            {
              SDCardInit = true;
              break;
            }
            delay(500);
      }
      /////Display SD Results
        if(SDCardInit){       
           lcd.drawString("SD Card Found", 70, 250, 4);
           delay(500);
        }
        else
        {
           #if LCDScreen
            lcd.drawString("SD Card Not Found", 70, 250, 4);
           #endif
           delay(1000); 
        }
        delay(1000);
      #endif
  
  ATSizeOfArray = ATNumRPMBands*ATNumBoostBands*2;//////////////////////
  ATSetRAMFromEEPROM(); ///Initialize RAM Map from Non-volite EEPROM values
  #if LCDScreen
    initMainScreen();
  #endif
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(RPMPinD), rpmInterrupt, RISING);
  //pinMode(RPMPinD, INPUT_PULLUP);
  Serial.println("Running Main Loop");
  delay(1000);

}
///////////////////////////////////////MAIN LOOP START////////////////////////////////////////////
void loop()
{
unsigned long loopStartTime = millis();
btDataCheck();
lookForLogButtonPush();
lookForInjButtonPush();
getInputs();
knockCounter();
checkForLeanCondition();
alarmHandler();
  if(btLogDeleteButtonPushed)
  {
    deleteAllLogs();
    btLogDeleteButtonPushed = false;
  }
  if(btLogPrintButtonPushed)
  {
    printRequestedLog();
    btLogPrintButtonPushed = false;
  }
  #if LCDScreen
    if(((millis()-lastMainPrint) > mainPrintFreq)&& screenMain)
    {
      //initScreen();
      printAFRS();
      lastMainPrint= millis();
    }
    if(screenPlotAFR)
    {
      plotData2(AFRLFiltered,AFRRFiltered, CYAN, RED);
    }
      if(screenPlotAFRBoost)
    {
      plotData3(AFRLFiltered,AFRRFiltered,BoostFiltered, CYAN, RED, GREEN, "AFRL", "AFRR", "BOOST","AFR", "AFR",  "PSI");
    }
    if(screenPlotAFRInj)
    {
      plotData3(AFRLFiltered,AFRRFiltered,injectorPulseWidth/10, CYAN, RED, GREEN, "AFRL", "AFRR", "IPW","AFR", "AFR",  "%/10");
    }
  #endif
  if(((millis()-lastLogTimemS) >logFrequencymS)&&logging)
  {
    logData();
    lastLogTimemS = millis();
  }  
  if((millis()-lastInjectorCallMillis) >injectorCallFrequency)
  {
    injStartCheck();
    injectorCall();
    lastInjectorCallMillis = millis();
  }
  ATwriteToEEPROM();
  avgLoopTimeMs = ((millis()-loopStartTime)+avgLoopTimeMs)/2;
  //Serial.print("loop time(ms)= "); Serial.println(avgLoopTimeMs);
}
///////////////////////////////////////MAIN LOOP END////////////////////////////////////////////

void getInputs()
{
  ////RPM Interrupt Stuff
     if (toothCount >= teeth) ///After we counted a full revolution worth of teeth
     {
     rpm = ((toothCount/teeth)*60*1000)/(millis() - timeLastRPM);
     timeLastRPM = millis();
     toothCount = 0;   
     RPMFiltered = RPMFiltered*RPMFilterTC + rpm*(1-RPMFilterTC);    
     }
  ////Read Input Voltages, convert to engineering units, then filter
  int AFRLeftValue = analogRead(AFRLeftPin);
  int AFRRightValue = analogRead(AFRRightPin);
  int BoostValue = analogRead(BoostPin);
  int MAFValue = analogRead(MAFPin);
  int KnockValue = analogRead(KnockPin);
  int ThrottleValue = analogRead(ThrottlePin);
  //Process Inputs to get voltages
  float AFRLeftVolts = AFRLeftValue * (5.0 / 1023.0);
  float AFRRightVolts = AFRRightValue * (5.0 / 1023.0);
  float BoostVolts = float(BoostValue) * (5.0 / 1023.0);
  float MAFVolts = float(MAFValue) * (5.0 / 1023.0);
  KnockVolts = KnockValue * (5.0 / 1023.0);
  float ThrottleVolts = ThrottleValue * (5.0 / 1023.0);
  //Get AFRS
  AFRLeft = (2 * AFRLeftVolts) + 9;
  AFRRight = (2 * AFRRightVolts) + 9;
  //////Change time constant during injector testing to get fast results
    if(injectorTestFireInjectors)
    {
        AFRTC = AFRfilterDuringInjTestTC;
    }
    else
    {
        AFRTC = AFRfilterTC;
    }
  AFRLFiltered = AFRLFiltered*AFRTC + AFRLeft*(1-AFRTC);
  AFRRFiltered = AFRRFiltered*AFRTC + AFRRight*(1-AFRTC);
  //Get Boost
  BoostPSI = (BoostVolts-0.82)*12;//map(BoostVolts,0,5,0,35);
  /////Account for shitty Boost gauge outputing a high boost when vaccum is less than -22 inhg
  if((ThrottleFiltered <=40) && (BoostPSI >= 7))
  {
   BoostPSI = 0; 
  }
  BoostFiltered = BoostFiltered*BoostfilterTC + BoostPSI*(1-BoostfilterTC);
  maxBoost = max(maxBoost,BoostFiltered);
  //MAF
  float MAFMapped = 14.7+(-27.8*MAFVolts)+14.6*pow(MAFVolts,2);//old method linear MAFVolts*48;//map(MAFVolts,0,5,0,240);//volts to g/s
  MAFFiltered = MAFFiltered*MAFFilterTC + MAFMapped*(1-MAFFilterTC);
  //Throttle
  float ThrottleMapped = billMap(constrain(ThrottleVolts,0.59,4.2),0.59,4.2,0,100);//map(ThrottleVolts,0,5,0,100);
  //Serial.println(ThrottleVolts);
  ThrottleFiltered = ThrottleFiltered*ThrottlefilterTC + ThrottleMapped*(1-ThrottlefilterTC);
}
  
void lookForLogButtonPush ()
{
   // read the state of the switch into a local variable:
  int readingLog = digitalRead(buttonLogPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (readingLog != lastLogButtonState) {
    // reset the debouncing timer
    lastLogDebounceTime = millis();
  }

  if (((millis() - lastLogDebounceTime) > debounceDelay) || btLogSelButtonPushed){
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (readingLog != logButtonState || btLogSelButtonPushed) {
      logButtonState = readingLog;

      // only toggle the LED if the new button state is HIGH
      if (logButtonState == LOW || btLogSelButtonPushed) {
        logLedState = !logLedState;
        logging = !logging;
        btLogSelButtonPushed = false;
            if(logging){
              startLogging();
               #if LCDScreen
                if(screenMain)
                {
                lcd.setTextColor(TFT_GREEN,TFT_BLACK);
                lcd.drawString("LOGGING             ", 275, 250, 4); 
                lcd.setTextColor(TFT_WHITE,TFT_BLACK);
                }
              #endif
            }
            else{
            stopLogging();
            
              #if LCDScreen
                if(screenMain)
                {
                lcd.setTextColor(TFT_MAROON,TFT_BLACK);
                lcd.drawString("NOT LOGGING", 275, 250, 4); 
                lcd.setTextColor(TFT_WHITE,TFT_BLACK);
                }
              #endif
            }
      }
    }
  }

  // set the LED:
  if(logLedState)
  {
  analogWrite(LEDPinG, 30);
  }
  else
  {
   digitalWrite(LEDPinG, logLedState); 
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastLogButtonState = readingLog;
}

float billMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

 void rpmInterrupt() //Called whenever an RPM pulse is detected by the arduino
 {
   toothCount++;
 }
