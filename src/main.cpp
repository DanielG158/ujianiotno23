#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include <BH1750.h>

// Pin definitions
const int DHT_PIN = 4; // DHT11 sensor
const int LED_PIN = 13; //  LED
const int LIGHT_SENSOR_ADDRESS = 0x23; // I2C address of light sensor

// Sensor type definitions
const int DHT_TYPE = DHT11; // DHT11 sensor
const int LIGHT_SENSOR_TYPE = BH1750::ONE_TIME_HIGH_RES_MODE; // 1 lx resolution

// Sensor objects
DHT dht(DHT_PIN, DHT_TYPE); // DHT11 sensor
BH1750 lightMeter; // I2C address 0x23

void setup() {
  Serial.begin(115200); // Initialize serial communication
  Wire.begin(21, 22); // SDA, SCL
  dht.begin(); // Initialize temperature/humidity sensor
  lightMeter.begin(); // Initialize light sensor
  delay(100);  // Wait for sensor to initialize
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  static unsigned long lastTempTime = 0; //this static variable is for the temperature to be read every 7 seconds
  static unsigned long lastHumidityTime = 0; //this static variable is for the humidity to be read every 4 seconds
  static unsigned long lastLightTime = 0; //this static variable is for the light to be read every 2 seconds

  // Take temperature reading every 7 seconds
  if (millis() - lastTempTime >= 7000) {
    float temperature = dht.readTemperature();  //make the DHT11 read the temperature
    if (!isnan(temperature)) { //if the temperature is not a number, print the temperature
      Serial.print("Temperature: "); //print the temperature
      Serial.print(temperature); 
      Serial.println(" C"); 
    } else {
      Serial.println("Error reading temperature!");  //if the temperature is a number, print an error message
    }
    lastTempTime = millis(); //this is to make the temperature read every 7 seconds
  }

  // Take humidity reading every 4 seconds
  if (millis() - lastHumidityTime >= 4000) { //this is to make the humidity read every 4 seconds
    float humidity = dht.readHumidity(); //make the DHT11 read the humidity
    if (!isnan(humidity)) { //if the humidity is not a number, print the humidity
      Serial.print("Humidity: "); //print the humidity
      Serial.print(humidity);
      Serial.println(" %");
    } else {
      Serial.println("Error reading humidity!"); //if the humidity is a number, print an error message
    }
    lastHumidityTime = millis();
  }

  // Take light reading every 2 seconds
  if (millis() - lastLightTime >= 2000) {
    uint16_t lux = lightMeter.readLightLevel();  //to make the BH1750 read the light level
    if (!isnan(lux)) { //if the light level is not a number, print the light level
      Serial.print("Light: ");
      Serial.print(lux);
      Serial.println(" lux");

      // Turn on LED if light is less than 400 lux
      if (lux > 400) {
        digitalWrite(LED_PIN, HIGH); //turn on the LED
      } else {
        digitalWrite(LED_PIN, LOW); //turn off the LED
      }
    } else {
      Serial.println("Error reading light level!"); //if the light level is a number, print an error message
    }
    lastLightTime = millis(); //this is to make the light level read every 2 seconds
  }
}
