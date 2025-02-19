void ATautoTune()
{
///Check for errors on sensors
  ATSensorError =  (BoostFiltered > ATMaxActiveBoost) || (RPMFiltered < ATMinActiveRPM);
  ATAFRLeftError = (AFRLFiltered > ATMaxActiveAFR) || (AFRLFiltered < ATMinActiveAFR);
  ATAFRRightError = (AFRRFiltered > ATMaxActiveAFR) || (AFRRFiltered < ATMinActiveAFR);
  ///Tunes the piggyback injectors when the injectors are firing based off AFR readings
  int ATCurrentRPMBand = min((int)max((RPMFiltered-ATMinRPM)/ATRPMSteps,0),ATNumRPMBands-1);
  int ATCurrentBoostBand = min((int)max((BoostFiltered-ATMinBoost)/ATBoostSteps,0),ATNumBoostBands-1);
  int ATCurrentWriteAddressL = (ATCurrentBoostBand*ATNumRPMBands)+ATCurrentRPMBand;//Write Address Calculation Left Size of engine
  int ATCurrentWriteAddressR = (ATCurrentBoostBand*ATNumRPMBands)+ATCurrentRPMBand + ATNumRPMBands*ATNumBoostBands;//Write Address Calculation Right Side of engine
  //int ATCurrentWriteAddressEEPROM = (ATCurrentBoostBand*ATNumRPMBands)+ATCurrentRPMBand;///Write Address X 4 for EEPROM due to float size of 4 bytes
  ///Get Current Fuel Adjustment at this Address
  //currentFuelAdjustment = EEPROM.get(ATCurrentWriteAddressEEPROM,currentFuelAdjustment);
   float currentFuelAdjustmentL = ATMapRAM[ATCurrentWriteAddressL];
   float currentFuelAdjustmentR = ATMapRAM[ATCurrentWriteAddressR];   
  //Check AFRS against target and calulate new Fueling
   float ATErrorL = AFRLFiltered - ATAFRTarget;
   float ATErrorR = AFRRFiltered - ATAFRTarget;
   float ATFuelAdjustmentL = ATErrorL*ATGain;
   float ATFuelAdjustmentR = ATErrorR*ATGain;
   ///Calc new fuel adjusment if sensors are good, if not use last RAM value
     if(ATSensorError || ATAFRLeftError)
     {
      ATnewMapValueL = currentFuelAdjustmentL;
     }
     else
     {
      ATnewMapValueL = constrain(currentFuelAdjustmentL + ATFuelAdjustmentL,(1-ATMaxAdjustPercent),(1+ATMaxAdjustPercent));
     }
     if(ATSensorError || ATAFRRightError)
     {
      ATnewMapValueR = currentFuelAdjustmentR;
     }
     else
     {
      ATnewMapValueR = constrain(currentFuelAdjustmentR + ATFuelAdjustmentR,(1-ATMaxAdjustPercent),(1+ATMaxAdjustPercent));
     }
   ///Write updated fuel adjustment to RAM
   ATMapRAM[ATCurrentWriteAddressL] = ATnewMapValueL;
   ATMapRAM[ATCurrentWriteAddressR] = ATnewMapValueR;
}

void ATwriteToEEPROM()
{
  if((millis()-ATLastWrite) > ATwriteFreq)
  {
      ///loop over RAM array and write it to EEPROM
      for(int i=0;i<ATSizeOfArray;i++)
      {
        if(autoInjFire||manualInjFire||injectorTestFireInjectors||logging)///Break EEPROM Write loop if injectors are firing or data is being logged
        {
          break;
        }
       float ATThisRAMVal = ATMapRAM[i];
       EEPROM.put(i*4, ATThisRAMVal);
      }
    ATLastWrite = millis();
  }
}
void ATSetRAMFromEEPROM() //loop to set Map array from EEPROM during Arduino initialization
{
       for(int i=0;i<ATSizeOfArray;i++)
      {
       EEPROM.get(i*4, ATMapRAM[i]);
      }
}

void ATResetEEPROM() ///Reset EEPROM Values to Defaults
{
      for(int i=0;i<ATSizeOfArray;i++)
      {
       float ATSetVal = 1;
       if(i >= (ATNumRPMBands*ATNumBoostBands)) //if on right side map
       {
        ATSetVal = 1.0;
       }
       else //Left Side of map
       {
        ATSetVal = 1.0;
       }
       ATMapRAM[i]=ATSetVal;
       EEPROM.put(i*4, ATSetVal);
      }
}
void ATPrintRAMMap()
{
      String requestedLog = "ATMAP.txt";
      Serial3.write("singleLog::");
      Serial3.print(requestedLog);
      Serial3.write("::");
      ///Print Left Map
      for(int r=0;r<ATNumRPMBands;r++)
      {
        for(int c=0;c<ATNumBoostBands;c++)
        {
         int currentArrayLocation =  (c*ATNumRPMBands)+r;
         float ATThisRAMVal = ATMapRAM[currentArrayLocation];
         Serial3.print(ATThisRAMVal);
         Serial3.print(",");
        }
         Serial3.println("");
//         delay(100); ///Needed to add delays to avoid overflowing BT Buffer
      }
      ///Print Right Map
      for(int r=0;r<ATNumRPMBands;r++)
      {
        for(int c=0;c<ATNumBoostBands;c++)
        {
         int currentArrayLocation =  (c*ATNumRPMBands)+r + ATNumRPMBands*ATNumBoostBands;
         float ATThisRAMVal = ATMapRAM[currentArrayLocation];
         Serial3.print(ATThisRAMVal);
         Serial3.print(",");
        }
         Serial3.println("");
//         delay(100); ///Needed to add delays to avoid overflowing BT Buffer
      }
      Serial3.write("::??");
}
