#ifndef SERIALPORT_FUNCTIONS_H
#define SERIALPORT_FUNCTIONS_H
#include <Arduino.h>

void sendConfirmationOfReceipt();
float readSerialPort();
boolean readSerialPort2();
void deserialize(String receivedFormSerialPort);

#endif