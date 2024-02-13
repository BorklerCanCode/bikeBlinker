////This version of the Life-Blinker is hardware+software
//The lifeBlinkerGyro adds a decelerometer to light up the three LEDS
//during a braking event, without a the need to add a traditional brake sensor/harness.

////Code in progress, pending results of gyro hardware test
//ic2 based gryo vs
//analog gyro
//eta Q1-Q2 2024, file a bug report against this .ino if you want to see it sooner!
//I will otherwise prioritize other features based on various constraints. -JB
 
 
//// Calibration values for D1 ADC
// think of calibrating Nintendo wii remote or Switch joycon (fun, I know).
// default values should suffice:
int ADCmin = 25;  // min is 0,   avg is 11.  Defect is any triple digit number at rest (e.g. 276 or 311)
int ADCmax = 975; // max is 1023 avg is 987. Defects not yet encountered in approx 10 units.
// separate calibration not anticipated for 
 
//// Statistical process control and ongoing reliability testing
//  Switch this code on to log a report of the arduino board and gryo
// 
// read A0
// 
// trim A0 by ints ADCmin ADCmax to round out
// 
// write pwm to D4/system LED.
