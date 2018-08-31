/*
 * Author: ALB
 * September 2018
 */

#include <SoftwareSerial.h>
#include <SimpleTimer.h>

SoftwareSerial loraSerial(10, 11);
String str;
volatile bool receiving_packets = 0;
SimpleTimer timer;
int packets_count = 0;

void tx_finished() {

  receiving_packets = 0;
  Serial.print("Received packets: ");
  Serial.println(packets_count);
  packets_count = 0;
  
  lora_init();

}

void lora_init(){ 

  //output LED pin
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(12, 0);
  delay(500);
  digitalWrite(12, 1);
  led_off();
  
  // Open serial communications and wait for port to open:
  
  Serial.begin(115200);
  
  loraSerial.begin(9600);
  loraSerial.setTimeout(1000);
  lora_autobaud();

  led_on();
  delay(2000);
  led_off();

//  Serial.println("Initing LoRa");
  
  loraSerial.listen();
  str = loraSerial.readStringUntil('\n');
//  Serial.println(str);
  loraSerial.println("sys get ver");
  str = loraSerial.readStringUntil('\n');
//  Serial.println(str);
  
  loraSerial.println("mac pause");
  str = loraSerial.readStringUntil('\n');
//  Serial.println(str);
  
//  loraSerial.println("radio set bt 0.5");
//  wait_for_ok();
  
  loraSerial.println("radio set mod lora");
  str = loraSerial.readStringUntil('\n');
//  Serial.println(str);
  
  loraSerial.println("radio set freq 868000000");
  str = loraSerial.readStringUntil('\n');
//  Serial.println(str);
  
  loraSerial.println("radio set pwr 14");
  str = loraSerial.readStringUntil('\n');
//  Serial.println(str);
  
  loraSerial.println("radio set sf sf7");
  str = loraSerial.readStringUntil('\n');
//  Serial.println(str);
  
  loraSerial.println("radio set afcbw 125");
  str = loraSerial.readStringUntil('\n');
//  Serial.println(str);
  
  loraSerial.println("radio set rxbw 250");
  str = loraSerial.readStringUntil('\n');
//  Serial.println(str);
  
//  loraSerial.println("radio set bitrate 50000");
//  wait_for_ok();
  
//  loraSerial.println("radio set fdev 25000");
//  wait_for_ok();
  
  loraSerial.println("radio set prlen 8");
  str = loraSerial.readStringUntil('\n');
//  Serial.println(str);
  
  loraSerial.println("radio set crc on");
  str = loraSerial.readStringUntil('\n');
//  Serial.println(str);
  
  loraSerial.println("radio set iqi off");
  str = loraSerial.readStringUntil('\n');
//  Serial.println(str);
  
  loraSerial.println("radio set cr 4/5");
  str = loraSerial.readStringUntil('\n');
//  Serial.println(str);
  
  loraSerial.println("radio set wdt 0"); //disable for continuous reception
  str = loraSerial.readStringUntil('\n');
//  Serial.println(str);
  
  loraSerial.println("radio set sync 12");
  str = loraSerial.readStringUntil('\n');
//  Serial.println(str);
  
  loraSerial.println("radio set bw 250");
  str = loraSerial.readStringUntil('\n');
//  Serial.println(str);

  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("*************");
  Serial.println("Lora RX Ready"); 
  Serial.println("*************");
  Serial.println("");

}

void setup() {

  lora_init();
  
  led_on();
  delay(2000);
  led_off();
}

void loop() {
  loraSerial.println("radio rx 0"); //wait for 60 seconds to receive
  str = loraSerial.readStringUntil('\n');
  
  if ( str.indexOf("ok") == 0 )
  {
    str = "";
    while(str == "")
    {
      str = loraSerial.readStringUntil('\n');
      if (receiving_packets == 1){
        break;
      }
    }
    if ( str.indexOf("radio_rx") == 0)
    {
      if (receiving_packets == 0){
        receiving_packets = 1;
        timer.setTimeout(65000, tx_finished);
      } 
      if (str.indexOf("0A") == (-1)){
        toggle_led();
        Serial.print(str[10]);
        Serial.print(str[11]);
        Serial.print(str[12]);
        Serial.println(str[13]);
        packets_count++;         
      }
    }
  }
  if (receiving_packets == 1){
    timer.run();
  }
}

void lora_autobaud()
{
  String response = "";
  while (response=="")
  {
    delay(1000);
    loraSerial.write((byte)0x00);
    loraSerial.write(0x55);
    loraSerial.println();
    loraSerial.println("sys get ver");
    response = loraSerial.readStringUntil('\n');
  }
}

/*
 * This function blocks until the word "ok\n" is received on the UART,
 * or until a timeout of 3*5 seconds.
 */
int wait_for_ok()
{
  str = loraSerial.readStringUntil('\n');
  if ( str.indexOf("ok") == 0 ) {
    return 1;
  }
  else return 0;
}

void toggle_led()
{
  digitalWrite(13, !digitalRead(13));
}

void led_on()
{
  digitalWrite(13, 1);
}

void led_off()
{
  digitalWrite(13, 0);
}


