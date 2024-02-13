////This is the bike-blinker-original Dec 2023 version
//as simple as it gets, with many optimizations and streamlines
//this base-variant uses a directly attached three-position switch
//to signal left, right, and coast.
//All other versions of the Life-Blinker build directly on this,
//replacing the code in this file for the new effect(s)

////NOTE: elder-care, in-home nurse light, and hearing-impaired doorbell
//use LifeBlinkerHome.ino for the LEDs, and LifeBlinkerHomeRemote.ino together
//(not this code)

//extra includes for future version 200 with wifi remote
#include <ESPWiFi.h>
#include <ESPHTTPClient.h>
#include <JsonListener.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

////Wifi sleep mode:
int radioEnabled = 0;

//// microcontroller layout
//left  led  TX pin (GPIO  1) 
//main  led  D4 pin (GPIO  2)
//right led  RX pin (GPIO  3)
//left  btn  D5 pin (GPIO 14)
//right btn  D6 pin (GPIO 12)

int SIG1led = 1;     // LED on pin 1 "TX" on most arduinos
int SIG2led = 2;     // LED on pin 2 "D4" system LED in most arduinos
int SIG3led = 3;     // LED on pin 3 "RX" on most arduinos

//ideally a I/0/II switch for a bicyle application, momentary switch would require holdover code and a cancel gesture, still
int SIG1btn = 14; // toggle switch, other side connected to ground/common
int SIG3btn = 12; // toggle switch, other side connected to ground/common
int SIG1read = 0;
int SIG3read = 0;

//boolean vars
bool SIG0state = false; //virtual/place-holder (just like the number zero)
bool SIG1state = false;
bool SIG2state = true;
bool SIG3state = false;
bool SIG4state = false;
bool SIG5state = false;

int buttonDelay = 125; //for debounce effect 1/10th of a second for most Real Time i/o, 250ms is a generous max

void pointLeft();
void pointRight();


void setup() {
  //boot splash
  Serial.begin(115200);
  Serial.print("Hello world, I'm alan turNing.  JK, this is a G2G (Give to Get) user license, please donate any amount to https://www.bikesafeim.org/donatesponsor/make-a-donation/ and reference BorklerCanCode-bikeBlinker1 (copyright J4CodeCo 2023)");
  delay(1250);

  //Code to disable wifi for power savings
  if (radioEnabled == 0) {
    WiFi.forceSleepBegin();
    delay(1250);
  }
  else {
    ////Radio is on, you can now set AP mode/ad-hoc/router.
    //WiFi.begin(WIFI_SSID, WIFI_PWD);
  }
  
  pinMode(SIG1btn, INPUT_PULLUP);
  pinMode(SIG3btn, INPUT_PULLUP);

  pinMode(SIG1led, OUTPUT);
  pinMode(SIG2led, OUTPUT);
  pinMode(SIG3led, OUTPUT);

}

void pointRight() {
  if (SIG1state == true) {
    SIG0state = false;
    SIG1state = false;
    SIG2state = true;
    SIG3state = false;
    digitalWrite(SIG1led, SIG1state);    
    digitalWrite(SIG2led, SIG2state);    
    digitalWrite(SIG3led, SIG3state);  
    delay(buttonDelay/3*2);                     
  }

  if (SIG2state == true) {
    SIG0state = false;
    SIG1state = false;
    SIG2state = false;
    SIG3state = true;
    digitalWrite(SIG1led, SIG1state);    
    digitalWrite(SIG2led, SIG2state);    
    digitalWrite(SIG3led, SIG3state);  
    delay(buttonDelay);                     
    delay(buttonDelay/2);                     
  }

  if (SIG3state == true) {
    SIG0state = true;
    SIG1state = false;
    SIG2state = false;
    SIG3state = false;
    digitalWrite(SIG1led, SIG1state);    
    digitalWrite(SIG2led, SIG2state);    
    digitalWrite(SIG3led, SIG3state);  
    delay(buttonDelay);                     
    delay(buttonDelay);                     
    delay(buttonDelay);                     
  }

  if (SIG0state == true) {
    SIG0state = false;
    SIG1state = true;
    SIG2state = false;
    SIG3state = false;
    digitalWrite(SIG1led, SIG1state);    
    digitalWrite(SIG2led, SIG2state);    
    digitalWrite(SIG3led, SIG3state);  
    delay(buttonDelay/3*2);                     
  }
}

void pointLeft() {
  if (SIG1state == true) {
    SIG0state = true;
    SIG1state = false;
    SIG2state = false;
    SIG3state = false;
    digitalWrite(SIG1led, SIG1state);    
    digitalWrite(SIG2led, SIG2state);    
    digitalWrite(SIG3led, SIG3state);  
    delay(buttonDelay/3*2);                     
    delay(buttonDelay);                     
    delay(buttonDelay);                     
  }

  if (SIG2state == true) {
    SIG0state = false;
    SIG1state = true;
    SIG2state = false;
    SIG3state = false;
    digitalWrite(SIG1led, SIG1state);    
    digitalWrite(SIG2led, SIG2state);    
    digitalWrite(SIG3led, SIG3state);  
    delay(buttonDelay);                     
    delay(buttonDelay/2);                     
  }

  if (SIG3state == true) {
    SIG0state = false;
    SIG1state = false;
    SIG2state = true;
    SIG3state = false;
    digitalWrite(SIG1led, SIG1state);    
    digitalWrite(SIG2led, SIG2state);    
    digitalWrite(SIG3led, SIG3state);  
    delay(buttonDelay);                     
  }

  if (SIG0state == true) {
    SIG0state = false;
    SIG1state = false;
    SIG2state = false;
    SIG3state = true;
    digitalWrite(SIG1led, SIG1state);    
    digitalWrite(SIG2led, SIG2state);    
    digitalWrite(SIG3led, SIG3state);  
    delay(buttonDelay/3*2);                     
  }
}
void loop() {

  SIG1read = digitalRead(SIG1btn);
  if (SIG1read == LOW) {
    // switch is pressed - pullup keeps pin high normally
    pointRight();
  }

  SIG3read = digitalRead(SIG3btn);
  if (SIG3read == LOW) {
    // switch is pressed - pullup keeps pin high normally
    pointLeft();
  }

  SIG1read = digitalRead(SIG1btn);
  SIG3read = digitalRead(SIG3btn);

  // if no switches are pressed
  if (SIG1read == HIGH && SIG3read == HIGH ) {
    SIG1state = false;
    SIG2state = true;
    SIG3state = false;
    digitalWrite(SIG1led, SIG1state);    
    digitalWrite(SIG2led, SIG2state);    
    digitalWrite(SIG3led, SIG3state);  
    delay(buttonDelay/2*3);                     
    SIG1state = true;
    SIG2state = false;
    SIG3state = true;
    digitalWrite(SIG1led, SIG1state);    
    digitalWrite(SIG2led, SIG2state);    
    digitalWrite(SIG3led, SIG3state);  
    delay(buttonDelay/2*3);                     
  }

}
