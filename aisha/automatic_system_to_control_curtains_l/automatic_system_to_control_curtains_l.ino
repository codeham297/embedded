//Automatic system to control curtains, lightings and fan
#include <LiquidCrystal.h>
LiquidCrystal lcd(6, 5, 1, 2, 3, 4);

#include <DHT.h>
#define DHTPIN 7           // DHT11 data pin is connected to Arduino pin 8

#define DHTTYPE DHT11       // DHT11 sensor is used
DHT dht(DHTPIN, DHTTYPE);   // Initialize DHT library

char temperature[] = "00";
char humidity[]    = "00";

pinMode(11, OUTPUT);

int ldr=A0;
int pir=0;
// int temp=A1;
int lamp=11;
int curtain=12;
int curtain2 = 10;
int fan=13;
int ldrStatus;
int pirStatus;
int tempStatus;
int normal_light = 400; //The value should be between 1021 and 1023 for ungrounded ldr
int darkness_status;
int normal_temperature_level = 20, temperature_level;
int person_status;
int light_status, fan_status, curtain_status = 1;
int turn_on_fan_delay = 200;
int curtain_time = 2000;

int detectPersonStatus();
int detectPresenceOfDarkness();
int checkTemperatureLevel();
void turnOnLight();
void turnOffLight();
void turnOnFan();
void turnOffFan();
void openCurtain();
void closeCurtain();


void setup() {
    lcd.begin(16, 4);

    pinMode(ldr, INPUT);
    pinMode(pir, INPUT);
    // pinMode(temp, INPUT);
    pinMode(lamp, OUTPUT);
    pinMode(curtain, OUTPUT);
    pinMode(curtain2, OUTPUT);
    pinMode(fan, OUTPUT);
}

void loop() {
start:
    if(detectPersonStatus() == 1){
        turnOnLight();
        turnOffLight();
        turnOnFan();
        turnOffFan();
        openCurtain();
        closeCurtain();
        goto start;
    }

    else if(detectPersonStatus() == 0){
        turnOnLight();
        turnOffLight();
        turnOnFan();
        turnOffFan();
        openCurtain();
        closeCurtain();
        goto start;

    }

}
 

 int detectPersonStatus(){
    if(digitalRead(pir)){
        return 1;
    }
    else if(!digitalRead(pir)){
        return 0;
    }
 }

int detectPresenceOfDarkness(){
    darkness_status = analogRead(ldr);
    if(normal_light < darkness_status){
        return 0;
    }
    else if(normal_light >= darkness_status){
        return 1;
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

void turnOnLight(){
    if(detectPersonStatus()){
           if(detectPresenceOfDarkness() == 1){
            if(light_status == 0){
               digitalWrite(lamp, HIGH);
                 light_status =1;
    }
    }
    }
    else if(!detectPersonStatus()){
        turnOffLight();
    }

}

void turnOffLight(){
    if(detectPersonStatus()){
            if(detectPresenceOfDarkness() == 1){
        turnOnLight();
    }
    else if(detectPresenceOfDarkness() == 0){
        digitalWrite(lamp, LOW);
        light_status = 0;
    }
    }
    else if(!detectPersonStatus()){
        digitalWrite(lamp, LOW);
        light_status = 0;
    }

}

void turnOnFan(){
    if(detectPersonStatus()){
        temperature_level = checkTemperatureLevel();
        if(temperature_level > normal_temperature_level){
            if(fan_status == 0){
            digitalWrite(fan, HIGH);
                fan_status = 1;
            }
        }
        }
    else if(!detectPersonStatus()){
        turnOffFan();
    }

}

void turnOffFan(){
    if(!detectPersonStatus()){
        digitalWrite(fan, LOW);
        fan_status = 0;
    }
    else if(detectPersonStatus()){
        if(temperature_level <= normal_temperature_level){
            if(fan_status == 1){
            digitalWrite(fan, LOW);
                fan_status = 000000;
            }
        }
        }

        turnOnFan();
}

void openCurtain(){
    if(!detectPresenceOfDarkness()){
        if(curtain_status == 0){
            digitalWrite(curtain, HIGH);
            digitalWrite(curtain2, LOW);
            delay(curtain_time);
            digitalWrite(curtain, LOW);
            curtain_status = 1;
    }
    }



}
void closeCurtain(){
    if(detectPresenceOfDarkness()){
            if(curtain_status == 1){
                digitalWrite(curtain2, HIGH);
                digitalWrite(curtain, LOW);
                delay(curtain_time);
                digitalWrite(curtain2, LOW);
                curtain_status = 0;
    }
    }

}


void displayText(int row, char text[]){
    lcd.setCursor(0, row);
    lcd.print(text);
    return;
}

void displayInt(int row, int value){
    lcd.setCursor(0, row);
    lcd.print(value);
    return;
}
void debug(){
    displayText(0, "DEBUGGING HERE");
}
void debugDel(){
    displayText(0, "DEBUGGINGDELAY");
    delay(500);
}
void debugVal(int text){
    lcd.setCursor(0, 0);
    lcd.print(text);
}
