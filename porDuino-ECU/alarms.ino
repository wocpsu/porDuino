////////////////////////////////////Alarms///////////////////////
void alarmHandler ()
{
///Adding Delays to AutoTune Alarms
  boolean ATSensorErrorDly = booleanDelay(ATSensorError,1000, ATSensorErrorState); //boolean alarm, unsigned long timedelay in ms,unsigned long saved state since true
  boolean ATAFRLeftErrorDly = booleanDelay(ATAFRLeftError,1000, ATAFRLeftErrorState); //boolean alarm, unsigned long timedelay in ms,unsigned long saved state since true
  boolean ATAFRRightErrorDly = booleanDelay(ATAFRRightError,1000, ATAFRRightErrorState); //boolean alarm, unsigned long timedelay in ms,unsigned long saved state since true
/////
  //Sensor Failures
  if(ATSensorErrorDly || ATAFRLeftErrorDly || ATAFRRightErrorDly)
  {
    if(ATSensorError)
    {
      ATSensorErrorString = "ATSens,";
    }
    if(ATAFRLeftError)
    {
      ATAFRLeftErrorString = "ATAFRLeft,";
    }
    if(ATAFRRightError)
    {
      ATAFRRightErrorString = "ATAFRRight,";
    }
    sensorFailure = ATSensorErrorString + ATAFRLeftErrorString + ATAFRRightErrorString;
  }
  
  boolean alarmActive = AFRLLean || AFRRLean || ATSensorErrorDly || ATAFRLeftErrorDly || ATAFRRightErrorDly;
  if(alarmActive)
  {
    alarmLatched = true;
    ackKnowledgeAlarm = false;
  }
  if(!alarmActive && ackKnowledgeAlarm)
  {
    alarmLatched = false;
    ackKnowledgeAlarm = false;
  }

  if(alarmLatched)
  {
   analogWrite(LEDPinError, 100); 
  }
  else
  {
   analogWrite(LEDPinError, 0);  
  }
  
}
////////////////////////////////////Lean ////////////////////////////
void checkForLeanCondition()
{
  if(autoInjFire)
  {
              ///Check left side for lean conditions with time delay
              if(AFRLFiltered >= (ATAFRTarget+1))///If AFR is greater than the Autotune AFR Target plus a margin
              {
                  if(((millis()-AFRLeanLeftStartTime) >= AFRLeanAlarmThresholdTD)&& firstCountLThisEvent)
                  {
                    AFRLLeanCount = AFRLLeanCount +1;
                    firstCountLThisEvent = false;
                    AFRLLean = true;
                  }    
               }
              else
               {
                    AFRLeanLeftStartTime = millis();
                    firstCountLThisEvent = true;
                    AFRLLean = false;
               }
                  ///Check Right side for lean conditions with time delay
              if(AFRRFiltered >= (ATAFRTarget+1)) ///If AFR is greater than the Autotune AFR Target plus a margin
              {
                  if(((millis()-AFRLeanRightStartTime) >= AFRLeanAlarmThresholdTD)&& firstCountRThisEvent)
                  {
                    AFRRLeanCount = AFRRLeanCount +1;
                    firstCountRThisEvent = false;
                    AFRRLean = true;
                  }    
               }
              else
               {
                    AFRLeanRightStartTime = millis();
                    firstCountRThisEvent = true;
                    AFRRLean = false;
               }
     
  }
  else
  {
    AFRLeanLeftStartTime = millis();
    AFRLeanRightStartTime = millis();
    firstCountLThisEvent = true;
    firstCountRThisEvent = true;
    AFRLLean = false;
    AFRRLean = false;
  }

}

//////////////////////////Knock////////////////////////////////
void knockCounter()
{
  if((KnockVolts < knockThreshold) && (millis()>knockCountStartTime) && knockReset)
  {
    knockCount = knockCount + 1;
    knockReset = false;
  }
  else if(KnockVolts >= (knockThreshold + 0.25))
  {
    knockReset = true;
  }
}
///////////////////////Sensor Failure////////////////////////
///to use
//float AFRLFiltered
//float AFRRFiltered
//float BoostFiltered;
//float MAFFiltered;
//float ThrottleFiltered;
////////////////////////////////////////////////////////////

///boolean delay routine
boolean booleanDelay(boolean conditionTrue,unsigned long delayTime, unsigned long& timeDelayed) //boolean alarm, unsigned long timedelay in ms,unsigned long saved state since true
{
  if(conditionTrue)
  {
    if((millis()-timeDelayed)>= delayTime)
    {
      return true;
    }
  }
  else
  {
    timeDelayed = millis();
    return false;
  }
  
}
