#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>

#define CLIENT_ADDRESS 2
#define SERVER_ADDRESS 1

// Singleton instance of the radio driver
RH_RF95 driver(6, 2);

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, CLIENT_ADDRESS);

void setup() 
{
  // Dramco uno - enable 3v3 voltage regulator
 pinMode(8, OUTPUT);
digitalWrite(8, HIGH);

  Serial.begin(9600);
  while(!Serial) ; // Wait for serial port to be available
  if(!manager.init())
    Serial.println("init failed");
    driver.setFrequency(868);
}

#warning "compiling lora ptp client code"
void loop()
{
  Serial.println("Sending to rf95_server");
  // Send a message to rf95_server
  uint8_t data[] = "Hello World!";
  
    // Send a message to manager_server
  if (manager.sendtoWait(data, sizeof(data), SERVER_ADDRESS))
  {
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
    uint8_t from;   

 if (manager.recvfromAckTimeout(buf, &len, 2000, &from))
    {
      Serial.print("got reply from : 0x");
      Serial.print(from, HEX);
      Serial.print(": ");
      Serial.println((char*)buf);
    } 
    
    else
    {
      Serial.println("recv failed");
    }
  }
  else
  {
    Serial.println("No reply, is rf95_server running?");
  }
  delay(400);
}
