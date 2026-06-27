#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>


//Module
const int ENA = 9;
const int IN1 = 8;
const int IN2 = 7;
const int IN3 = 4;
const int IN4 = 5;
const int ENB = 6;
const int Trig = 13;
const int Echo = 12;
const int servoPin = 10;

//Distance limit (cm)
const int distanceLimit = 30;
const int clearanceDistance = 40;
const unsigned long reverseduration = 4000; // Thời gian lùi lại (ms)

int pos = 0;    // variable to store the servo position

//Khai báo đối tượng servo
Servo servo;

//Tiến
void forward(int speed, int speed2)
{
  analogWrite(ENA, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENB, speed2);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

//Lùi
void backward(int speed, int speed2)
{
  analogWrite(ENA, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENB, speed2);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

//Rẽ phải
void turnright(int speed, int speed2)
{
  analogWrite(ENA, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENB, speed2);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

//Rẽ trái
void turnleft(int speed, int speed2)
{
  analogWrite(ENA, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENB, speed2);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

long getDistance()
{
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  long duration = pulseIn(Echo, HIGH);
  long dist = duration * 0.034 / 2;
  return dist;
}

void stop()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

long scanAt(int angle)
{
  servo.write(angle);
  delay(400);
  long distance = getDistance();
  return distance;
}



void setup() {

  Serial.begin(9600);
  //Khai báo chân
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);


  servo.attach(servoPin);


  //Chế độ ban đầu
  analogWrite(ENA, 255);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENB, 255);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  delay(100);
}

void loop() 
{
  //Servo default position
  // Serial.println("Servo default position");
  servo.write(90);
  delay(100);

  const int frontDistance = getDistance();
  Serial.print("Front distance: ");
  Serial.println(frontDistance);
  if (frontDistance < distanceLimit)
  {
    Serial.println("Stop");
    stop();
    delay(100);
    long leftDistance = scanAt(45);
    Serial.print("Left distance: ");
    Serial.println(leftDistance);
    long rightDistance = scanAt(135);
    Serial.print("Right distance: ");
    Serial.println(rightDistance);

    if (leftDistance > rightDistance && leftDistance > clearanceDistance)
    {
      turnleft(225, 225);
      Serial.println("Turning left");
      delay(400);
    }
    else if (rightDistance > leftDistance && rightDistance > clearanceDistance)
    {
      turnright(225, 225);
      Serial.println("Turning right");
      delay(400);
    }
    else
    {
      backward(225, 225);
      Serial.println("Moving backward");
      delay(400);
      delay(4000);
    }
  }
  else
  {
    Serial.println("Moving forward");
    forward(225, 225);
  }
}