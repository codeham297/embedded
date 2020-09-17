// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>
// Include dependant SPI Library 
#include <SPI.h> 
 
// Create Amplitude Shift Keying Object
RH_ASK rf_driver;
int red1 =A4;
int red2=A5;
int red3=2;
int red4=3;
int yellow1=4;
int yellow2=5;
int yellow3=6;
int yellow4=7;
int green1 =8;
int green2=9;
int green3=10;
int green4=12;
int blue = 13;
int buzzer = A0;
int red, yellow, green;
int waiting_time = 5000;
int delaytime =1000;
int buzzer_time=500;
int green_to_yellow_time, yellow_to_red_time;
int i = 1;
int caller;
int buzz_status;
int begin = 1;
unsigned long start, stop, duration;
int status;
void stopCars();
void allowCars();
int checkSensorStatus();
void buzz();
void lampsRedOn();
void lampBlueOn();
void otherLampsOff();

void setup()
{

    // Initialize ASK Object
    rf_driver.init();
    // Setup Serial Monitor
    Serial.begin(9600);
    pinMode(red1, OUTPUT);
    pinMode(red2, OUTPUT);
    pinMode(red3, OUTPUT);
    pinMode(red4, OUTPUT);
    pinMode(yellow1, OUTPUT);
    pinMode(yellow2, OUTPUT);
    pinMode(yellow3, OUTPUT);
    pinMode(yellow4, OUTPUT);
    pinMode(green1, OUTPUT);
    pinMode(green2, OUTPUT);
    pinMode(green3, OUTPUT);
    pinMode(green4, OUTPUT);
    pinMode(blue, OUTPUT);
    pinMode(buzzer, OUTPUT);
    green_to_yellow_time = delaytime/2;
    yellow_to_red_time = delaytime/8;
    buzz_status = 0;

    otherLampsOff();
    lampsRedOn();

    start = millis();
}
 
void loop(){
    checkSensorStatus();
    Serial.println(status);
    if(status == 0){
        allowCars();
    }
    else if(status == 1){
        stopCars();
    }
}

int checkSensorStatus(){
    // first_time = 1;
    status;
    // Set buffer to size of expected message
    uint8_t buf[11];
    uint8_t buflen = sizeof(buf);
    // Check if received packet is correct size
    if (rf_driver.recv(buf, &buflen)){
      
      // Message received with valid checksum
      if(buflen == 7){
        status = 1;
      }
      else if(buflen == 11){
        status = 0;
      }
    }
    else if(rf_driver.recv(buf, &buflen)){
        stop = millis();
        duration = stop - start;
        if(duration > waiting_time){
            status = 0;
        }
        else if(duration <= waiting_time){
            status = status;
        }
    }
    return status;

}



void stopCars(){
    caller = 4;
    otherLampsOff();
    lampsRedOn();
    lampBlueOn();
    buzz();
    loop();

}

void allowCars(){
    caller = 5;
    if(buzz_status == 1){
        lampBlueOff();
        buzz_status = 0;
        i++;
        buzz();
        
    }

    for(i; i<=4; i++){
        if(i==1){
            red = red1;
            yellow = yellow1;
            green = green1;
        }
        else if(i==2){
            red = red2;
            yellow = yellow2;
            green = green2;

        }
        else if(i==3){
            red = red3;
            yellow = yellow3;
            green = green3;
        }
        else if(i==4){
            red = red4;
            yellow = yellow4;
            green = green4;
        }

        lamps(red, yellow, green);
        if(checkSensorStatus()){
            stopCars();
    }

    }
}

void lamps(int red, int yellow, int green){
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);
    delay(green_to_yellow_time);
    digitalWrite(green, LOW);
    digitalWrite(yellow, HIGH);
    delay(yellow_to_red_time);
    digitalWrite(yellow, LOW);
    digitalWrite(red, HIGH);
    if(i == 4){
        i = 0;
    }
}

void lampsRedOn(){
    digitalWrite(red1, HIGH);
    digitalWrite(red2, HIGH);
    digitalWrite(red3, HIGH);
    digitalWrite(red4, HIGH);
}
void lampBlueOn(){
    digitalWrite(blue, HIGH);
}
void otherLampsOff(){
    digitalWrite(yellow1, LOW);
    digitalWrite(yellow2, LOW);
    digitalWrite(yellow3, LOW);
    digitalWrite(yellow4, LOW);
    digitalWrite(green1, LOW);
    digitalWrite(green2, LOW);
    digitalWrite(green3, LOW);
    digitalWrite(green4, LOW);
}
void lampBlueOff(){
    digitalWrite(blue, LOW);
}

void buzz(){
    if(buzz_status != 1){
        digitalWrite(buzzer, HIGH);
        if(caller==4){
            buzz_status = 1;
        }        
        delay(buzzer_time);
        digitalWrite(buzzer, LOW);   
    }
}
