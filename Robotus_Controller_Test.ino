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
  lcd.setCursor(0,1);
  // Face buttons are pulled HIGH, LOW input means the button is pressed
  if(!digitalRead(pin_button_A)){ 
    HC12.write("A");
    lcd.print("First Arm:A     ");
  }
  if(!digitalRead(pin_button_B)){
    HC12.write("B");
    lcd.print("Second Arm:B    ");
  }
  if(!digitalRead(pin_button_C)){
    HC12.write("C");
    lcd.print("Claw:C          ");
  }
  if(!digitalRead(pin_button_D)){
    HC12.write("D");
    lcd.print("Claw Grip:D     ");
  }
  //Should the start and select buttons be used, remove the comment marks
  //if(!digitalRead(pin_button_E)) HC12.write("E");
  //if(!digitalRead(pin_button_F)) HC12.write("F");

  // Checks the current position of the joystick and outputs data based on the comparison of preset values to that of the current position
  analog_x = analogRead(pin_button_X);
  analog_y = analogRead(pin_button_Y);
  if ((analog_x>=1000)&&(analog_y<1000)&&(analog_y>10)){ // Right
    HC12.write("2");
    lcd.setCursor(14,1);
    lcd.print("R ");
  }
  else if ((analog_x<=10)&&(analog_y<1000)&&(analog_y>10)){ // Left
    HC12.write("4");
    lcd.setCursor(14,1);
    lcd.print("L ");
  }
  else if ((analog_y>=1000)&&(analog_x<1000)&&(analog_x>10)){ // Up
    HC12.write("1");
    lcd.setCursor(14,1);
    lcd.print("U ");
  }
  else if ((analog_y<=10)&&(analog_x<1000)&&(analog_x>10)){ // Down
    HC12.write("3"); 
    lcd.setCursor(14,1);
    lcd.print("D ");
  }
  else if ((analog_x<1000)&&(analog_x>10)&&(analog_y<1000)&&(analog_y>10)){ // Neutral
    lcd.setCursor(14,1);
    lcd.print("  ");
  }
  else if ((analog_y<=10)&&(analog_x<=10)){ // Down Left
    HC12.write("5"); 
    lcd.setCursor(14,1);
    lcd.print("DL");
  }
  else if ((analog_y<=10)&&(analog_x>=1000)){ // Down Right
    HC12.write("6"); 
    lcd.setCursor(14,1);
    lcd.print("DR");
  }
  else if ((analog_y>=1000)&&(analog_x<=10)){ // Up Left
    HC12.write("7"); 
    lcd.setCursor(14,1);
    lcd.print("UL");
  }
  else if ((analog_y>=1000)&&(analog_x>=1000)){ // Up Right
    HC12.write("8"); 
    lcd.setCursor(14,1);
    lcd.print("UR");
  }
}
