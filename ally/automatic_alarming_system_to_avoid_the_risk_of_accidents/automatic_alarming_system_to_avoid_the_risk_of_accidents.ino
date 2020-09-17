#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int buzzer = 13, sensor1 = 0, sensor2 = 1, sensor3 = 7;
int smoke_status, driver_status, obstacle_status, fault, fault1, fault2, fault3, buzz_status;
void checkSmokeStatus();
void checkDriverStatus();
void checkObstacleStatus();
void buzz();
void displayLcd();

void setup() {
    lcd.begin(16, 4);
    pinMode(sensor1, INPUT);
    pinMode(sensor2, INPUT);
    pinMode(sensor3, INPUT);
    pinMode(buzzer, OUTPUT);
    lcd.setCursor(3, 1);
    lcd.print("WELCOME");
    delay (400);
    lcd.clear();
}

void loop(){
    checkSmokeStatus();
    checkDriverStatus();
    checkObstacleStatus();
}

void checkSmokeStatus(){
    smoke_status = digitalRead(sensor3);
    if(smoke_status == 1){
        fault1 = 1;
        displayLcd(1, "SMOKE DETECTED");
        buzz();
    }
    else if(smoke_status == 0){
        fault1 = 0;
        displayLcd(1, "NO SMOKE      ");
        buzz();
    }
    return;
}

void checkDriverStatus(){
    driver_status = digitalRead(sensor1);
    if(driver_status == 1){
        fault2 = 1;
        displayLcd(2, "DRIVER SLEEPING");
        buzz();
    }
    else if(driver_status == 0){
        fault2 = 0;        
        displayLcd(2, "DRIVER ACTIVE  ");
        buzz();
    }
    return;
}

void checkObstacleStatus(){
    obstacle_status = digitalRead(sensor2);
    if(obstacle_status == 1){
        fault3 = 1;
        displayLcd(3, "FRONT OBSTACLE");
        buzz();
    }
    else if(obstacle_status == 0){
        fault3 = 0;
        displayLcd(3, "NO OBSTACLE   ");
        buzz();
    }
    return;
}

void buzz(){
    if(fault1 == 1 or fault2 == 1 or fault3 ==1){
        fault = 1;
    }
    else{
        fault = 0;
        digitalWrite(buzzer, LOW);
    }
    if(fault == 1 && buzz_status == 0){
        digitalWrite(buzzer, HIGH);
    }
    return;
}

void displayLcd(int row, char text[]){
    lcd.setCursor(0, row);
    lcd.print(text);
    return;
}
