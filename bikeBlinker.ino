////blinker

//// microcontroller layout
//p2 main led
//...
//p4 left btn
//p5 right btn
//p1 left led
//p3 right led

bool SIG0state = false;
bool SIG1state = false;
bool SIG2state = true;
bool SIG3state = false;

bool SIG4state = false;
bool SIG5state = false;

int SIG1btn = 4; // momentary switch, other side connected to ground
int SIG3btn = 5; // momentary switch, other side connected to ground
int SIG1read = 0;
int SIG3read = 0;

int SIG1led = 1;     // LED on pin 1
int SIG2led = 2;     // LED on pin 2
int SIG3led = 3;     // LED on pin 3

int buttonDelay = 125; //for debounce effect 1/10th of a second for most Real Time i/o, 250ms is a generous max

void pointLeft();
void pointRight();

void setup() {
  //boot splash
  Serial.begin(115200);
  Serial.print("Hello world, I'm alan turNing.  JK, this is a G2G user license, please donate any amount to https://www.bikesafeim.org/donatesponsor/make-a-donation/ and reference BorklerCanCode-bikeBlinker1 (copyright J4CodeCo 2023");
  delay(1250);

  pinMode(SIG1btn, INPUT_PULLUP);

  pinMode(SIG3btn, INPUT_PULLUP);

  pinMode(SIG1led, OUTPUT);
  pinMode(SIG2led, OUTPUT);
  pinMode(SIG3led, OUTPUT);

}

void pointRight() {
    SIG1state = true;
    SIG2state = true;
    SIG3state = !SIG3state;
    digitalWrite(SIG1led, SIG1state);  // indicate via LED  
    digitalWrite(SIG2led, SIG2state);  // indicate via LED  
    digitalWrite(SIG3led, SIG3state);  // indicate via LED
    delay(buttonDelay/3*2);                     // delay to debounce switch
}

void pointLeft() {
    SIG1state = !SIG1state;
    SIG2state = true;
    SIG3state = true;
    digitalWrite(SIG1led, SIG1state);  // indicate via LED  
    digitalWrite(SIG2led, SIG2state);  // indicate via LED  
    digitalWrite(SIG3led, SIG3state);  // indicate via LED
    delay(buttonDelay/3*2);                     // delay to debounce switch
}

void loop() {

  //ktb need IF AND then (then two else?)

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

  if (SIG1read == HIGH && SIG3read == HIGH ) {
    // switch is pressed - pullup keeps pin high normally
    SIG1state = false;
    SIG2state = true;
    SIG3state = false;
    digitalWrite(SIG1led, SIG1state);  // indicate via LED  
    digitalWrite(SIG2led, SIG2state);  // indicate via LED  
    digitalWrite(SIG3led, SIG3state);  // indicate via LED
    delay(buttonDelay/2*3);                     // delay to debounce switch
    SIG1state = true;
    SIG2state = false;
    SIG3state = true;
    digitalWrite(SIG1led, SIG1state);  // indicate via LED  
    digitalWrite(SIG2led, SIG2state);  // indicate via LED  
    digitalWrite(SIG3led, SIG3state);  // indicate via LED
    delay(buttonDelay/2*3);                     // delay to debounce switch
  }

}
