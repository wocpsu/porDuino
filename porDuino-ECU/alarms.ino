////////////////////////////////////Alarms///////////////////////
void alarmHandler ()
{
  //Sensor Failures
  if(ATSensorError || ATAFRLeftError || ATAFRRightError)
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
  
  boolean alarmActive = AFRLLean || AFRRLean || ATSensorError || ATAFRLeftError || ATAFRRightError;
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
              if(AFRLFiltered >= AFRLeanAlarmThreshold)
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
              if(AFRRFiltered >= AFRLeanAlarmThreshold)
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
