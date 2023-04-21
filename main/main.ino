#include <NewPing.h>
#include <Wire.h>
#include <DRV8833.h>
#include <SparkFun_APDS9960.h>
#include <motor_handle.ino>
#include <color_handle.ino>
#include <blackLine_handle.ino>
#define MIDDLE_SENSOR_BLACK_LINE      
#define LEFT_SENSOR_BLACK_LINE
#define RIGHT_SENSOR_BLACK_LINE
#define RIGHT_90_DEGREES_SENSOR_BLACK_LINE
#define LEFT_90_DEGREES_SENSOR_BLACK_LINE
#define RIGHT_MOTOR_A_1
#define RIGHT_MOTOR_A_2
#define LEFT_MOTOR_B_1
#define LEFT_MOTOR_B_2
#define ULTRASONIC
#define TRIGGER_PIN_ULTRASONIC   // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_ULTRASONIC      // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 100            // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define RED_PIN_RGB
#define GREEN_PIN_RGB
#define BLUE_PIN_RGB
#define BLACK_LINE_LIMIT 200        // limit to check if the robot lost track
#define LEFT_MOTOR_BASE_SPEED
#define RIGHT_MOTOR_BASE_SPEED


NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
SparkFun_APDS9960  color_sensor = SparkFun_APDS9960(); // color detection
DRV8833 motor_shield = DRV8833();


//UltraSonic 
unsigned int prev_distance = 0;   
unsigned int distance = 0;

// color sensor
unsigned int red_light = 0;
unsigned int green_light = 0;
unsigned int blue_light = 0;

// black line sensors
unsigned int prev_middle_sensor_black_line = 0;
unsigned int prev_left_sensor_black_line = 0;
unsigned int prev_right_sensor_black_line = 0;
unsigned int prev_left_check_90_degree_turn = 0;
unsigned int prev_right_check_90_degree_turn = 0;

unsigned int current_sensor_input_middle=0;
unsigned int current_sensor_input_right=0;
unsigned int current_sensor_input_left=0;
unsigned int current_sensor_input_90_degree_left=0;
unsigned int current_sensor_input_90_degree_right=0;

// setup for black line sensor
unsigned int max_infra_middle =0;
unsigned int min_infra_middle = 1023;
unsigned int max_infra_right =0;
unsigned int min_infra_right = 1023;
unsigned int max_infra_left =0;
unsigned int min_infra_left = 1023;
unsigned int max_90_degree_check_left = 0;
unsigned int min_90_degree_check_left = 1023;
unsigned int max_90_degree_check_right = 0;
unsigned int min_90_degree_check_right = 1023;


// pid for error handle
float kd = 0;     // need to be fixed
float kp = 0;   // need to be fixed
int error = 0;
// error < 0 robot go left
// error > 0 robot go right 

void setup() {
  Serial.begin(9600);
  pinMode(MIDDLE_SENSOR_BLACK_LINE, INPUT_PULLUP);
  pinMode(LEFT_SENSOR_BLACK_LINE, INPUT_PULLUP);
  pinMode(RIGHT_SENSOR_BLACK_LINE, INPUT_PULLUP);
  pinMode(RIGHT_SENRIGHT_90_DEGREES_SENSOR_BLACK_LINESOR_BLACK_LINE, INPUT_PULLUP);
  pinMode(RIGHT_SENSOR_BLLEFT_90_DEGREES_SENSOR_BLACK_LINEACK_LINE, INPUT_PULLUP);
  pinMode(TRIGGER_PIN_ULTRASONIC, OUTPUT);
  pinMode(ECHO_PIN_ULTRASONIC, INPUT_PULLUP);
  motor_shield.attachMotorA(RIGHT_MOTOR_A_1, RIGHT_MOTOR_A_2);
  motor_shield.attachMotorB(LEFT_MOTOR_B_1, LEFT_MOTOR_B_2);

  for(int i=0;i<500;i++){
    currentRight = analogRead(MIDDLE_SENSOR_BLACK_LINE);
    if(current_sensor_input_middle > max_infra_middle) max_infra_middle = current_sensor_input_middle;
    if(current_sensor_input_middle < min_infra_middle) min_infra_middle = current_sensor_input_middle;
    currentLeft = analogRead(LEFT_SENSOR_BLACK_LINE);
    if(current_sensor_input_left > max_infra_left) max_infra_left = current_sensor_input_left;
    if(current_sensor_input_left < min_Infra_Left) min_infra_left = current_sensor_input_left;
    currentRight = analogRead(RIGHT_SENSOR_BLACK_LINE);
    if(currentRicurrent_sensor_input_rightght > max_infra_right)  max_infra_right = current_sensor_input_right;
    if(current_sensor_input_right < min_infra_right)  min_infra_right = current_sensor_input_right;
    currentLeft = analogRead(RIGHT_90_DEGREES_SENSOR_BLACK_LINE);
    if(current_sensor_input_90_degree_right > max_90_degree_check_right) max_90_degree_check_right = current_sensor_input_90_degree_right;
    if(current_sensor_input_90_degree_right < min_90_degree_check_right) min_90_degree_check_right = current_sensor_input_90_degree_right;
    currentRight = analogRead(LEFT_90_DEGREES_SENSOR_BLACK_LINE);
    if(current_sensor_input_90_degree_left > max_90_degree_check_left) max_90_degree_check_left = current_sensor_input_90_degree_left;
    if(current_sensor_input_90_degree_left < min_90_degree_check_left) min_90_degree_check_left = current_sensor_input_90_degree_left;
    delay(20);
  }
  motor_shield.motorAForward(MOTOR_BASE_SPEED);
  motor_shield.motorBForward(MOTOR_BASE_SPEED);

} 

void loop() {
  distance = sonar.ping_cm();
  prev_middle_sensor_black_line = current_sensor_input_middle ;
  prev_left_sensor_black_line = current_sensor_input_left;
  prev_right_sensor_black_line = current_sensor_input_right;
  prev_left_check_90_degree_turn = current_sensor_input_90_degree_left;
  prev_right_check_90_degree_turn = current_sensor_input_90_degree_right;
  current_sensor_input_middle = analogRead(MIDDLE_SENSOR_BLACK_LINE);
  current_sensor_input_right = analogRead(RIGHT_SENSOR_BLACK_LINE);
  current_sensor_input_left = analogRead(LEFT_SENSOR_BLACK_LINE);
  current_sensor_input_90_degree_left = analogRead(LEFT_90_DEGREES_SENSOR_BLACK_LINE);
  current_sensor_input_90_degree_right = analogRead(RIGHT_90_DEGREES_SENSOR_BLACK_LINE);
  if(distance > 0 && distance < 15){
    stopMotors();
    showColorF();
  }
  if (current_sensor_input_middle < BLACK_LINE_LIMIT){
    if(check_prev_edgeF(current_sensor_input_90_degree_left, prev_left_check_90_degree_turn,current_sensor_input_90_degree_right, prev_left_check_90_degree_turn)){
      /* function to go back to line */ 
  }
  if(current_sensor_input_right < BLACK_LINE_LIMIT){
    error = error+1;
    fix_drive_pidF(LEFT_MOTOR_BASE_SPEED, RIGHT_MOTOR_BASE_SPEED, error, error-1, kp, kd);

  }else if(current_sensor_input_left < BLACK_LINE_LIMIT){
    error = error-1;
    fix_drive_pidF(LEFT_MOTOR_BASE_SPEED, RIGHT_MOTOR_BASE_SPEED, error, error+1, kp, kd);
  }

}
