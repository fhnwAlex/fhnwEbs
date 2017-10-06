
#define BUZZER 16//buzzer is connected with D16 of Arduino

void setup()    //initialization
{
  pinMode(BUZZER,OUTPUT);//Set pin 16 to OUTPUT mode
  Serial.begin(9600);//Serial baudrate:9600
}

void loop()             
{
  int i=0,j=0;
  i=analogRead(5);      //i is the value read from analog pin 5
  Serial.println(i);    //print i to computer
  if(i<400)             //if light is on the left
  {
    for(i=0;i<80;i++)    //Buzzer rings
    {
      digitalWrite(BUZZER,HIGH);
      delay(1);                 
      digitalWrite(BUZZER,LOW);
      delay(1);  
    }
  }
  else if (i>600)         //if light is on the right side
  {
    for(i=0;i<80;i++)       //Buzzer rings at another frequency
    {
      digitalWrite(BUZZER,HIGH);
      delay(3);                 
      digitalWrite(BUZZER,LOW);
      delay(3);   
    }
  }
}



