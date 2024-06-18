#include "serial_port.h"
#include "global.h"

extern struct Global global;
extern struct Serwom serwom;
extern struct angle Angle;

void deserialize(String receivedFormSerialPort){
  global.deseriaize_data[0] = "";
  global.deseriaize_data[1] = "99999";
  global.deseriaize_data[2] = "99999";
  int j = 0;
  for(auto i : receivedFormSerialPort)
  {
    if(i == ',')
    {
      j += 1;
      global.deseriaize_data[j] = "";
    }
    else
    {
      global.deseriaize_data[j] += i;
    }
  }
  
  if(global.deseriaize_data[0] != "99999")
  {
    Angle.angleGiven = global.deseriaize_data[0].toFloat();
    Angle.angleGivenDeg = int(Angle.angleGiven * 0.087);
  }
  if(global.deseriaize_data[1] != "99999")
  {
    serwom.serwoGiven = global.deseriaize_data[1].toInt();
  }
  if(global.deseriaize_data[2] != "99999")
  {
    serwom.serwoGiven = global.deseriaize_data[1].toInt();
  }
}

void sendConfirmationOfReceipt()
{
  Serial.println("Arduino received data from serial port");
}

float readSerialPort()
{
  String receivedFormSerialPort = "";
  if (Serial.available()) {
    delay(10);
    while (Serial.available() > 0) {
      receivedFormSerialPort += (char)Serial.read();
    }
    Serial.flush();
  }
  if (receivedFormSerialPort != "")
  {
    //sendConfirmationOfReceipt();
  }
  float receivedFormSerialPort2 = receivedFormSerialPort.toFloat();
  return receivedFormSerialPort2;
}

boolean readSerialPort2()
{
  unsigned long readSerialTimer = millis();
  String receivedFormSerialPort = "";
  while (receivedFormSerialPort == "" ) 
  {
    if (Serial.available()) {
      delay(10);
      while (Serial.available() > 0) {
        receivedFormSerialPort += (char)Serial.read();
        if(readSerialTimer + 3000 < millis())
        {
          //Serial.println("Reading from serialPort Timerout");
          Serial.flush();
          return false;
        }
      }
      Serial.flush();
    }
  }

  sendConfirmationOfReceipt();
  //float receivedFormSerialPort2 = receivedFormSerialPort.toFloat();
  //deserialize(receivedFormSerialPort);
  //Serial.println(receivedFormSerialPort);
  deserialize(receivedFormSerialPort);
  return true;
}

