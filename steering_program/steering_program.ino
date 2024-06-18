/* -------------------------------------------- INCLUDES ---------------------------------------*/
#include <Wire.h>
#include <AS5600.h>
#include "motor.h"
#include "global.h"
#include "serial_port.h"
#include "angle.h"
#include "test_print.h"
#include "sundetect.h"
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
/* -------------------------------------------- DEFINE --------------------------------*/

#define button1 4
#define button2 5

/* -------------------------------------------- GLOBAL VARIABLE --------------------------------*/


angle Angle;
Global global;
Serwom serwom;
Servo serwoClass;



uint8_t button1PreventState = HIGH;
uint8_t button2PreventState = HIGH;


/* -------------------------------------------- MODULES ----------------------------------------*/
AMS_5600 ams5600;
LiquidCrystal_I2C lcd(0x27,20,4);

/* -------------------------------------------- SETUP ------------------------------------------*/
void setup() {


  pinMode(A0,INPUT);//left photoresistor
  pinMode(A1,INPUT);//right photoresistor
  pinMode(A4,INPUT);//up photoresistor
  pinMode(A5,INPUT);//down photoresistor
  
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);

  Serial.begin(460800);

  Wire.begin();
  if (ams5600.detectMagnet() == 1) {
    Serial.print("Magnes OK");
  } else {
    Serial.println("Magnes ERROR");
  }

  calculateAngleCorrection(0,0);

  global.timer[0] = millis();
  global.timer[1] = millis();
  global.timer[2] = millis();

  Angle.angleGiven = 2048;

  serwoClass.attach(10);
  serwom.serwoRaw = 20;
  serwom.serwoGiven = 20;
  serwom.serwoAfterCorrection = 20;
  serwoClass.write(serwom.serwoRaw);
  
  lcd.backlight();
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("starting");

  Angle.angleRaw = ams5600.getRawAngle();
  correctRawAngle();
  Angle.angleRawDeg = Angle.angleRaw * 0.087;
  Angle.angleRawAfterCorrection = Angle.angleRaw;
  Angle.angleGiven = Angle.angleRawAfterCorrection;
  CorrectAngle();
  validateMoveDirection();
  makeSerwoMove();
  delay(10);



  delay(1000);
  prSunDetect();
}

// -> LOOP -------------------------------------------------------------------------------------------------
void loop() {


  //loat tmp = readSerialPort();

  Angle.angleRaw = ams5600.getRawAngle();
  correctRawAngle();
  Angle.angleRawDeg = Angle.angleRaw * 0.087;
  Angle.angleRawAfterCorrection = Angle.angleRaw;
  CorrectAngle();
  validateMoveDirection();
  makeSerwoMove();
  delay(10);



  if (digitalRead(button1) == LOW && button1PreventState == HIGH)
  {
    button1PreventState = LOW;
    Serial.println("yellow button pressed: input angle to set...");
    //float temp = readSerialPort2().toFloat(); 
    Serial.print("odczytano: ");
    //Serial.println(temp);
    //calculateAngleCorrection(temp);
    //Angle.angleGiven = temp;
  }
  int tmp = Serial.available();
  if (tmp > 0)
  {
    lcd.setCursor(0,1);
    lcd.print("in");
    button2PreventState = LOW;
    //Serial.println("white button pressed: input destination angle...");
    readSerialPort2();
    serwom.serwoGiven = serwom.serwoGiven + serwom.serwoCorrection;

  }
    button1PreventState = digitalRead(button1);
    button2PreventState = digitalRead(button2);

  if (millis() > global.timer[0] + 500)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(int(Angle.angleGivenDeg));
    lcd.setCursor(4, 0);
    lcd.print(int(Angle.angleAfterCorrectionDeg));
    lcd.setCursor(8, 0);
    lcd.print(int(Angle.angleCorrectionDeg));

    lcd.setCursor(0, 1);
    lcd.print(int(serwom.serwoGiven));
    lcd.setCursor(4, 1);
    lcd.print(int(serwom.serwoRaw));
    lcd.setCursor(8, 1);
    lcd.print(int(serwom.serwoCorrection));

    lcd.print(tmp);


    //testPrint(); 
    global.timer[0] = millis();
  }
} 
