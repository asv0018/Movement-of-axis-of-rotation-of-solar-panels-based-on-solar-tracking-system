/*
 **************************************************************************************************************************************      
 *                                                    PROGRAMMED BY SHREEVALLABHA (A$V)                                               *
 **************************************************************************************************************************************
 * CONNECTION OF CIRCUIT DIAGRAM AND ITS DOCUMENTATION IS HERE :
 * 
 *  Connect Bluetooth as :
 *      Rx of HC-05 to 3th Pin of Arduino Shield
 *      Tx of HC-05 to 4rd Pin of Arduino Shield
 *  
 *  Rain Sensor to Pin 02 of Arduino Shield
 *  
 *  DHT11 Sensor to Pin 03 of Arduino Shield
 *  
 *  Make sure you connect Vcc to 5V and GND to GND Respectively
 *  
 *  Make sure you caliberate the vertical and horizontal angles based on your requirement of the prototype
 *  
 *  
 */

#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <dht.h>

#define dht_pin 3
#define rain_pin 2

dht DHT;
LiquidCrystal_I2C lcd(0x3F,16,02);
Servo base;
Servo top;
#define DHT11_PIN 7
//These are the Analog connection ports
const int TL = A0;
const int TR = A3;
const int BL = A1;
const int BR = A2;
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
  lcd.init();
  lcd.init();
  lcd.backlight();
  pinMode(rain_pin,INPUT);
}
int published_time = 0;
void loop() {
while(!Serial.available()){
  int tl = analogRead(TL);
  int tr = analogRead(TR);
  int bl = analogRead(BL);
  int br = analogRead(BR);
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
  int current_time = millis();
  DHT.read11(dht_pin);
  int humidity = DHT.humidity;
  float temperature = DHT.temperature;
  boolean isRaining = digitalRead(rain_pin);
  if (humidity > 0 && temperature > 0){
  lcd.setCursor(0,0);
  lcd.print("hum:");
  lcd.setCursor(4,0);
  lcd.print(String(humidity));
  lcd.setCursor(7,0);
  lcd.print("tem:");
  lcd.setCursor(11,0);
  lcd.print(String(temperature));
  lcd.setCursor(0,1);
  isRaining == false ? lcd.print("RAINING    "):lcd.print("NOT RAINING");
  if(current_time - published_time >= 10000){
    publish_data(temperature,humidity,isRaining);
    published_time = millis();
    }
  }
 }
}
void publish_data(float temperature,int humidity,bool isRaining){
  Serial.print("The Sensor Readings are : \n");
  Serial.print("temperature : "+String(temperature)+"\n");
  Serial.print("humidity : "+String(humidity)+"\n");
  Serial.print("Raining Status : ");
  isRaining == false ? Serial.print("RAINING\n"):Serial.print("NOT RAINING\n");
  }
