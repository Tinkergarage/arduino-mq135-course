#include <EEPROM.h>

#define RISCALDAMENTO 1 
#define CALIBRAZIONE 2
#define FUNZIONAMENTO 3

// Dati del riscaldamento
//unsigned long tempoDiRiscaldamento = 600000;              // 10 minuti - ma deve essere almeno 24 ore
unsigned long tempoDiRiscaldamento = 6000;
int tempoLampeggiamentoRiscaldamento = 500;               // Mezzo secondo

// Dati della calibrazione
//unsigned long tempoTraCalibrazioni = 300000;              // 5 minuti 
unsigned long tempoTraCalibrazioni = 3000;
int tempoLampeggiamentoCalibrazione = 200;               // 2 decimi di secondo
const int numeroLettureCalibrazione = 7;
float lettureCalibrazione[numeroLettureCalibrazione];
int numeroLettura = 0;

// Dati dello stato Funzionamento
float valoreLettura;                                        // Valore letto dal sensore
float valorePpm;
int buttonVal;

// Variabili di calibrazione
float ro = 0;                                               // Resistenza di Calibrazione
const float ppm = 398.78;
const float a = 116.6020682;
const float b = -2.769034857;
float voltage;
float rs;

// variabili usate per controllare il passare del tempo
unsigned long startTime; 
unsigned long readingTime; 

// Stato
int status[] = {RISCALDAMENTO, CALIBRAZIONE, FUNZIONAMENTO};
int currentStatus = 0;

// Pin
int statusLed1 = 4;
int statusLed2 = 5;
int statusLed3 = 6;
int sensorPin = A0;
int pushButton = 2;

void setup() {
    Serial.begin(9600);
    
    pinMode(statusLed1, OUTPUT);
    pinMode(statusLed2, OUTPUT);
    pinMode(statusLed3, OUTPUT);
    pinMode(pushButton, INPUT_PULLUP);
    
    startTime = millis();
    
    int value = EEPROM.read(0);
    Serial.print("value = ");
    Serial.println(value);
    if( value == 'C' ){
      Serial.print("Already calibrated!!!");
      currentStatus=2;
      ro = readRo();
      Serial.print("ro = ");
      Serial.println(ro);
    }
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
          for(int numeroLettura=0; numeroLettura < numeroLettureCalibrazione; numeroLettura++){
            Serial.print("lettureCalibrazione[numeroLettura] = ");
            Serial.println(lettureCalibrazione[numeroLettura]);
            valoreMedio+=lettureCalibrazione[numeroLettura];
          }
          valoreMedio = valoreMedio / numeroLettureCalibrazione;
          Serial.print("valoreMedio = ");
          Serial.println(valoreMedio);
          
          voltage = (valoreMedio / 1024.0) * 5.0;
          Serial.print("voltage = ");
          Serial.println(voltage);
          
          rs = 10000 * ((5 / voltage ) - 1);
          Serial.print("rs = ");
          Serial.println(rs);
          
          ro = rs * pow( (a / ppm) , (1 / b ) );
          Serial.print("ro = ");
          Serial.println(ro);
          currentStatus++;

          // Salvo il dato di calibrazione nella EEPROM
          EEPROM.write(0, 'C');
          writeRo(ro);
        }
      }
      break;
      
    case FUNZIONAMENTO:
      digitalWrite(statusLed1, HIGH);
      digitalWrite(statusLed2, LOW);
      digitalWrite(statusLed3, LOW);
      
      buttonVal = digitalRead(pushButton);
      if (buttonVal == LOW) {
        digitalWrite(statusLed1, LOW);

        ro=0;
        startTime = millis();

        EEPROM.write(0, ' ');
        for(int i=0; i<4; i++){          
          EEPROM.write(i+1, B00000000);
        }

        currentStatus=0;
        
        Serial.println("Resetted");
      } else {
        valoreLettura = analogRead(sensorPin);
        Serial.print("valoreLettura = ");
        Serial.println( valoreLettura );
  
        voltage = (valoreLettura / 1024.0) * 5.0;
        Serial.print("voltage = ");
        Serial.println(voltage);
        
        rs = 10000 * ((5 / voltage ) - 1);
        Serial.print("rs = ");
        Serial.println(rs);
  
        valorePpm = a * pow( (rs / ro), b);
        Serial.print("valorePpm = ");
        Serial.println(valorePpm);
        
        if( valorePpm > 800 ) {
          digitalWrite(statusLed2, HIGH);
        }
        if( valorePpm > 1000 ) {
          digitalWrite(statusLed3, HIGH);
        }
      }
      delay(10);
      break;
      
    default: 
      Serial.println("Stato sconosciuto!!!!");
      delay(1000);
  }
}

void lampeggia(unsigned long tempo) {
  digitalWrite(statusLed3, HIGH);
  delay(tempo);
  digitalWrite(statusLed3, LOW);
  delay(tempo);
}

void writeRo(float ro){
  union {
    byte b[4];
    float f;
  } data;

  data.f = ro;

  for(int i=0; i<4; i++){
    EEPROM.write(i+1, data.b[i]);
  }
}

float readRo(){
  union {
    byte b[4];
    float f;
  } data;

  for(int i=0; i<4; i++){
    data.b[i] = EEPROM.read(i+1);
  }

  return data.f;
}

