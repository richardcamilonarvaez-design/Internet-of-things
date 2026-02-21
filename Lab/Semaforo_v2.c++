int leds[3]={13,12,11};
int boton=2;

int modo=0;
bool ultimo=HIGH;

unsigned long tiempo=0;
int estado=0;
bool blink=false;

void setup(){
  for(int i=0;i<3;i++) pinMode(leds[i],OUTPUT);
  pinMode(boton,INPUT_PULLUP);

  tiempo=millis();
  digitalWrite(leds[0],HIGH);
}

void loop(){

  bool lectura=digitalRead(boton);

  if(ultimo==HIGH && lectura==LOW){
    modo++;
    if(modo>4) modo=0;
    tiempo=millis();
  }

  ultimo=lectura;

  switch(modo){
    case 0: semaforo(); break;
    case 1: solo(0); break;
    case 2: parpadeo(); break;
    case 3: parpadeoAmarillo(); break;
    case 4: apagar(); break;
  }
}

void solo(int n){
  for(int i=0;i<3;i++)
    digitalWrite(leds[i], i==n);
}

void apagar(){
  for(int i=0;i<3;i++)
    digitalWrite(leds[i],LOW);
}

void parpadeo(){
  if(millis()-tiempo>500){
    tiempo=millis();
    blink=!blink;
    for(int i=0;i<3;i++)
      digitalWrite(leds[i],blink);
  }
}

void parpadeoAmarillo(){
  if(millis()-tiempo>500){
    tiempo=millis();
    blink=!blink;

    digitalWrite(leds[0],LOW);
    digitalWrite(leds[1],blink);
    digitalWrite(leds[2],LOW);
  }
}

void semaforo(){
  if(millis()-tiempo>2000){
    tiempo=millis();
    estado++;
    if(estado>2) estado=0;
  }

  solo(estado);
}