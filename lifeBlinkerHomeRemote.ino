////This code for the remote is NOT deployed on life blinker classic, but it works in pairs with one.
//This remote is a one-button doorbell or call for help (off/on/off Momentary switch/MOM).
//If you need more than one on the home network, remotes can be n:1 to LED box
//and LED boxes can be many per home with a change of the target ip in LifeBlinkerHomeRemote.ino
//Any enclosure will work: teddy bear, wooden box, old pill container, etc.  Battery optional.
//Do not use metal containers/large metal surfaces to avoid the farday effect/signal loss.

const char* WIFI_SSID = "YOUR_WIFI_AP_NAME_HERE_PLS";
const char* WIFI_PWD = "YOUR_WIFI_AP_PASSWORD_HERE_PLS";

//ATTN: enter the pre-set/preferred DHCP Address below for planned/predictable IP
char serverName[] = "http://192.168.0.175"; //CHANGE THIS!!!
//SUPER CRITICAL to include the http:// portion our you will get -1 return codes from the library

//If you find the setup annoying or life somewhere that home wifi is scarce
//you can make a 3rd arduino into a host, OR
//File a bug report against this .ino if you want to see a mesh version!
//I will otherwise prioritize other features based on various constraints. -JB

//#include <JsonListener.h>
//#include <WiFi.h>
//#include <ESPHTTPClient.h>
//#include <ESPWiFi.h>
//#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
//extras
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <time.h>                       // time() ctime()
#include <sys/time.h>                   // struct timeval
#include <coredecls.h>                  // settimeofday_cb()

////Wifi sleep mode:
int radioEnabled = 1;

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
//ATTN: For the home remote, only one momentary switch is needed
int SIG1btn = 14; // switch, other side connected to ground/common
int SIG2btn = 2;  // switch, other side connected to ground/common
int SIG3btn = 12; // switch, other side connected to ground/common
int SIG1read = 0;
int SIG2read = 0;
int SIG3read = 0;

//boolean vars
bool SIG0state = false; //virtual/place-holder (just like the number zero)
bool SIG1state = false;
bool SIG2state = true;
bool SIG3state = false;
bool SIG4state = false;
bool SIG5state = false;

int alertLevel = 0;
int SELECTb = D6;
int OKb = D2;
int CANCELb = D5;

//sender can listen, too.  This will allow D4 LED to be used to show acknowledge
ESP8266WebServer server(80);
//String string0 = "...awaiting user input on:";
String webString = "";

String ipaddr = "0.0.0.0";

String blankString  = " ";
String tbdString    = "TBD";

//Begin Settings


#define TZ              -8       // (utc+) TZ in hours
#define DST_MN          60      // use 60mn for summer time in some countries

// Setup
int cleanupTime = 45; //900 is ~15 min
int actionCounter = 0; 
int rest = 0;
int foodOrDrink = 0;

// Display Settings
unsigned long previousMillis = 0;     // will store last time LED was updated
const long interval = 1000;           // interval at which to blink (milliseconds)
int remainingTimeBudget = 1000;
int buttonDelay = 100; //**shorten testing voltage level

//#endif

int delay1=120;
int delay2=180;
int delay3=240;
int cycLoops=1;


int uptime = 0;

#define TZ_MN           ((TZ)*60)
#define TZ_SEC          ((TZ)*3600)
#define DST_SEC         ((DST_MN)*60)
time_t now;

// declaring prototypes
void webActions(int setMode, String label);
void ledParty(int led1, int led2, int led3);
void alertLed();
void sendPOST();

void setup() {
  pinMode(SIG1btn, INPUT_PULLUP);
  pinMode(SIG2btn, INPUT_PULLUP);
  pinMode(SIG3btn, INPUT_PULLUP);

  pinMode(SIG1led, OUTPUT);
  pinMode(SIG2led, OUTPUT);
  pinMode(SIG3led, OUTPUT);

  ledParty(1,1,1);

  ////serial session will turn on BOTH The RX and RX lights, which are P1 and P3 in most boards! :(
  SIG1read = digitalRead(SIG1btn);
  SIG2read = digitalRead(SIG2btn);
  SIG3read = digitalRead(SIG3btn);
  if (SIG1read == LOW || SIG3read == LOW) {
    Serial.begin(115200);
    //this would be a good place to add a routine that spits out xxx.yyy.zzz.iii ip addr on the leds
  }
  Serial.println();
  Serial.print("Hello, I'm booting!");
  Serial.println();

  ////this pin d2 will be used for the local reset BUT don't leave flyer connected during programming!!!


  WiFi.begin(WIFI_SSID, WIFI_PWD);

  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(delay2);
    ledParty(1,0,1);
    counter++;
  }

  ledParty(0,1,0);  

  delay(delay2);
  String lcladdr = WiFi.localIP().toString().c_str();
  ipaddr = lcladdr;
  Serial.print("Network connected, my address is:");
  Serial.print(lcladdr);
  Serial.println();
  Serial.print("...starting web server...");

  ledParty(0,0,0);  
  delay(delay2);

  // Get time from network time service
  //configTime(TZ_SEC, DST_SEC, "pool.ntp.org");

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  server.begin();

  ///trash??? String uptimeStr = uptime.toString().c_str();

  delay(delay2);
  Serial.print("web server is up");
  //Serial.end();
}
//endfunc

