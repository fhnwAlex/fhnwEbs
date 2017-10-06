
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define PIN 10// RGB LED pin

#define EN1 5//left motor..speed  
#define IN1 12//left motor...direction
#define EN2 6//right motor..speed  
#define IN2 7//right motor...direction

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);//RGB LED

int Cmd_Num=0;//command data recived 
char Cmd_Str;//command sting recived 

void setup()
{
  pinMode(5,OUTPUT);//init the motor driver pins
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(16,OUTPUT);
  digitalWrite(16,LOW);
  
  Wire.begin(2);                // join i2c bus with address #2
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);
  
  strip.begin();//init RGB strip
  strip.show(); // Initialize all pixels to 'off'  
}

void loop()
{  
  Motor();//control motors
  Color();//change RGB color 
}

void Color()//display different color
{
  switch (Cmd_Str)
  {
    case 'F':  strip.setPixelColor(0, 0xbb0000);    strip.show(); break;
    case 'B':  strip.setPixelColor(0, 0x0000bb);    strip.show(); break;
    case 'L':  strip.setPixelColor(0, 0x00bb00);    strip.show(); break;
    case 'R':  strip.setPixelColor(0, 0x550055);    strip.show(); break;
    case 'S':  strip.setPixelColor(0, 0x005555);    strip.show(); break;
    default: break;
  }
}
void Motor()//run different action depend on the command
{
    switch (Cmd_Str)
  {
    case 'F': Forward(Cmd_Num,Cmd_Num);       break;
    case 'B': Back(Cmd_Num,Cmd_Num);          break;
    case 'L': Turn_Left(Cmd_Num,Cmd_Num);     break;
    case 'R': Turn_Right(Cmd_Num,Cmd_Num);    break;
    case 'S': Stop();                         break;
    default: break;
  }
}

void receiveEvent(int howMany)//IIC interrupt
{
  while(Wire.available()>1) // loop through all but the last
  {
    Cmd_Str = Wire.read(); // receive byte as a character
    Serial.print(Cmd_Str);     // print the character
  }
  Serial.print("  "); 
  Cmd_Num = Wire.read();    // receive byte as an integer
  Serial.println(Cmd_Num);         // print the integer
}


void Forward(int speed_x,int speed_y)
{
    digitalWrite(IN1,LOW);analogWrite(EN1,speed_x);
    digitalWrite(IN2,LOW);analogWrite(EN2,speed_y);
}

void Back(int speed_x,int speed_y)
{
    digitalWrite(IN1,HIGH);analogWrite(EN1,speed_x);
    digitalWrite(IN2,HIGH);analogWrite(EN2,speed_y);
}

void Turn_Left(int speed_x,int speed_y)
{
    digitalWrite(IN1,HIGH);analogWrite(EN1,speed_x);
    digitalWrite(IN2,LOW);analogWrite(EN2,speed_y);
}

void Turn_Right(int speed_x,int speed_y)
{
    digitalWrite(IN1,LOW);analogWrite(EN1,speed_x);
    digitalWrite(IN2,HIGH);analogWrite(EN2,speed_y);
}
void Stop()
{
    digitalWrite(IN1,HIGH);analogWrite(EN1,0);
    digitalWrite(IN2,HIGH);analogWrite(EN2,0);
}

