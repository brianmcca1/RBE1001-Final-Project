/**
 * RBE1001 Final Project
 * Team K
 * Jonathon Brownlon
 * Zachary Zlotnick
 * Brian McCarthy
 */

#include <DFW.h> // DFW include
#include <Servo.h> // servo library
#include <LiquidCrystal.h>

int ledpindebug = 13; //Wireless controller Debug pin. If lit then there is no communication.

DFW dfw(ledpindebug);  // Instantiates the DFW object and setting the debug pin. The debug pin will be set high if no communication is seen after 2 seconds
Servo driveMotorLeft; // Servo object
Servo driveMotorRight; // Servo object
Servo steerMotorLeft;
Servo steerMotorRight;
Servo intakeMotor;
const int clamp = 22;
const int rightPiston = 23;
const int leftPiston = 24;
const int leftDrivePort = 4;
const int rightDrivePort = 7;
const int leftSteerPort = 5;
const int rightSteerPort = 6;

LiquidCrystal lcd(40, 41, 42, 43, 44, 45);

void setup() {
  Serial.begin(9600); // Serial output begin. Only needed for debug
  dfw.begin(); // Serial1 output begin for DFW library. Buad and port #."Serial1 only"
  driveMotorLeft.attach(leftDrivePort, 1000, 2000); // left drive motor pin#, pulse time for 0,pulse time for 180
  driveMotorRight.attach(rightDrivePort, 1000, 2000); // right drive motor pin#, pulse time for 0,pulse time for 180
  steerMotorLeft.attach(leftSteerPort, 1000, 2000);
  steerMotorRight.attach(rightSteerPort, 1000, 2000);
  intakeMotor.attach(8, 1000, 2000);
  pinMode(clamp, OUTPUT);
  pinMode(rightPiston, OUTPUT);
  pinMode(leftPiston, OUTPUT);
  lcd.begin(16,2);
  lcd.clear();
  
  //Serial.println("start");

  while (dfw.start() == 0)
  { //waits for controller to init
    Serial.println("init");
    delay(20);
  }
}

/**
 * Handles the autonomous operation of the robot.
 * @param time The length of time that the robot should operate autonomously, in seconds
 */
void autonomous(volatile unsigned long time)
{
  lcd.print("AUTONOMOUS PHASE");
  while (dfw.start() == 1) { // waits for start button
    Serial.println("waiting for start");
    delay(20);
  }

  unsigned long startTime = millis(); // sets start time of autonomous
  time = time * 1000; // converts time from seconds to milliseconds
  while ((millis() - startTime <= time) && (dfw.select())) // compares start time to time entered in the autonomous function and
  {
    lcd.setCursor(0, 1);
    lcd.print((millis() - startTime) / 1000);
    // The select button can be used to skip the autonomous code.
    // Enter Autonomous User Code Here
    Serial.println("Autonomous"); //prints Autonomous over serial (usb com port)
    delay(20); //delay to prevent spamming the serial port and to keep servo and dfw libraries happy
  }
}

/**
 * Drives the robot, based on current input from the controller
 */
void TeleopDrive()
{
  dfw.run(); // Called to update the controllers output. Do not call faster than every 15ms.
  if(dfw.getCompetitionState() != powerup){
    driveMotorLeft.write(dfw.joysticklv());
    driveMotorRight.write(180 - dfw.joysticklv());
    steerMotorLeft.write(180 - dfw.joystickrh());
    steerMotorRight.write(dfw.joystickrh());
    intakeMotor.write(0);
    if (dfw.one())
    {
      digitalWrite(clamp, HIGH);
    }
    if (dfw.three())
    {
      digitalWrite(clamp, LOW);
    }
    if (dfw.r1());
    {
      digitalWrite(rightPiston, HIGH);
    }
    if (dfw.r2());
    {
      digitalWrite(rightPiston, LOW);
    }
    if (dfw.l1());
    {
      digitalWrite(leftPiston, HIGH);
    }
    if (dfw.l2());
    {
      digitalWrite(leftPiston, LOW);
    }  
  }
}

/**
 * Hands the Tele-operation of the robot
 * @param time The length of time that the robot should receive input, in seconds
 */
void teleop(unsigned long time) { 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TELE-OP PHASE");
  unsigned long startTime = millis(); // sets start time of teleop
  time = time * 1000; // converts time from seconds to milliseconds
  while (millis() - startTime <= time) // compares start time to time entered in the teleop function
  {
    lcd.setCursor(0, 1);
    lcd.print((millis() - startTime) / 1000);
    TeleopDrive();
    delay(20); // Servos do not like to be called faster than every 20 ms. Also the Serial data is sent every 15ms

    // Enter Teleop User Code Here

    Serial.println("TeleOp"); //prints Teleop over serial (usb com port)

  }
  exit(0); // exits program
}

void loop() 
{
  // autonomous(0); //time in seconds to run the autonomous code
  teleop(10000);
  exit(0);
}

