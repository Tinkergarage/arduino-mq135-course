
/*

  Programma sviluppato per far vedere l'uso di alcune strutture di controllo, operatori e funzioni

*/


// Pin associati a LED
byte ledPin[] = {3, 5, 6};  
int ledCount = sizeof( ledPin );

// Button
int pushButton = 2;

/*
  Variabili per accensione LED in sequenza
*/
int ledDelay = 65;          // Tempo in cui un LED resta acceso
int direction = 1;          // Direzione
int currentLED = 0;         // LED da accendere
unsigned long changeTime;   // Ultima momento in cui è stato accesso un LED

/*
  Variabili lampeggiamento LED
*/
int mixDelay = 3000;         // Tempo dopo cui accendere il LED random
float sinVal;                // Valore nella curva seno
int ledVal;                  // Valore di luminosità da dare al LED
int ledNumber = -1;          // LED da accendere (numerazione all'interno dell'array)
unsigned long startTime;   // Ultima momento in cui è stato accesso un LED

void setup() {
    Serial.begin(9600);
    for (int x=0; x<ledCount; x++) {
        pinMode(ledPin[x], OUTPUT); }
    pinMode(pushButton, INPUT_PULLUP);
    changeTime = millis();
    startTime = millis();
}

void loop() {
    if(ledNumber == -1){
      // Accende successivamente tutit i LED
      while((millis() - startTime) < ???){
        if (???) {
            Serial.println(changeTime);
            changeLED();
            changeTime = millis();
        }
      }

      // Seleziona un LED in modo casuale
      randomSeed(analogRead(0));
      ledNumber = random(0, ???) ;
    }
    
    switchOffLEDs();
    // Accende e spegne il LED usando la funzione seno
    for (int x=0; x<180; x++) {
      int buttonVal = digitalRead(pushButton);
      if (buttonVal == ???) {
        startTime = millis();
        changeTime = millis();
        ledNumber = -1;
        break;
      }
      else {
        sinVal = (sin(x*(3.1412/180)));
        ledVal = int(sinVal*255);
        analogWrite(ledPin[ledNumber], ledVal);
        delay(5);
      }
    }

    delay(1);
}

/*
  accende il LED successivo
*/
void changeLED() {
    switchOffLEDs();
    digitalWrite(ledPin[currentLED], HIGH);
    currentLED += direction;

    if (currentLED == (ledCount - 1)) {direction = -1;}
    if (currentLED == 0) {direction = 1;}
}

/*
  Spegne i LED definiti nell'array
*/
void switchOffLEDs(){
//    Serial.println("switchOffLEDs()\0");
    for (int x=0; x<ledCount; x++) {
         digitalWrite(ledPin[x], LOW);
    }
}


