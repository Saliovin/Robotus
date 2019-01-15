#include <SoftwareSerial.h> // Library for serial communication in digital pins

// LCD ini
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C  lcd(0x3F,2,1,0,4,5,6,7);

// Pin ini
const int pin_button_A = 2;
const int pin_button_B = 3;
const int pin_button_C = 4;
const int pin_button_D = 5;
const int pin_button_E = 6;
const int pin_button_F = 7;
const int pin_button_X = A0;
const int pin_button_Y = A1;
const int pin_button_TX = 12;
const int pin_button_RX = 13;

SoftwareSerial HC12(pin_button_TX, pin_button_RX); //HC12 pin ini

//Joystick ini values
int analog_x=505;
int analog_y=506;

void setup(){
  //Controller
  pinMode(pin_button_A, INPUT_PULLUP);
  pinMode(pin_button_B, INPUT_PULLUP);
  pinMode(pin_button_C, INPUT_PULLUP);
  pinMode(pin_button_D, INPUT_PULLUP);
  pinMode(pin_button_E, INPUT_PULLUP);
  pinMode(pin_button_F, INPUT_PULLUP);

  // LCD 
  lcd.begin (16,2);
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.clear();
  lcd.print("Controlled Motor");

  //Serial
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);                                                              
  Serial.begin(9600);
  HC12.begin(9600);                                                      
  delay(100);
  digitalWrite(13,HIGH);
}

void loop() {
  // Checks if any of the face buttons are pressed and outputs the corresponding data through the HC12 and LCD module
  // Face buttons are pulled HIGH, LOW input means the button is pressed
  if(!digitalRead(pin_button_A)){ 
    sendData('A', "First Arm:A", 0);
  }
  else if(!digitalRead(pin_button_B)){
    sendData('B', "Second Arm:B", 0);
  }
  else if(!digitalRead(pin_button_C)){
    sendData('C', "Claw:C", 0);
  }
  else if(!digitalRead(pin_button_D)){
    sendData('D', "Claw Grip:D", 0);
  }
  //Should the start and select buttons be used, remove the comment marks
  //else if(!digitalRead(pin_button_E)) sendData('E', "xxxxx", 0);
  //else if(!digitalRead(pin_button_F)) sendData('F', "xxxxx", 0);

  // Checks the current position of the joystick and outputs data based on the comparison of preset values to that of the current position
  analog_x = analogRead(pin_button_X);
  analog_y = analogRead(pin_button_Y);
  
  if ((analog_x>=1000)&&(analog_y<1000)&&(analog_y>10)){ // Right
    sendData('2', "R", 14);
  }
  else if ((analog_x<=10)&&(analog_y<1000)&&(analog_y>10)){ // Left
    sendData('4', "L", 14);
  }
  else if ((analog_y>=1000)&&(analog_x<1000)&&(analog_x>10)){ // Up
    sendData('1', "U", 14);
  }
  else if ((analog_y<=10)&&(analog_x<1000)&&(analog_x>10)){ // Down
    sendData('3', "D", 14);
  }
  else if ((analog_x<1000)&&(analog_x>10)&&(analog_y<1000)&&(analog_y>10)){ // Neutral
    lcd.setCursor(14,1);
    lcd.print("  ");
  }
  else if ((analog_y<=10)&&(analog_x<=10)){ // Down Left
    sendData('5', "DL", 14);
  }
  else if ((analog_y<=10)&&(analog_x>=1000)){ // Down Right
    sendData('6', "DR", 14);
  }
  else if ((analog_y>=1000)&&(analog_x<=10)){ // Up Left
    sendData('7', "UL", 14);
  }
  else if ((analog_y>=1000)&&(analog_x>=1000)){ // Up Right
    sendData('8', "UR", 14);
  }
}

void sendData(char toBeSent, String toBeDisplayed, int cursorLocation) { // Sends input data through the HC12 module and prints the input string at the cursor location
  lcd.setCursor(cursorLocation, 1);
  HC12.write(toBeSent);
  lcd.print(toBeDisplayed);
  lcd.print("                ");
}
