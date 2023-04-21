#include <NewPing.h>
#include <Wire.h>
#include <SparkFun_APDS9960.h>
#include <DRV8833.h>

stopMotors(){
  driver.motorAStop();
  driver.motorBStop();
}
