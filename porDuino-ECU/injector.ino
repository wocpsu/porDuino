
void lookForInjButtonPush ()
{
   // read the state of the switch into a local variable:
  int readingInj = digitalRead(buttonFireInjPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (readingInj != lastInjButtonState) {
    // reset the debouncing timer
    lastInjDebounceTime = millis();
  }

  if ((millis() - lastInjDebounceTime) > debounceDelay){
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (readingInj != injButtonState) {
      injButtonState = readingInj;
//
//      // only toggle the LED if the new button state is HIGH
//      if (logButtonState == LOW || btLogSelButtonPushed) {
//        logLedState = !logLedState;
//        logging = !logging;
//      }
    }
  }
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastInjButtonState = readingInj;
}

void injectorCall()
{
  manualInjFire = (!lastInjButtonState && injectorArmed);
  //manualInjFire = (injectorArmed);
  autoInjFire = ((BoostFiltered >= injectorBoostOnLevel)&& (ThrottleFiltered >= injectorOnThrottleLevel) &&  (MAFFiltered >= injectorOnMAFLevel));
  if(autoInjFire||manualInjFire||injectorTestFireInjectors)
  {  
    digitalWrite(InjectorRelayPin,relayClosed);
    #if sirenEnable
    analogWrite(sirenPin,150);
    #endif
    injectorsEnabled = true;
    if(autoInjFire)
    {
      ATautoTune(); ///Call autotune function and get injector multiplier
      injectorPulseWidth = min(billMap(BoostFiltered,0,7,injectorStartPWMVal,injectorEndPWMVal),injectorEndPWMVal);
      maxInjPulseWidth = max(maxInjPulseWidth,injectorPulseWidth);
    }
    else if(manualInjFire)
    {
      injectorPulseWidth = twoStepPWMVal;
    }
    else if (injectorTestFireInjectors)
    {
      //injectorPulseWidth = InjStrChkPW*(1+sin(4*(float(millis())/1000)));
      injectorPulseWidth = InjStrChkPW;
    }
    
  }
  else
  {
    injectorsEnabled = false;
    injectorPulseWidth = 0;
    analogWrite(sirenPin,0);
  }
////Write to the injectors/////////////////////
  if(autoInjFire) /////Applying MAP Trims to injectors to even out AFRs
  {
     injectorPulseWidthLeft = injectorPulseWidth*ATnewMapValueL;
     injectorPulseWidthRight = injectorPulseWidth*ATnewMapValueR;
  }
  else ////Get rid of MAP Trims during injector test and manual inj fire
  {
     injectorPulseWidthLeft = injectorPulseWidth;
     injectorPulseWidthRight = injectorPulseWidth; 
  }
  int pulseWidthToAnalogOutLeft = map(injectorPulseWidthLeft,0,100,0,255);
  int pulseWidthToAnalogOutRight = map(injectorPulseWidthRight,0,100,0,255);
  analogWrite(InjectorLeftPin,pulseWidthToAnalogOutLeft);
  analogWrite(InjectorRightPin,pulseWidthToAnalogOutRight);
   
  if(injectorsEnabled)
  {
   analogWrite(LEDPinInjFiring, 100); 
  }
  else
  {
   digitalWrite(LEDPinInjFiring, LOW); 
  }
}

void injStartCheck ()
{
#if injStartCheckerEnable
  if(ThrottleFiltered >= InjStrChkThrottleCancelThreshold)
  {
    injStartChkCancelHighThrottle = true;
    injectorTestRunning = false;
  }
//////////////Main test
  if(!injStartChkCancelHighThrottle && !injStartChkRanAlready && injectorTestEnabled) ///IF test has not been ran and the throttle hasn't gone above test threshold
  {
      boolean AFRLRReady = (AFRLFiltered <= InjStrChkAFRLessThanThres) && (AFRRFiltered <= InjStrChkAFRLessThanThres);
      AFRLRReady = AFRLRReady && ((AFRLFiltered >= InjStrChkAFRGreaterThanThres) && (AFRRFiltered >= InjStrChkAFRGreaterThanThres));
      boolean MAFReady = (MAFFiltered >= InjTestMinMAF) && (MAFFiltered <= InjTestMaxMAF);
      if((millis()>InjStrCheckWaitTime && AFRLRReady && MAFReady)|| injectorTestRunning)
      {
        injectorTestRunning = true;
        injectorTestFireInjectors = true;
        boolean AFRLBelowThres = AFRLFiltered <= (AFRLAtTestStr - InjStrChkAFRDropThreshold); 
        boolean AFRRBelowThres = AFRRFiltered <= (AFRRAtTestStr - InjStrChkAFRDropThreshold);
          if(AFRLBelowThres)
          {
            injStrLPass = (millis()-InjLeftBelowThresholdTime)>= (InjStrChkPulseTime/2); ////divide injector pulse time by 2 for AFR drop time check
          }
          else
          {
            InjLeftBelowThresholdTime = millis();
          }
          if(AFRRBelowThres)
          {
            injStrRPass = (millis()-InjRightBelowThresholdTime)>= (InjStrChkPulseTime/2); ////divide injector pulse time by 2 for AFR drop time check
          }
          else
          {
            InjRightBelowThresholdTime = millis();
          }
          //////Check to see if injector pulse time is over
          if((millis() -  InjTestStartTime) > InjStrChkPulseTime)
          {
            injectorTestFireInjectors = false;
          }
          ////Wait a little longer after injectors are shutoff to determine if test has passed
          if((millis() -  InjTestStartTime) > (InjStrChkPulseTime+InjStrChkAddTime))
          {
            injStartChkRanAlready = true;
            injectorTestRunning = false;
            injStrtCheck = "";
            if(injStrLPass)
            {
              injStrtCheck = "LPASS";
            }
            else
            {
              injStrtCheck = "LFAIL";
            }
            if(injStrRPass)
            {
              injStrtCheck = injStrtCheck + "/RPASS";
            }
            else
            {
              injStrtCheck = injStrtCheck + "/RFAIL";
            }
          }
           
      }
      else
      { 
        AFRLAtTestStr = AFRLFiltered;
        AFRRAtTestStr = AFRRFiltered;
        InjTestStartTime = millis();    
        injectorTestRunning = false;
        injectorTestFireInjectors = false;
      }
    
  }
  else
  {
    
    AFRLAtTestStr = AFRLFiltered;
    AFRRAtTestStr = AFRRFiltered;
    InjTestStartTime = millis();   
    injectorTestRunning=false;
    injectorTestFireInjectors = false;
  }

/////Check Results of Test////////////////
//  Serial.print(millis());
//  Serial.print("  Test Ran = ");
//  Serial.print(injStartChkRanAlready);
//  Serial.print("  Test Running = ");
//  Serial.print(injectorTestRunning);
//  Serial.print("  High Throttle Cancel = ");
//  Serial.print(injStartChkCancelHighThrottle);
//  Serial.print("  Test Status = ");
//  Serial.print(injStrtCheck);
//  Serial.print("  AFRLPASS = ");
//  Serial.print(injStrLPass);
//  Serial.print("  AFRRPASS = ");
//  Serial.println(injStrRPass);
  
  if (!injStartChkRanAlready)
  {
    injStrtCheck = "Test Not Run"; 
  }



#endif
}
