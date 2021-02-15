#include <Keypad.h>
#include <LiquidCrystal.h>
#define  rs 26
#define  en 27
#define  d7 31
#define  d6 30
#define  d5 29
#define  d4 28
#define green 37
#define red 38
#define buz 8

LiquidCrystal lcd = LiquidCrystal(rs , en , d4 , d5 , d6 , d7);
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
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16,2);
  pinMode(green , OUTPUT);
  pinMode(red , OUTPUT);
}

void loop() {
  lcd.clear();
  digitalWrite(green , HIGH);
  lcd.print("     Welcome    ");
  delay(100);
  lcd.clear();
  lcd.setCursor(0 , 0);
  lcd.print("See The Names: *");
  lcd.setCursor(0 , 1);
  lcd.print("To Vote: +");
  char key = keypad.waitForKey();
      if(key == '*'){
        digitalWrite(green , LOW);
        digitalWrite(red , HIGH);
        lcd.clear();
        lcd.print("Code  Name");
        lcd.setCursor(0,1);
        lcd.print("1     Mohammad");
        delay(200);
        digitalWrite(red , LOW);
        digitalWrite(green , HIGH);
        lcd.setCursor(0,1);
        lcd.print("2     MirHossein");
        delay(200);
        digitalWrite(green , LOW);
        digitalWrite(red , HIGH);
        lcd.setCursor(0,1);
        lcd.print("3     Hassan    ");
        delay(200);
        digitalWrite(red , LOW);
        digitalWrite(green , HIGH);
        lcd.setCursor(0,1);
        lcd.print("4     Mahmoud   ");
        delay(200);
      }else if(key == '+'){
        digitalWrite(green , LOW);
        lcd.clear();
        lcd.print("Enter ID: 2digit");
        lcd.setCursor(0 , 1);
        int ID = 0;
        int valid = 1;
        int hasVoted = 0;
        for(int i = 1 ; i > -1 ; i--){
          char entered = keypad.waitForKey();
          lcd.print(entered);
          if(0 <=((int)entered - (int)'0') && ((int)entered - (int)'0') < 10){
            ID += i== 1 ? ((int)entered - (int)'0') * 10 : ((int)entered - (int)'0'); 
          }else{
            valid = 0;
          }
        }
        if(valid == 1){
          Serial.println(ID);
          lcd.clear();
          lcd.print("   Verifying    ");
          delay(20);
          hasVoted = Serial.parseInt();
        }else{
            lcd.clear();
            digitalWrite(green , LOW);
            digitalWrite(red , HIGH);
            lcd.print("Invalid ID!!!");
            tone(buz , 300);
            delay(100);
            digitalWrite(red , LOW);
            noTone(buz);
        }
        if(valid == 1 && hasVoted == 0){
          lcd.clear();
          digitalWrite(green , HIGH);
          lcd.print("   ID is Valid  ");
          delay(50);
          digitalWrite(red , LOW);
          lcd.print("Enter The Code:");
          lcd.setCursor(0 , 1);
          lcd.print("Code Is: ");
          char code = keypad.waitForKey();
          if(0 <((int)code - (int)'0') && ((int)code - (int)'0') < 5){
            lcd.print(code);
            delay(100);
            lcd.setCursor(0 , 0);
            lcd.print("Confirm? +/-    ");
            char confirm = keypad.waitForKey();
            if(confirm == '+'){
              Serial.println(1);
              Serial.println((int)code - (int)'0');
              digitalWrite(green , HIGH);
              lcd.clear();
              lcd.print("   Thank you    ");
              lcd.setCursor(0 , 1);
              lcd.print(" For Your Vote  ");
              tone(buz , 1000);
              delay(100);
              noTone(buz);
            }else if(confirm == '-'){
              Serial.println(0);
              digitalWrite(red , HIGH);
              lcd.clear();
              lcd.print("   Aborting!!!  ");
              tone(buz , 300);
              delay(50);
              digitalWrite(red , LOW);
              noTone(buz);
            }
          }else{
            Serial.println(0);
            lcd.clear();
            digitalWrite(green , LOW);
            digitalWrite(red , HIGH);
            lcd.print("Invalid Key!!!");
            tone(buz , 300);
            delay(100);
            digitalWrite(red , LOW);
            noTone(buz);
          }
        }else{
            lcd.clear();
            digitalWrite(green , LOW);
            digitalWrite(red , HIGH);
            lcd.print("You Have Voted!!");
            tone(buz , 300);
            delay(100);
            digitalWrite(red , LOW);
            noTone(buz);
        }
      }else{
        lcd.clear();
        digitalWrite(green , LOW);
        digitalWrite(red , HIGH);
        lcd.print("Invalid Key!!!");
        tone(buz , 300);
        delay(100);
        digitalWrite(red , LOW);
        noTone(buz);
      }


}
