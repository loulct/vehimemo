#include <Servo.h>
Servo servo;

#include<SoftwareSerial.h>
SoftwareSerial BluetoothConnection(6,11);

int motor1_enablePin = 9;
int motor1_in1Pin = 12;
int motor1_in2Pin = 13;

int motor2_enablePin = 10;
int motor2_in1Pin = 7;
int motor2_in2Pin = 8;

int trig = 2;
int echo = 3;

long lecture_echo;
long distance_cm;

int BluetoothData;

void setup(){
  pinMode(motor1_in1Pin, OUTPUT);
  pinMode(motor1_in2Pin, OUTPUT);
  pinMode(motor1_enablePin, OUTPUT);

  pinMode(motor2_in1Pin, OUTPUT);
  pinMode(motor2_in2Pin, OUTPUT);
  pinMode(motor2_enablePin, OUTPUT);

  servo.attach(5);

  pinMode(trig, OUTPUT);
  digitalWrite(trig, LOW);
  pinMode(echo, INPUT);

  BluetoothConnection.begin(9600);
  BluetoothConnection.println("Bluetooth ready...");
}

void loop(){
  servoMotor(30,70);
}

void servoMotor(int angle_start, int angle_end){
  for(int position = angle_start; position <= angle_end; position++){
    servo.write(position);
    ultrasonicSensor();
    ifObstacleStop();
    delay(50);
  }for(int position = angle_end; position >= angle_start; position--){
    servo.write(position);
    ultrasonicSensor();
    ifObstacleStop();
    delay(50);
  }
}

void ifObstacleStop(){
  if(distance_cm < 25){
      Motor(0, 0, true);
      while(distance_cm < 25){
        ultrasonicSensor();
      }
  }else if(BluetoothConnection.available()){
    BluetoothData = BluetoothConnection.read();
    switch(BluetoothData){
      case '0':
        Motor(0, 0, true);
        break;
      case '1':
        Motor(255, 255, true);
        break;
      case '2':
        Motor(255, 255, false);
        break;
      case '3':
        Motor(0, 255, true);
        break;
      case '4':
        Motor(255, 0, true);
        break;
      case '5':
        Motor(0, 255, false);
        break;
      case '6':
        Motor(255, 0, false);
        break;
    }
    delay(50);
  }else{
    Motor(0, 0, true)
  }
}

void ultrasonicSensor(){
  digitalWrite(trig, HIGH);
  delayMicroseconds(30);
  digitalWrite(trig, LOW);

  lecture_echo = pulseIn(echo, HIGH);
  distance_cm = lecture_echo / 58;
}

void Motor(int PWM2, int PWM1, boolean sens){
  SetMotor2(PWM2, sens);
  SetMotor1(PWM1, ! sens);
}

void SetMotor1(int speed, boolean reverse){
  analogWrite(motor1_enablePin, speed);
  digitalWrite(motor1_in1Pin, ! reverse);
  digitalWrite(motor1_in2Pin, reverse);
}

void SetMotor2(int speed, boolean reverse){
  analogWrite(motor2_enablePin, speed);
  digitalWrite(motor2_in1Pin, ! reverse);
  digitalWrite(motor2_in2Pin, reverse);
}
