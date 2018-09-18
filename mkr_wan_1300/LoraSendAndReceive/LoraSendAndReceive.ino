/*
  Lora Send And Receive
  This sketch demonstrates how to send and receive data with the MKR WAN 1300 LoRa module.
  This example code is in the public domain.
*/

#include <MKRWAN.h>
#include <LoRa.h>


LoRaModem modem;

#include "arduino_secrets.h" 
// Please enter your sensitive data in the Secret tab or arduino_secrets.h
String appEui = SECRET_APP_EUI;
String appKey = SECRET_APP_KEY;
const int led = 6;
int i;
int counter = 0;
bool sending_packets = 0;

void setup() {

  modem.dumb();

  pinMode(led, OUTPUT);
  pinMode(7,INPUT_PULLUP);

  
  LoRa.setPins(LORA_IRQ_DUMB, 6, 1); // set CS, reset, IRQ pin
  LoRa.setTxPower(20, PA_OUTPUT_PA_BOOST_PIN);
  LoRa.setSPIFrequency(125E3);
  LoRa.setSignalBandwidth(7.8E3);
  LoRa.setSpreadingFactor(12);
  LoRa.setSyncWord(0x34);
  LoRa.setCodingRate4(8);
  LoRa.setPreambleLength(8);

  
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial);
  // change this to your regional band (eg. US915, AS923, ...)
  if (!modem.begin(EU868)) {
    Serial.println("Failed to start module");
    while (1) {}
  };
  Serial.print("Your module version is: ");
  Serial.println(modem.version());
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());
  Serial.println("");
  Serial.println("");

  int connected = modem.joinOTAA(appEui, appKey);
  if (!connected) {
    Serial.println("Something went wrong; are you indoor? Move near a window and retry");
    while (1) {}
  }

  // Set poll interval to 60 secs.
//  modem.minPollInterval(60);
  // NOTE: independently by this setting the modem will
  // not allow to send more than one message every 2 minutes,
  // this is enforced by firmware and can not be changed.
  delay(3000);
}

void loop() {

  counter=0;
  sending_packets = !digitalRead(7);

  if (sending_packets){
    for (i=0;i<10;i++){
      String msg = "packet"; 
      int err;
      modem.beginPacket();
      modem.print(msg);
      err = modem.endPacket(true);
      if (err > 0) {
        Serial.print("Message ");
        Serial.print(counter);
        Serial.println(" sent");
          
      } else {
        Serial.println("Error sending message :(");
        Serial.println("(you may send a limited amount of messages per minute, depending on the signal strength");
        Serial.println("it may vary from 1 message every couple of seconds to 1 message every minute)");
      }
      digitalWrite(led, HIGH);
      delay(500);
      digitalWrite(led, LOW);
      delay(500);
      counter++;
      if (!modem.available()) {
        Serial.println("No downlink message received at this time.");
      }
      Serial.println();
    }
  }
    
}
