#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(0 ,1);
int rain;
int close_sensor=8;
int rainsensor=6;
int open_sensor=13;
int open_window_pin = 10;
int close_window_pin = 9;
int open_window_btn= 7;
void openWindow();
void closeWindow();
void displayText(int row, char text[]);
char text[20], previoustext[20];

int open_and_close_time = 1000, system_turn_on_delay = 500;
int window_status = 1;
int rain_status,  message_status;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  pinMode(open_sensor, INPUT);
  pinMode(close_sensor, INPUT);
  pinMode(rain,INPUT);
  pinMode(open_window_pin, OUTPUT);
  pinMode(close_window_pin, OUTPUT);
  pinMode(open_window_btn, INPUT);

  Serial.begin(9600);
  lcd.begin(16, 2);
  strcpy(text, "SYSTEM ACTIVE");
  displayText(0, text);
  delay(system_turn_on_delay);
   lcd.clear();
}

void loop(){
  sM:
  sendMessage();
  dR:
    if(detectRain()){
      strcpy(text, "ITS RAINING");
      displayText(0, text);
      serialText(text);
      closeWindow();
    }

    else if(!detectRain()){
      strcpy(text, "NOT RAINING");
       displayText(0, text);
       serialText(text);
      goto sM;
    }
}

int detectRain(){
  rain_status = digitalRead(rainsensor);
  return rain_status;
}

void openWindow(){
	int window_sensor_cont = 1;
  if(window_status == 0){
  digitalWrite(open_window_pin, HIGH);
  strcpy(text, "OPENING WINDOW");
  displayText(1, text);

  while(window_sensor_cont){
  	if(digitalRead(open_sensor)){
  		digitalWrite(open_window_pin, LOW);
  		window_sensor_cont = 0;
  	}
  }

  strcpy(text, "WINDOW IS OPEN");
  displayText(1, text);
  window_status = 1;
  message_status = 0;
  }
}

void sendMessage(){
  if(digitalRead(open_window_btn)){
   strcpy(text, "SMS SENT SUCCESSFULLY");
    openWindow();
  }
}

void closeWindow(){
	int window_sensor_cont = 1;
  if(window_status == 1){
  digitalWrite(close_window_pin, HIGH);
  strcpy(text, "CLOSING WINDOW");
  displayText(1, text);
  serialText(text);


  while(window_sensor_cont){
  	if(digitalRead(open_sensor)){
  		digitalWrite(close_window_pin, LOW);
  		window_sensor_cont = 0;
  	}
  }

  strcpy(text, "WINDOW CLOSED");
  displayText(1, text);
  serialText(text);
  window_status = 0;
  message_status = 1;

  }
}


void displayText(int row, char text[]){
    lcd.setCursor(0, row);
    lcd.print(text);
    serialText(text);
    return;
}

void serialText(char text[]){
  int change = strcmp(previoustext, text);
  if(change){
    Serial.println(text);
    strcpy(previoustext, text);
  }
}

// void displayInt(int row, int value){
//   lcd.setCursor(0, row);
//   lcd.print(value);
//   return;
// }
// void debug(){
//   displayText(0, "DEBUGGING HERE");
// }
// void debugDel(){
//   displayText(0, "DEBUGGINGDELAY");
//   delay(500);
// }
// void debugVal(int text){
//   lcd.setCursor(0, 0);
//     lcd.print(text);
// }
