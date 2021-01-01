/*
Drive two stepmotors with 28BYJ-48 by ESP32 via ULN2003A
Control by joystick HW-504
MIT License
https://github.com/satr/control-stepmotors-by-joystick/blob/main/LICENSE
Copyright © 2021, github.com/satr
https://github.com/satr/control-stepmotors-by-joystick
*/

#include <Arduino.h>
#include <AccelStepper.h>

#define JOYSTICK_X_PIN 32
#define JOYSTICK_Y_PIN 33
#define JOYSTICK_MAX_VAL 4095

#define STEPPER_FULL_TURN_STEPS 2038
#define STEPPER_MAX_SPEED 500
#define STEPPER_ACCELERATION 200
//Stepmotor X
#define STEPPER_X_PIN_IN1 4
#define STEPPER_X_PIN_IN2 16
#define STEPPER_X_PIN_IN3 17
#define STEPPER_X_PIN_IN4 5
//Stepmotor Y
#define STEPPER_Y_PIN_IN1 18
#define STEPPER_Y_PIN_IN2 19
#define STEPPER_Y_PIN_IN3 23
#define STEPPER_Y_PIN_IN4 25

#define VAL_THRESHOLD 100

AccelStepper stepperX(AccelStepper::FULL4WIRE, STEPPER_X_PIN_IN1, STEPPER_X_PIN_IN3, STEPPER_X_PIN_IN2, STEPPER_X_PIN_IN4);
AccelStepper stepperY(AccelStepper::FULL4WIRE, STEPPER_Y_PIN_IN1, STEPPER_Y_PIN_IN3, STEPPER_Y_PIN_IN2, STEPPER_Y_PIN_IN4); 

int targetX = 0;
int targetY = 0;
int count = 0;

void setup()
{  
  Serial.begin(115200);
  stepperX.setMaxSpeed(STEPPER_MAX_SPEED);
  stepperY.setMaxSpeed(STEPPER_MAX_SPEED);
  // Set the maximum acceleration in steps per second^2:
  stepperX.setAcceleration(STEPPER_ACCELERATION);
  stepperY.setAcceleration(STEPPER_ACCELERATION);

  pinMode(JOYSTICK_X_PIN, INPUT);
  pinMode(JOYSTICK_Y_PIN, INPUT);
}

void setTargetX(int x);
void setTargetY(int y);
void printStatus(int x, int y, int valX, int valY);

void loop()
{  
  stepperX.run();
  stepperY.run();
  
  int valX = analogRead(JOYSTICK_X_PIN);
  int valY = analogRead(JOYSTICK_Y_PIN);
  int x = map(valX, 0, JOYSTICK_MAX_VAL, 0, STEPPER_FULL_TURN_STEPS);
  int y = map(valY, 0, JOYSTICK_MAX_VAL, 0, STEPPER_FULL_TURN_STEPS);
  
  setTargetX(x);
  setTargetY(y);
  printStatus(x, y, valX, valY);
}

void setTargetX(int x) {
  if (abs(targetX - x) < VAL_THRESHOLD) 
    return;
  targetX = x;	
  stepperX.moveTo(targetX);
  //Serial.println("set x:" + String(targetX));
}

void setTargetY(int y) {
  if (abs(targetY - y) < VAL_THRESHOLD) 
    return;
  targetY = y;	
  stepperY.moveTo(targetY);
  //Serial.println("set y:" + String(targetY));
}

void printStatus(int x, int y, int valX, int valY) {
  if (count++ < 50000) 
    return;
  count = 0;
  //Serial.println("valX:" + String(valX) + " valY:" + String(valY));
  Serial.println("x:" + String(x) + " (" + String(stepperX.targetPosition()) + "->" + String(stepperX.currentPosition()) + ")"
              + " y:" + String(y) + " (" + String(stepperY.targetPosition()) + "->" + String(stepperY.currentPosition()) + ")");
}
