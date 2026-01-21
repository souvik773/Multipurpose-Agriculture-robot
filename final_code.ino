/******** ESP8266 AGRICULTURE ROBOT – FULL VERSION ********/

#define BLYNK_TEMPLATE_ID   "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "YOUR_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN    "YOUR_BLYNK_AUTH_TOKEN"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiManager.h>
#include <Servo.h>

// ---------------- SAFE PIN MAPPING ----------------
// Drive Motors (BTS7960 – Left & Right)
#define L_RPWM   D1
#define L_LPWM   D2
#define R_RPWM   D5
#define R_LPWM   D6

// Weed Cutter (BTS7960)
#define CUT_RPWM D7
#define CUT_LPWM D0

// Water Pump (BTS7960)
#define PUMP_RPWM D3
#define PUMP_LPWM D4

// Seed Servo
#define SERVO_PIN D9   // GPIO3

// ---------------- Servo ----------------
Servo seedServo;
int servoUp = 90;
int servoDown = 60;

// ---------------- Blynk Virtual Pins ----------------
#define V_FORWARD   V1
#define V_BACKWARD  V2
#define V_LEFT      V3
#define V_RIGHT     V4

#define V_WEEDCUT   V5
#define V_PUMP      V6
#define V_SERVO     V7

#define V_FL        V8
#define V_FR        V9
#define V_BL        V10
#define V_BR        V11

// ---------------- MOTOR FUNCTIONS ----------------
void stopMotors() {
  digitalWrite(L_RPWM, LOW);
  digitalWrite(L_LPWM, LOW);
  digitalWrite(R_RPWM, LOW);
  digitalWrite(R_LPWM, LOW);
}

// Straight movements
void forward() {
  stopMotors();
  digitalWrite(L_RPWM, HIGH);
  digitalWrite(R_RPWM, HIGH);
}

void backward() {
  stopMotors();
  digitalWrite(L_LPWM, HIGH);
  digitalWrite(R_LPWM, HIGH);
}

void left() {
  stopMotors();
  digitalWrite(L_LPWM, HIGH);
  digitalWrite(R_RPWM, HIGH);
}

void right() {
  stopMotors();
  digitalWrite(L_RPWM, HIGH);
  digitalWrite(R_LPWM, HIGH);
}

// Diagonal movements
void forwardLeft() {
  stopMotors();
  digitalWrite(R_RPWM, HIGH);
}

void forwardRight() {
  stopMotors();
  digitalWrite(L_RPWM, HIGH);
}

void backwardLeft() {
  stopMotors();
  digitalWrite(R_LPWM, HIGH);
}

void backwardRight() {
  stopMotors();
  digitalWrite(L_LPWM, HIGH);
}

// ---------------- CUTTER & PUMP ----------------
void cutterOn() {
  digitalWrite(CUT_RPWM, HIGH);
  digitalWrite(CUT_LPWM, LOW);
}

void cutterOff() {
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

// ---------------- BLYNK CONTROLS ----------------
BLYNK_WRITE(V_FORWARD)  { param.asInt() ? forward()        : stopMotors(); }
BLYNK_WRITE(V_BACKWARD) { param.asInt() ? backward()       : stopMotors(); }
BLYNK_WRITE(V_LEFT)     { param.asInt() ? left()           : stopMotors(); }
BLYNK_WRITE(V_RIGHT)    { param.asInt() ? right()          : stopMotors(); }

BLYNK_WRITE(V_FL)       { param.asInt() ? forwardLeft()    : stopMotors(); }
BLYNK_WRITE(V_FR)       { param.asInt() ? forwardRight()   : stopMotors(); }
BLYNK_WRITE(V_BL)       { param.asInt() ? backwardLeft()   : stopMotors(); }
BLYNK_WRITE(V_BR)       { param.asInt() ? backwardRight()  : stopMotors(); }

BLYNK_WRITE(V_WEEDCUT)  { param.asInt() ? cutterOn()       : cutterOff(); }
BLYNK_WRITE(V_PUMP)     { param.asInt() ? pumpOn()         : pumpOff(); }

BLYNK_WRITE(V_SERVO) {
  param.asInt() ? seedServo.write(servoDown) : seedServo.write(servoUp);
}

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(9600);

  pinMode(L_RPWM, OUTPUT);
  pinMode(L_LPWM, OUTPUT);
  pinMode(R_RPWM, OUTPUT);
  pinMode(R_LPWM, OUTPUT);

  pinMode(CUT_RPWM, OUTPUT);
  pinMode(CUT_LPWM, OUTPUT);
  pinMode(PUMP_RPWM, OUTPUT);
  pinMode(PUMP_LPWM, OUTPUT);

  stopMotors();
  cutterOff();
  pumpOff();

  seedServo.attach(SERVO_PIN);
  seedServo.write(servoUp);

  // -------- WiFiManager --------
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  wm.setConfigPortalTimeout(180);

  if (!wm.autoConnect("ESP8266-Setup")) {
    ESP.restart();
  }

  // -------- Blynk --------
  Blynk.config(BLYNK_AUTH_TOKEN);
  Blynk.connect();
}

// ---------------- LOOP ----------------
void loop() {
  Blynk.run();
}

