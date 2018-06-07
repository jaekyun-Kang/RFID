#include<SPI.h>
#include<MFRC522.h>

//creating mfrc522 instance
#define RSTPIN 9
#define SSPIN 10
MFRC522 rc(SSPIN, RSTPIN);

int readsuccess;

/* the following are the UIDs of the card which are authorised
    to know the UID of your card/tag use the example code 'DumpInfo'
    from the library mfrc522 it give the UID of the card as well as 
    other information in the card on the serial monitor of the arduino*/

    //byte defcard[4]={0xC6,0xBA,0xFD,0x1B}; // if you only want one card
//    byte defcard[][4]={{0xC6,0xBA,0xFD,0x1B},{0x5A,0x7A,0xDB,0x73},{0x53,0x60,0x6F,0x21}}; //for multiple cards
////////////////////
byte defcard[][4]={{0xC6,0xBA,0xFD,0x1B}}; //for multiple cards////  e.g. scan the card you want to pair with the reader, and the serial port will print out the UID of this card, change the number to your UID here.If your UID is C6BAFD1B, just type {0xC6,0xBA,0xFD,0x1B}.  
///////////////////
int N=1; //change this to the number of cards/tags you will use
byte readcard[4]; //stores the UID of current tag which is read

void setup() {
Serial.begin(9600);

SPI.begin();
rc.PCD_Init(); //initialize the receiver  
rc.PCD_DumpVersionToSerial(); //show details of card reader module

pinMode(6,OUTPUT); //led for authorised
pinMode(5,OUTPUT); //led for not authorised

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
}


void loop() {
  
readsuccess = getid();

if(readsuccess){
 
  int match=0;

//this is the part where compare the current tag with pre defined tags
  for(int i=0;i<N;i++){
    Serial.print("Testing Against Authorised card no: ");
    Serial.println(i+1);
    if(!memcmp(readcard,defcard[i],4)){
      match++;
      }
    
  }
    
  
   if(match)
      {Serial.println("CARD AUTHORISED");
        digitalWrite(6,HIGH);
        Serial.println("match!!!!!! STOP THE CAR!!!!!STOP THE CAR!!!!!STOP THE CAR!!!!!STOP THE CAR!!!!!STOP THE CAR!!!!!STOP THE CAR!!!!!"); //////STOP THE CAR 
        delay(2000);
        digitalWrite(6,LOW);
        delay(500); 
        
      }
     else {
      Serial.println("CARD NOT Authorised");
      digitalWrite(5,HIGH);
        delay(2000);
        digitalWrite(5,LOW);
        delay(500); 
   
      }
  
  }
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



