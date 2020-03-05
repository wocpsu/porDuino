///////////////////////////////////////BT DATA CHECK//////////////////////////
void btDataCheck()
{
    #if btButton //getting values from BT for the time being
    if(Serial3.available())
    {
      float myBTFloat = Serial3.parseFloat();
      
      if (myBTFloat == 55.5)
      {
        btLogSelButtonPushed = true;
      }
      else if (myBTFloat == 123.3)
      {
        btLogDeleteButtonPushed = true;
      }
      else if (myBTFloat == 75.1)
      {
        int secondaryDiagSelect = Serial3.parseInt();
        if(secondaryDiagSelect == 1)
        {
        printDiagnosticsToBT();
        }
        if(secondaryDiagSelect == 2)
        {
        ackKnowledgeAlarm = true;
        }
      }
      else if (myBTFloat == 66.4)
      {
        int secondaryScreenSelect = Serial3.parseInt();
          if(secondaryScreenSelect == 1)
          {
          screenMain = true;
          initMainScreen();
          ////Other Screen Booleans to false
          screenPlotAFR = false;
          screenPlotAFRInj = false;
          screenPlotAFRBoost = false;
          }
          else if(secondaryScreenSelect == 2)
          {
          screenPlotAFR = true;
          reInitGraph = true;
          ////Other Screen Booleans to false          
          screenMain = false;
          screenPlotAFRInj = false;
          screenPlotAFRBoost = false;
          }          
          else if(secondaryScreenSelect == 3)
          {
          screenPlotAFRBoost = true;
          reInitGraph = true;
          ////Other Screen Booleans to false          
          screenMain = false;
          screenPlotAFR = false;
          screenPlotAFRInj = false;
          }          
          else if(secondaryScreenSelect == 4)
          {
          screenPlotAFRInj = true;
          reInitGraph = true;
          ////Other Screen Booleans to false          
          screenMain = false;
          screenPlotAFR = false;
          screenPlotAFRBoost = false;
          }
          else if(secondaryScreenSelect == 6)
          {
            printSettingsToBT();
          }          
          else if(secondaryScreenSelect == 7)
          {
            injectorArmed =!injectorArmed;
          }          
          else if(secondaryScreenSelect == 8)
          {
            getSettingsFromBT();
          }
          else if(secondaryScreenSelect == 9)
          {
            injectorTestEnabled =!injectorTestEnabled;
          }  
          else if(secondaryScreenSelect == 10)
          {
            ATPrintRAMMap();
          }
          else if(secondaryScreenSelect == 231)
          {
            ATResetEEPROM();
          }   

      }
      else if (myBTFloat == 234.1)
      {
        btLogPrintButtonPushed = true;
        myBTInt = Serial3.parseInt();
      }
      while(Serial3.available()) ///Clear buffer afterwards
      {
        Serial3.read();
      }
    }
    #endif  
}
////////////////////////////////////Settings///////////////////////////////////////////////////////////////
void printSettingsToBT()
{
    Serial3.write("settings::");
    Serial3.write("empty for now::");
    Serial3.print(injectorArmed);
    Serial3.print(",");
    Serial3.print(injectorStartPWMVal);
    Serial3.print(",");
    Serial3.print(injectorEndPWMVal);
    Serial3.print(",");
    Serial3.print(twoStepPWMVal);
    Serial3.print(",");
    Serial3.print(injectorTestEnabled);
    Serial3.print(",");
    Serial3.write("::??");
}

void getSettingsFromBT()
{
  
float setting1 = Serial3.parseFloat();
float setting2 = Serial3.parseFloat();
float setting3 = Serial3.parseFloat();
  if((setting1 >= 0.1) && (setting1 <= 30.0))
  {
    injectorStartPWMVal = setting1;
  }
  if((setting2 >= 10.0 ) && (setting2 <= 100.0))
  {
    injectorEndPWMVal = setting2;
  }
  if((setting3 >= 0.1) && (setting3 <= 100.0))
  {
    twoStepPWMVal = setting3;
  }
}

void printDiagnosticsToBT()
{
    Serial3.write("diag::");
    Serial3.write("empty for now::");
    Serial3.print(knockCount);
    Serial3.print(",");
    Serial3.print(AFRLLeanCount);
    Serial3.print(",");
    Serial3.print(AFRRLeanCount);
    Serial3.print(",");
    Serial3.print(maxBoost);
    Serial3.print(",");
    Serial3.print(maxInjPulseWidth);
    Serial3.print(",");
    Serial3.print(injStrtCheck);
    Serial3.print(",");
    Serial3.print(sensorFailure);
    Serial3.write("::??");  
}
