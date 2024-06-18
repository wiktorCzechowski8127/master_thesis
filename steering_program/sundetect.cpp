#include "sundetect.h"
#include "global.h"
#include "motor.h"
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <AS5600.h>


#define PRDELAY

extern struct angle Angle;
extern struct Serwom serwom;
extern struct Servo serwoClass;
extern struct Global global;

extern LiquidCrystal_I2C lcd;
extern AMS_5600 ams5600;




void readPrValue(){
  int downright = analogRead(A0);//dwon-right
  int downleft = analogRead(A1);//dwon-left
  int upleft = analogRead(A3);
  int upright = analogRead(A2);

  int prUp =  (upleft + upright) / 2;
  int prDown = (downright + downleft) / 2;;
  int prLeft = (upleft + downleft) / 2;;
  int prRight = (upright + downright) / 2;;

  global.ycenter= prLeft - prRight;
  global.xcenter= prUp - prDown;

}

void printLedStatus(unsigned long *lcdTimer, boolean *isSetFinishTimerPasibleX, boolean *isSetFinishTimerPasibleY)
{
  if(*lcdTimer + 150 < millis())
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PRc");
    lcd.setCursor(5, 0);
    lcd.print(Angle.angleGiven);
    lcd.setCursor(11, 0);
    lcd.print(serwom.serwoGiven);
    lcd.setCursor(0, 1);
    lcd.print("x: ");
    lcd.setCursor(3, 1);
    if( *isSetFinishTimerPasibleX == false)
    {
      lcd.print("OK ");
    }
    else
    {
      lcd.print(global.xcenter);
    }

    lcd.setCursor(7, 1);
    lcd.print("y: ");
    lcd.setCursor(10, 1);
    if( *isSetFinishTimerPasibleY == false)
    {
      lcd.print("OK ");
    }
    else
    {
      lcd.print(global.ycenter);
    }
    *lcdTimer = millis();
  }
}


void prSunDetect()
{
  readPrValue();



  Serial.print("R1");
  Serial.flush();
  boolean recievedData = false;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Waiting for data");
  delay(2000);
  int tmp = Angle.angleGiven;
  readSerialPort2();
  int recievedAngle = Angle.angleGiven;
  int recievedSerwo = serwom.serwoGiven;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(Angle.angleGiven);
  lcd.setCursor(0, 1);
  lcd.print(serwom.serwoGiven);

  Angle.angleGiven = tmp;
  serwom.serwoGiven = 20;
  delay(2000);



  unsigned long lcdTimer = millis();

  unsigned long xDelayTimer = millis();
  unsigned long xCenterFinishTimer = 0;
  int xDelay = 0;

  unsigned long yDelayTimer = millis();
  unsigned long yCenterFinishTimer = 0;
  int yDelay = 0;

  boolean isSetFinishTimerPasibleX = true;
  boolean isSetFinishTimerPasibleY = true;
  global.timer[4]= millis();

  while(1)
  {
    readPrValue();

    if( abs(global.xcenter) > 15 && xDelayTimer + xDelay < millis())
    {
      (global.xcenter< 20) ? xDelay = 200: xDelay = 200;
      if(global.xcenter> 0)
      {
        serwom.serwoGiven++;
        makeSerwoMove();
      }
      else
      {              
        serwom.serwoGiven--;
        makeSerwoMove();
      }
      xDelayTimer = millis();
      isSetFinishTimerPasibleX = true;
    }
    else if(xDelayTimer + xDelay < millis())
    {
      if (isSetFinishTimerPasibleX == true)
      {
        xCenterFinishTimer = millis();
        isSetFinishTimerPasibleX = false;
      }
    }

    //----------------------------------------------------------------------------------------------------------------
    if( abs(global.ycenter) > 10 && yDelayTimer + yDelay < millis())
    {
      Angle.angleRaw = ams5600.getRawAngle();
      correctRawAngle();
      CorrectAngle();
      (abs(global.ycenter) < 20) ? yDelay = 20: yDelay = 20;

      if(global.ycenter > 0)
      {
        if(Angle.angleGiven + 50 >= Angle.angleAfterCorrection && Angle.angleGiven - 50 <= Angle.angleAfterCorrection)
        {
          Angle.angleGiven--;
        }
      }
      else
      {
        if(Angle.angleGiven + 50 >= Angle.angleAfterCorrection && Angle.angleGiven - 50 <= Angle.angleAfterCorrection)
        {
          Angle.angleGiven++;
        }

      }
      validateMoveDirection();
      yDelayTimer = millis();
      isSetFinishTimerPasibleY = true;
    }
    else if(yDelayTimer + yDelay < millis())
    {
      if (isSetFinishTimerPasibleY == true)
      {
        yCenterFinishTimer = millis();
        isSetFinishTimerPasibleY = false;
      }
    }

    if (millis() > global.timer[4] + 250)
    {
      printLedStatus(&lcdTimer, &isSetFinishTimerPasibleY, &isSetFinishTimerPasibleX);
      global.timer[4] = millis();
    }


    if(isSetFinishTimerPasibleY == false && isSetFinishTimerPasibleX == false && yCenterFinishTimer + 3000 < millis() &&  xCenterFinishTimer + 3000 < millis())
    {
      lcd.clear();



      //Angle.angleGiven = recievedAngle;
      Angle.angleRaw = ams5600.getRawAngle();
      correctRawAngle();
      Angle.angleRawDeg = Angle.angleRaw * 0.087;
      Angle.angleRawAfterCorrection = Angle.angleRaw;
      CorrectAngle();
      calculateAngleCorrection(recievedAngle,recievedSerwo);
      Serial.print("R2");

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(Angle.angleCorrection);
      lcd.setCursor(0, 1);
      lcd.print(serwom.serwoCorrection);
      lcd.setCursor(4, 1);
      lcd.print(serwom.serwoGiven);
      lcd.setCursor(8, 1);
      lcd.print(serwom.serwoRaw);


      delay(3000);
      return;
    }

    


  }
};