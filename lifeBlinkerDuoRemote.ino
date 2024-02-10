////This version of the Life-Blinker is hardware+software mod of the classic OR gyro version.
//A remote (lifeBlinkerDuoRemote.ino) sends left/right/coast sstate from your handlebars 
//without the need for any wiring added in between.  Dedicated battery required.

////Code refactor in progress, pending results new network modes
//Ad-hoc mode available, file a bug report against this .ino if you want it published
//I will otherwise publish with new deps/features once mesh version is perfect. -JB

//CODE FLAGS
gyroEquipped=1; //value of 0 is no, value of 1 is yes, code will NOT select readGyro nor setBrake if "0".
