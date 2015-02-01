
#define RISCALDAMENTO 1 
#define CALIBRAZIONE 2
#define FUNZIONAMENTO 3

// Tempi delle singole fasi
unsigned long tempoDiRiscaldamento = 600000;              // 10 minuti - ma deve essere almeno 24 ore
int tempoLampeggiamentoRiscaldamento = 500;               // Mezzo secondo

unsigned long startTime; 

int status[] = {RISCALDAMENTO, CALIBRAZIONE, FUNZIONAMENTO};
int currentStatus = 0;

int statusLed = 6;

void setup() {
    Serial.begin(9600);
    
    pinMode(statusLed, OUTPUT);
    
    startTime = millis();
}

void loop() {
  switch (status[currentStatus]) {
    case RISCALDAMENTO:
      if((millis() - startTime) < tempoDiRiscaldamento){
        lampeggiaPerRiscaldamento();
      } else {
        currentStatus++;
      }
      break;
    case CALIBRAZIONE:
      // Da implementare
      Serial.println("Stato Calibrazione!!!!");
      delay(10000);
      break;
    case FUNZIONAMENTO:
      // Da implementare
      Serial.println("Stato Funzionamento!!!!");
      delay(10000);
      break;
    default: 
      Serial.println("Stato sconosciuto!!!!");
      delay(1000);
  }
}

void lampeggiaPerRiscaldamento() {
  digitalWrite(statusLed, HIGH);
  delay(tempoLampeggiamentoRiscaldamento);
  digitalWrite(statusLed, LOW);
  delay(tempoLampeggiamentoRiscaldamento);
}


