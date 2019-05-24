#include<ESP8266WiFi.h>
#include<ESP8266HTTPClient.h>
#include<EEPROM.h>
#include<PubSubClient.h>
#include<Wire.h>

#define mqtt_server "m15.cloudmqtt.com"
#define mqtt_port 38034
#define mqtt_user "board"
#define mqtt_password "board"

const char* ssid = "PUNCH";
const char* password = "punchpunch";

char message_buffer[100];

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(2, OUTPUT); // declare LED as output 
  digitalWrite(2, 0); //Start LED turn off

  Serial.println("Welcome to Smart Recycled Trash Project");
  Serial.println("connecting Wifi");
  WiFi.begin(ssid,password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
     Serial.print(".");
  }

  Serial.println("WiFi, Connected!!!");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, mqtt_port); 
  client.setCallback(callback);
  Serial.println("connected");
}

void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.println("] ");

    int i;
    for (i = 0; i < 10; i++)
    {
        message_buffer[i] = payload[i];
    }
    message_buffer[i] = '\0';

    String msgString = String(message_buffer);
    Serial.println(msgString);
}

void loop() {
    int i;
    if(i == 3) {
        return;
    }
    if (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        String clientId = "board_";
        clientId += String(random(0xffff), HEX);
        if (client.connect(clientId)) {
            Serial.println("board, Connected to MQTT");
            client.publish("/admin", "board: cennected, Hello world!!");
            client.subscribe("/admin");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println("try again in 3 seconds");
            delay(3000);
            return;
        }
    }
    i++;
    client.loop();
}

/// ============================================


// Config MQTT Server
#define mqtt_server "m14.cloudmqtt.com"
#define mqtt_port 17657
#define mqtt_user "FAN/LED"
#define mqtt_password "170740"
#define LED_PIN 2
#define FAN 14

WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
  pinMode(16, INPUT);
  pinMode(4, INPUT);

  pinMode(LED_PIN, OUTPUT);
  pinMode(FAN, OUTPUT);
  pinMode(A0, INPUT);
  
  Serial.begin(115200);
  delay(10);

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
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("/ESP/LED");
    } else {

      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
      return;
    }
  }
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  int value = digitalRead(16);
  int value2 = digitalRead(4);library
  
  int vi = analogRead(A0);

  if((value==1)||(value2==1)){ 
    digitalWrite(FAN, HIGH);
    client.publish("/ESP/LED", (digitalRead(LED_PIN) ? "LEDON" : "LEDON"));

    delay(3000);


      if(vi>0) {
        Serial.println(vi);
        client.publish("/ESP/LED", (digitalRead(LED_PIN) ? "LEDX" : "LEDX"));
        }
        else {
          Serial.println(vi);
          client.publish("/ESP/LED", (digitalRead(LED_PIN) ? "LEDX" : "LEDY"));
        }
    
  }
  else {
      client.publish("/ESP/LED", (digitalRead(LED_PIN) ? "LEDON" : "LEDOFF"));
    digitalWrite(FAN, LOW);
  }
     
}
   

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ethernet.h>
#include <SPI.h>

//For reciever
// Update these with values suitable for your network.
const char* ssid = "AndroidAP";
const char* password = "k@ew030313";

// Config MQTT Server
#define mqtt_server "m14.cloudmqtt.com"
#define mqtt_port 17657
#define mqtt_user "FAN/LED"
#define mqtt_password "170740"
#define LED_PIN 2
#define FAN 14

WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
  pinMode(16, INPUT);
  pinMode(4, INPUT);

  pinMode(LED_PIN, OUTPUT);
  pinMode(FAN, OUTPUT);
  pinMode(A0, INPUT);
  
  Serial.begin(115200);
  delay(10);

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
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("/ESP/LED");
    } else {

      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
      return;
    }
  }
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  int value = digitalRead(16);
  int value2 = digitalRead(4);library
  
  int vi = analogRead(A0);

  if((value==1)||(value2==1)){ 
    digitalWrite(FAN, HIGH);
    client.publish("/ESP/LED", (digitalRead(LED_PIN) ? "LEDON" : "LEDON"));

    delay(3000);


      if(vi>0) {
        Serial.println(vi);
        client.publish("/ESP/LED", (digitalRead(LED_PIN) ? "LEDX" : "LEDX"));
        }
        else {
          Serial.println(vi);
          client.publish("/ESP/LED", (digitalRead(LED_PIN) ? "LEDX" : "LEDY"));
        }
    
  }
  else {
      client.publish("/ESP/LED", (digitalRead(LED_PIN) ? "LEDON" : "LEDOFF"));
    digitalWrite(FAN, LOW);
  }
     
}
   
