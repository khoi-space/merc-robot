#include <AFMotor.h>
#include <Servo.h>
//--------------NOTE-----------------
/* Commands:
  - A: switch between fast/slow mode
  - F: move forward
  - B: move backward
  - R: turn right
  - L: turn left
  - X: stop

  - U: lift arms
  - D: unlift arms
  - S: stop action

  - G: clamp
  - T: unclamp
  - W: stop action

  - Q: AutoMode (line following)
*/
// ----------------------------------
#define left A0
#define right A1
#define mid A2
char cmd = 0;
int spd = 0;
int slow_spd = 100;
int fast_spd = 255;
int count = 0;
int time = 0;
bool fast_mode = true;
bool auto_mode = false;
bool is_waiting = false;
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);
Servo arm; // attached to port 9
Servo palm; // attached to port 10
void setup() {  
    Serial.begin(9600);           // Open Serial monitor on baud 9600 bps 
    fast_mode = true;
    auto_mode = false;
    motor1.setSpeed(0);
    motor2.setSpeed(0);
    motor3.setSpeed(0);
    motor4.setSpeed(0);
    palm.attach(9);     // Servo2   
    arm.attach(10);     // Servo1
    pinMode(left, INPUT);
    pinMode(right, INPUT);
    pinMode(mid, INPUT);
}  
 
void loop() {
  if (Serial.available()) {
    cmd = Serial.read();
  }

  if (!auto_mode) { // ----------------------------- MANUAL MODE --------------------------------
    switch (cmd) {
      case 'A': // Switch fast/slow mode
        fast_mode = !fast_mode;
        cmd = 0;
        break;
      
      case 'Q': // Trigger AutoMode
        auto_mode = !auto_mode;
        delay(5000);
        cmd = 0;
        break;

      case 'X': // Stop moving
        stop();
        break;

      case 'F': // Go forward
        goForward(fast_mode ? fast_spd : slow_spd);
        break;

      case 'B': // Go backward
        goBackward(fast_mode ? fast_spd : slow_spd);
        break;

      case 'L': // Turn left
        rotateLeft(fast_mode ? fast_spd : slow_spd + 20);
        break;

      case 'R': // Turn right
        rotateRight(fast_mode ? fast_spd : slow_spd + 20);
        break;

      // ------------------SERVO ARM-------------------
      case 'U': // Lift arms
        arm.attach(10);
        arm.write(1);
        cmd = 0;
        break;

      case 'D': // Unlift arms
        arm.attach(10);
        arm.write(181);
        cmd = 0;
        break;

      case 'S': // Stop action
        arm.write(90);
        arm.detach();
        break;

      // -------------------SERVO PALM----------------
      case 'G': // Clamp
        palm.attach(9);
        palm.write(1);
        delay(10);
        break;

      case 'T': // Unclamp
        palm.attach(9);
        palm.write(181);
        delay(10);
        break;

      case 'W': // Stop action
        palm.write(palm.read());
        palm.detach();
        break;

      default:
        stop();
        break;
    }
    delay(100);
  } else { // ---------------------------- AUTOMODE -------------------------
    //Back-up
    if (digitalRead(left)==1 && digitalRead(mid)==1 && digitalRead(right)==1) {
      ++count;
      goForward(slow_spd);
      delay(300);
      if (count == 2) {
        count = 0;
        stop();
        auto_mode = false;
      }
    } else if (digitalRead(left)==1) {
      rotateLeft(slow_spd + 50);
    } else if (digitalRead(right)==1) {
      rotateRight(slow_spd + 50);
    } else {
      goForward(slow_spd);
    }
  }
}

void goForward(int spd) {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor1.setSpeed(spd);
  motor2.setSpeed(spd);

  motor4.run(FORWARD);
  motor3.run(FORWARD);
  motor4.setSpeed(spd);
  motor3.setSpeed(spd);
}

void goBackward(int spd) {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor1.setSpeed(spd);
  motor2.setSpeed(spd);

  motor4.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.setSpeed(spd);
  motor3.setSpeed(spd);
}

void turnRight(int spd) {
  motor1.run(BACKWARD);
  motor2.run(RELEASE);
  motor1.setSpeed(spd);
  motor2.setSpeed(0);

  motor4.run(BACKWARD);
  motor3.run(RELEASE);
  motor4.setSpeed(spd);
  motor3.setSpeed(0);
}

void rotateRight(int spd) {
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  motor1.setSpeed(spd);
  motor2.setSpeed(spd);

  motor4.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.setSpeed(spd);
  motor3.setSpeed(spd);
}

void turnLeft(int spd) {
  motor1.run(RELEASE);
  motor2.run(BACKWARD);
  motor1.setSpeed(0);
  motor2.setSpeed(spd);

  motor4.run(RELEASE);
  motor3.run(BACKWARD);
  motor4.setSpeed(0);
  motor3.setSpeed(spd);
}

void rotateLeft(int spd) {
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  motor1.setSpeed(spd);
  motor2.setSpeed(spd);

  motor4.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.setSpeed(spd);
  motor3.setSpeed(spd);
}

void slightTurnLeft(int spd) {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor1.setSpeed(spd - 50);
  motor2.setSpeed(spd);

  motor4.run(FORWARD);
  motor3.run(FORWARD);
  motor4.setSpeed(spd - 50);
  motor3.setSpeed(spd);
}

void slightTurnRight(int spd) {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor1.setSpeed(spd);
  motor2.setSpeed(spd - 50);

  motor4.run(FORWARD);
  motor3.run(FORWARD);
  motor4.setSpeed(spd);
  motor3.setSpeed(spd - 50);
}

void stop() {
  motor1.run(RELEASE);
  motor1.setSpeed(0);
  motor2.run(RELEASE);
  motor2.setSpeed(0);

  motor3.run(RELEASE);
  motor4.setSpeed(0);
  motor3.run(RELEASE);
  motor4.setSpeed(0);
}