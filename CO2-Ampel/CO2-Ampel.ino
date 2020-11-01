#include <MHZ19.h>
#include <SoftwareSerial.h>

// pin for uart reading
#define RX_PIN 2
#define TX_PIN 3
#define BAUDRATE 9600

MHZ19 myMHZ19;
SoftwareSerial mySerial(RX_PIN, TX_PIN);

//MHZ-19B
//Color  Pin   Terminal pin Definition
//Red    Pin 1 Analog Output Vo
//Black  Pin 2 None
//Blue   Pin 3 Negtive Pole(GND)
//Green  Pin 4 Positive Pole(Vin)
//Yellow Pin 5 UART(RXD)TTL Level data input
//Brown  Pin 6 UART(TXD)TTL Level data output
//White  Pin 7 None

#define DELAYTIME 10000
unsigned long previousMillis = 0;

const int R1p = 7;
const int R2p = 6;
const int R3p = 5;
// const int R4p = 4;

int CO2value = 0;
int temperature = 0;

void setup() {
  Serial.begin(9600);
  delay(100);
  mySerial.begin(BAUDRATE);
  delay(100);
  myMHZ19.begin(mySerial);
  delay(100);
  myMHZ19.autoCalibration();
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(R1p, OUTPUT);
  pinMode(R2p, OUTPUT);
  pinMode(R3p, OUTPUT);
  // pinMode(R4p, OUTPUT);
}

void loop() {
  if ((millis() - previousMillis) > DELAYTIME)
  {
    CO2value = myMHZ19.getCO2();
    temperature = myMHZ19.getTemperature();
    if (CO2value < 300) {
      digitalWrite(R3p, HIGH);
      digitalWrite(R1p, LOW);
      digitalWrite(R2p, LOW);
    } else if (CO2value < 700) {
      digitalWrite(R1p, HIGH);
      digitalWrite(R2p, LOW);
      digitalWrite(R3p, LOW);
    } else if (CO2value < 1000) {
      digitalWrite(R2p, HIGH);
      digitalWrite(R1p, LOW);
      digitalWrite(R3p, LOW);
    } else {
      digitalWrite(R3p, HIGH);
      digitalWrite(R1p, LOW);
      digitalWrite(R2p, LOW);
    }
    Serial.print(F("CO2_PPM: "));
    if (CO2value > 0) {
      Serial.print(CO2value);
    } else {
      Serial.print(F("Error"));
    }
    Serial.print(", ");
    Serial.print(F("Temp_C: "));
    if (temperature > 0) {
      Serial.print(temperature);
    } else {
      Serial.print(F("Error"));
    }
    Serial.println();
    previousMillis = millis();
  }
}
