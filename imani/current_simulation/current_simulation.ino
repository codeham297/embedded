int voltage_value = A0, meter_relay = 13, motor_forward_pin = 12, motor_reverse_pin = 11;
float small_volt, volt;
int meter_turn_off_voltage, mcb_turn_off_voltage;
int mcb_status=1, meter_status=1, motor_delay = 1000;
void motorReverse();
void motorForward(); 
void turnOffMeter();
void turnOnMeter();
int checkVoltageLevel();

void setup() {
	pinMode(voltage_value, INPUT);
	pinMode(meter_relay, OUTPUT);
	pinMode(motor_forward_pin, OUTPUT);
	pinMode(motor_reverse_pin, OUTPUT);
	meter_turn_off_voltage = 300;
	mcb_turn_off_voltage = 250;
}

void loop() {
	volt = checkVoltageLevel();
	if(volt<mcb_turn_off_voltage){

		if(meter_status == 1){
			if(mcb_status == 1){
				loop();
			}
			else if(mcb_status == 0){
				motorForward();
				loop();
			}
		}
		else if(meter_status == 0){
			turnOnMeter();
			if(mcb_status == 1){
				loop();
			}
			else if(mcb_status == 0){
				motorForward();
				loop();
			}
		}
	}
	else if(volt >= mcb_turn_off_voltage && volt < meter_turn_off_voltage){

		if(meter_status == 1){
			if(mcb_status == 1){
				motorReverse();
				loop();
			}
			else if(mcb_status == 0){
				loop();
			}
		}
		else if(meter_status == 0){
			turnOnMeter();
			if(mcb_status == 1){
				motorReverse();
				loop();
			}
			else if(mcb_status == 0){
				loop();
			}
		}
	}

	else if(volt >=meter_turn_off_voltage){
		if(meter_status == 1){
			turnOffMeter();
			loop();
		}
		else if(meter_status == 0){
			loop();
		}
	}
}

void motorReverse(){
	if(mcb_status==0){
		return;
	}
	else if(mcb_status==1){
		digitalWrite(motor_forward_pin, HIGH);
		digitalWrite(motor_reverse_pin, LOW);
		delay(motor_delay);
		digitalWrite(motor_forward_pin, LOW);
		mcb_status = 0;
	}
}

void motorForward(){
	if(mcb_status == 0){
		digitalWrite(motor_reverse_pin, HIGH);
		digitalWrite(motor_forward_pin, LOW);
		delay(motor_delay);
		digitalWrite(motor_reverse_pin, LOW);
		mcb_status = 1;
	}
	else if(mcb_status == 1){
		return;
	}
}

void turnOffMeter(){
	if(meter_status == 0){
		return;
	}
	else if(meter_status == 1){
		digitalWrite(meter_relay, HIGH);
		meter_status = 0;
	}
}

void turnOnMeter(){
	if(meter_status == 1){
		return;
	}
	else if(meter_status == 0){
		digitalWrite(meter_relay, LOW);
		meter_status = 1;
	}
}

int checkVoltageLevel(){
	small_volt = analogRead(voltage_value);
	volt = small_volt*5/1023;
	volt = volt * 100;
	return volt;
}