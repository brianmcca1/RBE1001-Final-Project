// RBE1001 Final Project
// Team K
// Brian McCarthy, Jonathon Brownlon, Zachary Zlotnick

#include <DFW.h>
#include <Servo.h>

enum state{
  AUTONOMOUS,
  TELEOP
};

state projectState;
// TODO: Update to reflect actual Solenoid Pin
int solenoidPinLeft = 4; // Solenoid for the left pneumatic 
int solenoidPinRight = 5; // Solenoid for the right pneumatic
int solenoidPinPEN = 6; // Solenoid for the pneumatic grabbing the PEN
Servo rightMotor;
Servo leftMotor;

void setup() {
  // put your setup code here, to run once:
  projectState = AUTONOMOUS;
  pinMode(solenoidPinLeft, OUTPUT);
  pinMode(solenoidPinRight, OUTPUT);
  pinMode(solenoidPinPEN, OUTPUT);
  dfw.begin();
  leftMotor.attach(7, 1000, 2000);
  rightMotor.attach(8, 1000, 2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  switch(projectState){
    case AUTONOMOUS:
      // AUTONOMOUS CODE
      break;
    case TELEOP:
      // TELEOP CODE
      dfw.run();
      break;
  }
}
