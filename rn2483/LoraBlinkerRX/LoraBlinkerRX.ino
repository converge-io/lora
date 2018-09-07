/*
 * Author: ALB
 * September 2018
 */

#include <SoftwareSerial.h>
#include <SimpleTimer.h>

SoftwareSerial loraSerial(10, 11);
String str;
String snr;
int rx_state_machine_index = 0;
volatile bool receiving_packets = 0;
SimpleTimer timer;
int packets_count = 0;
int digits_count = 0;

void tx_finished() {

  receiving_packets = 0;
  rx_state_machine_index = 0;
  Serial.println("");
  Serial.print("Received packets: ");
  Serial.println(packets_count);
  packets_count = 0;
  
  lora_init();

}

void lora_init(){ 

  //output LED pin
  pinMode(9, OUTPUT); //external led
  pinMode(13, OUTPUT); //internal led
  pinMode(12, OUTPUT); //rn2483 reset
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
  
  loraSerial.listen();
  str = loraSerial.readStringUntil('\n');

  loraSerial.println("sys get ver");
  str = loraSerial.readStringUntil('\n');
  
//  loraSerial.println("radio set bt 0.5");
//  wait_for_ok();
  
  loraSerial.println("radio set mod lora");
  str = loraSerial.readStringUntil('\n');
  if(str.indexOf("ok") != 0){
    Serial.println(str);
    Serial.println("error set mod lora");
    while(1);
  }
  
  loraSerial.println("radio set freq 868100000");
  str = loraSerial.readStringUntil('\n');
  if(str.indexOf("ok") != 0){
    Serial.println(str);
    Serial.println("error set freq");
    while(1);
  }
  
//  loraSerial.println("radio set pwr 14");
//  str = loraSerial.readStringUntil('\n');
//  if(str.indexOf("ok") != 0){
//    Serial.println(str);
//    Serial.println("error set pwr");
//    while(1);
//  }
  
  loraSerial.println("radio set sf sf12");
  str = loraSerial.readStringUntil('\n');
  if(str.indexOf("ok") != 0){
    Serial.println(str);
    Serial.println("error set sf");
    while(1);
  }
  
  loraSerial.println("radio set afcbw 125");
  str = loraSerial.readStringUntil('\n');
  if(str.indexOf("ok") != 0){
    Serial.println(str);
    Serial.println("error set afcbw");
    while(1);
  }
  
  loraSerial.println("radio set rxbw 250");
  str = loraSerial.readStringUntil('\n');
  if(str.indexOf("ok") != 0){
    Serial.println(str);
    Serial.println("error set rxbw");
    while(1);
  }
  
//  loraSerial.println("radio set bitrate 50000");
//  wait_for_ok();
//  
//  loraSerial.println("radio set fdev 0");
//  str = loraSerial.readStringUntil('\n');
//  if(str.indexOf("ok") != 0){
//    Serial.println(str);
//    Serial.println("error set rxbw");
//    while(1);
//  }
  
  loraSerial.println("radio set prlen 8");
  str = loraSerial.readStringUntil('\n');
  if(str.indexOf("ok") != 0){
    Serial.println(str);
    Serial.println("error set prlen");
    while(1);
  }
  
  loraSerial.println("radio set crc on");
  str = loraSerial.readStringUntil('\n');
  if(str.indexOf("ok") != 0){
    Serial.println(str);
    Serial.println("error set crc");
    while(1);
  }
  
  loraSerial.println("radio set iqi off");
  str = loraSerial.readStringUntil('\n');
  if(str.indexOf("ok") != 0){
    Serial.println(str);
    Serial.println("error set iqi");
    while(1);
  }
  
  loraSerial.println("radio set cr 4/8");
  str = loraSerial.readStringUntil('\n');
  if(str.indexOf("ok") != 0){
    Serial.println(str);
    Serial.println("error set cr");
    while(1);
  }
  
  loraSerial.println("radio set wdt 0"); //disable for continuous reception
  str = loraSerial.readStringUntil('\n');
  if(str.indexOf("ok") != 0){
    Serial.println(str);
    Serial.println("error set wdt");
    while(1);
  }
  
  loraSerial.println("radio set sync 12");
  str = loraSerial.readStringUntil('\n');
  if(str.indexOf("ok") != 0){
    Serial.println(str);
    Serial.println("error set sync");
    while(1);
  }
  
  loraSerial.println("radio set bw 125");
  str = loraSerial.readStringUntil('\n');
  if(str.indexOf("ok") != 0){
    Serial.println(str);
    Serial.println("error set bw");
    while(1);
  }

  loraSerial.println("mac pause");
  str = loraSerial.readStringUntil('\n');
  if(str.indexOf("0") == 0){
    Serial.println(str);
    Serial.println("error set mod lora");
    while(1);
  }

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
  timer.run();

  if (rx_state_machine_index == 0){
    loraSerial.println("radio rx 0"); 
    str = loraSerial.readStringUntil('\n'); 

    rx_state_machine_index = 1; 
  } 
  if (rx_state_machine_index == 1 && str.indexOf("ok") != 0){
    str = loraSerial.readStringUntil('\n');

  }
  if (rx_state_machine_index == 1 && str.indexOf("ok") == 0){

    rx_state_machine_index = 2;  
  } 
  if (rx_state_machine_index == 2){

    str = loraSerial.readStringUntil('\n');

    if ( str.indexOf("radio_rx") == 0){
      Serial.print(str[10]);
      Serial.print(str[11]);
      Serial.print(", ");
      loraSerial.println("radio get snr");
      snr = loraSerial.readStringUntil('\n');
      Serial.println(snr);
      packets_count++;
      rx_state_machine_index = 0;
      if (receiving_packets==0){
        receiving_packets = 1;
        timer.setTimeout(100000, tx_finished);
      }
    } 

    if ( str.indexOf("radio_err") == 0){
      Serial.println("radio error");
//      Serial.print(", ");
//      loraSerial.println("radio get snr");
//      snr = loraSerial.readStringUntil('\n');
//      Serial.println(snr);
      packets_count++;
      rx_state_machine_index = 0;
    }     
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
  digitalWrite(9, !digitalRead(9));
}

void led_on()
{
  digitalWrite(9, 1);
}

void led_off()
{
  digitalWrite(9, 0);
}


