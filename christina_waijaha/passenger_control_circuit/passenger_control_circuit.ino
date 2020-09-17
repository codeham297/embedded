#include <LiquidCrystal.h>

LiquidCrystal lcd(11, 10, 9, 8, 7, 6);
int enter_IR = 5;
int exit_IR = 4;
int corridor_IR = 3;
int switch_for_car_motion = 2;
int car_engine = 1; //Indicated by relay and motor
int gsm = 12;//Indicated by led green
int gps = 13;//Indicated by led yellow
int passenger_count;
int car_motion_status;
int check_location_status = 0;
int send_message_status = 0;
int car_capacity = 10;
int monitor_time_call = 0;
int passenger_on_door_status = 0;
int bus_corridor_status = 0;
int car_ignition_switch = 0;
int exit_code_status;
int enter_code_status;
int allowed_to_move = 1;
int enter_time = 100, exit_time = 100;
int passenger_exceed_status, message_status, enter_status, exit_status; 
int checkCarMotion();
int checkPassengerOnDoorStatus();
int checkExitStatus();
int checkEnterStatus();
int checkBusCorridor();
int checkIfPassengerExceedandDisplay();
int checkIgnitionSwitch();
int checkMessageStatus();

void lockIgnitionSystem();
void unlockIgnitionSystem();
int turnOnCar();
void turnOffCar();

void checkLocation();
void sendMessage();
void callWithTime(int function(), int time);
void updateNumberOfPassengerandDisplay(int change);
void displayText(int row, char text[]);
void displayInt(int row, int value);

void debug();
void debugDel();
void debugVal(int text);
int bus_corridor_monitor_time = 100;
int location_to_sms_delay = 100;

//The below 3 variables are used by the callWithTime fx;
int running_time = 0;
int start_time = 400;

void setup() {
	lcd.begin(16, 4);
	pinMode(enter_IR, INPUT);
	pinMode(exit_IR, INPUT);
	pinMode(corridor_IR, INPUT);
	pinMode(switch_for_car_motion, INPUT);
	pinMode(gsm, OUTPUT);
	pinMode(gps, OUTPUT);
	pinMode(car_engine, OUTPUT);

	lcd.setCursor(1, 1);
    lcd.print("WELCOME");
    delay (start_time);
    lcd.clear();

    displayInt(1, 0);
    displayText(3, "NO PASSENGER");
}

void loop() {
cCM:
	car_motion_status = checkCarMotion();
	if(car_motion_status == 0){
		car_ignition_switch = checkIgnitionSwitch();
		if(car_ignition_switch == 0){
			turnOffCar();
	pOD:
			passenger_on_door_status = checkPassengerOnDoorStatus();
			if(passenger_on_door_status == 0){
				goto cCM;
			}
			else if(passenger_on_door_status == 1){
				displayText(2, "PASSENG ENTER  ");
				delay(enter_time);
				displayText(2, "                 ");
				updateNumberOfPassengerandDisplay(1);
				passenger_exceed_status	= checkIfPassengerExceedandDisplay();
				if(passenger_exceed_status == 1){
					lockIgnitionSystem();
					displayText(3, "PASSENG EXCEED  ");
					goto cCM;
				}
				else if(passenger_exceed_status == 0){
					unlockIgnitionSystem();
					displayText(3, "PASSENG NORMAL  ");
					goto cCM;
				}
			}
			else if(passenger_on_door_status == 2){
				displayText(2, "PASSENG EXITN   ");
				delay(exit_time);
				displayText(2, "                 ");
				updateNumberOfPassengerandDisplay(-1);
				if(passenger_count < car_capacity){
					unlockIgnitionSystem();
					displayText(3, "PASSENGER NORMAL  ");
				}
				else if(passenger_count== car_capacity){
					unlockIgnitionSystem();
					displayText(3, "THE CAR IS FULL     ");
				}
				goto cCM;
			}
			else{
				displayText(2, "             ");
			}
		}
		else if(car_ignition_switch == 1){
			turnOnCar();
			passenger_exceed_status = checkIfPassengerExceedandDisplay();
			if(passenger_exceed_status == 1){
				informAuthority();
			}

			if(turnOnCar() == 0){
				goto pOD;
			}
			goto cCM;
		}

	}
	if(car_motion_status == 1){
	cIS:
		car_ignition_switch = checkIgnitionSwitch();
		if(car_ignition_switch == 0){
			turnOffCar();
			goto cCM;
		}

	cBC:
		bus_corridor_status = checkBusCorridor();
		if(bus_corridor_status == 1){
			updateNumberOfPassengerandDisplay(1);
			passenger_exceed_status = checkIfPassengerExceedandDisplay();

			if(passenger_exceed_status){
				message_status = checkMessageStatus();
				if(message_status == 1){
					updateNumberOfPassengerandDisplay(1);
					goto cIS;
				}
				else if(message_status == 0){
					car_ignition_switch = checkIgnitionSwitch();
					informAuthority();
					goto cCM;
				}

			}
			else if(passenger_exceed_status == 0){
				goto cCM;
			}
		}
		else if(bus_corridor_status == 0){
			goto cCM;
		}
	}

}

