#include "angle.h"
#include "global.h"


extern struct angle Angle;
extern struct Serwom serwom;



void correctRawAngle()
{
  Angle.angleRaw = Angle.angleRaw - 109;
  if (Angle.angleRaw < 0)
  {
    Angle.angleRaw = Angle.angleRaw + 4096;
  }
}

void CorrectAngle()
{
  Angle.angleAfterCorrectionDeg = Angle.angleRawDeg + Angle.angleCorrectionDeg;
  Angle.angleAfterCorrection = Angle.angleRaw + Angle.angleCorrection;

  if (Angle.angleAfterCorrection > 4096 || 
      Angle.angleAfterCorrection < 0)
  {
    if (Angle.angleAfterCorrection > 4096)
    {
      Angle.angleAfterCorrection = Angle.angleAfterCorrection - 4096;
      Angle.angleAfterCorrectionDeg = Angle.angleAfterCorrectionDeg - 360;
    }
    else
    {
      Angle.angleAfterCorrection = Angle.angleAfterCorrection + 4096;
      Angle.angleAfterCorrectionDeg = Angle.angleAfterCorrectionDeg + 360;
    }
  }
}




void calculateAngleCorrection(float angleToSet, float angleToSet2)
{
  //Serial.print("zadano kat: ");
  //Serial.println(angleToSet);
  serwom.serwoCorrection = serwom.serwoRaw - angleToSet2;
  Angle.angleCorrection = angleToSet - Angle.angleRaw;
  Angle.angleCorrectionDeg = Angle.angleCorrection * 0.087;
  CorrectAngle();
  serwom.serwoGiven = serwom.serwoGiven + serwom.serwoCorrection;

}







