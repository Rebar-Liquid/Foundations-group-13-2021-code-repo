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
  moveLogic();
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

void moveLogic()
{
  float lineone = panDetect(-20);
  float linetwo = panDetect(-10);
  float linethree = panDetect(0);
  float linefour = panDetect(10);
  float linefive = panDetect(20);
  float avgRight = ((lineone + linetwo) / 2 ) + 5;
  float avgLeft = ((linefour + linefive) / 2 ) + 5;
  if (avgRight < avgLeft)
  {
    if (avgRight < linethree)
    {
      float modleft = avgLeft * 8;   //turn left
      modForward(240,modleft);
    }
  }
  else if (avgRight > avgLeft)
  {
    if (avgLeft < linethree)
    {
      float modright = avgRight * 8;   //turn right
      modForward(modright,240);
    }
  }
  else if (linethree < 10)
  {
    halt();
  }
  else
  {
    fullForward();
  }
}

void roomba()
{
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
}

float panDetect(int angle)
{
  int currentpos = mainservo.read();
  float reading = sensorDistance();
  mainservo.write(currentpos - angle);
  if (reading < 50) { return reading; }
  delay(20);
}

void fullForward()        //set both motors to maximum forward
{
  analogWrite(enablepin1,240);
  digitalWrite(forwardpin1,HIGH);
  digitalWrite(backwardpin1,LOW);
  analogWrite(enablepin2,240);
  digitalWrite(forwardpin2,HIGH);
  digitalWrite(backwardpin2,LOW);
  //Serial.println("Forward");
}

void modForward(int rightpower, int leftpower)     //set both motors to a set forward value
{  
  analogWrite(enablepin1,rightpower);
  digitalWrite(forwardpin1,HIGH);
  digitalWrite(backwardpin1,LOW);
  analogWrite(enablepin2,leftpower);
  digitalWrite(forwardpin2,HIGH);
  digitalWrite(backwardpin2,LOW);
  //Serial.println("Modular Forward");
}

void spinLeft()         //Spin counterclockwise
{
  analogWrite(enablepin1,255);
  digitalWrite(forwardpin1,HIGH);
  digitalWrite(backwardpin1,LOW);
  analogWrite(enablepin2,255);
  digitalWrite(forwardpin2,LOW);
  digitalWrite(backwardpin2,HIGH);
  //Serial.println("Left");
}

void spinRight()        //spin clockwise
{
  analogWrite(enablepin1,255);
  digitalWrite(forwardpin1,LOW);
  digitalWrite(backwardpin1,HIGH);
  analogWrite(enablepin2,255);
  digitalWrite(forwardpin2,HIGH);
  digitalWrite(backwardpin2,LOW);
  //Serial.println("Right");
}

void back()
{
  analogWrite(enablepin1,255);
  digitalWrite(forwardpin1,LOW);
  digitalWrite(backwardpin1,HIGH);
  analogWrite(enablepin2,255);
  digitalWrite(forwardpin2,LOW);
  digitalWrite(backwardpin2,HIGH);
  //Serial.println("back");
}

void halt()             //stop all motion
{
  analogWrite(enablepin1,0);
  digitalWrite(forwardpin1,LOW);
  digitalWrite(backwardpin1,LOW);
  analogWrite(enablepin2,0);
  digitalWrite(forwardpin2,LOW);
  digitalWrite(backwardpin2,LOW);
  //Serial.println("Halt");
}

void stop()       //Stops the function by putting it into an infinite loop
{
  while(1);
}
