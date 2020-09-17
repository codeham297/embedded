#include <LiquidCrystal.h>
#include <TimeLib.h>
LiquidCrystal lcd(11, 10, 9, 8, 7, 6);
void displayText(int row, char text[]);
void displayInt(int row, int value);
void debug();
void debugDel();
void debugVal(int text);
int continuity();
int insulation250();
int insulation500();
void continuitytester();
void meggertester(int voltage_level);

int start_time = 200;
int continuity_pin = 5, insulation250_pin = 4, insulation500_pin = 3, relay250_pin = 2, relay500_pin = 1;
int continuity_status = 0, insulation250_status = 0, insulation500_status = 0;
int continuityion_count = 1, button_delay = 200, continuity_press = 1;
int running_time;
int voltage500_pin = A0;
int voltage250_pin = A1;
int insulation0_pin = A3;

void setup() {
	lcd.begin(16, 4);
	lcd.setCursor(1, 1);
    lcd.print("WELCOME");
    delay (start_time);
    lcd.clear();

    pinMode(continuity_pin, INPUT); pinMode(insulation250_pin, INPUT); pinMode(insulation500_pin, INPUT); pinMode(relay250_pin, OUTPUT); pinMode(relay500_pin, OUTPUT), pinMode(voltage250_pin, INPUT), pinMode(voltage500_pin, INPUT), pinMode(insulation0_pin, INPUT);
}



void loop() {
continuity_status = digitalRead(continuity_pin);
insulation250_status = digitalRead(insulation250_pin);
insulation500_status = digitalRead(insulation500_pin);
if(continuity_status == 1){
	displayText(1, "CONTINUITY      ");
	continuity();
}
else if(insulation250_status == 1){
	displayText(1, "INSULATION  250V");
	insulation250();
}

else if(insulation500_status == 1){
	displayText(1, "INSULATIONV 500V");
	insulation500();
}
}



int continuity(){
	digitalWrite(0, HIGH);
digitalWrite(relay250_pin, LOW);
digitalWrite(relay500_pin, LOW);


}
int insulation250(){
digitalWrite(relay250_pin, HIGH);
digitalWrite(0, LOW);
digitalWrite(relay500_pin, LOW);

}
int insulation500(){
digitalWrite(relay500_pin, HIGH);
digitalWrite(relay250_pin, LOW);
digitalWrite(0, LOW);
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

int callWithTime(int function(), int return_val, int time){
	if(running_time == 0){
		start_time = now();
		if(function() == return_val){
			return return_val;
		}
		running_time = 1;
	}
	else if(running_time > 0){
		if(running_time < time){
			running_time = now();
			debugVal(running_time);
			if(function() == return_val){
				return return_val;
			}
			callWithTime(function, return_val, time);
		}

	}
	else if(running_time >= time){
		running_time = 0;
		return;
	}
}