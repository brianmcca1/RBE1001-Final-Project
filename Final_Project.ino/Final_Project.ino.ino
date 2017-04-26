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
  int lastPos;
  int pinA;
  int pinB;
  int lastAValue;
} Encoder;

enum RobotDirection {N, S, E, W};
enum IntakeState {IN, STOP, OUT};
int ledpindebug = 13; //Wireless controller Debug pin. If lit then there is no communication.

DFW dfw(ledpindebug);  // Instantiates the DFW object and setting the debug pin. The debug pin will be set high if no communication is seen after 2 seconds
Servo driveMotorLeft; // Servo object
Servo driveMotorRight; // Servo object
Servo steerMotorLeft;
Servo steerMotorRight;
Servo intakeMotor;
const int clamp = 29; 
const int rightPiston = 28; 
const int leftPiston = 27; 
const int leftDrivePort = 4;
const int rightDrivePort = 7;
const int leftSteerPort = 5;
const int rightSteerPort = 6;
const int intakeMotorPort = 8;
Encoder rightEncoder;
boolean clampDown = false;
boolean armsUp = false;

IntakeState intakeState = STOP;

LiquidCrystal lcd(40, 41, 42, 43, 44, 45);

void setup() {
  Serial.begin(9600); // Serial output begin. Only needed for debug
  dfw.begin(); // Serial1 output begin for DFW library. Buad and port #."Serial1 only"
  driveMotorLeft.attach(leftDrivePort, 1000, 2000); // left drive motor pin#, pulse time for 0,pulse time for 180
  driveMotorRight.attach(rightDrivePort, 1000, 2000); // right drive motor pin#, pulse time for 0,pulse time for 180
  steerMotorLeft.attach(leftSteerPort, 1000, 2000);
  steerMotorRight.attach(rightSteerPort, 1000, 2000);
  intakeMotor.attach(intakeMotorPort, 1000, 2000);
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

int encoderChange(Encoder encoder){
    int encoderAVal = digitalRead(encoder.pinA);
    if(encoder.lastAValue == LOW && encoderAVal == HIGH){
      if(digitalRead(encoder.pinB) == LOW){
        encoder.pos++;
      } else {
        encoder.pos--;
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
  int point = 0;
  // Points:
  // 0: Starting position
  // 1: Parallel with Pen
  // 2: Adjacent to Pen
  // 3: Linear with Ramp
  // 4: On ramp
  
  float robotX = 0; // X-axis displacement of the robot from the starting point, in inches
  float robotY = 0; // Y-axis displacement of the robot from the starting point, in inches 
  RobotDirection robotDirection = N;
  rightEncoder.pos = 0;
  rightEncoder.pinA = 22;
  rightEncoder.pinB = 23;
  unsigned long startTime = millis(); // sets start time of autonomous
  
  int rightEncoderPosition = 0; // Current rotational position of the right encoder
  int rightEncoderALast = LOW; // The value of encoder pin A last time it was checked
  lcd.print("AUTONOMOUS PHASE");


  driveMotorLeft.write(125);
  driveMotorRight.write(45);
  delay(1000);
  driveMotorLeft.write(90);
  driveMotorRight.write(90);
  steerMotorLeft.write(20);
  delay(2000);
  steerMotorLeft.write(90);
  driveMotorLeft.write(125);
  driveMotorRight.write(45);
  delay(3500);
  driveMotorLeft.write(90);
  driveMotorRight.write(90);
  return;
  /**
  driveMotorLeft.write(150);
  driveMotorRight.write(30);
  delay(1500);
  driveMotorLeft.write(30);
  driveMotorRight.write(30);
  delay(400);
  driveMotorLeft.write(90);
  driveMotorRight.write(90);
  delay(100);
  driveMotorLeft.write(150);
  driveMotorRight.write(30);
  delay(1200);
  driveMotorLeft.write(90);
  driveMotorRight.write(90);
  delay(100);
  digitalWrite(clamp, HIGH);
  delay(100);
  driveMotorLeft.write(150);
  driveMotorRight.write(30);
  delay(300);
  driveMotorLeft.write(150);
  driveMotorRight.write(150);
  delay(200);
  return;
  
  time = time * 1000; // converts time from seconds to milliseconds
  while ((millis() - startTime <= time) && (dfw.select())) // compares start time to time entered in the autonomous function and
  {
    
    float distanceTraveled;
    int targetLocationX; // TODO: Make a struct for points (x, y)
    int targetLocationY;
    lcd.setCursor(0, 0);
    /**
    lcd.print("encoderA: ");
    lcd.print(digitalRead(rightEncoder.pinA));
    lcd.setCursor(0, 1);
    lcd.print("encoderB: ");
    lcd.print(digitalRead(rightEncoder.pinB));
    
    lcd.print("encoderPos: ");
    lcd.print(rightEncoder.pos);
    lcd.setCursor(0, 1);
    lcd.print("robotY: ");
    lcd.print(robotY);
    switch(point){
      case 0:
        targetLocationX = 0;
        targetLocationY = 39; // TODO: Update
        break;
      case 1:
        targetLocationX = -16;
        targetLocationY = 39;
        break;
      case 2:
        targetLocationX = -52;
        targetLocationY = 10;
        break;
      case 3:
        targetLocationX = -52;
        targetLocationY = 34;
        break;
    }
    /**rightEncoder.lastPos = rightEncoder.pos;
    rightEncoder.pos = encoderChange(rightEncoder);
    distanceTraveled = (12.6/ 90) * (rightEncoder.pos - rightEncoder.lastPos); // Distance traveled in past increment, in inches
    // 12.6 is the circumference
    switch(robotDirection){
      case N:
        robotY += distanceTraveled;
        break;
      case E:
        robotX += distanceTraveled;
        break;
      case S:
        robotY -= distanceTraveled;
        break;
      case W:
        robotX -= distanceTraveled;
        break;
    }

      point++;
      switch(point){
        case 1:
        driveMotorLeft.write(150);
        driveMotorRight.write(30);
        delay(1000);
        driveMotorLeft.write(30);
        driveMotorRight.write(150);
        return;
          driveMotorLeft.write(90);
          driveMotorRight.write(90);
          /** TODO: Turn 90 degrees CCW 
          driveMotorLeft.write(180);
          driveMotorRight.write(180);
          break;
        case 2:
          // Pick up the pen
          driveMotorLeft.write(90);
          driveMotorRight.write(90);
          delay(100);
          digitalWrite(clamp, HIGH);
          delay(100);
          driveMotorLeft.write(180);
          driveMotorRight.write(180);
          break;
        case 3:
          driveMotorLeft.write(90);
          driveMotorRight.write(90);
          /** TODO: Turn 90 degrees CW 
          driveMotorLeft.write(180);
          driveMotorRight.write(180);
          break;
        case 4:
          return;
      }
    }

    
    rightEncoder.lastAValue = digitalRead(rightEncoder.pinA);
    
    // The select button can be used to skip the autonomous code.
    // Enter Autonomous User Code Here
    
    
    Serial.println("Autonomous"); //prints Autonomous over serial (usb com port)
    //delay(10); //delay to prevent spamming the serial port and to keep servo and dfw libraries happy
  }
  */
  
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
    
    if (dfw.r1()){
        digitalWrite(rightPiston, HIGH);
        digitalWrite(leftPiston, HIGH);
    }
    if (dfw.l1())
    {
      digitalWrite(clamp, HIGH);
    }
    if (dfw.one()){
      intakeState = IN;
    }
    if(dfw.three()){
      intakeState = OUT;
    }
    if(dfw.two()){
      digitalWrite(rightPiston, LOW);
      digitalWrite(leftPiston, LOW);
    }
    if(dfw.four()){
      digitalWrite(clamp, LOW);
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
  autonomous(20); //time in seconds to run the autonomous code
  delay(10000);
  teleop(120);
  exit(0);
}

