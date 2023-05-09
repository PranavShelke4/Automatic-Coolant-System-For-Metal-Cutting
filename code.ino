#define BLYNK_TEMPLATE_ID "TMPLredbJKrW"
#define BLYNK_TEMPLATE_NAME "IoT based Automatic Coolant for Metal Cutting"
#define BLYNK_AUTH_TOKEN "0ED4hBpUkiFKeRhvIDPSuMt7-r5r_-5b"

// Including Node MCU And Blynk Libraries
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include <Wire.h>
#include <Adafruit_MLX90614.h>

WidgetLED led1(V7);

char auth[] = BLYNK_AUTH_TOKEN; //It will Verify Auth Code with the one Provided Above

/* Enter Wifi Credientials*/
char ssid[] = "vivo 1909";
char pass[] = "helloworld06";

BlynkTimer timer;        //--> Begin Blynk Timer

// Motor and Buzzer pins
#define motorPin D3
#define buzzerPin D2

// Initialize the MLX90614 sensor
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  // Initialize the motor and buzzer pins as output
  pinMode(motorPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
     digitalWrite(motorPin, HIGH);
     digitalWrite(buzzerPin, HIGH);
  // Initialize the serial port for debugging
  Serial.begin(9600);

  // Initialize the I2C communication for the MLX90614 sensor
  Wire.begin(D5, D6);
  mlx.begin();
    Blynk.begin(auth, ssid, pass); //--> Verifying provided auth code and Wifi credentials
}

void loop() {
  // Prompt user to select the metal type
  Serial.println("Select metal type: 1-MS 2-SS");
  Blynk.virtualWrite(V1,"Select metal type: 1-MS 2-SS");

  // Wait for user input and set the metal type variable
  int metalType = 0;
  while (metalType == 0) {
    if (Serial.available()) {
      char input = Serial.read();
      if (input == '1' || input == '2') {
        metalType = input - '0';
      }
    }
  }

  // Prompt user to select the metal size
  Serial.println("Select metal size: 1-10mm 2-20mm");
  Blynk.virtualWrite(V1,"Select metal size: 1-10mm 2-20mm");

  // Wait for user input and set the metal size variable
  int metalSize = 0;
  while (metalSize == 0) {
    if (Serial.available()) {
      char input = Serial.read();
      if (input == '1' || input == '2') {
        metalSize = input - '0';
      }
    }
  }

  // Calculate the threshold temperature based on metal type and size
  float thresholdTemp = 0;
  if (metalType == 1) {
    if (metalSize == 1) {
      thresholdTemp = 42.0;
    } else {
      thresholdTemp = 46.0;
    }
  } else {
    if (metalSize == 1) {
      thresholdTemp = 39.0;
    } else {
      thresholdTemp = 42.0;
    }
  }

  while (true) {
    // Read the ambient and object temperatures from the MLX90614 sensor
    float ambientTemp = mlx.readAmbientTempC();
    float objectTemp = mlx.readObjectTempC();

    // Display the temperatures on the serial monitor
    Serial.print("Ambient: ");
    Blynk.virtualWrite(V1,"Ambient: ");
    Serial.print(ambientTemp);
    Blynk.virtualWrite(V1,ambientTemp);
    Blynk.virtualWrite(V0,ambientTemp);
    Serial.print("  C, ");
    Blynk.virtualWrite(V1,"C,\n");
    Serial.print("Object: ");
    Blynk.virtualWrite(V1,"Object: ");
    Serial.print(objectTemp);
    Blynk.virtualWrite(V1,objectTemp);
    Blynk.virtualWrite(V3,objectTemp);
    Serial.println("  C");
    Blynk.virtualWrite(V1,"C \n");

    // Check if the object temperature exceeds the threshold
    if (objectTemp >= thresholdTemp) {
      // Activate the motor and buzzer
      digitalWrite(motorPin, LOW);
      Blynk.virtualWrite(V2,HIGH);
      digitalWrite(buzzerPin, LOW);
      Blynk.virtualWrite(V4,HIGH);
    } else {
      // Deactivate the motor and buzzer
      digitalWrite(motorPin, HIGH);
      Blynk.virtualWrite(V2,LOW);
      digitalWrite(buzzerPin, HIGH);
      Blynk.virtualWrite(V4,LOW);
    }

    // Wait for 500ms before taking the next temperature reading
    delay(500);
  }
}
