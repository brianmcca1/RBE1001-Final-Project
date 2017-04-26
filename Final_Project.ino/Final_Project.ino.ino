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

typedef struct{
  int pos;
  static int pinA;
  static int pinB;
  int lastAValue;
} Encoder;


enum IntakeState {IN, STOP, OUT};
int ledpindebug = 13; //Wireless controller Debug pin. If lit then there is no communication.

DFW dfw(ledpindebug);  // Instantiates the DFW object and setting the debug pin. The debug pin will be set high if no communication is seen after 2 seconds
Servo driveMotorLeft; // Servo object
Servo driveMotorRight; // Servo object
Servo steerMotorLeft;
Servo steerMotorRight;
Servo intakeMotor;
const int clamp = 29; // TODO: CHANGE
const int rightPiston = 28; // TODO: CHANGE
const int leftPiston = 27; // TODO CHANGE
const int leftDrivePort = 4;
const int rightDrivePort = 7;
const int leftSteerPort = 5;
const int rightSteerPort = 6;
Encoder rightEncoder;
Encoder leftEncoder;
boolean clampDown = false;
boolean armsUp = false;

IntakeState intakeState = OUT;

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
//  pinMode(encoderPinA, INPUT);
 // pinMode(encoderPinB, INPUT);
  lcd.begin(16,2);
  lcd.clear();
  
  //Serial.println("start");

  while (dfw.start() == 0)
  { //waits for controller to init
    Serial.println("init");
    delay(20);
  }
}

int encoderChange(Encoder encoder){
    int encoderAVal = digitalRead(encoder.pinA);
    if(encoder.lastAValue == LOW && encoderAVal == HIGH){
      if(digitalRead(encoder.pinB) == LOW){
        encoder.pos--;
      } else {
        encoder.pos++;
      }
    }
    return encoder.pos;
}
/**
 * Handles the autonomous operation of the robot.
 * @param time The length of time that the robot should operate autonomously, in seconds
 */
void autonomous(unsigned long time)
{
  int robotX = 0; // X-axis displacement of the robot from the starting point, in inches
  int robotY = 0; // Y-axis displacement of the robot from the starting point, in inches 
  int robotAngle = 0; // Rotational displacement of the robot from the starting angle, in degrees
  rightEncoder.pos = 0;
  rightEncoder.pinA = 22;
  rightEncoder.pinB = 23;
  leftEncoder.pos = 0;
  leftEncoder.pinA = 12;
  leftEncoder.pinB = 13;
  unsigned long startTime = millis(); // sets start time of autonomous
  
  int rightEncoderPosition = 0; // Current rotational position of the right encoder
  int leftEncoderPosition = 0; // Current rotational position of the left encoder
  int rightEncoderALast = LOW; // The value of encoder pin A last time it was checked
  
  lcd.print("AUTONOMOUS PHASE");
  while (dfw.start() == 1) { // waits for start button
    Serial.println("waiting for start");
    delay(20);
  }


  time = time * 1000; // converts time from seconds to milliseconds
  while ((millis() - startTime <= time) && (dfw.select())) // compares start time to time entered in the autonomous function and
  {
    lcd.setCursor(0, 1);
    lcd.print("Time left: ");
    lcd.print((time - (millis() - startTime)) / 1000);
    
    rightEncoder.pos = encoderChange(rightEncoder);
    leftEncoder.pos = encoderChange(leftEncoder);
    rightEncoder.lastAValue = digitalRead(rightEncoder.pinA);
    leftEncoder.lastAValue = digitalRead(leftEncoder.pinA);
    
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
    switch(intakeState){
      case IN:
        intakeMotor.write(0);
        break;
      case STOP:
        intakeMotor.write(90);
        break;
      case OUT:
        intakeMotor.write(180);
        break;
    }
    
    if (dfw.r1())
    {
      if(armsUp){
        digitalWrite(rightPiston, LOW);
        digitalWrite(leftPiston, LOW);
        armsUp = false;
      } else {
        digitalWrite(rightPiston, HIGH);
        digitalWrite(leftPiston, HIGH);
        armsUp = true;
      }
    }
    if (dfw.l1())
    {
      if(clampDown){
        digitalWrite(clamp, LOW);
        clampDown = false;
      } else {
        digitalWrite(clamp, HIGH);
        clampDown = true;
      }
    }
    if (dfw.one()){
      intakeState = IN;
    }
    if(dfw.three()){
      intakeState = OUT;
    }
    if(dfw.two()){
      intakeState = STOP;
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
    lcd.print("Time left: ");
    lcd.print((time - (millis() - startTime)) / 1000);
    TeleopDrive();
    delay(20); // Servos do not like to be called faster than every 20 ms. Also the Serial data is sent every 15ms

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

