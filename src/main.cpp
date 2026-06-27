#include <Arduino.h>
#include <Wire.h>
#include <stdio.h>
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
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

//Rẽ trái
void turnleft(int speed, int speed2)
{
  analogWrite(ENA, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENB, speed2);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

//Đo khoảng cách — trả về khoảng cách (cm)
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

void setup() {
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
}

void loop() 
{
  //Servo default position
  servo.write(0);
  delay(100);

  long dist = getDistance();
  if (dist <= 20) {
    servo.write(90);
    delay(1000); // Wait for the servo to move
  } else {
    servo.write(0); // Return to default position
  }
  if (dist <= 20) {
    // Stop the motors if an obstacle is detected
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
    getDistance(); // Update distance reading
  } else {
    // Continue moving forward if no obstacle is detected
    forward(240, 240);
  }

  
  //Forward 100%
  forward(240, 240);
  delay(5000);
  
  //Backwards 60%
  backward(153, 153);
  delay(5000);

  //turnright sharp
  turnright(180, 150);
  delay(5000);

  //turnleft sharp
  turnleft(150, 180);
  delay(5000);
}