int checkCarMotion(){
	if(car_motion_status == 1){
		car_motion_status = 1;
	}
	else if(car_motion_status == 0){
		car_motion_status = 0;
	}
	if(car_ignition_switch){
		turnOnCar();
	}
	else if(!car_ignition_switch){
		turnOffCar();
	}
	return car_motion_status;
}

int checkIgnitionSwitch(){
	car_ignition_switch = digitalRead(switch_for_car_motion);
	return car_ignition_switch;
}

int checkPassengerOnDoorStatus(){
	int passenger_on_door_status = 0;
	enter_status = checkEnterStatus();
	exit_status = checkExitStatus();
	if(enter_status == 1){
			passenger_on_door_status = 1;
	}
	else if(exit_status == 1){

			passenger_on_door_status  = 2;
	}
	return passenger_on_door_status;
}


int checkBusCorridor(){
	if(digitalRead(corridor_IR)){
		bus_corridor_status = 1;
		delay(bus_corridor_monitor_time);
		bus_corridor_status = digitalRead(corridor_IR);
		if(bus_corridor_status == 1){
			bus_corridor_status = 1;
		}
	}
	return bus_corridor_status;
}

int checkExitStatus(){
  	exit_status = digitalRead(exit_IR);
  	if(exit_status == 1){
  		delay(exit_time);
  		exit_status = digitalRead(exit_IR);
  		if(exit_status == 1){
  			exit_status == 1;
  		}
  	}
	return exit_status;
}

int checkEnterStatus(){
	enter_status = digitalRead(enter_IR);
	if(enter_status == 1){
		delay(enter_time);
		enter_status = digitalRead(enter_IR);
	}
	return enter_status;
}

int checkIfPassengerExceedandDisplay(){
	passenger_exceed_status = -1;
	if(passenger_count > car_capacity){
		passenger_exceed_status = 1;
	}
	else if(passenger_count < car_capacity){
		passenger_exceed_status = 0;
	}
	if(passenger_exceed_status == 1)
	displayText(3, "PASSENGER EXCEED");
	else if(passenger_exceed_status == 0){
		displayText(3, "PASSENGER NORMAL");
	}
	else{
		displayText(3, "THE CALL IS FULL");
	}
	return passenger_exceed_status;
}
int checkMessageStatus(){
	return send_message_status;
}

void updateNumberOfPassengerandDisplay(int change){
	if(change == 1){
		passenger_count++;
	}
	else if(change == -1){
		if(passenger_count > 0){
			passenger_count--;
		}
	}
	displayText(1, "         ");
	displayInt(1, passenger_count);
}



void lockIgnitionSystem(){
		allowed_to_move = 0;
		turnOffCar();
		return;
}

void unlockIgnitionSystem(){
	allowed_to_move = 1;
}

int turnOnCar(){
	if(car_motion_status == 0){
		if(allowed_to_move == 1){
			digitalWrite(car_engine, HIGH);
			car_motion_status = 1;
			return 1;
		}
	}
	return;
}

void turnOffCar(){
	if(car_motion_status == 1){
			digitalWrite(car_engine, LOW);
			send_message_status = 0;
			check_location_status =0;


		digitalWrite(gsm, LOW);
		digitalWrite(gps, LOW);
		displayText(0, "              ");
			car_motion_status = 0;
	}
	return;
}


void checkLocation(){
	//Waiting for GPS code;
	if(check_location_status == 0){
		digitalWrite(gps, HIGH);
		displayText(0, "LOCATION READ");
		check_location_status = 1;
		// delay(location_to_sms_delay);
	}
return;
}

void sendMessage(){
	//Waiting for GSM cod;
	if(send_message_status == 0){
		digitalWrite(gsm, HIGH);
		displayText(0, "MESSAGE SENT ");
		send_message_status = 1;
	}
return;
}

void informAuthority(){
	if(passenger_exceed_status){
		checkLocation();
		sendMessage();
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
