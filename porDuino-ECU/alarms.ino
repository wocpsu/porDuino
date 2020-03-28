////////////////////////////////////Alarms///////////////////////
void alarmHandler ()
{
///Adding Delays to AutoTune Alarms
  boolean ATSensorErrorDly = booleanDelay(ATSensorError,1000, ATSensorErrorState); //boolean alarm, unsigned long timedelay in ms,unsigned long saved state since true
  boolean ATAFRLeftErrorDly = booleanDelay(ATAFRLeftError,1000, ATAFRLeftErrorState); //boolean alarm, unsigned long timedelay in ms,unsigned long saved state since true
  boolean ATAFRRightErrorDly = booleanDelay(ATAFRRightError,1000, ATAFRRightErrorState); //boolean alarm, unsigned long timedelay in ms,unsigned long saved state since true
/////
  //Sensor Failures
  if(ATSensorErrorDly || ATAFRLeftErrorDly || ATAFRRightErrorDly || MAFFailAlarm || throttleFailAlarm || RPMFailAlarm || boostFailAlarm)
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
    if(MAFFailAlarm)
    {
      MAFFailAlarmString = "MAFFAIL,";
    }
    if(throttleFailAlarm)
    {
      throttleFailAlarmString = "THROTFAIL,";
    }
    if(RPMFailAlarm)
    {
      RPMFailAlarmString = "RPMFAIL,";
    }
    if(boostFailAlarm)
    {
      boostFailAlarmString = "BOOSTFAIL,";
    }
    sensorFailure = ATSensorErrorString + ATAFRLeftErrorString + ATAFRRightErrorString + MAFFailAlarmString + throttleFailAlarmString + RPMFailAlarmString + boostFailAlarmString;
  }
  if (ackKnowledgeAlarm)
  {
    sensorFailure = "Sensors Ok";
    ATSensorErrorString = "";
    ATAFRLeftErrorString = "";
    ATAFRRightErrorString = "";
    MAFFailAlarmString = "";
    throttleFailAlarmString = "";
    RPMFailAlarmString = "";
    boostFailAlarmString = "";
  }
  
  boolean alarmActive = AFRLLean || AFRRLean || ATSensorErrorDly || ATAFRLeftErrorDly || ATAFRRightErrorDly || MAFFailAlarm || throttleFailAlarm || RPMFailAlarm || boostFailAlarm;
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
void knockCounter()///Knock voltage will go to ground when a knock is detected
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
////////////////////////Sensor Alarms/////////////////////////
void sensorAlarms() //This routine will detect any failures with the sensors used for the piggyback ecu, if the engine isn't running after 20 secs expect to see a lot of alarms
///It may be a good exercise to add a pull down resistor to MAF, AFR, Boost and throttle so they aren't floating in an open circuit even
{
  //float ThrottleVolts
  
  if(firstCallSensorAlarms) ///Set alarm states to current time on first pass to avoid accidential instantaneous trigger of an alarm
  {
    RPMFailAlarmState = millis();
    throttleFailAlarmState = millis();
    MAFFailAlarmState = millis();
    boostFailAlarmState = millis();
    firstCallSensorAlarms = false;
  }
  
  ///RPM
  boolean RPMLow = (RPMFiltered < 500.0) || (RPMFiltered < 1500.0 && ThrottleFiltered > 30.0);
  RPMFailAlarm = booleanDelay(RPMLow,5000, RPMFailAlarmState); //boolean alarm, unsigned long timedelay in ms,unsigned long saved state since true
  ///Throttle
  boolean throttleVoltsBad = (ThrottleVolts < 0.3) || (ThrottleVolts > 4.5);
  throttleFailAlarm = booleanDelay(throttleVoltsBad,5000, throttleFailAlarmState); //boolean alarm, unsigned long timedelay in ms,unsigned long saved state since true
  ///MAF
  boolean MAFLow = (RPMFiltered > 2000) && (MAFFiltered < 10.0);
  MAFFailAlarm = booleanDelay(MAFLow,5000, MAFFailAlarmState); //boolean alarm, unsigned long timedelay in ms,unsigned long saved state since true
  ///Boost
  boolean boostFail = (ThrottleFiltered < 5.0) && (BoostFiltered > -2.0); ///If we have very little vaccum at idle
  boostFailAlarm = booleanDelay(boostFail,5000, boostFailAlarmState); //boolean alarm, unsigned long timedelay in ms,unsigned long saved state since true
  
  }



///boolean delay routine
boolean booleanDelay(boolean conditionTrue,unsigned long delayTime, unsigned long& timeDelayed) //boolean alarm, unsigned long timedelay in ms,unsigned long saved state since true
///I had to build this routine since arduino didn't have one. It uses a pointer to the passed in delay state (timeDelayed) in order to make the routine reusable
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
