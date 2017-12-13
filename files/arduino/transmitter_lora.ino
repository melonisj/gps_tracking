// LoRa 9x_TX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (transmitter)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example LoRa9x_RX

#include <SPI.h>
#include <RH_RF95.h>
#include <SoftwareSerial.h>


#define RFM95_CS 4
#define RFM95_RST 2
#define RFM95_INT 3

#define RXPIN 9
#define TXPIN 10

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

SoftwareSerial mySerial = SoftwareSerial(RXPIN,TXPIN);//Rx:9, Tx: 10

int gps_len = 0;
char gps_string[249];

void setup() 
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  while (!Serial);
  Serial.begin(9600);
  delay(100);

  Serial.println("Arduino LoRa TX Test!");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The   default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);

  SPI.begin();
  SPI.transfer(0x39 | 0x80);    //set sync word regster to 0x6b
  SPI.transfer(0x32);
  delay(100);

  pinMode(RXPIN, INPUT);
  pinMode(TXPIN, OUTPUT);
  mySerial.begin(9600);
  char test_string[] = "hello";
  rf95.send((uint8_t *)test_string, 6);
  rf95.waitPacketSent();
  delay(1000);
  gps_string[0] = 7;
}

int16_t packetnum = 0;  // packet counter, we increment per xmission
void loop()
{
    while(!mySerial.available());
    mySerial.readBytesUntil('\n',gps_string,150);
    gps_string[83] = 0;
    if(gps_string[5] == 'L') 
    {
      Serial.println(gps_string);
      rf95.send((uint8_t *)gps_string, 84);
      rf95.waitPacketSent();
    }
//  if(raw_gps_in =='*')
//  {
//    Serial.println("data sent");
//    gps_string[gps_len+1] = '\n';
//    gps_string[gps_len+2] = '\r';
//    gps_string[gps_len+3] = '\n';
//    gps_string[gps_len+4] = '\r';
//    gps_string[gps_len+5] = '\0';
//    Serial.println(gps_string);
//    rf95.send((uint8_t *)gps_string, gps_len+5);
//    rf95.waitPacketSent();
//    gps_len = 0;
//  }
//  else if(raw_gps_in == '\r');
//  else
//  {
//    gps_string[++gps_len] = raw_gps_in;
//  }
}














  #if 0
  while(!mySerial.available());
  char raw_gps_in = mySerial.read();
  //    rf95.send((uint8_t *)raw_gps_in, 1);
  //    rf95.waitPacketSent();
  //    Serial.println("here");
  if(raw_gps_in == '\n')
  {
  rf95.send((uint8_t *)test_string_two, 5);
  rf95.waitPacketSent();
  //transmit previous
  if(0)
  //        gps_string[0] == '$') //&&
  //      gps_string[1] == 'G' &&
  //      gps_string[2] == 'P' &&
  //      gps_string[3] == 'G' &&
  //      gps_string[4] == 'L' &&
  //      gps_string[5] == 'L')
  {
  
  
  
  
  Serial.println("Sending to rf95_server");
  // Send a message to rf95_server
  
  //      itoa(packetnum++, radiopacket+13, 10);
  Serial.println("Sending "); 
  //      Serial.println(gps_string);
  //      radiopacket[19] = 0;
  
  Serial.println("Sending..."); delay(10);
  rf95.send((uint8_t *)gps_string, gps_len);
  
  Serial.println("Waiting for packet to complete..."); delay(10);
  rf95.waitPacketSent();
  // Now wait for a reply
  //      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  //      uint8_t len = sizeof(buf);
  gps_len = 0;
  //      delay(1000);
  }
  }
  gps_string[gps_len] = raw_gps_in;
  gps_len++;
  
  //    Serial.print(raw_gps_in);
  
  /////////////////////////////////////////
  
  
  //  Serial.println("Sending to rf95_server");
  //  // Send a message to rf95_server
  //  
  //  char radiopacket[20] = "Goodbye Mars #     ";
  //  itoa(packetnum++, radiopacket+13, 10);
  //  Serial.print("Sending "); Serial.println(radiopacket);
  //  radiopacket[19] = 0;
  //  
  //  Serial.println("Sending..."); delay(10);
  //  rf95.send((uint8_t *)radiopacket, 20);
  //
  //  Serial.println("Waiting for packet to complete..."); delay(10);
  //  rf95.waitPacketSent();
  //  // Now wait for a reply
  //  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  //  uint8_t len = sizeof(buf);
  //
  //  Serial.println("Waiting for reply..."); delay(10);
  //  if (rf95.waitAvailableTimeout(1000))
  //  { 
  //    // Should be a reply message for us now   
  //    if (rf95.recv(buf, &len))
  //   {
  //      Serial.print("Got reply: ");
  //      Serial.println((char*)buf);
  //      Serial.print("RSSI: ");
  //      Serial.println(rf95.lastRssi(), DEC);    
  //    }
  //    else
  //    {
  //      Serial.println("Receive failed");
  //    }
  //  }
  //  else
  //  {
  //    Serial.println("No reply, is there a listener around?");
  //  }
  //  delay(1000);
}
#endif
