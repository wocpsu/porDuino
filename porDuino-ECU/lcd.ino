void printAFRS()
{
#if LCDScreen
    //lcd.setTextFont(4);
    //lcd.setCursor(10, 50);
    //lcd.print(AFRLFiltered, 1);
    lcd.setTextColor(TFT_WHITE,TFT_BLACK);
    //First Line
    lcd.drawFloat(AFRLFiltered,1,0,40,4); //Val,Percision,X,Y,size
    lcd.drawFloat(AFRRFiltered,1,100,40,4);
    if(BoostFiltered > 5.0)
    {
    lcd.setTextColor(TFT_RED,TFT_BLACK);
    lcd.drawFloat(BoostFiltered,1,200,40,4);
    lcd.setTextColor(TFT_WHITE,TFT_BLACK);
    }
    else
    {
    lcd.drawFloat(BoostFiltered,1,200,40,4); 
    }
    //Second Line
    lcd.drawFloat(MAFFiltered,0,      0,120,4); //Val,Percision,X,Y,size
    lcd.drawNumber(knockCount,       150,120,4);
    lcd.drawFloat(ThrottleFiltered,1,275,120,4);    
    //Third Line    
    if(injectorPulseWidth > 0.0)
    {
    lcd.setTextColor(TFT_RED,TFT_BLACK);
    lcd.drawFloat(injectorPulseWidth,0,      0,200,4); //Val,Percision,X,Y,size
    lcd.setTextColor(TFT_WHITE,TFT_BLACK);
    }
    else
    {
    lcd.drawFloat(injectorPulseWidth,2,      0,200,4); //Val,Percision,X,Y,size
    }
    lcd.drawFloat(RPMFiltered,0,           150,200,4); //Val,Percision,X,Y,size
    if(injectorArmed)
    {
    lcd.setTextColor(TFT_RED,TFT_BLACK);
    lcd.drawString("ARMED            ",        275,200,4);
    lcd.setTextColor(LTGREY,TFT_BLACK);
    }
    else
    {
    lcd.drawString("DISARMED",        275,200,4);
    }
#endif   
}
void initMainScreen()
{
#if LCDScreen
  lcd.setTextDatum(TL_DATUM);
  lcd.fillScreen(TFT_BLACK);
  lcd.setTextColor(ORANGE,TFT_BLACK);
  //First Line
  lcd.drawString("AFR L",        0, 7, 4);
  lcd.drawString("AFR R",      100, 7, 4);
  lcd.drawString("Boost(PSI)", 200, 7, 4);
  //Divider Line
  lcd.drawFastHLine(0, 70,480, LTGREY);
  lcd.drawFastHLine(0, 72,480, LTGREY);
  //Second Line  
  lcd.drawString("MAF(g/s)",        0,   80, 4);
  lcd.drawString("Knock(cnt)",      125, 80, 4);
  lcd.drawString("Throttle(%)",        275, 80, 4);
  //Divider Line
  lcd.drawFastHLine(0, 150,480, LTGREY);
  lcd.drawFastHLine(0, 152,480, LTGREY);
  //Third Line  
  lcd.drawString("INJPW(%)",        0,   160, 4);
  lcd.drawString("RPM",      135, 160, 4);
  lcd.drawString("Injector Status",        270, 160, 4);  
  //Divider Line
  lcd.drawFastHLine(0, 230,480, LTGREY);
  lcd.drawFastHLine(0, 232,480, LTGREY);
  ///Button Text
  lcd.setTextColor(LTGREY,TFT_BLACK);
  lcd.drawString("<--Fire Injectors",      5, 300, 2);
  lcd.drawString("Start/Stop Log",      155, 300, 2);
  ///Logging Text
  if(logging)
  {
  lcd.setTextColor(TFT_GREEN,TFT_BLACK);
  lcd.drawString("LOGGING             ", 275, 250, 4); 
  lcd.setTextColor(LTGREY,TFT_BLACK);
  }
  else
  {
  lcd.setTextColor(TFT_MAROON,TFT_BLACK);
  lcd.drawString("NOT LOGGING", 275, 250, 4); 
  lcd.setTextColor(LTGREY,TFT_BLACK);
  }
///print current file name
    lcd.setTextColor(LTGREY,TFT_BLACK);
    lcd.setTextFont(4);
    lcd.setCursor(275, 275);
    lcd.print(fileName + "   ");
#endif
}
