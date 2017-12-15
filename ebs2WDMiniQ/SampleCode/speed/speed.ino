#define R 2  //right encoder interrupt num
#define L 3  //left  encoder interrupt num
#define EN1 6//pin for run the right motor 
#define IN1 7//pin for control right motor direction
#define EN2 5//pin for run the left motor 
#define IN2 12//pin for control left motor direction
float  time_step= 500; //delay time
volatile int r_val=0;  //store right wheel num
volatile int l_val=0;  //store left wheel num
volatile int r_speed=255; //right speed
volatile int l_speed=255;  //left speed
float l_speed_reality; //reality right speed
float r_speed_reality;  //reality left speed

#define FORW 0
#define BACK 1
int cmd;
void setup() {
// put your setup code here, to run once:
  pinMode(5,OUTPUT);//init the motor driver pins
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(12,OUTPUT);
  
  attachInterrupt(R,right_count , CHANGE); //attach isr function
  attachInterrupt(L,left_count, CHANGE);  //attach isr function
  Serial.begin(9600);
  Motor_Control(FORW,r_speed,FORW,l_speed);
}

void loop() {
  
  delay(time_step);
  // put your main code here, to run repeatedly:
  //calculate speed
  r_speed_reality=r_val/(time_step/1000);
     Serial.print("r_val:");
     Serial.print(r_val);
     Serial.print("\t r_speed_reality:");
     Serial.print(r_speed_reality);
   //  calculate speed
   l_speed_reality=l_val/(time_step/1000);
     Serial.print("\t l_val:");
     Serial.print(l_val);
     Serial.print("\t l_speed_reality:");
     Serial.println(l_speed_reality);
  r_val=0;
  l_val=0;
  
}
//the function to setup motor
void Motor_Control(int M1_DIR,int M1_EN,int M2_DIR,int M2_EN)//control motor
{
  //////////M1////////////////////////
  if(M1_DIR==FORW)//M1 motor direction
    digitalWrite(IN1,FORW);//forward
  else
    digitalWrite(IN1,BACK);//back
  if(M1_EN==0)
    analogWrite(EN1,LOW);//stop
  else
    analogWrite(EN1,M1_EN);//set speed

  ///////////M2//////////////////////
  if(M2_DIR==FORW)
    digitalWrite(IN2,FORW);
  else
    digitalWrite(IN2,BACK);
  if(M2_EN==0)
    analogWrite(EN2,LOW);
  else
    analogWrite(EN2,M2_EN);
}
//the right motor isr function
void right_count()
{
  //Serial.println("r");
  r_val++;
}
//the left motor isr function
void left_count()
{
  //Serial.println("L");
  l_val++;
}
