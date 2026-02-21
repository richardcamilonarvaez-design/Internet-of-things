
int LEDS[3]={13, 12, 11};
  
void setup() {
  for(int r=0; r<3; r++){
  pinMode(LEDS[r], OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int r=0; r<3; r++){
  	digitalWrite(LEDS[r], HIGH);
    delay(1000);
  	digitalWrite(LEDS[r], LOW);
  	delay(1000);
  }
}