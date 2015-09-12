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
const uint16_t this_node = 0001;

// Address of the other node in Octal format (01,021, etc)
//const uint16_t other_node = 01;

struct payload_p {
      float health;
      float lives;
      float powerup;
};
struct payload_two {
      float health;
      float lives;
      float powerup;
};

unsigned int p1health;
unsigned int p1laps;
unsigned int p1powerups;

unsigned int p2health;
unsigned int p2laps;
unsigned int p2powerups;

int main(int argc, char** argv)
{
    radio.begin();
    radio.setRetries(7,7);
    delay(5);
    network.begin(/*channel*/ 90, /*node address*/ this_node);
    radio.printDetails();

  while(1)
  {

//      network.update();
//      while ( network.available() )
//      {
        RF24NetworkHeader header;
        network.peek(header);
      
      payload_p payload;
      payload_two payloadTwo;
         
      network.read(header,&payload,sizeof(payload));
      network.read(header,&payloadTwo,sizeof(payloadTwo));

        
      //Player One Data Vars defined
      p1health=payload.health;
      p1laps=payload.lives;
      p1powerups=payload.powerup;
      
      //Player Two Data Vars defined
      p2health=payloadTwo.health;
      p2laps=payloadTwo.lives;
      p2powerups=payloadTwo.powerup;

      
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
        ss << "p1h=" << p1health << "&p1l=" << p1laps << "&p1p=" << p1powerups << "&p2h=" << p2health << "&p2l=" << p2laps << "&p2p=" << p2powerups;
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
  //}     
     delay(2000);
  }
  return 0;
}
