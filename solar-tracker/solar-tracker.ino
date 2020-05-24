#include <Servo.h>
#include <dht.h>
dht DHT;
Servo base;
Servo top;
#define DHT11_PIN 7
//These are the Analog connection ports
const int TL = A0;
const int TR = A3;
const int BL = A1;
const int BR = A2;
const int RAIN = A3;
//These are some of the assumptions
int servov = 45;
int servoh = 180;
//These are some of the thresholds
int servovLimitHigh = 170;
int servovLimitLow = 120;
int servohLimitHigh = 120;
int servohLimitLow = 50;
int tol = 50; //this is tolerance
void setup() {
  Serial.begin(9600);
  base.attach(9);
  top.attach(10);
}

void loop() {
  int chk = DHT.read11(DHT11_PIN);
  int temp = DHT.temperature;
  int humidity = DHT.humidity;
  int tl = analogRead(TL);
  int tr = analogRead(TR);
  int bl = analogRead(BL);
  int br = analogRead(BR);
  int rain = digitalRead(RAIN);
  
  int avt = (tl + tr) / 2;
  int avd = (bl + br) / 2;
  int avl = (tl + bl) / 2;
  int avr = (tr + br) / 2;
  int dv = avt - avd;
  int dh = avl - avr;
  if(-1*tol > dv || dv > tol){
    if (avt > avd){
      ++servov;
      if(servov > servovLimitHigh){
        servov = servovLimitHigh;
      }
    }
    else if(avt < avd){
      --servov;
      if (servov < servovLimitLow){
        servov = servovLimitLow;
      }
    }
    base.write(servov);
  }
  if ( -1*tol > dh || dh > tol){
    if (avl>avr){
      --servoh;
      if (servoh < servohLimitLow){
        servoh = servohLimitLow;
      }
    }
    else if(avl<avr){
      ++servoh;
      if(servoh>servohLimitHigh){
        servoh = servohLimitHigh;
      }
    }
    top.write(servoh);
  }
  delay(15);
  //display everything:

  Serial.print("horizontal"+String(servoh));
 
    Serial.println("verticle"+String(servov));

}
