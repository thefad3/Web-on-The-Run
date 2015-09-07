/*
 Copyright (C) 2012 Victor Cabieles. <victorcabieles@gmail.com>

 This program is NOT free software; you MAY NOT redistribute it and/or
 modify it.
 
 */

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 6
#define RST_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

RF24 radio(9,10);                // nRF24L01(+) radio attached using Getting Started board 

RF24Network network(radio);      // Network uses that radio
const uint16_t this_node = 0;    // Address of our node
const uint16_t other_node = 1;   // Address of the other node

    struct payload_t
    {
      int yaw;
      int pitch;
      int roll;
    };



void setup(void)
{
  Serial.begin(57600);
  SPI.begin();
  mfrc522.PCD_Init(); // Init MFRC522 card
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
}

void loop(void){
  
  network.update();                  // Check the network regularly

  while ( network.available() ) {     // Is there anything ready for us?
    
    RF24NetworkHeader header;        // If so, grab it and print it out
    payload_t payload;
    network.read(header,&payload,sizeof(payload));
    Serial.print("Received packet #");
    Serial.print(payload.yaw);
    Serial.print(" at ");
    Serial.print(payload.pitch);
    Serial.print(" at ");
    Serial.println(payload.roll);

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

