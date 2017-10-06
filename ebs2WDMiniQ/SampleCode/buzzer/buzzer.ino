#define BUZZER 16//Buzzer is connected to D16 with Arduino

void setup()   //initialization
{
  pinMode(BUZZER,OUTPUT);//set D16 to output module
}
void loop() //main function,it will looping execution
{
  int i=0;
  //Give a sound
  for(i=0;i<80;i++)
  {
    digitalWrite(BUZZER,HIGH);//turn on power supply
    delay(1);                 // delay for 1ms
    digitalWrite(BUZZER,LOW);//Turn off power supply
    delay(1);               //delay for 1ms
  }
  //Play another kind of sound
  for(i=0;i<100;i++)
  {
    digitalWrite(BUZZER,HIGH);
    delay(2);			
    digitalWrite(BUZZER,LOW);
    delay(2);			
  }
}


