#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

// =====================================
// WiFi Credentials
// =====================================

const char* ssid = "Your WIFI SSID";
const char* password = "Your WIFI Password";

// =====================================
// ThingSpeak Details
// =====================================

unsigned long channelNumber = 1234567; // Replace with Your ThingSpeak Channel Number

const char* writeAPIKey = "Your ThingSpeak API Key";

WiFiClient client;

// =====================================
// Pin Definitions
// =====================================

// LDR
#define LDR_PIN A0

// IR Sensors
#define IR1_PIN D5
#define IR2_PIN D6
#define IR3_PIN D7

// LEDs
#define LED1_PIN D1
#define LED2_PIN D2
#define LED3_PIN D4

// Relay
#define RELAY_PIN D3

// =====================================
// Settings
// =====================================

#define LDR_THRESHOLD 300
#define LED_ON_TIME 5000

// =====================================
// Variables
// =====================================

unsigned long led1Start = 0;
unsigned long led2Start = 0;
unsigned long led3Start = 0;

bool led1State = false;
bool led2State = false;
bool led3State = false;

// ThingSpeak Timer
unsigned long lastUpdate = 0;

// =====================================
// SETUP
// =====================================

void setup() {

  Serial.begin(115200);

  // IR Sensors
  pinMode(IR1_PIN, INPUT_PULLUP);
  pinMode(IR2_PIN, INPUT_PULLUP);
  pinMode(IR3_PIN, INPUT_PULLUP);

  // LEDs
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  // Relay
  pinMode(RELAY_PIN, OUTPUT);

  // Initial State
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);

  // Relay OFF
  digitalWrite(RELAY_PIN, HIGH);

  // =====================================
  // WiFi Connect
  // =====================================

  WiFi.begin(ssid, password);

  Serial.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");

  // =====================================
  // ThingSpeak Start
  // =====================================

  ThingSpeak.begin(client);

  Serial.println("ThingSpeak Connected");
}

// =====================================
// LOOP
// =====================================

void loop() {

  // Read LDR
  int ldrValue = analogRead(LDR_PIN);

  // Read IR Sensors
  int ir1 = digitalRead(IR1_PIN);
  int ir2 = digitalRead(IR2_PIN);
  int ir3 = digitalRead(IR3_PIN);

  // =====================================
  // Serial Monitor
  // =====================================

  Serial.print("LDR: ");
  Serial.print(ldrValue);

  Serial.print(" | IR1: ");
  Serial.print(ir1);

  Serial.print(" | IR2: ");
  Serial.print(ir2);

  Serial.print(" | IR3: ");
  Serial.println(ir3);

  // =====================================
  // DAY MODE
  // =====================================

  if (ldrValue < LDR_THRESHOLD) {

    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);

    digitalWrite(RELAY_PIN, HIGH);

    led1State = false;
    led2State = false;
    led3State = false;
  }

  // =====================================
  // NIGHT MODE
  // =====================================

  else {

    // IR1 Detection
    if (ir1 == LOW && !led1State) {

      delay(30);

      if (digitalRead(IR1_PIN) == LOW) {

        digitalWrite(LED1_PIN, HIGH);

        digitalWrite(RELAY_PIN, LOW);

        led1State = true;

        led1Start = millis();
      }
    }

    // IR2 Detection
    if (ir2 == LOW && !led2State) {

      delay(30);

      if (digitalRead(IR2_PIN) == LOW) {

        digitalWrite(LED2_PIN, HIGH);

        digitalWrite(RELAY_PIN, LOW);

        led2State = true;

        led2Start = millis();
      }
    }

    // IR3 Detection
    if (ir3 == LOW && !led3State) {

      delay(30);

      if (digitalRead(IR3_PIN) == LOW) {

        digitalWrite(LED3_PIN, HIGH);

        digitalWrite(RELAY_PIN, LOW);

        led3State = true;

        led3Start = millis();
      }
    }

    // =====================================
    // LED OFF TIMER
    // =====================================

    if (led1State && millis() - led1Start >= LED_ON_TIME) {

      digitalWrite(LED1_PIN, LOW);

      led1State = false;
    }

    if (led2State && millis() - led2Start >= LED_ON_TIME) {

      digitalWrite(LED2_PIN, LOW);

      led2State = false;
    }

    if (led3State && millis() - led3Start >= LED_ON_TIME) {

      digitalWrite(LED3_PIN, LOW);

      led3State = false;
    }

    // Relay OFF if all LEDs OFF

    if (!led1State &&
        !led2State &&
        !led3State) {

      digitalWrite(RELAY_PIN, HIGH);
    }
  }

  // =====================================
  // SEND DATA TO THINGSPEAK
  // EVERY 20 SECONDS
  // =====================================

  if (millis() - lastUpdate >= 20000) {

    ThingSpeak.setField(1, ldrValue);
    ThingSpeak.setField(2, ir1);
    ThingSpeak.setField(3, ir2);
    ThingSpeak.setField(4, ir3);

    int response = ThingSpeak.writeFields(channelNumber, writeAPIKey);

    if (response == 200) {

      Serial.println("Data Sent Successfully");
    }

    else {

      Serial.print("ThingSpeak Error: ");
      Serial.println(response);
    }

    lastUpdate = millis();
  }
}