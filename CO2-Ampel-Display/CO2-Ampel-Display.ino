#include <NOKIA5110_TEXT.h>
// LCD Nokia 5110 pinout left to right
// RST / CE / DC / DIN / CLK / VCC /LIGHT / GND
// pin 13 - Serial clock out (SCLK)
// pin 11 - Serial data out (DIN)
// pin 12 - Data/Command select (D/C)
// pin 10 - LCD chip select (CS)
// pin 9 - LCD reset (RST)
#define RST 10
#define CE 9
#define DC 12
#define DIN 11
#define CLK 13

// Create an LCD object
NOKIA5110_TEXT mylcd(RST, CE, DC, DIN, CLK);

#define dinverse  false // set to true to invert display pixel color
#define dcontrast 0xB2 // default is 0xBF set in LCDinit, Try 0xB1 <-> 0xBF if your display is too dark
#define dbias 0x13 // LCD bias mode 1:48: Try 0x12 or 0x13 or 0x14

#include <MHZ19.h>
#include <SoftwareSerial.h>
#define RX_PIN 2                                           // Rx pin which the MHZ19 Tx pin is attached to
#define TX_PIN 3                                           // Tx pin which the MHZ19 Rx pin is attached to
#define BAUDRATE 9600                                      // Device to MH-Z19 Serial baudrate (should not be changed)
MHZ19 myMHZ19;                                             // Constructor for library
SoftwareSerial mySerial(RX_PIN, TX_PIN);                   // (Uno example) create device to MH-Z19 serial
#define ANALOGPIN A5

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
  pinMode(ANALOGPIN, INPUT_PULLUP);
  Serial.begin(9600);
  delay(100);
  mylcd.LCDInit(dinverse, dcontrast, dbias); // init  the lCD
  mylcd.LCDClear(0x00); // Clear whole screen
  mylcd.LCDFont(9);

  mySerial.begin(BAUDRATE);                               // (Uno example) device to MH-Z19 serial start
  //myMHZ19.printCommunication();
  myMHZ19.begin(mySerial);                                // *Serial(Stream) refence must be passed to library begin().
  myMHZ19.setRange(5000);
  myMHZ19.autoCalibration(true);

  char myVersion[4];          
  myMHZ19.getVersion(myVersion);

  Serial.print("\nFirmware Version: ");
  for(byte i = 0; i < 4; i++)
  {
    Serial.print(myVersion[i]);
    if(i == 1)
      Serial.print(".");    
  }
   Serial.println("");

   Serial.print("Range: ");
   Serial.println(myMHZ19.getRange());   
   Serial.print("Background CO2: ");
   Serial.println(myMHZ19.getBackgroundCO2());
   Serial.print("Temperature Cal: ");
   Serial.println(myMHZ19.getTempAdjustment());
   Serial.print("ABC Status: "); myMHZ19.getABC() ? Serial.println("ON") :  Serial.println("OFF");

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
    //float adjustedADC = analogRead(A0);
    //Serial.print("Analog raw: ");
    //Serial.println(adjustedADC);
    //adjustedADC = 6.4995 * adjustedADC - 590.53; // format; y=mx+c
    //Serial.print("Analog CO2: ");
    //Serial.println(adjustedADC);
    if (CO2value > 0) {
      mylcd.LCDClear(0x00);
      mylcd.LCDgotoXY(0, 0);
      char buf[6];
      mylcd.LCDString(itoa(CO2value, buf, 10));
      //mylcd.LCDString(",");
      //mylcd.LCDString(itoa(temperature, buf, 10));
      Serial.print(CO2value);
      Serial.print(", ");
      Serial.println(temperature);
    } else {
      //mylcd.LCDString("Error");
      Serial.print(F("Error"));
    }
    previousMillis = millis();
  }
}
