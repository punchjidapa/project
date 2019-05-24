#include <Servo.h>
#include <ArduinoJson.h>

Servo myservo;
Servo my2servo;
Servo my3servo;

int pos = 0;

const int trigPin = A1;
const int echoPin = A0;
// defines variables
long duration;
int distance;

#define Dout A5
float metalDetected;
int monitoring;
int metalDetection = 1;

//#include <Stepper.h>

// defines pins numbers
const int dirPin  = 3;
const int stepPin = 4;
const int enPin   = 5;

//DC motor
byte ena = 6;
byte in1 = 7;
byte in2 = 8;

int hallSensorPin = 2;
int ledPin =  13;
int state = 0;

#include <HX711.h>
#include "HX711.h"

float calibration_factor = 424150.00;
#define zero_factor 8429421


#define DOUT  A3
#define CLK   A2
#define DEC_POINT  4

float offset = 0;
float get_units_kg();

//void firstmotor();
void motorStep( int x);

HX711 scale(DOUT, CLK);

#include <SoftwareSerial.h>
// UNO Pin 12 -> Pin D5 NodeNCU, UNO Pin 13 -> Pin D6 NodeMCU
SoftwareSerial NodeSerial(12, 13);
byte positionData = 0;
float weightData = 0;
bool metalData = 0;

//-----------------------------------------------------------------------------------------

void setup()
{
  myservo.attach(9);
  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  // Serial.begin(115200); // Starts the serial communication

  Serial.begin(9600);
  NodeSerial.begin(9600);
  //Serial.println("Load Cell");
  scale.set_scale(calibration_factor);
  scale.set_offset(zero_factor);

  // Sets the two pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enPin, OUTPUT);

  //DC motor
  pinMode( ena, OUTPUT );
  pinMode( in1, OUTPUT );
  pinMode( in2, OUTPUT );
  
  /*pinMode(ledPin, OUTPUT);
  pinMode(hallSensorPin, INPUT);
  digitalWrite(enPin, LOW);
  // Set Dir to Home switch
  digitalWrite(dirPin, HIGH); // Enables the motor to move in a particular direction*/

  my2servo.attach(10);
  my3servo.attach(11);
}
//void(*resetFunc)(void)=0;

