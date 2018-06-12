const int lockPin = 3;
const int lockOperationTime = 2500; 
void setup() {
  // put your setup code here, to run once:
  pinMode(lockPin, OUTPUT);
  pinMode(13,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  digitalWrite(lockPin, HIGH);
  digitalWrite(13,HIGH);
  delay(3000);
  digitalWrite(lockPin, LOW);
  digitalWrite(13,LOW);
  delay(3000);

  
}



