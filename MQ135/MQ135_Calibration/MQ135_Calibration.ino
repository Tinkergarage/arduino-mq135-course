
#define RISCALDAMENTO 1 
#define CALIBRAZIONE 2
#define FUNZIONAMENTO 3

// Dati del riscaldamento
unsigned long tempoDiRiscaldamento = 600000;              // 10 minuti - ma deve essere almeno 24 ore
int tempoLampeggiamentoRiscaldamento = 500;               // Mezzo secondo

// Dati della calibrazione
unsigned long tempoTraCalibrazioni = 300000;              // 5 minuti 
int tempoLampeggiamentoCalibrazione = 200;               // 2 decimi di secondo
const int numeroLettureCalibrazione = 7;
float lettureCalibrazione[numeroLettureCalibrazione - 1];
int numeroLettura = 0;

float ro;                                                  // Resistenza di Calibrazione
const float ppm = 398.78;
const float a = 116.6020682;
const float b = -2.769034857;

// variabili usate per controllare il passae del tempo
unsigned long startTime; 
unsigned long readingTime; 

// Stato
int status[] = {RISCALDAMENTO, CALIBRAZIONE, FUNZIONAMENTO};
int currentStatus = 0;

// Pin
int statusLed = 6;
int sensorPin = A0;

void setup() {
    Serial.begin(9600);
    
    pinMode(statusLed, OUTPUT);
    
    startTime = millis();
}

void loop() {
  switch (status[currentStatus]) {
    case RISCALDAMENTO:
      if((millis() - startTime) < tempoDiRiscaldamento){
        lampeggia(tempoLampeggiamentoRiscaldamento);
      } else {
        currentStatus++;
        readingTime = millis();
      }
      break;
    case CALIBRAZIONE:
      if((millis() - readingTime) < tempoTraCalibrazioni){
        lampeggia(tempoLampeggiamentoCalibrazione);
      } else {
        lettureCalibrazione[numeroLettura] = analogRead(sensorPin);
  
        numeroLettura++;
        Serial.print("Lettura numero ");
        Serial.println(numeroLettura);
  
        if(numeroLettura < numeroLettureCalibrazione){
          readingTime = millis();
        } else {
          float valoreMedio = 0;
          float resistenza;
          for(numeroLettura=0; numeroLettura < numeroLettureCalibrazione; numeroLettura++){
            Serial.print("lettureCalibrazione[numeroLettura] = ");
            Serial.println(lettureCalibrazione[numeroLettura]);
            valoreMedio+=lettureCalibrazione[numeroLettura];
          }
          valoreMedio = valoreMedio / numeroLettureCalibrazione;
          Serial.print("valoreMedio = ");
          Serial.println(valoreMedio);
          
          float voltage = (valoreMedio / 1024.0) * 5.0;
          Serial.print("voltage = ");
          Serial.println(voltage);
          
          float rs = 10000 * ((5 / voltage ) - 1);
          Serial.print("rs = ");
          Serial.println(rs);
          
          ro = rs * pow( (a / ppm) , (1 / b ) );
          Serial.print("ro = ");
          Serial.println(ro);
          currentStatus++;
        }
      }
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

void lampeggia(unsigned long tempo) {
  digitalWrite(statusLed, HIGH);
  delay(tempo);
  digitalWrite(statusLed, LOW);
  delay(tempo);
}



