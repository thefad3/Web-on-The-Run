/*
 Copyright (C) 2015 Victor Cabieles & Chris Lynch <victorcabieles@gmail.com, thefad3@gmail.com>

 This program is NOT free software; you MAY NOT redistribute it and/or
 modify it.
 
 */

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h> 
#include <stdlib.h>

Servo drivingWheel;
Servo esc;
#define SS_PIN 6
#define RST_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

RF24 radio(9,10);                // nRF24L01(+) radio attached using Getting Started board 

RF24Network network(radio);      // Network uses that radio
const uint16_t this_node = 0100;    // Address of our node
const uint16_t other_node = 1010;   // Address of the other node
const uint16_t hub_node = 0001;   // Address of Hub

int fsrAnalogPin = 0; // FSR is connected to analog 0
int fsrReading;      // the analog reading from the force sensitive resistor
int lap=0;
int lives=3;
int randomDamage;
int health=100;
int enmDmg=0;
int powerup;

    struct payload_t
    {
      int yaw;
      int pitch;
      int roll;
      bool redButton; //Reset to 90 d
      bool yellowButton; //Run
    };

    struct payload_pTwo
    {
      float health;
      float lives;
      float lap;
      float powerup;
    };



void setup(void)
{
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init(); // Init MFRC522 card
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
  drivingWheel.attach(8);
  esc.attach(11);
}

void loop(void){
 
  fsrReading = analogRead(fsrAnalogPin); 
  
      //Pressure Hit score reducer
      if(fsrReading>450){
        health-=7;
        delay(2000);
      }

        RF24NetworkHeader header(hub_node);
        payload_pTwo payload = {health, lives, lap};
        bool ok = network.write(header,&payload,sizeof(payload));
        
  network.update();                  // Check the network regularly
  while(network.available()){     // Is there anything ready for us?
    RF24NetworkHeader header;        // If so, grab it and print it out
    payload_t payload;
    network.read(header,&payload,sizeof(payload));
    drivingWheel.write(payload.yaw);
    esc.write(payload.pitch);

    if(payload.yellowButton==1){
      Serial.println("Yellow button");
      
    }
    
    if(payload.redButton==1){
      Serial.println("Red Button");
      drivingWheel.write(90);
      delay(3000);
    }

    if ( ! mfrc522.PICC_IsNewCardPresent()) {
        return;
    }

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) {
        return;
    }
    
  unsigned long UID_unsigned;
  UID_unsigned =  mfrc522.uid.uidByte[0] << 24;
  UID_unsigned += mfrc522.uid.uidByte[1] << 16;
  UID_unsigned += mfrc522.uid.uidByte[2] <<  8;
  UID_unsigned += mfrc522.uid.uidByte[3];
  String UID_string =  (String)UID_unsigned;

  //Powerup Here
  if(UID_string){    
    Serial.println(UID_string);
    
    if(UID_string == "20518"){
      health-=20;
    }
    
    if(UID_string == "27173"){
      health+=20;
    }

    if(UID_string == "2853"){
      Serial.println("fsdkjhfkjdhfkjsd");
      delay(2000);
    }
    if(UID_string == "4294940993"){
      
    }

    if(UID_string == "4294939457" || UID_string == "4294946881" || UID_string == "4294951233" || UID_string == "4294938709" || UID_string == "4294940481" || UID_string == "4294938433"){
      lap++;
      if(lap>=4){
        lap=0;
      }
    } 
  } 
  }
}

