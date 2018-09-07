#include <SPI.h>
#include <LoRa.h>
#include <MKRWAN.h>

const int analogInPin = A0;  // Analog input pin that the sensor is attached to
int sensorValue = 0;        // value read from the sensor

const long freq = 868E6;

int counter = 1;
const int led = 6;
LoRaModem modem;

void setup()
{
  Serial.begin(9600);


  modem.dumb();

  pinMode(led, OUTPUT);
  
  LoRa.setPins(LORA_IRQ_DUMB, 6, 1); // set CS, reset, IRQ pin
  LoRa.setTxPower(17, PA_OUTPUT_RFO_PIN);
  LoRa.setSPIFrequency(125E3);
  LoRa.setSignalBandwidth(31.25E3);
  LoRa.setSpreadingFactor(9);
  LoRa.setSyncWord(0x34);
  LoRa.setCodingRate4(5);
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
  digitalWrite(led, HIGH);
  Serial.print("Sending packet: ");
  Serial.println(counter);
  
  // read the analog in value:
  sensorValue = analogRead(analogInPin);


  // send packet
  LoRa.beginPacket();
//  LoRa.print("Time: ");
  LoRa.print(millis());
  LoRa.print(", ");
  LoRa.print(sensorValue);
  LoRa.endPacket();

  counter++;

  digitalWrite(led, LOW);

  delay(100);
}
