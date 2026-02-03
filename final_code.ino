/******** ESP8266 AGRICULTURE ROBOT – FULL VERSION ********/

#define BLYNK_TEMPLATE_ID   "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "YOUR_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN    "YOUR_BLYNK_AUTH_TOKEN"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiManager.h>
#include <Servo.h>

// ---------- PWM SPEED ----------
#define MOTOR_PWM 700   // 0–1023 (safe start)

// ---------- DRIVE MOTORS (BTS7960) ----------
#define L_RPWM   D1   // GPIO5
#define L_LPWM   D2   // GPIO4
#define R_RPWM   D5   // GPIO14
#define R_LPWM   D6   // GPIO12

// ---------- WEED CUTTER (BTS7960) ----------
#define CUT_RPWM D7   // GPIO13
#define CUT_LPWM D8   // GPIO15   (CHANGED from D0 ❌)

// ---------- WATER PUMP (BTS7960) ----------
#define PUMP_RPWM D3  // GPIO0
#define PUMP_LPWM D4  // GPIO2

// ---------- SERVO ----------
#define SERVO_PIN D9  // GPIO3 (RX)

// ---------- SERVO ----------
Servo seedServo;
int servoUp = 90;
int servoDown = 60;

// ---------- MOTOR CONTROL ----------
void stopDrive() {
  analogWrite(L_RPWM, 0);
  analogWrite(L_LPWM, 0);
  analogWrite(R_RPWM, 0);
  analogWrite(R_LPWM, 0);
}

void forward() {
  stopDrive();
  analogWrite(L_RPWM, MOTOR_PWM);
  analogWrite(R_RPWM, MOTOR_PWM);
}

void backward() {
  stopDrive();
  analogWrite(L_LPWM, MOTOR_PWM);
  analogWrite(R_LPWM, MOTOR_PWM);
}

void left() {
  stopDrive();
  analogWrite(L_LPWM, MOTOR_PWM);
  analogWrite(R_RPWM, MOTOR_PWM);
}

void right() {
  stopDrive();
  analogWrite(L_RPWM, MOTOR_PWM);
  analogWrite(R_LPWM, MOTOR_PWM);
}

// ---------- CUTTER ----------
void cutterOn() {
  analogWrite(CUT_RPWM, MOTOR_PWM);
  analogWrite(CUT_LPWM, 0);
}

void cutterOff() {
  analogWrite(CUT_RPWM, 0);
  analogWrite(CUT_LPWM, 0);
}

// ---------- PUMP ----------
void pumpOn() {
  analogWrite(PUMP_RPWM, MOTOR_PWM);
  analogWrite(PUMP_LPWM, 0);
}

void pumpOff() {
  analogWrite(PUMP_RPWM, 0);
  analogWrite(PUMP_LPWM, 0);
}

// ---------- BLYNK ----------
#define V_FORWARD   V1
#define V_BACKWARD  V2
#define V_LEFT      V3
#define V_RIGHT     V4
#define V_WEEDCUT   V5
#define V_PUMP      V6
#define V_SERVO     V7

BLYNK_WRITE(V_FORWARD)  { param.asInt() ? forward()  : stopDrive(); }
BLYNK_WRITE(V_BACKWARD) { param.asInt() ? backward() : stopDrive(); }
BLYNK_WRITE(V_LEFT)     { param.asInt() ? left()     : stopDrive(); }
BLYNK_WRITE(V_RIGHT)    { param.asInt() ? right()    : stopDrive(); }

BLYNK_WRITE(V_WEEDCUT)  { param.asInt() ? cutterOn() : cutterOff(); }
BLYNK_WRITE(V_PUMP)     { param.asInt() ? pumpOn()   : pumpOff(); }

BLYNK_WRITE(V_SERVO) {
  param.asInt() ? seedServo.write(servoDown)
                : seedServo.write(servoUp);
}

// ---------- SETUP ----------
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

  stopDrive();
  cutterOff();
  pumpOff();

  seedServo.attach(SERVO_PIN);
  seedServo.write(servoUp);

  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  wm.setConfigPortalTimeout(180);
  if (!wm.autoConnect("ESP8266-Setup")) ESP.restart();

  Blynk.config(BLYNK_AUTH_TOKEN);
  Blynk.connect();
}

void loop() {
  Blynk.run();
}
