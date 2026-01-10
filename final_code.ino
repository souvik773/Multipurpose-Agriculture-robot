/******************* WiFi Agricultural Robot - NodeMCU + 4x BTS7960 + Blynk IoT ********************/

//#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"       // Replace with your Blynk Template ID
//#define BLYNK_DEVICE_NAME "AgriRobot"              // Replace with your Blynk Template Name
//#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"   // Replace with your Device Auth Token

#define BLYNK_TEMPLATE_ID "TMPLxxxxxx"
#define BLYNK_TEMPLATE_NAME "WiFi RC Car"
#define BLYNK_AUTH_TOKEN "your_auth_token_here"


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

// ----- WiFi Credentials -----
char ssid[] = "YOUR_WIFI_SSID";   // WiFi Name
char pass[] = "YOUR_WIFI_PASS";   // WiFi Password

// ----- Drive Motor Pins -----
const int L_RPWM = D1;
const int L_LPWM = D2;
const int R_RPWM = D3;
const int R_LPWM = D4;

// ----- Weed Cutter Motor (BTS) -----
const int CUT_RPWM = D5;
const int CUT_LPWM = D6;

// ----- Water Pump Motor (BTS) -----
const int PUMP_RPWM = D7;
const int PUMP_LPWM = D8;

// ----- Servo for Seed Sowing -----
Servo seedServo;
const int servoPin = D0;
int servoCenter = 90;   // Center position
int servoDown = 60;     // Active position for sowing

// ----- Blynk Virtual Pins -----
#define V_FORWARD     V1
#define V_BACKWARD    V2
#define V_LEFT        V3
#define V_RIGHT       V4
#define V_WEEDCUT     V5
#define V_PUMP        V6
#define V_SEEDSERVO   V7
#define V_FL          V8
#define V_FR          V9
#define V_BL          V10
#define V_BR          V11

// ----- Helper Functions -----
void stopMotors() {
  digitalWrite(L_RPWM, LOW);
  digitalWrite(L_LPWM, LOW);
  digitalWrite(R_RPWM, LOW);
  digitalWrite(R_LPWM, LOW);
}

void forward() {
  digitalWrite(L_RPWM, HIGH);
  digitalWrite(L_LPWM, LOW);
  digitalWrite(R_RPWM, HIGH);
  digitalWrite(R_LPWM, LOW);
}

void backward() {
  digitalWrite(L_RPWM, LOW);
  digitalWrite(L_LPWM, HIGH);
  digitalWrite(R_RPWM, LOW);
  digitalWrite(R_LPWM, HIGH);
}

void left() {
  digitalWrite(L_RPWM, LOW);
  digitalWrite(L_LPWM, HIGH);
  digitalWrite(R_RPWM, HIGH);
  digitalWrite(R_LPWM, LOW);
}

void right() {
  digitalWrite(L_RPWM, HIGH);
  digitalWrite(L_LPWM, LOW);
  digitalWrite(R_RPWM, LOW);
  digitalWrite(R_LPWM, HIGH);
}

void forwardLeft() {
  digitalWrite(L_RPWM, LOW);
  digitalWrite(L_LPWM, LOW);
  digitalWrite(R_RPWM, HIGH);
  digitalWrite(R_LPWM, LOW);
}

void forwardRight() {
  digitalWrite(L_RPWM, HIGH);
  digitalWrite(L_LPWM, LOW);
  digitalWrite(R_RPWM, LOW);
  digitalWrite(R_LPWM, LOW);
}

void backwardLeft() {
  digitalWrite(L_RPWM, LOW);
  digitalWrite(L_LPWM, HIGH);
  digitalWrite(R_RPWM, LOW);
  digitalWrite(R_LPWM, LOW);
}

void backwardRight() {
  digitalWrite(L_RPWM, LOW);
  digitalWrite(L_LPWM, LOW);
  digitalWrite(R_RPWM, LOW);
  digitalWrite(R_LPWM, HIGH);
}

// ----- Weed Cutter & Pump Control -----
void weedCutterOn() {
  digitalWrite(CUT_RPWM, HIGH);
  digitalWrite(CUT_LPWM, LOW);
}

void weedCutterOff() {
  digitalWrite(CUT_RPWM, LOW);
  digitalWrite(CUT_LPWM, LOW);
}

void pumpOn() {
  digitalWrite(PUMP_RPWM, HIGH);
  digitalWrite(PUMP_LPWM, LOW);
}

void pumpOff() {
  digitalWrite(PUMP_RPWM, LOW);
  digitalWrite(PUMP_LPWM, LOW);
}

// ----- Seed Servo -----
void servoDownAction() {
  seedServo.write(servoDown);
}

void servoUpAction() {
  seedServo.write(servoCenter);
}

// ----- Blynk Control -----
BLYNK_WRITE(V_FORWARD)   { if (param.asInt()) forward(); else stopMotors(); }
BLYNK_WRITE(V_BACKWARD)  { if (param.asInt()) backward(); else stopMotors(); }
BLYNK_WRITE(V_LEFT)      { if (param.asInt()) left(); else stopMotors(); }
BLYNK_WRITE(V_RIGHT)     { if (param.asInt()) right(); else stopMotors(); }
BLYNK_WRITE(V_FL)        { if (param.asInt()) forwardLeft(); else stopMotors(); }
BLYNK_WRITE(V_FR)        { if (param.asInt()) forwardRight(); else stopMotors(); }
BLYNK_WRITE(V_BL)        { if (param.asInt()) backwardLeft(); else stopMotors(); }
BLYNK_WRITE(V_BR)        { if (param.asInt()) backwardRight(); else stopMotors(); }

BLYNK_WRITE(V_WEEDCUT)   { if (param.asInt()) weedCutterOn(); else weedCutterOff(); }
BLYNK_WRITE(V_PUMP)      { if (param.asInt()) pumpOn(); else pumpOff(); }

BLYNK_WRITE(V_SEEDSERVO) { if (param.asInt()) servoDownAction(); else servoUpAction(); }

// ----- Setup -----
void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  pinMode(L_RPWM, OUTPUT);
  pinMode(L_LPWM, OUTPUT);
  pinMode(R_RPWM, OUTPUT);
  pinMode(R_LPWM, OUTPUT);
  pinMode(CUT_RPWM, OUTPUT);
  pinMode(CUT_LPWM, OUTPUT);
  pinMode(PUMP_RPWM, OUTPUT);
  pinMode(PUMP_LPWM, OUTPUT);

  seedServo.attach(servoPin);
  seedServo.write(servoCenter);

  stopMotors();
  weedCutterOff();
  pumpOff();

  Serial.println("AgriRobot Ready - Connected to Blynk!");
}

// ----- Loop -----
void loop() {
  Blynk.run();
}
