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
const int distanceLimit = 30;       // Khoảng cách phát hiện vật cản
const int clearanceDistance = 40;   // Khoảng cách tối thiểu để rẽ
const int turnDuration = 400;       // Thời gian rẽ (ms)
const int reverseDuration = 1000;   // Thời gian lùi (ms)
const int scanDelay = 400;          // Thời gian chờ servo quét (ms)

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

void stop()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

long getDistance()
{
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  long duration = pulseIn(Echo, HIGH, 30000); // Timeout 30ms (~500cm)
  if (duration == 0) {
    return 999;} // if no echo received, return a large distance <=> no obstacle detected

  long dist = duration * 0.034 / 2;
  return dist;
}


long getDistanceAvg()
{
  long total = 0;
  for (int i = 0; i < 3; i++)
  {
    total += getDistance();
    delay(20);
  }
  return total / 3;
}

long scanAt(int angle)
{
  servo.write(angle);
  delay(scanDelay);
  long distance = getDistanceAvg();
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
  servo.write(90);
  delay(100);

  const int frontDistance = getDistanceAvg();
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
      delay(turnDuration);
    }
    else if (rightDistance > leftDistance && rightDistance > clearanceDistance)
    {
      turnright(225, 225);
      Serial.println("Turning right");
      delay(turnDuration);
    }
    else
    {
      backward(225, 225);
      Serial.println("Moving backward");
      delay(reverseDuration);
    }
  }
  else
  {
    Serial.println("Moving forward");
    forward(225, 225);
  }
}