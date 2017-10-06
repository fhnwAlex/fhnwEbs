
#include <Wire.h>

#define Address_Bottom 2        //slave address

char cmd[]={'F','B','L','R','S'};//F-->Forward，B-->Back，L-->Left，R-->Right，S-->Stop
int Distance[6];//store the analog value of the six analog pins
int Dist_Max=0,Num_Max=0;//the max value and which pin
void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
}

void loop()
{
  Num_Max=0;
  Dist_Max=0;
  for(int i=0;i<6;i++)
  {
    Distance[i]=analogRead(i);
  }
  for(int i=0;i<6;i++)        //find the max value of analog pins and which pin gives that value
  {
      if(Distance[i]>Dist_Max)
        {
          Dist_Max=Distance[i];
          Num_Max=i;
        }  
   Serial.print(Distance[i]);
   Serial.print(" ");
  }
  Serial.print("        ");
  Serial.print(Dist_Max);
  Serial.print("   ");
  Serial.println(Num_Max);
  if(Dist_Max>100)                            //send command
  {
       if(Num_Max<2) {Send_Command(cmd[2],100);}//pin 0 or 1，send "L100", means turn left, speed 100
  else if(Num_Max==4 || Num_Max==5) {Send_Command(cmd[3],100); }//pin 4 or 5, send "R100" means turn right, speed 100
  else if(Num_Max==2 || Num_Max==3) 
        {
          if(Dist_Max>800)//如果距离很近
          {
            Send_Command(cmd[4],100);Serial.println(cmd[4]);delay(200);
          }
          else
          {
            Send_Command(cmd[0],100);Serial.println(cmd[0]); 
          }
         }
  }
  else 
  {
    Send_Command(cmd[4],80);Serial.println(cmd[2]); //
  }
   
   delay(100);
   //Get_Value(Num_Max);
}

 void Send_Command(char command,int num)
 {
  Wire.beginTransmission(Address_Bottom); //send data to address 2
  Wire.write(command);        //send command
  Wire.write(num);        //send data
  Wire.endTransmission();    // end 
  Serial.println(num);//for debug, you can delete this 
 }
 

