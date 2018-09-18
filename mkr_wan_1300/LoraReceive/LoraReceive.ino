#include <SPI.h>
#include <LoRa.h>
#include <MKRWAN.h>
#include <SimpleTimer.h>

const long freq = 868E6;

int counter = 1;
int packets_count = 0;
const int led = 6;
volatile bool receiving_packets = 0;
LoRaModem modem;
SimpleTimer timer;

void tx_finished() {
  receiving_packets = 0;
  Serial.println("");
  Serial.print("Received packets: ");
  Serial.println(packets_count);
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  packets_count = 0;
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting");

  modem.dumb();

  pinMode(led, OUTPUT);
  
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
  timer.run();
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {

    if (receiving_packets==0){
        receiving_packets = 1;
        timer.setTimeout(101000, tx_finished);
      }
      
    // received a packet
    digitalWrite(led, HIGH);
//    Serial.print("Received packet '");

    // read packet
    while (LoRa.available())
    {
      Serial.print((char)LoRa.read());
    }
    

    // print RSSI of packet
    Serial.print(", ");
    Serial.println(LoRa.packetSnr());

    packets_count++;
    LoRa.parsePacket();

    
    digitalWrite(led, LOW);


    
  }
}
