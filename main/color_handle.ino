#include <NewPing.h>
#include <Wire.h>
#include <SparkFun_APDS9960.h>
#include <DRV8833.h>



void showColorF(){
  color_sensor.init(); //initialization
  color_sensor.enableLightSensor(false); //enabling light sensor with no interrups 
  delay(500); //delay for init
  analogWrite(RED_PIN_RGB, color_sensor.readRedLight(red_light));
  analogWrite(GREEN_PIN_RGB, color_sensor.readRedLight(green_light));
  analogWrite(BLUE_PIN_RGB, color_sensor.readRedLight(blue_light));
  while(sonar.ping_cm() < 12){}
  analogWrite(RED_PIN_RGB,0);
  analogWrite(GREEN_PIN_RGB,0);
  analogWrite(BLUE_PIN_RGB,0);
  return;
}