void loop()
{
  //---------------------------------------------------------------------------------------------------------------------- Send to node mcu
  //JSON Object to send to the node mcu
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject & root = jsonBuffer.createObject();

 
  {
      //motorStep(1);
      digitalRead(enPin)== LOW;
      digitalRead(dirPin)== LOW;
      digitalWrite(stepPin, LOW);
      delayMicroseconds(2000);
      digitalWrite(trigPin, LOW); // Clears the trigPin
      digitalWrite(trigPin, HIGH); // Sets the trigPin on HIGH state for 10 micro seconds
      delay(100);
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH); // Reads the echoPin
      // Calculate the distance (Garbage in the bin)
      // lid of the bin open
  distance = duration * 0.034 / 2;

 Serial.print("Distance: ");
      Serial.println(distance);
      {
        if (distance < 45 && distance >= 0) {
{
          //------------------------------------------------------------------------------------------------------------------------------------------------

 analogWrite( ena, 300 );

  digitalWrite( in1, HIGH );
  digitalWrite( in2, LOW );
  Serial.print("Uppppppp");
  delay(10000);

  analogWrite( ena, 0 );

  digitalWrite( in1, LOW );
  digitalWrite( in2, LOW );
  Serial.print("hold on");
  delay(4000);

  analogWrite( ena, -300 );

  digitalWrite( in1, LOW );
  digitalWrite( in2, HIGH );
  Serial.print("downnnnnn");
  delay(9000);

    analogWrite( ena, 0 );
    digitalWrite( in1, LOW );
    digitalWrite( in2, LOW );
   // Serial.println("Stop");


    //------------------------------------------------------------------------------------------------------------------------------------------------
        }

          //waste separation process****************************************************
          monitoring = analogRead(Dout);
          metalDetected = (float) monitoring * 100 / 1024.0;
          //Serial.print("14CORE METAL DETECTOR TEST");
          //delay(500);
          //Serial.print("Initializing Proximity Sensor");
          //delay(500);
          Serial.print("Metal is Proximited = ");
          Serial.print(metalDetected);
          Serial.println("%");
          //Metal trash detection
          if (monitoring > 250)
          {
            
      Serial.println("Metal is Detected");
            delay(500);
            float weight = get_units_kg()+0.0135;
            Serial.print("Reading: ");
            Serial.print(weight + offset, DEC_POINT);
            Serial.println(" kg");
            Serial.println(weight + offset, 4);
            //if ((weight+offset > 0.005 && weight+offset <= 0.016) || (weight+offset > 0.0161 && weight+offset <= 0.030))
            //if ((0.005 < weight+offset < 0.009) || (0.016 < weight+offset <=0.024))
            if (weight + offset <= 0.000) {
              Serial.println("Put your trash");
              Serial.println("----------------------------------------------------------");
              delay(5000);
            }

            //else if ((weight + offset <= 0.0115) || (weight + offset >= 0.0150 && weight + offset <= 0.020 ))
           // else if (weight + offset <= 0.017)
            else if (weight + offset <= 0.02)

        //---------------------------------------------------------------------------------------------------------------------- Send to node mcu
        //Send only positionData to node mcu
        /*positionData = 3;
          Serial.println(positionData);
          if (NodeSerial.available() > 0)
          {
          NodeSerial.write(positionData);
          }*/

        //Send positionData, weightData and metalData to node mcu
        positionData = 3;
        weightData = weight + offset;
        metalData = 1;
        root["data1"] = positionData;
        root["data2"] = weightData;
        root["data3"] = metalData;
        if (NodeSerial.available() > 0)
        {
          root.printTo(NodeSerial);
        }


                     Serial.println("CARTON");
              Serial.println("----------------------------------------------------------");
              //delay(5000);

              //my3servo.write(-60);
              //delay(1000);
              my3servo.write(0);
              delay(1000);
              my3servo.write(60);
              delay(1000);

              //180 degree
              digitalWrite(dirPin, HIGH); // Enables the motor to move in a particular direction
              // Makes 200 pulses for making one full cycle rotation
              for (int x = 0; x < 1500; x++)
              {
                digitalWrite(stepPin, HIGH);
                delayMicroseconds(2000);
                digitalWrite(stepPin, LOW);
                delayMicroseconds(2000);
              }
              delay(1000); // One second delay
              my2servo.write(0);
              delay(2000);
              my2servo.write(130);
              delay(2000);
              digitalWrite(dirPin, LOW); // Enables the motor to move in a particular direction
              // Makes 200 pulses for making one full cycle rotation
              for (int x = 0; x < 1500; x++)
              {
                digitalWrite(stepPin, HIGH);
                delayMicroseconds(2000);
                digitalWrite(stepPin, LOW);
                delayMicroseconds(2000);
              }
              //delay(10000); // One second delay
            }
      else
      {

        //---------------------------------------------------------------------------------------------------------------------- Send to node mcu
        //Send only positionData to node mcu
        //Send both positionData and weightData to node mcu
        /*positionData = 4;
          Serial.println(positionData);
          if (NodeSerial.available() > 0)
          {
          NodeSerial.write(positionData);
          }*/

        //Send positionData, weightData and metalData to node mcu
        positionData = 4;
        weightData = weight + offset;
        metalData = 1;
        root["data1"] = positionData;
        root["data2"] = weightData;
        root["data3"] = metalData;
        if (NodeSerial.available() > 0)
        {
          root.printTo(NodeSerial);
        }

 Serial.println("CAN");
              Serial.println("----------------------------------------------------------");
            
              //my3servo.write(-60);
              //delay(1000);
              my3servo.write(0);
              delay(1000);
              my3servo.write(60);
              delay(1000);

              //270 degree
              digitalWrite(dirPin, HIGH); // Enables the motor to move in a particular direction
              // Makes 200 pulses for making one full cycle rotation
              for (int x = 0; x < 2300; x++)
              {
                digitalWrite(stepPin, HIGH);
                delayMicroseconds(2000);
                digitalWrite(stepPin, LOW);
                delayMicroseconds(2000);
              }
              delay(1000); // One second delay

              my2servo.write(0);
              delay(2000);
              my2servo.write(130);
              delay(2000);
              digitalWrite(dirPin, LOW); // Enables the motor to move in a particular direction
              // Makes 200 pulses for making one full cycle rotation
              for (int x = 0; x < 2300; x++)
              {
                digitalWrite(stepPin, HIGH);
                delayMicroseconds(2000);
                digitalWrite(stepPin, LOW);
                delayMicroseconds(2000);
              }
              delay(10000); // One second delay
               
            }
          }
   if (monitoring < 250)
          {
            Serial.println("NON-Metal is Detected");
            Serial.print("Reading: ");
            float weight = get_units_kg()+0.0135;
            Serial.print("Reading: ");
            Serial.print(weight + offset, DEC_POINT);
            Serial.println(" kg");
            if (weight + offset <= 0.005) {
              Serial.println("Put your trash");
              Serial.println("----------------------------------------------------------");
              delay(5000);
            }
           // else if (weight + offset > 0.12)
            else if (weight + offset > 0.03)
            {
        //---------------------------------------------------------------------------------------------------------------------- Send to node mcu
        //Send only positionData to node mcu
        /*positionData = 2;
          Serial.println(positionData);
          if (NodeSerial.available() > 0)
          {
          NodeSerial.write(positionData);
          }*/

        //Send positionData, weightData and metalData to node mcu
        positionData = 2;
        weightData = weight + offset;
        metalData = 0;
        root["data1"] = positionData;
        root["data2"] = weightData;
        root["data3"] = metalData;
        if (NodeSerial.available() > 0)
        {
          root.printTo(NodeSerial);
        }

        Serial.println("GLASS");
              Serial.println("----------------------------------------------------------");
              //delay(5000);

             // my3servo.write(-60);
             //delay(1000);
              my3servo.write(0);
              delay(1000);
              my3servo.write(60);
              delay(1000);

              //0 degree
              digitalWrite(dirPin, HIGH); // Enables the motor to move in a particular direction
              // Makes 200 pulses for making one full cycle rotation

              for (int x = 0; x++;)
              {
                digitalWrite(stepPin, HIGH);
                delayMicroseconds(2000);
                digitalWrite(stepPin, LOW);
                delayMicroseconds(2000);
              }
              delay(1000); // One second delay

              my2servo.write(0);
              delay(2000);
              my2servo.write(130);
              delay(2000);

            }
      else
      {
        //---------------------------------------------------------------------------------------------------------------------- Send to node mcu
        //Send only positionData to node mcu
        /*positionData = 1;
          Serial.println(positionData);
          if (NodeSerial.available() > 0)
          {
          NodeSerial.write(positionData);
          }*/

        //Send positionData, weightData and metalData to node mcu
        positionData = 1;
        weightData = weight + offset;
        metalData = 0;
        root["data1"] = positionData;
        root["data2"] = weightData;
        root["data3"] = metalData;
        if (NodeSerial.available() > 0)
        {
          root.printTo(NodeSerial);
        }

        Serial.println("PLASTIC BOTTLE");
              Serial.println("----------------------------------------------------------");
              delay(500);

             // my3servo.write(-60);
             // delay(1000);
              my3servo.write(0);
              delay(1000);
              my3servo.write(60);
              delay(1000);

              //90 degree
              digitalWrite(dirPin, HIGH); // Enables the motor to move in a particular direction
              // Makes 200 pulses for making one full cycle rotation
              for (int x = 0; x < 700; x++)
              {
                digitalWrite(stepPin, HIGH);
                delayMicroseconds(2000);
                digitalWrite(stepPin, LOW);
                delayMicroseconds(2000);
              }
              delay(1000); // One second delay


              my2servo.write(0);
              delay(2000);
              my2servo.write(130);
              delay(2000);

              digitalWrite(dirPin, LOW); // Enables the motor to move in a particular direction
              // Makes 200 pulses for making one full cycle rotation
              for (int x = 0; x < 700; x++)
              {
                digitalWrite(stepPin, HIGH);
                delayMicroseconds(2000);
                digitalWrite(stepPin, LOW);
                delayMicroseconds(2000);
              }
              delay(10000); // One second delay
            }
          }
        }
      }
    }
/*  else {
      digitalRead(dirPin)== HIGH;
      digitalWrite(stepPin, HIGH);
       digitalWrite(enPin, HIGH);
      delayMicroseconds(2000);
      //digitalWrite(stepPin, LOW);
     // delayMicroseconds(2000);
    }*/
  }


float get_units_kg()
{
  return (scale.get_units() * 0.453592);
}

void motorStep( int x)
{
  for (int x = 0; x <= 1600; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
  }
}
