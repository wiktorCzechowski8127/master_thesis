#include "Arduino.h"
#include "motor.h"
#include "global.h"
#include <Servo.h>

boolean moveState = LOW;
extern struct angle Angle;
extern struct Serwom serwom;
extern struct Servo serwoClass;
extern struct Global global;
void checkmaxdeg()
{
  if(Angle.maxdir == 3 && Angle.angleRaw < 512) // -------------------------------> -3
  {
    Angle.maxdir = 2;
  }
  else if(Angle.maxdir == 2) // -------------------------------> -2
  {
    if(Angle.angleRaw > 512 && Angle.angleRaw < 3584)
    {
      Angle.maxdir = 3;
    }
    if(Angle.angleRaw > 3584 && Angle.angleRaw < 4096)
    {
      Angle.maxdir = 1;
    }
  }
  else if(Angle.maxdir == 1) // -------------------------------> 1
  {
    if(Angle.angleRaw < 3584)
    {
      Angle.maxdir = 0;
    }
    if(Angle.angleRaw < 512 )
    {
      Angle.maxdir = 2;
    }
  }
  else if(Angle.maxdir == 0) // ------------------------------> 0
  {
    if (Angle.angleRaw < 512)
    {
      Angle.maxdir = -1;
    }
    if (Angle.angleRaw > 3584)
    {
      Angle.maxdir = 1;
    }
  }
  else if(Angle.maxdir == -1) // -------------------------------> -1
  {
    if(Angle.angleRaw > 512)
    {
      Angle.maxdir = 0;
    }
    if(Angle.angleRaw > 3584)
    {
      Angle.maxdir = -2;
    }
  }
  else if(Angle.maxdir == -2) // -------------------------------> -2
  {
    if(Angle.angleRaw > 0 && Angle.angleRaw < 512)
    {
      Angle.maxdir = -1;
    }
    if(Angle.angleRaw < 3584 && Angle.angleRaw > 512)
    {
      Angle.maxdir = -3;
    }
  }
  if(Angle.maxdir == -3 && Angle.angleRaw > 3584) // -------------------------------> -3
  {
    Angle.maxdir = -2;
  }
}

void LowDir()
{
  if (Angle.maxdir != -3)
  {
    digitalWrite(11, LOW);
    digitalWrite(7, LOW);
    makeMove();
  }
}

void HighDir()
{
  if (Angle.maxdir != 3)
  {
    digitalWrite(11, HIGH);
    digitalWrite(7, HIGH);
    makeMove();
  }
}

void makeMove()
{
  if (global.timer[2] + 0 < millis())
  {
    moveState = !moveState;
  
    digitalWrite(12, moveState);
    global.timer[2] = millis();
  }
}


void validateMoveDirection()
{
  if (Angle.angleGiven == 0)
  {
    if (Angle.angleAfterCorrection < 5 || Angle.angleAfterCorrection > 4091) 
    {
      digitalWrite(3, HIGH);
      //Angle.angleAfterCorrectionDeg = 0;
    } 
    else
    {
      digitalWrite(3, HIGH);
      LowDir();
    } 
  }
  else if (Angle.angleGiven == 360)
  {
    if (Angle.angleAfterCorrection < 5 || Angle.angleAfterCorrection > 4091) 
    {
      digitalWrite(3, HIGH);
      //Angle.angleAfterCorrectionDeg = 360;
    } 
    else
    {
      digitalWrite(3, HIGH);
      HighDir();
    } 
  }
  else
  {
    if (Angle.angleAfterCorrection < Angle.angleGiven + 5 && Angle.angleAfterCorrection > Angle.angleGiven - 5) {
      digitalWrite(3, HIGH);
    } 
    else if (Angle.angleAfterCorrection > Angle.angleGiven)
    {
      digitalWrite(3, HIGH);
      LowDir();
    } 
    else if (Angle.angleAfterCorrection < Angle.angleGiven)
    {
      digitalWrite(3, HIGH);
      HighDir();
    }
  }
  checkmaxdeg();
}

void makeSerwoMove()
{
  if (global.timer[1]+50 < millis())
  {
    if (serwom.serwoRaw < serwom.serwoGiven && serwom.serwoRaw < 120)
    {
      serwom.serwoRaw++;
    }
    else if (serwom.serwoRaw > serwom.serwoGiven && serwom.serwoRaw > 20)
    {
      serwom.serwoRaw--;
    }
    serwoClass.write(serwom.serwoRaw);
    global.timer[1] = millis();
  } 
}




