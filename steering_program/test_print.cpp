#include "angle.h"
#include "global.h"

extern struct angle Angle;
extern struct Serwom serwom;

void testPrintAngle()
{
  Serial.print("AngleRaw: ");
  Serial.print(Angle.angleRaw);
  Serial.print(" | ");
  Serial.print(" AngleAfterCorrection: ");
  Serial.print(Angle.angleAfterCorrection);
  Serial.print(" | ");
  Serial.print(" AngleCorrection: ");
  Serial.print(Angle.angleCorrection);
  Serial.println(" | ");
}

void testPrintAngleDeg()
{
  Serial.print("AngleDeg: ");
  Serial.print(Angle.angleRawDeg);
  Serial.print(" | ");
  Serial.print(" AngleAfterCorrectionDeg: ");
  Serial.print(Angle.angleAfterCorrectionDeg);
  Serial.print(" | ");
  Serial.print(" AngleCorrectionDeg: ");
  Serial.println(Angle.angleCorrectionDeg);
}

void testSerwom()
{
  Serial.print(" ServoRaw: ");
  Serial.println(serwom.serwoRaw);
}

void testPrintGiven()
{
  Serial.print("AngleGiven: ");
  Serial.print(Angle.angleGiven);
  Serial.print(" | ");
  Serial.print(" ServoGiven: ");
  Serial.println(serwom.serwoGiven);
}




void testLine()
{
  Serial.println("---------------------------------------------------------------------------------------------");
  Serial.println();
}


void testPrint()
{
  /*
  Serial.write(27);       // ESC command
  Serial.print("[2J");    // clear screen command
  Serial.write(27);
  Serial.print("[H");
  */
  testPrintAngle();
  testPrintAngleDeg();
  testSerwom();
  testPrintGiven();
  testLine();
}

void testPrint2()
{
  Serial.print("AngleAfterCorrectionDeg: ");
  Serial.print(Angle.angleAfterCorrectionDeg);
  Serial.print(" AngleRaw: ");
  Serial.print(Angle.angleRaw);
  Serial.print(" maxdir: ");
  Serial.println(Angle.maxdir);
  testLine();
}
