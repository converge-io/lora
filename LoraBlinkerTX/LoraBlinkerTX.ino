/*
 * Author: ALB
 * September 2018
 */

#include <SoftwareSerial.h>

SoftwareSerial loraSerial(10, 11);

String str;
char data[14]="";
char i_string[4];
volatile bool transmitting_packets = 0;
int loops_count = 0;


void lora_init(){ 
  
  //output LED pin
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  
  //input LED pin
  pinMode(2, INPUT);           
  digitalWrite(2, HIGH);       

  
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

  Serial.println("Initing LoRa");
  
  loraSerial.listen();
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  loraSerial.println("sys get ver");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("mac pause");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
//  loraSerial.println("radio set bt 0.5");
//  wait_for_ok();
  
  loraSerial.println("radio set mod lora");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set freq 868000000");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set pwr 14");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set sf sf7");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set afcbw 125");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set rxbw 250");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
//  loraSerial.println("radio set bitrate 50000");
//  wait_for_ok();
  
//  loraSerial.println("radio set fdev 25000");
//  wait_for_ok();
  
  loraSerial.println("radio set prlen 8");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set crc on");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set iqi off");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set cr 4/5");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set wdt 0"); //disable for continuous reception
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set sync 12");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set bw 250");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);

  Serial.println("starting loop");  

  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("***********************************");
  Serial.println("Press Button to Start Transmissions"); 
  Serial.println("***********************************");
  Serial.println("");
  
}

void setup() {

  lora_init();
  
  led_on();
  delay(2000);
  led_off();
}





void loop() {


  if(!transmitting_packets){
    transmitting_packets = !digitalRead(2);
  }
  
  if(transmitting_packets){
    for (int i=0; i<100; i++){
      if (loops_count == 9){
      toggle_led();  
      loops_count = 0;
      }

      delay(10);     
      loraSerial.println("radio tx a");
      str = loraSerial.readStringUntil('\n');
      Serial.println(str);
      str = loraSerial.readStringUntil('\n');
      Serial.println(str);    
      loops_count++;
    }  
    for (int i=0; i<1000; i++){
      toggle_led();
      delay(10);
      memset(data, 0, sizeof(data));
      strcat(data, "radio tx ");
      sprintf(i_string, "%d", i);
      strcat(data, i_string);
    
      loraSerial.println(data);
      str = loraSerial.readStringUntil('\n');
      Serial.println(str);
      str = loraSerial.readStringUntil('\n');
      Serial.println(str);
    }
    transmitting_packets=0;
    lora_init();
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
