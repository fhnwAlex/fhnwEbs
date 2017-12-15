

#define EN1 5//pin for run the left motor 
#define IN1 12//pin for control left motor direction
#define EN2 6//pin for run the right motor 
#define IN2 7//pin for control right motor direction


#define Forward 0      // value for forward
#define Back 1         //value for go back

int data[5]={};

void Motor(int M1_DIR,int M1_EN,int M2_DIR,int M2_EN)//control the motor
{
  //////////M1-->left motor////////////////////////
  if(M1_DIR==Forward)//distinguish the command
    digitalWrite(IN1,Forward);
  else
    digitalWrite(IN1,Back);
  if(M1_EN==0)
    analogWrite(EN1,LOW);
 else
    analogWrite(EN1,M1_EN);

  ///////////M2-->right motor//////////////////////
  if(M2_DIR==Forward)
    digitalWrite(IN2,Forward);
  else
    digitalWrite(IN2,Back);
  if(M2_EN==0)
    analogWrite(EN2,LOW);
  else
    analogWrite(EN2,M2_EN);
}

void Read_Value(void)//read the five sensors
{  
  char i;
  for(i=0;i<5;i++)
  {
    data[i]=analogRead(i);//store the value read from the sensors
  }
}

void huntline_deal( )//function for line-hunt, white and black line, the value is less than 700 when detect line and for paper is bigger than 800
{
  if((data[0]<650 || data[1] <650) && (data[3]>800 && data[4]>800))//left detect black line
  { 
    Motor(Forward,0,Forward,110);//turn right
    delay(10);
  }
  else  if((data[3]<650 || data[4] <650) && (data[0]>800 && data[1]>800))//black line in the right
  {
    Motor(Forward,110,Forward,0);//turn left
    delay(10);
  }
  else if(data[2]<700)//line in middle
  {
    Motor(Forward,90,Forward,90);//go forward
    delay(20);
  }
  else 
  {
    Motor(Forward,70,Forward,70);//undefine state just go slowly
  }
}


void setup()
{
  pinMode(5,OUTPUT);//init the pins for motor
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(12,OUTPUT); 
  Motor(Forward,0,Forward,0);//电机停转
}

void loop()
{
  Read_Value();//read the value of sensors
  huntline_deal();//function for line hunt
}









