// RBE1001 Final Project
// Team K
// Brian McCarthy, Jonathon Brownlon, Zachary Zlotnick

#include <DFW.h>
enum state{
  AUTONOMOUS,
  TELEOP
};

state projectState;

void setup() {
  // put your setup code here, to run once:
  projectState = AUTONOMOUS;
  
}

void loop() {
  // put your main code here, to run repeatedly:
  switch(projectState){
    case AUTONOMOUS:
      // AUTONOMOUS CODE
      break;
    case TELEOP:
      // TELEOP CODE
      break;
  }
}
