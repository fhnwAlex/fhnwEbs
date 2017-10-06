

#define EN1 6//enable the right motor driver port 
#define IN1 7//direction of the right motor
#define EN2 5//enable the left motor driver port
#define IN2 12//direction of the left motor

#define FORW 0//forward
#define BACK 1//back

#define IR_IN  17//infrared receiver
#define L_IR 13//infrared transmitter in the left side
#define R_IR 8//infrared transmitter in the right side

int   count;//the count of the pulse received

//M1_DIR for left motor direction, M1_EN for speed of left motor, so does the right motor
void Motor_Control(int M1_DIR,int M1_EN,int M2_DIR,int M2_EN)
{
  //////////M1////////////////////////
  if(M1_DIR==FORW)  digitalWrite(IN1,FORW); else digitalWrite(IN1,BACK);
  if(M1_EN==0)       analogWrite(EN1,LOW);  else analogWrite(EN1,M1_EN);
  ///////////M2//////////////////////
  if(M2_DIR==FORW) digitalWrite(IN2,FORW);  else digitalWrite(IN2,BACK);
  if(M2_EN==0)     analogWrite(EN2,LOW);    else analogWrite(EN2,M2_EN);
}

void L_Send40KHZ(void)//  left infrared LED send 40kHZ pulse
{
  int i;
  for(i=0;i<24;i++)
  {
    digitalWrite(L_IR,LOW);
    delayMicroseconds(8);
    digitalWrite(L_IR,HIGH);
    delayMicroseconds(8);
  }
}

void R_Send40KHZ(void)//right infrared LED send 40kHZ pulse
{
  int i;
  for(i=0;i<24;i++)
  {
    digitalWrite(R_IR,LOW);//
    delayMicroseconds(8);
    digitalWrite(R_IR,HIGH);
    delayMicroseconds(8);
  }
}

void pcint0_init(void)//interrupt initialize
{
  PCICR = 0X01;//enable pins in set 0 
  PCMSK0 = 0X01;//enable pin of number 0
}
ISR(PCINT0_vect)//Pin PB0 interrupt function
{
  count++;//count the pulse
}

void Obstacle_Avoidance(void)
{   
  char i;
  count=0;
  for(i=0;i<20;i++)//send 20 pulse
  {
    L_Send40KHZ();
    delayMicroseconds(600);    
  }
  if(count>20)//if count more than 20, it is obstacle 
  {
      Motor_Control(BACK,100,BACK,100);//turn back
      delay(200);
      Motor_Control(BACK,100,FORW,100);//turn right
      delay(200);
  }
  else
  {
      Motor_Control(FORW,100,FORW,100);
  }
  count=0;
  for(i=0;i<20;i++)
  {
    R_Send40KHZ(); 
    delayMicroseconds(600);        
  }
  if(count>20)
  {
      Motor_Control(BACK,100,BACK,100);
      delay(200);
      Motor_Control(FORW,100,BACK,100);
      delay(200);
  }
  else
  {
      Motor_Control(FORW,100,FORW,100);//forward
  }
}

void setup()
{
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(12,OUTPUT);
  
  pinMode(8,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(14,OUTPUT);
  pinMode(16,OUTPUT);
  pinMode(17,INPUT);

  pcint0_init();
}
void loop()
{
  Obstacle_Avoidance();
}
