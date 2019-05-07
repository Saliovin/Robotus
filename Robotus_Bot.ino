#include <SoftwareSerial.h> // Library for serial communication in digital pins

//Pin ini
const int TX_pin = 12; // HC12 TX pin
const int RX_pin = 11; // HC12 RX pin
const int dirstep[6][2]={ // Dir Step Pins
                          {2,3}, // Hand Side 1
                          {4,5}, // Hand Side 2
                          {6,7}, // Grip
                          {15,14}, // 2nd Joint
                          {17,16}, // Missing
                          {19,18} // Base
                        };
                            
SoftwareSerial HC12(TX_pin, RX_pin); // HC12 pin ini

//Variable creation
char command;
int motorSpeed = 0; // Speed of the motor. The higher the value, the slower the motor moves
int setMotor, timeStart;
boolean HC12end = false;

void setup() {
  // Pins
  for (int j=0; j<6; j++){
    for (int k=0; k<2; k++){
      pinMode(dirstep[j][k], OUTPUT);
    }
  }

  //Serial
  Serial.begin(9600);
  HC12.begin(9600);
}

void loop() {
  while(HC12.available()) { // Runs when the serial buffer contains data
    command = HC12.read(); // Save read character to a variable
    Serial.write(command); // Debug
    
    if(!(command == '1'||command == '2'||command == '3'||command == '4'||command == '5'||command == '6'||command == '7'||command == '8')) { //Check if new motor has been selected by user
      switch(command) { //Set current motor control based on user input
        case 'A': //A=1st Joint
          setMotor=4;
          break;
        case 'B': //B=2nd Joint
          setMotor=3;
          break;
        case 'C': //C=Arm
          setMotor=0;
          break;
        case 'D': //D=Grip
          setMotor=2;
          break;
      //case 'E': //E=motor x // Should the start and select buttons be used, remove the comment marks
      //  setMotor=5;
      //  break;
      //case 'F': //F=motor x
      //  setMotor=6;
      //  break;
      }
    }
    else {
      HC12end = true;
    }
    
    if(HC12end) break; // Breaks out of loop if a motor command is found in the received data
  }
  
  if(HC12end) { // Runs if motor command is inputed
    switch(command) { // Runs the corresponding case based on motor command
                      // setMotor = 0 controls the two hand motors
                      // cases 5 to 8 moves the set motor as well as the base motor
      case '1': // 1 = Up on the joystick
        if(setMotor == 0) {
          moveTwoMotors(setMotor, setMotor+1, LOW, HIGH);
        }
        else {
          moveMotor(setMotor,HIGH);
        }
        break;
      case '2': // 2 = Right on the joystick
        if(setMotor == 0) {
          moveTwoMotors(setMotor, setMotor+1, HIGH, HIGH);
        }
        else {
          moveMotor(5,HIGH);
        }
        break;
      case '3': // 3 = Down on the joystick
        if(setMotor == 0) {
          moveTwoMotors(setMotor, setMotor+1, HIGH, LOW);
        }
        else {
          moveMotor(setMotor,LOW);
        }
        break;
      case '4': // 4 = Left on the joystick
        if(setMotor == 0) {
          moveTwoMotors(setMotor, setMotor+1, LOW, LOW);
        }
        else {
          moveMotor(5,LOW);
        }
        break;
      case '5': // 5 = Down Left on the joystick
        moveTwoMotors(5, setMotor, LOW, LOW);
        break;
      case '6': // 6 = Down Right on the joystick
        moveTwoMotors(5, setMotor, HIGH, LOW);
        break;
      case '7': // 7 = Up Left on the joystick
        moveTwoMotors(5, setMotor, LOW, HIGH);
        break;
      case '8': // 8 = Up Right on the joystick
        moveTwoMotors(5, setMotor, HIGH, HIGH);
        break;
    }
    
    HC12end = false;
  }
}

void motorDelay(int del) { // Delay function that clears the serial buffer while waiting
  unsigned long start = micros();
  while((micros()-start)<=del) {
    HC12.read();
  }
}

void moveMotor(int motor, int dir) { // Moves a single motor according to the input direction
  digitalWrite(dirstep[motor][0], dir); 
  digitalWrite(dirstep[motor][1], HIGH); 
  motorDelay(motorSpeed); 
  digitalWrite(dirstep[motor][1], LOW); 
  motorDelay(50);
}

void moveTwoMotors(int motor1, int motor2, int dir1, int dir2) { // Moves two motors according to the input directions
  digitalWrite(dirstep[motor1][0], dir1);
  digitalWrite(dirstep[motor2][0], dir2);
  digitalWrite(dirstep[motor1][1], HIGH);
  digitalWrite(dirstep[motor2][1], HIGH); 
  motorDelay(motorSpeed); 
  digitalWrite(dirstep[motor1][1], LOW);
  digitalWrite(dirstep[motor2][1], LOW); 
  motorDelay(50);
}
