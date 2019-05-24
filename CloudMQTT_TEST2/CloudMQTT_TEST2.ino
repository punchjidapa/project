#include <ESP8266WiFi.h>
#include <MQTTClient.h>

const char* ssid = "ModDevLab_2.4GHz";
const char* password = "3313033130A*";

WiFiClient WiFiclient;
MQTTClient client;

unsigned long lastMillis = 0;

const int trigPin = 2;  //D4
const int echoPin = 0;  //D3
long duration;
int distance;

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  delay(10);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(2000);

  Serial.print("connecting to MQTT broker...");
  client.begin("m16.cloudmqtt.com", 16629, WiFiclient);
  connect();
}

void connect() {
  while (!client.connect("ESP8266Client", "Node1", "aabbccdd")) {
    Serial.print(".");
  }

  Serial.println("\nconnected!");
  client.subscribe("/sensor/level");
}

void loop() {
  
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  distance = duration * 0.034 / 2;

  distance = random(0, 100);
  
  client.loop();
  if (!client.connected()) {
    connect();
  }

  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    client.publish("/sensor/level", (String)distance);
  }
}
