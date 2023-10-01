#include "arduino_secrets.h"
#include "DHT.h"  // Include the DHT library
#include "thingProperties.h"

#define DHTPIN 4      // Define the pin for the DHT11 sensor (replace with appropriate GPIO pin)
#define DHTTYPE DHT11   // Define the type of DHT sensor

#define MQ2PIN 34     // Define the pin for the MQ-2 sensor (replace with appropriate GPIO pin)
#define RELAY_PIN 18  // Define the pin for the relay (replace with appropriate GPIO pin)
#define LED_PIN 19    // Define the pin for the LED (replace with appropriate GPIO pin)
#define BUZZER_PIN 5  // Define the pin for the buzzer (replace with appropriate GPIO pin)

DHT dht(DHTPIN, DHTTYPE);  // Create an instance of the DHT sensor

void setup() {
  Serial.begin(9600);
  delay(1500);

  pinMode(MQ2PIN, INPUT);   // Set MQ-2 pin as input
  pinMode(RELAY_PIN, OUTPUT);  // Set relay pin as output
  pinMode(LED_PIN, OUTPUT);    // Set LED pin as output
  pinMode(BUZZER_PIN, OUTPUT); // Set buzzer pin as output

  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  dht.begin();
}

void loop() {
  ArduinoCloud.update();

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (!isnan(temperature)) {
    temp = temperature;
  }
  if (!isnan(humidity)) {
    humid = humidity;
  }

  float gasLevel = readGasLevel();

  if (!isnan(gasLevel)) {
    gas = gasLevel;

    // Check if gas level is more than 60 ppm
    if (gasLevel > 60) {
      fan = false;  // Turn off the fan
      light = false; // Turn off the light
      activateBuzzer(); // Activate the buzzer
    }
  }

  // Your code here

  // Control the fan based on IoT Cloud variable
  if (fan) {
    digitalWrite(RELAY_PIN, HIGH); // Turn on the fan
  } else {
    digitalWrite(RELAY_PIN, LOW);  // Turn off the fan
  }

  // Control the LED based on IoT Cloud variable
  if (light) {
    digitalWrite(LED_PIN, HIGH);  // Turn on the LED
  } else {
    digitalWrite(LED_PIN, LOW);   // Turn off the LED
  }
}

void onGasChange() {
  // Add your code here to act upon Gas change
}

void onHumidChange() {
  // Add your code here to act upon Humid change
}

void onTempChange() {
  // Add your code here to act upon Temp change
}

void onLightChange() {
  // Add your code here to act upon Light change
}

void onFanChange() {
  // Add your code here to act upon Fan change
}

float readGasLevel() {
  int sensorValue = analogRead(MQ2PIN);
  float voltage = sensorValue * (5.0 / 1024.0);
  float ppm = (voltage - 0.2) * 50.0;
  return ppm;
}

void activateBuzzer() {
  digitalWrite(BUZZER_PIN, HIGH); // Turn on the buzzer
  delay(5000);  // Buzz for 1 second
  digitalWrite(BUZZER_PIN, LOW);  // Turn off the buzzer
}
