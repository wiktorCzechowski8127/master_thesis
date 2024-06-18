#ifndef GLOBAL_H
#define GLOBAL_H
#include <Arduino.h>

struct angle{
  int angleRaw; //max 4096
  int angleRawDeg; //max 360*
  int angleAfterCorrection;
  int angleAfterCorrectionDeg;
  int angleCorrection;
  int angleCorrectionDeg;
  int angleGiven;
  int angleGivenDeg;
  int angleRawAfterCorrection;
  int angleRawAfterCorrectionDeg;
  int maxdir = 0;
};

struct Serwom{
  int serwoRaw;
  int serwoAfterCorrection;
  int serwoGiven;
  int serwoCorrection = 0 ;
};

struct Global{
  String deseriaize_data[3];
  unsigned long timer[5];
  int ycenter = 0;
  int xcenter = 0;

};

#endif