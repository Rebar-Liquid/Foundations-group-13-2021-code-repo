#include <Servo.h>
#include <IRremote.h>

//Servo
const int servopin = 7;
Servo mainservo;

//Sensor
const int trigpin = 12;
const int echopin = 13;
float duration;
float distance;

//Motor 1 (Left)
const int enablepin1 = 11;
const int forwardpin1 = 5;
const int backwardpin1 = 6;

//Motor 2 (Right)
const int enablepin2 = 3;
const int forwardpin2 = 8;
const int backwardpin2 = 10;

//Remote
const int RECV_PIN = 9;

void setup() {
  // put your setup code here, to run once:
  mainservo.attach(servopin);
  mainservo.write(5);
  pinMode(echopin,INPUT);
  pinMode(trigpin,OUTPUT);
  pinMode(enablepin1,OUTPUT);         //Set all pins to output
  pinMode(forwardpin1,OUTPUT);
  pinMode(backwardpin1,OUTPUT);
  pinMode(enablepin2,OUTPUT);
  pinMode(forwardpin2,OUTPUT);
  pinMode(backwardpin2,OUTPUT);
  Serial.begin(9600);
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (sensorDistance() < 15)
  {
    halt();
    delay(500);
    back();
    delay(300);
    spinRight();
    delay(300);
    fullForward();
  }
  if (IrReceiver.decodedIRData.command == 24)
  {
    fullForward();
  }
  if (IrReceiver.decodedIRData.command == 28)
  {
    halt();
    while(1);
  }
}

float sensorDistance()
{
  pinMode(echopin,OUTPUT);
  digitalWrite(echopin,LOW);
  pinMode(echopin,INPUT);
  digitalWrite(trigpin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin,LOW);
  duration = pulseIn(echopin,HIGH);
  distance = duration*0.0343 /2;
  //Serial.print(distance);
  //Serial.println(" cm ");
  return distance;
}

float panDetect(int angle)
{
  int currentpos = mainservo.read();
  mainservo.write(currentpos - angle);
  delay(20);
  return sensorDistance();
}

void fullForward()        //set both motors to maximum forward
{
  analogWrite(enablepin1,255);
  digitalWrite(forwardpin1,HIGH);
  digitalWrite(backwardpin1,LOW);
  analogWrite(enablepin2,255);
  digitalWrite(forwardpin2,HIGH);
  digitalWrite(backwardpin2,LOW);
  Serial.println("Forward");
}

void spinLeft()         //Spin counterclockwise
{
  analogWrite(enablepin1,255);
  digitalWrite(forwardpin1,HIGH);
  digitalWrite(backwardpin1,LOW);
  analogWrite(enablepin2,255);
  digitalWrite(forwardpin2,LOW);
  digitalWrite(backwardpin2,HIGH);
  Serial.println("Left");
}

void spinRight()        //spin clockwise
{
  analogWrite(enablepin1,255);
  digitalWrite(forwardpin1,LOW);
  digitalWrite(backwardpin1,HIGH);
  analogWrite(enablepin2,255);
  digitalWrite(forwardpin2,HIGH);
  digitalWrite(backwardpin2,LOW);
  Serial.println("Right");
}

void back()
{
  analogWrite(enablepin1,255);
  digitalWrite(forwardpin1,LOW);
  digitalWrite(backwardpin1,HIGH);
  analogWrite(enablepin2,255);
  digitalWrite(forwardpin2,LOW);
  digitalWrite(backwardpin2,HIGH);
  Serial.println("back");
}

void halt()             //stop all motion
{
  analogWrite(enablepin1,0);
  digitalWrite(forwardpin1,LOW);
  digitalWrite(backwardpin1,LOW);
  analogWrite(enablepin2,0);
  digitalWrite(forwardpin2,LOW);
  digitalWrite(backwardpin2,LOW);
  Serial.println("Halt");
}

void stop()       //Stops the function by putting it into an infinite loop
{
  while(1);
}
