#include <LiquidCrystal.h>
#include <Keypad.h>
#include <EEPROM.h>
#define  rs 9
#define  en 8
#define  d7 2
#define  d6 3
#define  d5 4
#define  d4 5
#define  green 22
#define  red   23

LiquidCrystal lcd = LiquidCrystal(rs , en , d4 , d5 , d6 , d7);
int votes[4] = {0 , 0, 0 ,0};
int password = 123;
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'7','8','9','/'},
  {'4','5','6','*'},
  {'1','2','3','-'},
  {'O','0','=','+'}
};

byte rowPins[ROWS] = {46,47, 48 , 49}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {50,51,52,53}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void doWork(int x , int ID){
    int value = EEPROM.read(x - 1);
    if(value == 255) value = 0;
    EEPROM.write(x - 1 , value+1);
    votes[x - 1] += 1;
    digitalWrite(green , HIGH);
    //delay(5);
    digitalWrite(green , LOW);
    EEPROM.write(ID + 4 , ID);
}

int hasVoted(int ID){
  int value = EEPROM.read(ID + 4);
  return (value == ID ? 1 : 0);
}

void getMemoryValues(){
  for(int i = 0; i < 4; i++){
    int value = EEPROM.read(i);
    if(value == 255) value = 0;
    votes[i] = value;
  }
}

void showResults(){
    lcd.clear();
    lcd.print("  Vote Counts:  ");
    lcd.setCursor(0 , 1);
    for(int i = 0 ;i < 4 ; i++){
      lcd.print(i + 1);
      lcd.print(": ");
      lcd.print(votes[i]);
      lcd.print("      ");
      delay(50);
      lcd.setCursor(0 , 1); 
    }
}

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16 , 2);
  lcd.clear();
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  pinMode(green , OUTPUT);
  pinMode(red , OUTPUT);
  getMemoryValues();
  lcd.print(" Enter Password ");
  lcd.setCursor(0 , 1);
  lcd.print("     Press =    ");
  int savedPass = EEPROM.read(200);
  password = savedPass != 255 ? savedPass : password; 
}

void loop() {
  // put your main code here, to run repeatedly:
  char key = keypad.getKey();
  if(key){
    if(key && key == '='){
      String entered = "";
      lcd.clear();
      for(int i = 0 ; i < 3 ; i++){
        char temp = keypad.waitForKey();
        entered += temp;
        lcd.print(temp);
      }
      if(entered.toInt() == password){
        digitalWrite(green , HIGH);
        lcd.clear();
        lcd.print(" Show Results: * ");
        lcd.setCursor(0 , 1);
        lcd.print("  Change Pass:+ ");
        char temp = keypad.waitForKey();
        if(temp == '*'){
          showResults();
        }else if(temp == '+'){
          digitalWrite(green , LOW);
          lcd.clear();
          entered = "";
          for(int i = 0 ; i < 3 ; i++){
            char temp = keypad.waitForKey();
            entered += temp;
            lcd.print(temp);
          }
          if(entered.toInt() > 254){
            digitalWrite(red , HIGH);
            lcd.clear();
            lcd.print(" Not Changed  ");
            delay(50);
            digitalWrite(red , LOW);
          }else{
            digitalWrite(green , HIGH);
            password = entered.toInt();
            EEPROM.write(200 , password);
            lcd.clear();
            lcd.print("Password Changed"); 
            delay(50);
            digitalWrite(green , LOW);
          }
        }else{
          digitalWrite(red , HIGH);
          lcd.clear();
          lcd.print("Invalid Input!!!");
          lcd.setCursor(0,1);
          lcd.print("    Aborting    ");
          delay(50);
          digitalWrite(red , LOW);
        }
      }else{
        digitalWrite(red , HIGH);
        lcd.clear();
        lcd.print("    Wrong!!!    ");
        delay(50);
        digitalWrite(red , LOW);
      }
      digitalWrite(green , LOW);
      digitalWrite(red , LOW);
      lcd.clear();
      lcd.print(" Enter Password ");
      lcd.setCursor(0 , 1);
      lcd.print("     Press =    ");
    }
  }else{
    if(Serial.available() >= 2){
      int ID = Serial.parseInt();
      if(ID != 0){
        int temp = hasVoted(ID);
        Serial.println(temp);
        if(temp == 0){
          int confirm = Serial.parseInt();
          if(confirm == 1){
            int x = Serial.parseInt();
            doWork(x , ID); 
          } 
        } 
      }
    }
    if(Serial1.available() >= 2){
      int ID = Serial1.parseInt();
      if(ID != 0){
        int temp = hasVoted(ID);
        Serial1.println(temp);
        if(temp == 0){
          int confirm = Serial1.parseInt();
          if(confirm == 1){
            int x = Serial1.parseInt();
            doWork(x , ID);  
          }
        } 
      }
    }
    if(Serial2.available() >= 2){
      int ID = Serial2.parseInt();
      if(ID != 0){
        int temp = hasVoted(ID);
        Serial2.println(temp);
        if(temp == 0){
          int confirm = Serial2.parseInt();
          if(confirm == 1){
            int x = Serial2.parseInt();
            doWork(x , ID);
          }  
        } 
      }
    }
    if(Serial3.available() >= 2){
      int ID = Serial3.parseInt();
      if(ID != 0){
        int temp = hasVoted(ID);
        Serial3.println(temp);
        if(temp == 0){
          int confirm = Serial3.parseInt();
          if(confirm == 1){
            int x = Serial3.parseInt();
            doWork(x , ID); 
          }  
        } 
      }
    }
  }
  
  
}
