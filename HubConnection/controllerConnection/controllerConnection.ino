#include <stdio.h>
#include <curl/curl.h>
#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>
#include <iostream>
#include <ctime>
#include <time.h>
#include <string>
#include <sstream>

using namespace std;

RF24 radio(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);  

RF24Network network(radio);

// Address of our node in Octal format
const uint16_t this_node = 00;

// Address of the other node in Octal format (01,021, etc)
const uint16_t other_node = 01;

const unsigned long interval = 2000; //ms  // How often to send 'hello world to the other unit

unsigned long last_sent;             // When did we last send?
unsigned long packets_sent;          // How many have we sent already

struct payload_t {                  // Structure of our payload
  unsigned long ms;
  unsigned long counter;
};

unsigned int health;
unsigned int laps;
unsigned int powerups;

int main(int argc, char** argv)
{
  radio.begin();
  delay(5);
  network.begin(/*channel*/ 90, /*node address*/ this_node);
  radio.printDetails();

  while(1)
  {

      network.update();
        while ( network.available() ) {     // Is there anything ready for us?
          
      RF24NetworkHeader header;        // If so, grab it and print it out
         payload_t payload;
         network.read(header,&payload,sizeof(payload));
      
      printf("Received payload # %lu at %lu \n",payload.counter,payload.ms);
      health = payload.counter;
      laps = payload.ms;
      powerups = "Lap 1 lolz";
      
  CURL *curl;
  CURLcode res;

  /* In windows, this will init the winsock stuff */
  curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */
  curl = curl_easy_init();
  if(curl) {

     curl_easy_setopt(curl, CURLOPT_URL, "https://data.sparkfun.com/input/5JLl6mw4mpU17dn35551?private_key=7BKWRGJoGjiP9qp5NNNP");

        //Data Stream includes "health" "laps" and "powerups"
        //Jon's teh fucking man for this
        
        stringstream ss;
        ss << "health=" << health << "&laps=" << laps << "&powerups=" << powerups;
        string s = ss.str();
        const char* cstr = s.c_str();
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, cstr);
        
    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
        /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  }     
     delay(2000);
  }
  return 0;
}


