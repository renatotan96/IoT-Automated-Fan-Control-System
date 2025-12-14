//Include libraries
#include <DHT11.h>
#include <LiquidCrystal_I2C.h>

//Define pins
const int sensePin = 2; //DHT sensor pin
const int INA = 9; //Fan control pin
const int INB = 10; //Fan control pin

//define constants
const float FAN_ON_TEMP = 31.0; //define turn on threshold
const float FAN_OFF_TEMP = 28.5; //define turn off threshold
const float HYS = 0.5; //Define hysteresis value
const float FAN_ON_TEMP_HYS = FAN_ON_TEMP + HYS; //define hysteresis-adjusted on threshold
const int MIN_FAN_SPEED = 25; //minimum fan speed

//Define objects
DHT11 dht11(sensePin); //Define DHT11
LiquidCrystal_I2C lcd(0x27, 16, 2); //Define LCD display

void setup() {
	pinMode(INA, OUTPUT);
	pinMode(INB, OUTPUT);
	//setup lcd
	lcd.init();
	lcd.backlight();
	lcd.begin(16, 2);
	lcd.setCursor(0, 0);
	lcd.print("Temperature: ");
}

void loop() {
	//read temperature
	int temperature = dht11.readTemperature();
	//Debug message
	if (isnan(temperature)) {
		Serial.println("Failed to read temperature");
		return;
	}
	//Printing to LCD and clearing second line
	Serial.print("Temperature: ");
	Serial.print(temperature);
	Serial.println(" °C");
	lcd.setCursor(0, 1); //Set to second line
	lcd.print("                  "); //Clear the line
	
	//Turn fan on at maximum speed
	if (temperature > FAN_ON_TEMP_HYS) {
		analogWrite(INA, 150); //Max speed
		digitalWrite(INB, LOW); 
		delay(2000); //Delay for 2 seconds
		lcd.setCursor(0, 1); //Set to second line
		lcd.print("Fan Speed: Max"); //print fan speed to LCD
	}
	//turn fan off
	else if (temperature < FAN_OFF_TEMP) {
		digitalWrite(INA, LOW);
		digitalWrite(INB, LOW);
		delay(5000); //Delay for 5 seconds
		lcd.setCursor(0, 1); //set to second line
		lcd.print("Fan Speed: OFF"); //print fan speed to LCD
	}
	//Set dynamic fan speed based on temperature
	else {
		int fanSpeed = map(temperature, FAN_OFF_TEMP, FAN_ON_TEMP, MIN_FAN_SPEED, 150);
		analogWrite(INA, fanSpeed); //Sets the voltage on INA pin to control fan speed from map
		digitalWrite(INB, LOW); //sets INB to low, for motro directions
		delay(2000);
		lcd.setCursor(0, 1);
		lcd.print("Fan Speed: ");
		lcd.print(fanSpeed); //prints fan speed value
	}
	//Display temperature and update LCD
	lcd.setCursor(12, 0);
	lcd.print(temperature); 
	lcd.display();
	
	//Delay between readings 5s
	delay(5000);
}