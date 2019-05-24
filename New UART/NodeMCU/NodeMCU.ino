#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>

// UNO Pin 12 -> Pin D5 NodeNCU, UNO Pin 13 -> Pin D6 NodeMCU
SoftwareSerial UNOSerial(D6, D5);

const char* ssid = "ModDevLab_2.4GHz";
const char* password = "3313033130A*";

WiFiClient WiFiclient;
MQTTClient client;

unsigned long lastMillis = 0;

uint16_t analogData = 0;
//String positionData = "";

void setup() {

  pinMode(A0, INPUT);
  Serial.begin(9600);
  UNOSerial.begin(9600);
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
  client.loop();
  if (!client.connected()) {
    connect();
  }

  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    analogData = analogRead(A0);
    analogData = map(analogData, 0, 1023, 0, 100); //////measure bin///////
    //positionData = random(0, 3);
    Serial.print("IR : ");
    Serial.println(analogData);
    client.publish("/sensor/level", "L" + (String)analogData);
  }

  //JSON Object to be received from UNO
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject & root = jsonBuffer.parseObject(UNOSerial);
  if (root == JsonObject::invalid()) {
    return;
  }

  //Receive positionData, weightData and metalData from UNO
  String positionData = root["data1"];
  String weightData = root["data2"];
  String metalData = root["data3"];
  Serial.print("Position : ");
  Serial.println(positionData);
  Serial.print("Weight : ");
  Serial.println(weightData);
  Serial.print("Metal type : ");
  Serial.println(metalData);
  Serial.print("Trash type : ");
  if (positionData == "1") {
    Serial.println("Plastic bottle");
  }
  if (positionData == "2") {
    Serial.println("Glass");
  }
  if (positionData == "3") {
    Serial.println("Carton");
  }
  if (positionData == "4") {
    Serial.println("Can");
  }

  //Receive only positionData from UNO
  //UNOSerial.write("s");
  /*if (UNOSerial.available() > 0) {
    positionData = UNOSerial.read();
    Serial.print("Position : ");
    Serial.println(positionData);
    Serial.print("Type : ");
    if (positionData == "1") {
      Serial.println("Plastic bottle");
      }
     if (positionData == "2") {
      Serial.println("Glass");
      }
     if (positionData == "3") {
      Serial.println("Carton");
      }
     if (positionData == "4") {
      Serial.println("Can");
      }*/

  client.publish("/sensor/position", "P" + positionData);
}
delay(200);
}
