#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Key.h>

//Remove these two lines
#include <SoftwareSerial.h>
SoftwareSerial mySerial(0,1);

int motor_turnon_pin1=7;
int motor_turnon_pin2=8;
int buzzer=1;
int led_red=40;
int led_green=41;
int card = 38;
int correct_finger_print_pin_1=35;
int correct_finger_print_pin_2=36;
int incorrect_finger_print_pin=37;
int display_delay_time = 1;
int password_length = 8;
char password[8];

const int rs = 22, en = 23, d4 = 24, d5 = 25, d6 = 26, d7 = 27;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const byte ROWS = 4;
const byte COLS = 3; 

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'D','0','E'},
};
byte rowPins[ROWS] = {31, 32, 33, 34}; 
byte colPins[COLS] = {28, 29, 30}; 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int readFingerprintStatus();
int readRFid();
int matchFingerprint();
char receivePassword();
int matchPassword();
int allowMoreTransaction();
int askMoreTransaction();
int matchCard();

void accessAccount();
void performTransaction();
void closeAccount();
void recordWrongPassword();
void recordWrongFingerprint();
void blockAccount();
void displayLcd();
void buzz();

int allowed_wrong_password_status;
int current_wrong_password_status;
int allowed_wrong_fingerprint_status;
int current_wrong_fingerprint_status;
int buzz_status, buzzer_time;
int finger_print_status, rfid_status, match_fingerprint_status;
int updateI();

int i = 0;
//Remove this lines;
char previoustext[20], text[20];
char password_received = "12345678";

void setup(){
    //Remove these lines
    Serial.begin(9600);
    lcd.begin(20,4);
    pinMode(card,INPUT);
    pinMode(motor_turnon_pin1,OUTPUT);
    pinMode(motor_turnon_pin2,OUTPUT);
    pinMode(correct_finger_print_pin_1,INPUT);
    pinMode(correct_finger_print_pin_2,INPUT);
    pinMode(incorrect_finger_print_pin,INPUT);
    pinMode(buzzer,OUTPUT);
    pinMode(led_red,OUTPUT);
    pinMode(led_green,OUTPUT);
  
    displayLcd(0, "WELCOME TO ATM SYSTEM");
    displayLcd(1, "OUR SYSTEM IS ON");
    delay(display_delay_time);
    displayLcd(2, "WE CARE YOUR PRESENCE");
    delay(display_delay_time);
    displayLcd(3, "ENJOY OUR SERVICE");
    delay(display_delay_time);
    displayLcd(1, "                       ");
    displayLcd(2, "                       ");
    displayLcd(3, "                       ");
  }

 void loop(){
    password_received = receivePassword();
    displayLcd(3, password_received);
// rFS:
//      finger_print_status = readFingerprintStatus();
//     if(finger_print_status == 1){
//         mF:
//         match_fingerprint_status = matchFingerprint();
//         if(match_fingerprint_status == 1){
//             rP:
//             password = receivePassword();
//         }
//         else if(match_fingerprint_status == 0){

//         }
//      }
//      if(finger_print_status == 0){
//         goto rRI;
//      }

// rRI:
//      rfid_status = readRFid();

//      if(rfid_status){
//      }
        
}

int readFingerprintStatus(){
    if(digitalRead(correct_finger_print_pin_1)){
        finger_print_status = 1;
        match_fingerprint_status = 1;
    }
    else if(digitalRead(correct_finger_print_pin_2)){
        finger_print_status = 1;
        match_fingerprint_status = 1;
    }
    else if(digitalRead(incorrect_finger_print_pin)){
        finger_print_status = 1;
        match_fingerprint_status = 0;
    }
    return finger_print_status;
}

int readRFid(){
    if(digitalRead(card)){
        rfid_status = 1;
    }
    return rfid_status;
}
int matchFingerprint(){
   return match_fingerprint_status;
}

char receivePassword(){
    gK:
    char key = keypad.getKey();
    if(key){
        while(i < password_length){
            password[i] = key;
            lcd.setCursor(0, 2);
            lcd.print(password);
            // if(i == 7){
            //     i = 0;
            //     return password;
            // }
            updateI();
            goto gK;
        }
    }
}

int updateI(){
    i = i+1;
    if(i == 8){
        i = 0;
        return atoi(password);
    }
}
int matchPassword();
int allowMoreTransaction();
int askMoreTransaction();
int matchCard();

void accessAccount();
void performTransaction();
void closeAccount();
void recordWrongPassword();
void recordWrongFingerprint();
void blockAccount();

void displayLcd(int row, char text[]){
    lcd.setCursor(0, row);
    lcd.print(text);
    return;
}
void buzz(){
    if(buzz_status != 1){
        digitalWrite(buzzer, HIGH);
        delay(buzzer_time);
        digitalWrite(buzzer, LOW);
    }
}

//Remove all below this line
void serialText(char text[]){
  int change = strcmp(previoustext, text);
  if(change){
    Serial.println(text);
    strcpy(previoustext, text);
  }
}

void serialPrint(char text[]){
    Serial.println(text);
}

void debug(){
  displayLcd(3, "DEBUGGING HERE");
}
void debugDel(){
  displayLcd(3, "DEBUGGINGDELAY");
  delay(500);
}
void debugVal(int text){
  lcd.setCursor(0, 3);
    lcd.print(text);
}