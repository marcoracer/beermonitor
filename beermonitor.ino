/*
    Beer Monitor Driver firmware version 1.0
    Marco Borges and Luis Salgado, BH 2018
*/

#include <OneWire.h>
#include <DallasTemperature.h>

const int ver = 1;
const bool debug = HIGH;  // debugging flag

int rele1 = PD2;
int rele2 = PD3;
int rele3 = PD4;
int rele4 = PD5;
int rele5 = PD6;
int rele6 = PD7;
//int rele7 = PD6;
//int rele8 = PD7;

// plugged into port D1 on the Arduino Nano (a 4.7K resistor is necessary)
OneWire oneWire(PD1);
DallasTemperature sensors(&oneWire);
DeviceAddress insideThermometer;

byte releStates;
unsigned long lastActivity;

void setup() {
  // put your setup code here, to run once:
  pinMode(rele1, OUTPUT);
  pinMode(rele2, OUTPUT);
  pinMode(rele3, OUTPUT);
  pinMode(rele4, OUTPUT);
  pinMode(rele5, OUTPUT);
  pinMode(rele6, OUTPUT);
  // pinMode(rele7, OUTPUT);
  // pinMode(rele8, OUTPUT);
  digitalWrite(rele1, LOW);
  digitalWrite(rele2, LOW);
  digitalWrite(rele3, LOW);
  digitalWrite(rele4, LOW);
  digitalWrite(rele5, LOW);
  digitalWrite(rele6, LOW);
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  sensors.begin();
  if (debug) {
    Serial.println("BEER MONITOR READY");
    Serial.print("Locating devices...");
    Serial.print("Found ");
    Serial.print(sensors.getDeviceCount(), DEC);
    Serial.println(" devices.");
  }
  if (!sensors.getAddress(insideThermometer, 0))
  {
    if (debug) {
      Serial.println("Unable to find address for Device 0");
    }
  }
  if (debug) {
    Serial.print("Device 0 Address: ");
    printAddress(insideThermometer);
    Serial.println();
  }
  // sensors.setResolution(insideThermometer, 9);
}

void loop() {
  static int s = -1;  // state
  int val = 0;        // value read
  float celsius, raw;

  if (Serial.available() > 0) {
    val = Serial.read();
    lastActivity = millis();
    s = -2;
  }

  switch (s) {
    case -2:
      if (val >= 90 && val <= 118)
        s = 10 * (val - 65);
      if (debug) {
        Serial.print("S= ");
        Serial.println(s);
      }
      break;
    case 250:      /* Get Software Version */
      s = -1;
      break;

    case 260:      /* Get Relay States */
      s = -1;
      break;

    case 280:      /* Get DC Voltage */
      s = -1;
      break;

    case 320:      /* Get Temperature Raw */
      s = -1;
      break;

    case 330:      /* Get Temperature Char */
      sensors.requestTemperatures();
      celsius = sensors.getTempC(insideThermometer);
      Serial.print(celsius);
      s = -1;
      break;

    case 350:      /* Set All Relays to One */
      digitalWrite(rele1, HIGH);
      digitalWrite(rele2, HIGH);
      digitalWrite(rele3, HIGH);
      digitalWrite(rele4, HIGH);
      digitalWrite(rele5, HIGH);
      digitalWrite(rele6, HIGH);
      s = -1;
      break;

    case 360:      /* Set Relay 1 (D2) to One */
      digitalWrite(rele1, HIGH);
      s = -1;
      break;

    case 370:      /* Set Relay 2 (D3) to One */
      digitalWrite(rele2, HIGH);
      s = -1;
      break;

    case 380:      /* Set Relay 3 (D4) to One */
      digitalWrite(rele3, HIGH);
      s = -1;
      break;

    case 390:      /* Set Relay 4 (D5) to One */
      digitalWrite(rele4, HIGH);
      s = -1;
      break;

    case 400:      /* Set Relay 5 (D6) to One */
      digitalWrite(rele5, HIGH);
      s = -1;
      break;

    case 410:      /* Set Relay 6 (D7) to One */
      digitalWrite(rele6, HIGH);
      s = -1;
      break;

    case 450:      /* Set All Relays to Zero */
      digitalWrite(rele1, LOW);
      digitalWrite(rele2, LOW);
      digitalWrite(rele3, LOW);
      digitalWrite(rele4, LOW);
      digitalWrite(rele5, LOW);
      digitalWrite(rele6, LOW);
      s = -1;
      break;

    case 460:      /* Set Relay 1 (D2) to Zero */
      digitalWrite(rele1, LOW);
      s = -1;
      break;

    case 470:      /* Set Relay 2 (D3) to Zero */
      digitalWrite(rele2, LOW);
      s = -1;
      break;

    case 480:      /* Set Relay 3 (D4) to Zero */
      digitalWrite(rele3, LOW);
      s = -1;
      break;

    case 490:      /* Set Relay 4 (D5) to Zero */
      digitalWrite(rele4, LOW);
      s = -1;
      break;

    case 500:      /* Set Relay 5 (D6) to Zero */
      digitalWrite(rele5, LOW);
      s = -1;
      break;

    case 510:      /* Set Relay 6 (D7) to Zero */
      digitalWrite(rele6, LOW);
      s = -1;
      break;
  }
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
