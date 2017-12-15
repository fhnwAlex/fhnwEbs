#define EN1 6//pin for run the right motor 
#define IN1 7//pin for control right motor direction
#define EN2 5//pin for run the left motor 
#define IN2 12//pin for control left motor direction

#define FORW 0//go forward
#define BACK 1//go back

#define IR_IN  17//IR receiver pin
#define L_IR 13//left ir transmitter pin
#define R_IR 8//right ir transmitter pin

int count;//count the motor speed pulse

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

void L_Send40KHZ(void)//left ir transmitter sends 40kHZ pulse
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
void R_Send40KHZ(void)//right ir transmitter sends 40kHZ pulse
{
  int i;
  for(i=0;i<24;i++)
  {
    digitalWrite(R_IR,LOW);
    delayMicroseconds(8);
    digitalWrite(R_IR,HIGH);
    delayMicroseconds(8);
  }
}
void pcint0_init(void)//init the interrupt
{
  PCICR = 0X01;//
  PCMSK0 = 0X01;//
}

ISR(PCINT0_vect)//motor encoder interrupt
{
  count++;
}
void Obstacle_Avoidance(void)
{
  char i;
  count=0;
  for(i=0;i<20;i++)//left transmitter sends 20 pulses
  {
    L_Send40KHZ();
    delayMicroseconds(600);    
  }
  if(count>20)//if recieved a lot pulse , it means there's a obstacle
  {
    Motor_Control(BACK,100,BACK,100);
    delay(300);
    Motor_Control(BACK,100,FORW,100);
    delay(500);
  }
  else
  {
    count=0;
    for(i=0;i<20;i++)//right transmitter sends 20 pulses
    {
      R_Send40KHZ(); 
      delayMicroseconds(600);        
    }
    if(count>20)
    {
      Motor_Control(BACK,100,BACK,100);
      delay(300);
      Motor_Control(FORW,100,BACK,100);
      delay(500);
    }
    else
    {
      Motor_Control(FORW,100,FORW,100);
    }
  }
}
void setup()
{
  pinMode(5,OUTPUT);//init the motor driver pins
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(12,OUTPUT);
  
  pinMode(L_IR,OUTPUT);//init the left transmitter pin
  pinMode(R_IR,OUTPUT);//init the right transmitter pin
  pinMode(IR_IN,INPUT);//init the ir receiver pin
  
  digitalWrite(R_IR,HIGH);
  digitalWrite(L_IR,HIGH);
  pcint0_init();
  sei();               //enable the interrupt
}
void loop()
{
  Motor_Control(FORW,100,FORW,100);//run motor
  while(1)
  {
    Obstacle_Avoidance();//obstacle avoidance
  }
}
