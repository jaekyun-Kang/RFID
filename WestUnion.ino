#include<SPI.h>
#include<MFRC522.h>
#include<CapacitiveSensor.h>

/* pin structures
SDA -> 10
SCK -> 13
MOSI -> 11
MI -> 12
RST -> 9

Red Led - 7
Green Led -6
*/

#define RSTPIN 9
#define SSPIN 10
MFRC522 rc(SSPIN, RSTPIN);

int readsuccess;

/*
byte defcard[4]={0xC6,0xBA,0xFD,0x1B}; // if you only want one card
byte defcard[][4]={{0xC6,0xBA,0xFD,0x1B},{0x5A,0x7A,0xDB,0x73},{0x53,0x60,0x6F,0x21}}; //for multiple cards
*/

byte defcard[][4]={{0x53,0xA6,0xDE,0x2E},{0x53,0x4B,0x15,0x2E}}; 
//e.g. scan the card you want to pair with the reader, and the serial port will print out the UID of this card, 
//change the number to your UID here.If your UID is C6BAFD1B, just type {0xC6,0xBA,0xFD,0x1B}.  

int N=2; //change this to the number of cards/tags you will use
byte readcard[4]; //stores the UID of current tag which is read

bool cardFlag = false;
const int redLed = 6;
const int greenLed = 7;
const int lockPin = 5;
/*
CapacitiveSensor capSensor = CapacitiveSensor(4,2);
int threshold = 800;
const int ledPin = 13;
bool led_State = LOW;
bool pass = false;
*/

/*

5
6
7

motor+
motor-

VIN
GND
GND

Redlight
+
-
GreenLight
+
-
*/


void setup() {
  Serial.begin(9600);

  SPI.begin();
  rc.PCD_Init(); //initialize the receiver  
  rc.PCD_DumpVersionToSerial(); //show details of card reader module

  pinMode(redLed,OUTPUT); //red LED for LOCK
  pinMode(greenLed,OUTPUT); //green LED for UnLock
  pinMode(lockPin,OUTPUT);
  //pinMode(ledPin, OUTPUT); //Debug led for Capacity


  Serial.println(F("the authorised cards are")); //display authorised cards just to demonstrate you may comment this section out
  for(int i=0;i<N;i++){ 
    Serial.print(i+1);
    Serial.print("  ");
    for(int j=0;j<4;j++){
      Serial.print(defcard[i][j],HEX);
    }
    Serial.println("");
  }
  Serial.println("");
  Serial.println(F("Scan Access Card to see Details"));
  digitalWrite(redLed,HIGH);
  digitalWrite(lockPin, LOW);
  delay(50);
}

void loop() {
  
  readsuccess = getid();
  //TouchSensor();
  int test = digitalRead(3);
  Serial.println(test);
  if(readsuccess){
 
  int match=0;
 
  //this is the part where compare the current tag with pre defined tags
    for(int i=0;i<N;i++){

      if(!memcmp(readcard,defcard[i],4)){
        match++;
        i=i+1;
      }
       
      if(match) {
        Serial.println("Found Matched Card"); //Found Card 

        if(i==1) { 
          Serial.println("Right Card");
          /*
          digitalWrite(redLed,LOW);
          digitalWrite(greenLed,HIGH);
          delay(200);
          digitalWrite(greenLed,LOW);
          delay(100); 
          digitalWrite(greenLed,HIGH);
          digitalWrite(lockPin, HIGH);
          */
          UnLock();
          delay(3000);
          Lock();
          /*
          digitalWrite(lockPin, LOW);
          digitalWrite(greenLed, LOW);
          digitalWrite(redLed,HIGH);
          */

        } 
      }

      else{ 
        /*
        digitalWrite(lockPin, LOW);
        digitalWrite(greenLed,LOW);
        digitalWrite(redLed,HIGH);
        delay(100);
        digitalWrite(redLed,LOW);
        delay(100);
        digitalWrite(redLed,HIGH);
        */
       Lock();
        Serial.println("CARD NOT Authorised");
      }
    }
  }
  delay(10);
}

//function to get the UID of the card
int getid(){  
  if(!rc.PICC_IsNewCardPresent()){
    return 0;
  }
  if(!rc.PICC_ReadCardSerial()){
    return 0;
  }
  
  Serial.println("THE UID OF THE SCANNED CARD IS:");
 
  for(int i=0;i<4;i++){
    readcard[i]=rc.uid.uidByte[i]; //storing the UID of the tag in readcard
    Serial.print(readcard[i],HEX);
  }
  Serial.println("");
  Serial.println("Now Comparing with Authorised cards");
  rc.PICC_HaltA();

  return 1;
}

void UnLock(){
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, HIGH);
  delay(200);
  digitalWrite(greenLed, LOW);
  delay(200);
  digitalWrite(greenLed, HIGH);
  digitalWrite(lockPin, HIGH);
}

void Lock(){
  digitalWrite(greenLed, LOW);
  digitalWrite(redLed, HIGH);
  delay(200);
  digitalWrite(redLed, LOW);
  delay(200);
  digitalWrite(redLed, HIGH);
  digitalWrite(lockPin, LOW);
}
/*
void TouchSensor(){
  long sensorValue = capSensor.capacitiveSensor(30);
  
  Serial.println(sensorValue);

  if(sensorValue > threshold){
    if(pass==false){
      pass = true;
      led_State = !led_State;
      digitalWrite(ledPin, led_State);
    }
    else{
      pass = false;
    }
  }
}
*/





