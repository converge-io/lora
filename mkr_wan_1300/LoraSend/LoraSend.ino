#include <SPI.h>
#include <LoRa.h>
#include <MKRWAN.h>

const long freq = 868E6;

int i;
int counter = 1;
const int led = 6;
bool sending_packets = 0;
LoRaModem modem;

void setup()
{
  Serial.begin(115200);


  modem.dumb();

  pinMode(led, OUTPUT);
  pinMode(7,INPUT_PULLUP);
  
  LoRa.setPins(LORA_IRQ_DUMB, 6, 1); // set CS, reset, IRQ pin
  LoRa.setTxPower(14, PA_OUTPUT_RFO_PIN);
  LoRa.setSPIFrequency(125E3);
  LoRa.setSignalBandwidth(7.8E3);
  LoRa.setSpreadingFactor(12);
  LoRa.setSyncWord(0x34);
  LoRa.setCodingRate4(8);
  LoRa.setPreambleLength(65535);
  LoRa.begin(freq);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(freq))
  {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.print("Frequency ");
  Serial.println(freq);
}

void loop()
{

  counter=0;
  sending_packets = !digitalRead(7);

  if (sending_packets){
    for (i=0;i<100;i++){
      digitalWrite(led, HIGH);
      Serial.print("Sending packet: ");
      Serial.println(counter);
    
      // send packet
      LoRa.beginPacket();
      LoRa.print(counter);
      LoRa.endPacket();
    
      counter++;

      delay(500);
      digitalWrite(led, LOW);     
      delay(500);
    }
  }

  


}
