#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

// LCD settings
LiquidCrystal lcd(12, 11, 10, A0, A1, A2); // RS, E, D4, D5, D6, D7

// KEYPAD settings
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; // row pins
byte colPins[COLS] = {5, 4, 3};    // columns pins 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// SERVO settins
Servo myServo;
const int servoPin = 13; // The middle pin of servo connection to the 13 pin of the arduino.
int servoOpenAngle = 90; //  The angle that servo opens
int servoCloseAngle = 0; // The angle that servo closes
bool isLocked = true;

// The password managements
String defaultPassword = "0000";
String inputPassword = "";
bool isAuthenticated = false;

void setup() {
  lcd.begin(16, 2);
  // enter password setups:
  lcd.print("Enter Password:");
  myServo.attach(servoPin);
  myServo.write(servoCloseAngle); // servo setup in lock condition
}

void loop() {
  char key = keypad.getKey();//Here the user should enter the key of password

  if (key) {
    if (isAuthenticated) {
      handleAuthenticated(key);
    } else {
      handleAuthentication(key);
    }
  }
}

void handleAuthentication(char key) {
  // anlayzing when the user enters key(#)
  if (key == '#') {
    checkPassword();
  } else if (key == '*') {
    // In order to clear the entered password:
    inputPassword = "";
    lcd.setCursor(0, 1);
    lcd.clear();
    lcd.print("Cleared          ");
    lcd.clear();
    lcd.print("Enter Password:");
  } else {
    inputPassword += key;
    lcd.setCursor(0, 1);
    lcd.print(inputPassword);
  }
}

void checkPassword() {
  if (inputPassword == defaultPassword) {
    isAuthenticated = true;
    lcd.clear();
    lcd.print("Access Granted");
    myServo.write(servoOpenAngle); // lock decryption
    isLocked = false;
    delay(2000);
    lcd.clear();
    lcd.print("1: Change Pass");// Here, the lcd shows two options :1- change the password ,2- exit
    lcd.setCursor(0, 1);
    lcd.print("2: Exit          ");
  } else {
    lcd.clear();
    lcd.print("Access Denied");
    delay(2000);
    lcd.clear();
    lcd.print("Enter Password:");
    inputPassword = "";
  }
}

void handleAuthenticated(char key) {
  if (key == '1') {// goto change password situations.
    changePassword();
  } else if (key == '2') {
    logout();
    myServo.write(servoCloseAngle); // The servo should be closed when exiting.
    isLocked = true;
  }
}

void changePassword() {
  lcd.clear();
  lcd.print("New Password:");
  inputPassword = "";
  while (true) {
    char key = keypad.getKey();
    if (key) {
      if (key == '#') {
        // After decrypting the password, the user should not enter the same password.
        if (defaultPassword == inputPassword){
          lcd.clear();
          lcd.print("Same Password!");
          lcd.clear();
          delay(1000);
          lcd.print("New Password:");
          inputPassword = "";
        }
        else if (inputPassword.length() == 4&& inputPassword !=defaultPassword) {
          defaultPassword = inputPassword;
          lcd.clear();
          lcd.print("Password Saved");//To save the decrypted password:
          delay(2000);
          logout();
          myServo.write(servoCloseAngle); // servo opens after password accessments.
          isLocked = true;
          return;
        } else {// To check that entered password is only 4 digits.
          lcd.clear();
          lcd.print("4 digits only!");
          delay(2000);
          lcd.clear();
          lcd.print("New Password:");
          inputPassword = "";
        }
        // To remove the entered password:
      } else if (key == '*') {
        inputPassword = "";
        lcd.setCursor(0, 1);
        lcd.print("password cleared.");
        delay(2000);
        lcd.clear();
        lcd.print("Enter Password:");
      } else {
        inputPassword += key;
        lcd.setCursor(0, 1);
        lcd.print(inputPassword);
      }
    }
  }
}
// In order to reset:
void logout() {
  isAuthenticated = false;
  inputPassword = "";
  lcd.clear();
  lcd.print("Enter Password:");
}