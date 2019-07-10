#include <SoftwareSerial.h>

SoftwareSerial mySerial(9, 10); // tx and rx of gsm module respectively
float sensorValue;
float sensorVolts;
int buzzer = 12;

void setup()
{
  pinMode(buzzer,OUTPUT);
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  delay(2000);
  mySerial.println("AT+CNMI=2,2,0,0,0");
  delay(20000);    // allow the MQ-6 to warm up
}


void loop()
{
   for(int i = 0; i < 100; i++){ 
      sensorValue = sensorValue + analogRead(0); // read analog input pin 0 
    } 
    sensorValue = sensorValue / 100; // get average reading 
    sensorVolts = sensorValue/1024*5.0; //convert to voltage 
    Serial.println(sensorVolts);
    if(sensorVolts > 1.20){
     digitalWrite(buzzer,HIGH);
     delay(500);
     digitalWrite(buzzer,LOW);
     SendMessage();
     delay(1000);
    }
    else{
    digitalWrite(buzzer,LOW);
    Serial.println("Have A safe Home!");
    }
}


 void SendMessage()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+919944288651\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("GAS Leakage Detected");// The SMS text you want to send
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}


 void RecieveMessage()
{
  mySerial.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
  delay(1000);
  if (Serial.available()>0)
   switch(Serial.read())
  {
    case 's':
      SendMessage();
      break;
    case 'r':
      RecieveMessage();
      break;
  }

 if (mySerial.available()>0)
   Serial.write(mySerial.read());
 }
 
