void startLogging()
{
  #if SDCARD
   ///Loop until we find a file extension that hasn't been used
  for(int fileExtention = 0; fileExtention < 1000; fileExtention ++){
    fileName = "log" + String(fileExtention) + ".txt";
    if(!SD.exists(fileName)){
      break; ////Break because we found a file name that hasn't been used yet
    }
  }
    if(screenMain)
    {
    lcd.setTextFont(4);
    lcd.setCursor(275, 275);
    lcd.print(fileName + "   ");
    }
  //lcd.drawString((fileName + "   "), 10, 275, 2);
  myFile = SD.open(fileName, FILE_WRITE);
  myFile.println(logHeader);
  logStartMillis = millis();
  #endif
}
void deleteAllLogs()
{
#if SDCARD
  ///Stop Logging and Update screen and logging parameter to reflect that
  stopLogging();
  logging = false;
  //Update Screen
  if(screenMain)
  {
    lcd.setTextColor(TFT_MAROON,TFT_BLACK);
    lcd.drawString("NOT LOGGING", 275, 250, 4); 
    lcd.setTextColor(TFT_DARKGREY,TFT_BLACK);
    ///
    lcd.setTextFont(4);
    lcd.setCursor(275, 275);
    lcd.print("log0.txt       ");
  }
  ///Loop through all files and delete them
  for(int fileExtention = 0; fileExtention < 1000; fileExtention ++){
    fileName = "log" + String(fileExtention) + ".txt";
    if(!SD.exists(fileName)){
      break; ////Break because we found a file name that hasn't been used yet
    }
    else
    {
     SD.remove(fileName); 
    }
  }
#endif  
}
void printRequestedLog()
{
#if SDCARD
   ///Stop Logging and Update screen and logging parameter to reflect that
  stopLogging();
  logging = false;
  //Update Screen
  if(screenMain)
  {
  lcd.setTextColor(TFT_MAROON,TFT_BLACK);
  lcd.drawString("NOT LOGGING", 275, 250, 4); 
  lcd.setTextColor(TFT_WHITE,TFT_BLACK);
   }
  ///
  if(myBTInt == -1) ///Print list of log files available
  {
    ///Loop through all files and print the name to bluetooth
    Serial3.write("logNames::");
    for(int fileExtention = 0; fileExtention < 1000; fileExtention ++){
      fileName = "log" + String(fileExtention) + ".txt";
      if(!SD.exists(fileName)){
        break; ////Break because we found a file name that hasn't been used yet
      }
      else
      {
        Serial3.print(fileName);
        Serial3.write(",");
      }
    }
    Serial3.write("::empty for now");
    Serial3.write("::??");

  }
  else if (myBTInt > 0) //Print Requested Log
  {
          String requestedLog = "log" + String(myBTInt-1) + ".txt";
      File printFile = SD.open(requestedLog);
      if(printFile){
            Serial3.write("singleLog::");
            Serial3.print(requestedLog);
            Serial3.write("::");
           while (printFile.available()) {
             Serial3.write(printFile.read());
           }
           Serial3.write("::??");
         printFile.close();
      }
      else ///printFile Doesn't Exist
      {
         Serial3.println("Sorry Requested Log: " + requestedLog + " Doesn't Exist");///File Doesn't Exist
      }
  }
#endif
}
void stopLogging()
{
   #if SDCARD
   myFile.close();
   #endif
}
void logData()
{
  #if SDCARD
    myFile.print(float((millis()-logStartMillis))/1000,2);
    myFile.print(",");
    myFile.print(AFRLFiltered);
    myFile.print(",");
    myFile.print(AFRRFiltered);
    myFile.print(",");
    myFile.print(BoostFiltered);
    myFile.print(",");
    myFile.print(MAFFiltered);
    myFile.print(",");
    myFile.print(ThrottleFiltered);
    myFile.print(",");
    myFile.print(RPMFiltered);
    myFile.print(",");
    myFile.print(injectorPulseWidth);
    myFile.print(",");
    myFile.print(injectorPulseWidthLeft);
    myFile.print(",");
    myFile.print(injectorPulseWidthRight);
    myFile.print(",");
    myFile.println(knockCount);
  #endif
    
}
