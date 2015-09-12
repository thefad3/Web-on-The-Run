/*
 Copyright (C) 2015 Victor Cabieles. <victorcabieles@gmail.com>

 This program is NOT free software; you MAY NOT redistribute it and/or
 modify it.

 I DO WHAT I WANT - Chris Lynch.
 
 */

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h> 

Servo drivingWheel;
Servo esc;
#define SS_PIN 6
#define RST_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

RF24 radio(9,10);                // nRF24L01(+) radio attached using Getting Started board 

RF24Network network(radio);      // Network uses that radio
const uint16_t this_node = 04;    // Address of our node
const uint16_t other_node = 05;   // Address of the other node

int fsrAnalogPin = 0; // FSR is connected to analog 0
int fsrReading;      // the analog reading from the force sensitive resistor

    struct payload_t
    {
      int yaw;
      int pitch;
      int roll;
    };

    struct payload_hub
    {
      int fsr;
      String cardNumber;
      int roll;
    };



void setup(void)
{
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init(); // Init MFRC522 card
  radio.begin();
  network.begin(/*channel*/ 70, /*node address*/ this_node);
  drivingWheel.attach(8);
  esc.attach(11);
}

void loop(void){
  
  fsrReading = analogRead(fsrAnalogPin);
  
  network.update();                  // Check the network regularly
  
  while ( network.available() ) {     // Is there anything ready for us?
    
    RF24NetworkHeader header;        // If so, grab it and print it out
    payload_t payload;
    network.read(header,&payload,sizeof(payload));
    Serial.print("Received yaw # ");
    Serial.println(payload.yaw);
    Serial.print(" piych ");
    Serial.print(payload.pitch);
    Serial.print(" roll ");
    Serial.print(payload.roll);
    Serial.print("Analog reading = ");
    Serial.println(fsrReading);
    drivingWheel.write(payload.yaw);
    esc.write(payload.pitch);
    
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
  Serial.println("UID String :");
  Serial.println(UID_string);
  
  }
}

