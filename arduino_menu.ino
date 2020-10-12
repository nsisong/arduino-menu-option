/* ARDUINO menu code - four buttons actions
 *  
 *  DEVELOPED BY NSISONG PETER ©2020 @KODEHAUZ
 *  this code demonstrate the use of button for menu sellections in arduino
 *  the library used in this code is the liquid crystal distplay which can be downloaded from arduino .cc
 *  for this project demonstration, i will be using a 16 by 2 lcd display
 *  
 *  this code print power perameters and their values below
 *  
 *  the Pin configurations are as follows
 *  sda = A4 
 *  scl = A5
 * 
 */
#include "Wire.h" // For I2C
#include "LCD.h" // For LCD
#include "LiquidCrystal_I2C.h" // Added library*
//Set the pins on the I2C chip used for LCD connections
//ADDR,EN,R/W,RS,D4,D5,D6,D7 
//#include <LiquidCrystal.h>
//LiquidCrystal lcd(6, 7, 5, 4, 3, 2);
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7); // 0x27 is the default I2C bus address of the backpack-see article

//Input & Button Logic
const int numOfInputs = 4;
const int inputPins[numOfInputs] = {8,9,10,11};
int inputState[numOfInputs];
int lastInputState[numOfInputs] = {LOW,LOW,LOW,LOW};
bool inputFlags[numOfInputs] = {LOW,LOW,LOW,LOW};
long lastDebounceTime[numOfInputs] = {0,0,0,0};
long debounceDelay = 5;

//LCD Menu Logic
const int numOfScreens = 4;// number of menues
int currentScreen = 0;
String screens[numOfScreens][2] = {{"Tidal volume","TV"}, {"In time/Out time", "I:E"}, 
{"Positive .. pressure","PEEP"},{"Respiratory Rate.","breaths/min"}};
int parameters[numOfScreens];

void setup() {
   Serial.begin(9600);
  lcd.begin(20, 4);
  lcd.setBacklightPin(3,POSITIVE); // BL, BL_POL
  lcd.setBacklight(HIGH);
  lcd.setCursor(5,1);
  lcd.print("KODEHAUZ");
  lcd.setCursor(4,2);
  lcd.print("COVID VENT");
  delay (5000);
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("PLEASE READ");
  lcd.setCursor(4,1);
  lcd.print("USER MANUAL");
  lcd.setCursor(4,2);
  lcd.print("BEFORE USE !!!");
  delay(5000);
  
  for(int i = 0; i < numOfInputs; i++) {
    pinMode(inputPins[i], INPUT);
    digitalWrite(inputPins[i], HIGH); // pull-up 20k
  }
}

void loop() {
  setInputFlags();
  resolveInputFlags();
}

void setInputFlags() {
  for(int i = 0; i < numOfInputs; i++) {
    int reading = digitalRead(inputPins[i]);
    if (reading != lastInputState[i]) {
      lastDebounceTime[i] = millis();
    }
    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      if (reading != inputState[i]) {
        inputState[i] = reading;
        if (inputState[i] == HIGH) {
          inputFlags[i] = HIGH;
        }
      }
    }
    lastInputState[i] = reading;
  }
}

void resolveInputFlags() {
  for(int i = 0; i < numOfInputs; i++) {
    if(inputFlags[i] == HIGH) {
      inputAction(i);
      inputFlags[i] = LOW;
      printScreen();
    }
  }
}

void inputAction(int input) {
  if(input == 0) {
    if (currentScreen == 0) {
      currentScreen = numOfScreens-1;
    }else{
      currentScreen--;
    }
  }else if(input == 1) {
    if (currentScreen == numOfScreens-1) {
      currentScreen = 0;
    }else{
      currentScreen++;
    }
  }else if(input == 2) {
    parameterChange(0);
  }else if(input == 3) {
    parameterChange(1);
  }
}

void parameterChange(int key) {
  if(key == 0) {
    parameters[currentScreen]++;
  }else if(key == 1) {
    parameters[currentScreen]--;
  }
}

void printScreen() {
  lcd.clear();
  lcd.print("RunTime : ");
  lcd.setCursor(0,1);
  lcd.print(screens[currentScreen][0]);
  lcd.setCursor(0,2);
  lcd.print(parameters[currentScreen]);
  lcd.print(" ");
  lcd.print(screens[currentScreen][1]);

  Serial.println(screens[currentScreen][0]);
//  Serial.setCursor(0,1);
  Serial.println(parameters[currentScreen]);
//  Serial.print(" ");
  Serial.println(screens[currentScreen][1]);
}
