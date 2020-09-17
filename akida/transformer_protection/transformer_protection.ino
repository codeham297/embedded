#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <DHT.h>
#define DHTPIN 6          // DHT11 data pin is connected to Arduino pin 6

#define DHTTYPE DHT11       // DHT11 sensor is used
DHT dht(DHTPIN, DHTTYPE);   // Initialize DHT library

char temperature[] = "00";
char humidity[]    = "00";
int normal_temperature_level = 30, temperature_level;
float small_volt, volt;
int normal_voltage_level = 220;

SoftwareSerial mySerial(0 ,1);
int red=8;
int temperature_sensor=6;
int relay=13;
int open_relay_pin = 10;
int close_relay_pin = 9;
void openRelay();
void closeRelay();
void displayText(int row, char text[]);
int checkTemperatureLevel();
void serialText(char text[]);
char text[20], previoustext[20];
int voltage_value = A0;

int open_and_close_time = 1000, system_turn_on_delay = 500;
int relay_status = 0;
int message_status;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  pinMode(relay,OUTPUT);
  pinMode(red,OUTPUT);
  pinMode(relay,INPUT);
  pinMode(open_relay_pin, OUTPUT);
  pinMode(close_relay_pin, OUTPUT);
  pinMode(voltage_value, INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  strcpy(text, "SYSTEM ACTIVE    ");
  displayText(0, text);
  delay(system_turn_on_delay);
   lcd.clear();
}

void loop(){
    if(checkTemperatureLevel() > normal_temperature_level){
      if(digitalRead(7)){
        strcpy(text, "EXCESS TEMP,VOLT  ");
        openRelay();
        displayText(0, text);
        serialText(text);
      }
      else if(!digitalRead(7)){
        strcpy(text, "EXCESS TEMP       ");
        openRelay();
        displayText(0, text);
        serialText(text);
      }
    }
    else if(checkTemperatureLevel()<= normal_temperature_level){
      if(digitalRead(7)){
        strcpy(text, "EXCESS VOLT      ");
        openRelay();
        displayText(0, text);
        serialText(text);
      }
      else if(!digitalRead(7)){
        strcpy(text, "NORMAL TEMP,VOLT   ");
        closeRelay();
        displayText(0, text);
        serialText(text);
      }
    }
}

void openRelay(){
  if(relay_status == 0){
  digitalWrite(open_relay_pin, HIGH);
  strcpy(text, "OPENING RELAY  ");
  displayText(1, text);
  delay(open_and_close_time);
  strcpy(text, "RELAY IS OPEN   ");
  displayText(1, text);
  relay_status = 1;
  message_status = 0;
  }
}

void closeRelay(){
  if(relay_status == 1){
  digitalWrite(open_relay_pin, LOW);
  strcpy(text, "CLOSING RELAY   ");
  displayText(1, text);
  serialText(text);
  delay(open_and_close_time);
  strcpy(text, "RELAY CLOSED    ");
  displayText(1, text);
  serialText(text);
  relay_status = 0;
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

  int checkTemperatureLevel(){
    int temperature_int_1;
    byte RH = dht.readHumidity();
    byte Temp = dht.readTemperature();
    temperature[0] = Temp/ 10 + 48;
    temperature[1] = Temp % 10 + 48;
    humidity[0] = RH/ 10 + 48;
    humidity[1] = RH % 10 + 48;
    lcd.setCursor(0, 0);
    temperature_int_1 = atoi(temperature);
    temperature_level = temperature_int_1;
    return temperature_level;
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