////start webserver code:

const char INDEX_HTML[] =
"<!DOCTYPE HTML>"
"<html>"
"<head>"
"<meta name = \"viewport\" content = \"width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0\">"
"<title>ESP8266 wifi remote</title>"
"<style>"
"\"body { background-color: #808080; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }\""
"</style>"
"</head>"
"<body>"
"<center><h1>This remote is for calling XXX from YYY</h1><center>"
"<FORM action=\"/\" method=\"post\">"
"<P>"
"<CENTER>"
"Inputs<br>"
"<INPUT type=\"text\" name=\"myInput\"<BR>"
"<BR>"
"<INPUT type=\"submit\" value=\"Submit\">"
"<BR>"
"</CENTER>"
"</FORM>"
"</body>"
"</html>";

void handleRoot()
{
  if (server.hasArg("myInput")) {
    handleSubmit();
  }
  else {
    server.send(200, "text/html", INDEX_HTML);
  }
}

//probably will remain unused
void returnFail(String msg)
{
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(500, "text/plain", msg + "\r\n");
}

void handleSubmit()
{

  if (server.hasArg("myInput")) {
    webString = server.arg("myInput");
      if (webString == "1") {
        alertLevel += 1;
      }
      else if (webString == "reset") {
        alertLevel = 0;
      }
  }

  server.send(200, "text/html", INDEX_HTML);
}

void returnOK()
{
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "OK\r\n");
}


void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

////end webserver code:
//^^^ this is NOT needed for the remote control design

String readString;

//client function to send/receive GET request data.
void sendPOST() {
  ledParty(0,1,0);
  HTTPClient http;

  String postData = "myInput=1";
  // Your Domain name with URL path or IP address with path
  http.begin(serverName);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
  int httpCode = http.POST(postData);   //Send the request    
  httpCode = http.POST(postData);   //Send the request again
  ////getting the return code is expensive, takes the call from realtime to painfully slow, so skipping
  //String payload = http.getString();    //Get the response payload

  //Serial.println(httpCode);   //Print HTTP return code
  //Serial.println(payload);    //Print request response payload

  //// Free resources
  http.end();
  ledParty(0,1,0);
}

//@@main
void loop() {

  //Serial.print("looping");

  unsigned long currentMillis = millis();

  ////move to budget, if you need a remote that is also a server
  //server.handleClient();
  //webActions(buttonDelay, "Main Loop");
  //alertLed();

  //int remainingTimeBudget = ui.update();
  int remainingTimeBudget = 1000;

  //poc code, probably best that the remote sends SIG2led high so the on-board led turns off tbd
  SIG1read = digitalRead(SIG1btn);
  SIG2read = digitalRead(SIG2btn);
  SIG3read = digitalRead(SIG3btn);
  if (SIG2read == LOW || SIG3read == LOW) {
    alertLevel += 1;
    sendPOST();
  }


  if (currentMillis - previousMillis >= interval) {
    actionCounter += 1;
    uptime += 1;
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    
   //this will trigger a left click, which will fade after a few minutes
   if (actionCounter % cleanupTime == 0) {
     // if WiFi is down, try reconnecting
     if (WiFi.status() != WL_CONNECTED) {
       WiFi.disconnect();
       WiFi.reconnect();
       previousMillis = currentMillis;
     }
     if ( alertLevel > 0 ) {
       alertLevel -= 1;
     }
   }

  }
  


}
//endfunc

void ledParty(int led1, int led2, int led3) {
    SIG1state = !SIG1state*led1;
    SIG2state = !SIG2state*led2;
    SIG3state = !SIG3state*led3;
    digitalWrite(SIG1led, SIG1state);    
    digitalWrite(SIG2led, !SIG2state);    
    digitalWrite(SIG3led, SIG3state);  
}

void alertLed() {
    if (alertLevel == 0) {
      SIG1state = false;
      SIG2state = false;
      SIG3state = false;
    }
    else if (alertLevel == 1) {
      SIG1state = true;
      SIG2state = false;
      SIG3state = false;
    }
    else if (alertLevel == 2) {
      SIG1state = true;
      SIG2state = true;
      SIG3state = false;
    }
    else if (alertLevel == 3) {
      SIG1state = true;
      SIG2state = true;
      SIG3state = true;
    }
    digitalWrite(SIG1led, SIG1state);    
    digitalWrite(SIG2led, !SIG2state);    
    digitalWrite(SIG3led, SIG3state);

    delay(delay1);
    if (alertLevel > 9) {
      SIG1state = !SIG1state;
      SIG2state = !SIG2state;
      SIG3state = !SIG2state;
    }
    else if (alertLevel > 6) {
      SIG1state = !SIG1state;
      SIG2state = !SIG2state;
    }
    else if (alertLevel > 3) {
      SIG1state = !SIG1state;
    }
    digitalWrite(SIG1led, SIG1state);    
    digitalWrite(SIG2led, !SIG2state);    
    digitalWrite(SIG3led, SIG3state);
    
}


////future functionality for front/back door, family member, etc.
void webActions(int setMode, String label) {

  webString = "";
    
}


//endfunc
