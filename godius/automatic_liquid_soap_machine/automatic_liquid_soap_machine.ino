#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Key.h>

int pump1 = 13, pump2 = 12, pump3 = 11;
int en_mixer = 10;
int in1_mixer = 9;
int in2_mixer = 8;
int button1 = 14, button2 = 15, button3 = 16;
int sensor = 1;
int pump1_time, pump2_time, pump3_time;
int filling_time = 100, closing_delay = 200;
int mixing_time;
int volume = 0, volume1 = 0, volume2 = 0, volume3 = 0;

const byte ROWS = 4;
const byte COLS = 3; 
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'D','0','E'},
};
byte rowPins[ROWS] = {25, 26, 27, 28}; 
byte colPins[COLS] = {24, 23, 22}; 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

void setup(){
	lcd.begin(16, 4);
	pinMode(pump1, OUTPUT);
	pinMode(pump2, OUTPUT);
	pinMode(pump3, OUTPUT);
	pinMode(en_mixer, OUTPUT);
	pinMode(in1_mixer, OUTPUT);
	pinMode(in2_mixer, OUTPUT);
}

void loop(){
	setVolumes();
}

void setVolumes(){
	displayText(0, "SET ING VOLUMES ");
	displayInt(1, volume1);
	displayInt(2, volume2);
	displayInt(3, volume3);
	    char key = keypad.getKey();
	    int exit_set_vol = 1;
        if(key){
        	if((key-48) == 21){
        		if(volume1> 0){
    	    		fillIngredients();	
        		}
        		else if(volume2 > 0){
        			fillIngredients();
        		}
        		else if(volume3 > 0){
        			fillIngredients();
        		}
        	}
        	else if((key-48) == 20){
        		volume = 0;
        		volume1 = 0;
        		volume2 = 0;
        		volume3 = 0;
        	}
        	else if((key-48) >= 0){
        		if((key-48) <= 9){
        		volume = (key-48);
        		while(exit_set_vol){
        			if(digitalRead(button1)){
        				volume1 = volume;
        				pump1_time = volume1 * filling_time;
        				exit_set_vol = 0;
        				mixing_time += pump1_time;
        			}
        			else if(digitalRead(button2)){
        				volume2 = volume;
        				pump2_time = volume2 * filling_time;
        				exit_set_vol = 0;
        				mixing_time += pump2_time;
        			}
        			else if(digitalRead(button3)){
        				volume3 = volume;
        				pump3_time = volume3 * filling_time;
        				exit_set_vol = 0;
        				mixing_time += pump3_time;
        			}
        		}
        		}
 
        	}
        }
}

void fillIngredients(){
	displayText(2, "FILLING INGRED'S");
	displayText(0, "                ");
	displayText(1, "                ");
	displayText(3, "                ");
	digitalWrite(pump1, HIGH);
	digitalWrite(pump2, HIGH);
	digitalWrite(pump3, HIGH);
	if(pump1_time >= pump2_time){
		if(pump2_time >= pump3_time ){		
			delay(pump3_time);
			digitalWrite(pump3, LOW);
			delay(pump2_time - pump3_time);
			digitalWrite(pump2, LOW);
			delay(pump1_time - pump2_time);
			digitalWrite(pump1, LOW);
		}
	}
	if(pump1_time >= pump3_time){
		if(pump3_time >= pump2_time){
			delay(pump2_time);
			digitalWrite(pump2, LOW);
			delay(pump3_time - pump2_time);
			digitalWrite(pump3, LOW);
			delay(pump1_time - pump3_time);
			digitalWrite(pump1, LOW);
		}
	}
	if(pump2_time >= pump3_time){
		if(pump3_time >= pump1_time){
			delay(pump1_time);
			digitalWrite(pump1, LOW);
			delay(pump3_time - pump1_time);
			digitalWrite(pump3, LOW);
			delay(pump2_time - pump3_time);
			digitalWrite(pump2, LOW);
		}
	}
	if(pump2_time >= pump1_time){
		if(pump1_time >= pump3_time){
			delay(pump3_time);
			digitalWrite(pump3, LOW);
			delay(pump1_time - pump3_time);
			digitalWrite(pump1, LOW);
			delay(pump2_time - pump1_time);
			digitalWrite(pump2, LOW);
		}
	}

	if(pump3_time >= pump1_time){
		if(pump1_time >= pump2_time){
			delay(pump2_time);
			digitalWrite(pump2, LOW);
			delay(pump1_time - pump2_time);
			digitalWrite(pump1, LOW);
			delay(pump3_time - pump1_time);
			digitalWrite(pump3, LOW);
		}
	}
	if(pump3_time >= pump2_time){
		if(pump2_time >= pump1_time){
			delay(pump1_time);
			digitalWrite(pump1, LOW);
			delay(pump2_time - pump1_time);
			digitalWrite(pump2, LOW);
			delay(pump3_time - pump2_time);
			digitalWrite(pump3, LOW);
		}
	}
	displayText(0, "                ");
	displayText(1, "                ");
	displayText(2, "                ");
	displayText(3, "                ");
	mixingIngredients();
}

void mixingIngredients(){
	if(mixing_time > 0){
		if(mixing_time != pump1_time){
			if(mixing_time != pump2_time){
				if(mixing_time != pump3_time){
					displayText(2, "MIXING INGRED'S ");
					displayText(0, "                ");
					displayText(1, "                ");
					displayText(3, "                ");
					digitalWrite(en_mixer, HIGH);
					digitalWrite(in1_mixer, HIGH);
					digitalWrite(in2_mixer, LOW);
					delay(mixing_time);
					digitalWrite(en_mixer, LOW);
					digitalWrite(in1_mixer, LOW);
					displayText(2, "FINISHED MIXING ");
					displayText(1, "                ");
					displayText(0, "                ");
					displayText(3, "                ");
					delay(closing_delay);
					displayText(2, "                ");
				}
			}
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