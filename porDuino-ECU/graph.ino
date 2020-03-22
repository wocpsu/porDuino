#include "Arduino.h"
#include "graph.h"
#if LCDScreen
void Trace(TFT_HX8357 &tft, double x,  double y,  byte dp,
           double gx, double gy,
           double w, double h,
           double xlo, double xhi, double xinc,
           double ylo, double yhi, double yinc,
           char *title, char *xlabel, char *ylabel,
           boolean &update1, unsigned int color, int xtitlestart, int yScaleOffset, boolean &redraw)
{
  double ydiv, xdiv;
  double i;
  double temp;
  int rot, newrot;

  //unsigned int gcolor = DKBLUE;   // gcolor = graph grid color
  unsigned int acolor = RED;        // acolor = main axes and label color
  unsigned int pcolor = color;      // pcolor = color of your plotted data
  unsigned int tcolor = WHITE;      // tcolor = text color
  unsigned int bcolor = BLACK;      // bcolor = background color

  // initialize old x and old y in order to draw the first point of the graph
  // but save the transformed value
  // note my transform funcition is the same as the map function, except the map uses long and we need doubles
  if (update1) {
    update1 = false;
    
    ox = (x - xlo) * ( w) / (xhi - xlo) + gx;
    oy = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;

    if ((ox < gx) || (ox > gx+w)) {update1 = true; return;}
    if ((oy < gy-h) || (oy > gy)) {update1 = true; return;}
    
    if (redraw) {
      redraw = false;
          tft.setTextDatum(MR_DATUM);

    // draw y scale
    for ( i = ylo; i <= yhi; i += yinc) {
      // compute the transform
      temp =  (i - ylo) * (gy - h - gy) / (yhi - ylo) + gy;

      if (i == 0) {
        tft.setTextColor(acolor, bcolor);
        tft.drawString(xlabel, (int)(gx + w) , (int)temp, 2);
      }
      // draw the axis labels
      tft.setTextColor(tcolor, bcolor);
      // precision is default Arduino--this could really use some format control
      tft.drawFloat(i, dp, gx - 4 - yScaleOffset , temp , 1);
    }

    // draw x scale
    for (i = xlo; i <= xhi; i += xinc) {

      // compute the transform
      temp =  (i - xlo) * ( w) / (xhi - xlo) + gx;
      if (i == 0) {
        tft.setTextColor(acolor, bcolor);
        tft.setTextDatum(BC_DATUM);
        tft.drawString(ylabel, (int)temp - yScaleOffset, (int)(gy - h - 8) , 2);
      }

      // draw the axis labels
      tft.setTextColor(tcolor, bcolor);
      tft.setTextDatum(TC_DATUM);
      // precision is default Arduino--this could really use some format control
      tft.drawFloat(i, dp, temp, gy + 7, 1);
    }

    //now draw the graph labels
    tft.setTextColor(color, bcolor);
    tft.drawString(title, (xtitlestart) , (int)(gy - h - 30), 4);
    }

  }

  // the coordinates are now drawn, plot the data
  // the entire plotting code are these few lines...
  // recall that ox and oy are initialized above
  x =  (x - xlo) * ( w) / (xhi - xlo) + gx;
  y =  (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;

  if ((x < gx) || (x > gx+w)) {update1 = true; return;}
  if ((y < gy-h) || (y > gy)) {update1 = true; return;}
    
    
  tft.drawLine(ox, oy, x, y, pcolor);
  // it's up to you but drawing 2 more lines to give the graph some thickness
  //tft.drawLine(ox, oy + 1, x, y + 1, pcolor);
  //tft.drawLine(ox, oy - 1, x, y - 1, pcolor);
  ox = x;
  oy = y;

}

void GraphSetup(TFT_HX8357 &tft, double x, double y, byte dp,
                           double gx, double gy, double w, double h,
                           double xlo, double xhi, double xinc,
                           double ylo, double yhi, double yinc,
                           char *title, char *xlabel, char *ylabel,
                           boolean &redraw, unsigned int color) {

  double ydiv, xdiv;
  double i;
  double temp;
  int rot, newrot;

  // gcolor = graph grid colors
  // acolor = axes line colors
  // pcolor = color of your plotted data
  // tcolor = text color
  // bcolor = background color
  unsigned int gcolor = DKBLUE;
  unsigned int acolor = RED;
  unsigned int pcolor = color;
  unsigned int tcolor = WHITE;
  unsigned int bcolor = BLACK;

  if (redraw == true) {

    redraw = false;
    // initialize old x and old y in order to draw the first point of the graph
    // but save the transformed value
    // note my transform funcition is the same as the map function, except the map uses long and we need doubles
    //ox = (x - xlo) * ( w) / (xhi - xlo) + gx;
    //oy = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;

    tft.setTextDatum(MR_DATUM);

    // draw y scale
    for ( i = ylo; i <= yhi; i += yinc) {
      // compute the transform
      temp =  (i - ylo) * (gy - h - gy) / (yhi - ylo) + gy;

      if (i == 0) {
        tft.drawLine(gx, temp, gx + w, temp, acolor);
        tft.setTextColor(acolor, bcolor);
        tft.drawString(xlabel, (int)(gx + w) , (int)temp, 2);
      }
      else {
        tft.drawLine(gx, temp, gx + w, temp, gcolor);
      }
      // draw the axis labels
      tft.setTextColor(tcolor, bcolor);
      // precision is default Arduino--this could really use some format control
      tft.drawFloat(i, dp, gx - 4, temp, 1);
    }

    // draw x scale
    for (i = xlo; i <= xhi; i += xinc) {

      // compute the transform
      temp =  (i - xlo) * ( w) / (xhi - xlo) + gx;
      if (i == 0) {
        tft.drawLine(temp, gy, temp, gy - h, acolor);
        tft.setTextColor(acolor, bcolor);
        tft.setTextDatum(BC_DATUM);
        tft.drawString(ylabel, (int)temp, (int)(gy - h - 8) , 2);
      }
      else {
        tft.drawLine(temp, gy, temp, gy - h, gcolor);
      }
      // draw the axis labels
      tft.setTextColor(tcolor, bcolor);
      tft.setTextDatum(TC_DATUM);
      // precision is default Arduino--this could really use some format control
      tft.drawFloat(i, dp, temp, gy + 7, 1);
    }

    //now draw the graph labels
    tft.setTextColor(tcolor, bcolor);
    tft.drawString(title, (int)(gx + w / 2) , (int)(gy - h - 30), 4);
  }

  // the coordinates are now drawn, plot the data
  // the entire plotting code are these few lines...
  // recall that ox and oy are initialized above
  //x =  (x - xlo) * ( w) / (xhi - xlo) + gx;
  //y =  (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
  //tft.drawLine(ox, oy, x, y, pcolor);
  // it's up to you but drawing 2 more lines to give the graph some thickness
  //tft.drawLine(ox, oy + 1, x, y + 1, pcolor);
  //tft.drawLine(ox, oy - 1, x, y - 1, pcolor);
  //ox = x;
  //oy = y;

}
void plotData2(float y1Value,float y2Value, unsigned int color1, unsigned int color2 )
{
 if(reInitGraph)
  {
    display1 = true;
    update1 = true;
    redraw1 = true;
    redraw2 = true;
    lcd.fillScreen(BLACK);
    GraphSetup(lcd, timeAt, y1Value, 1, 60, 290, 390, 260, 0, maxPlotTime, 2.0, plotYMin, plotYMax, 1, "", "", "", display1, YELLOW);
    reInitGraph = false;
    offsetTime = millis();
  }
  if((millis()-lastTimePlotCalled) >= plotCallIter)
  {
     if(timeAt >= maxPlotTime) 
      {
        reInitGraph = true;
        offsetTime = millis();
        timeAt = float(millis()-offsetTime)/1000;
      }
      else
      { 
        timeAt = float(millis()-offsetTime)/1000;        
        update1 = true;
        Trace(lcd, timeAt, y1Value, 1, 60, 290, 390, 260, 0, maxPlotTime, 2.0, plotYMin, plotYMax, 1, "AFRLEFT", "", "AFR", update1, color1, 150 , 0, redraw1);
        update1 = true;
        Trace(lcd, timeAt, y2Value, 1, 60, 290, 390, 260, 0, maxPlotTime, 2.0, plotYMin, plotYMax, 1, "AFRRIGHT", "", "AFR", update1, color2, 350 , 0 ,redraw2);
      }
      lastTimePlotCalled = millis();
  } 
}	
void plotData3(float y1Value,float y2Value,float y3Value, unsigned int color1, unsigned int color2, unsigned int color3, char *title1, char *title2, char *title3,char *YLabel1 , char *YLabel2,  char *YLabel3 )
{
 if(reInitGraph)
  {
    display1 = true;
    update1 = true;   
    redraw1 = true;  
    redraw2 = true;  
    redraw3 = true;
    lcd.fillScreen(BLACK);
    GraphSetup(lcd, timeAt, y1Value, 1, 60, 290, 390, 260, 0, maxPlotTime, 2.0, plotYMin, plotYMax, 1, "", "", "", display1, YELLOW);
    reInitGraph = false;
    offsetTime = millis();
  }
  if((millis()-lastTimePlotCalled) >= plotCallIter)
  {
     if(timeAt >= maxPlotTime) 
      {
        reInitGraph = true;
        offsetTime = millis();
        timeAt = float(millis()-offsetTime)/1000;
      }
      else
      { 
        timeAt = float(millis()-offsetTime)/1000;
        update1 = true;       
        Trace(lcd, timeAt, y1Value, 0, 60, 290, 390, 260, 0, maxPlotTime, 2.0, plotYMin, plotYMax, 1, title1, "", YLabel1, update1, color1, 150, 0 , redraw1);
        update1 = true;
        Trace(lcd, timeAt, y2Value, 0, 60, 290, 390, 260, 0, maxPlotTime, 2.0, plotYMin, plotYMax, 1, title2, "", YLabel2, update1, color2, 280, 0, redraw2);
        update1 = true;
        Trace(lcd, timeAt, y3Value, 0, 60, 290, 390, 260, 0, maxPlotTime, 2.0, 0, 10, 1, title3, "", YLabel3, update1, color3, 400, 40,redraw3);
      }
      lastTimePlotCalled = millis();
  } 
}               
#endif					   
						   
