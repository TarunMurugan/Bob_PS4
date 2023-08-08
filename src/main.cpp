//#include <PS4Controller.h>
#include <ps5Controller.h>
#include <Arduino.h>

unsigned long lastTimeStamp = 0;

#define PI 3.1415926536
int leftwheel[2]={12,14},rightwheel[2]={27,26},output[2],shooter[2]={23,22};
int rawvalx,rawvaly;
int shoot_flag=0;


void out(int rawval, int pin[2]){
  if(rawval>0){ 
    digitalWrite(pin[1],1);
    if(pin[0]==12){
    ledcWrite(0,rawval);
    }
    else{
      ledcWrite(1,rawval);
    }
    
  }
  else{
    digitalWrite(pin[1],0);
    if(pin[0]==12){
    ledcWrite(0,-rawval);
    }
    else{
      ledcWrite(1,-rawval);
    }
  }

}
void differential(int x,int y,int output[]){
  int rawRight,rawLeft;
  float z = sqrt(x * x + y * y);
  float rad = acos(fabs(x) / z);
  float angle = rad * 180 /PI;

  float tcoeff = -1 + (angle / 90) * 2;
    

  int mov = max(fabs(y), fabs(x));
  float turn = tcoeff * mov;

  if((x >= 0 && y >= 0)||(x < 0 && y < 0)){
        rawRight = mov;
        rawLeft = turn;
  }
  else{
        rawLeft = mov;
        rawRight = turn;
  }

    
  if(y < 0){
        rawLeft = 0 - rawLeft;
        rawRight = 0 - rawRight;}

  output[0]=rawLeft;
  output[1]=rawRight;


}


void notify()
{  
    rawvalx=((ps5.LStickX()>-10 && 10>ps5.LStickX())?0:ps5.LStickX())*2;
    rawvaly=((ps5.LStickY()>-10 && 10>ps5.LStickY())?0:ps5.LStickY())*2;
    if(ps5.L1()){
      rawvalx=-255; 
      rawvaly=0;
    }
    if(ps5.R1()){
      rawvalx=255;
      rawvaly=0;
    }
    if(ps5.Down()){
      rawvalx=0;
      rawvaly=-255;
    }
    if(ps5.Up()){
      rawvalx=0;
      rawvaly=255;
    }
  


  //Only needed to print the message properly on serial monitor. Else we dont need it.
  if (millis() - lastTimeStamp > 20)
  {
    differential(rawvalx,rawvaly,output);
    output[0]=output[0]>255?0:output[0];
    out(output[0],leftwheel);
    out(output[1],rightwheel);
    Serial.print(output[0]);
    Serial.print(",");
    Serial.println(output[1]);
  if(ps5.Square()==1 && shoot_flag==0)
  { Serial.println("square pressed");
    digitalWrite(shooter[1],1);
    ledcWrite(2,255);
    delay(60);
    ledcWrite(2,0);
    digitalWrite(shooter[1],0);
    ledcWrite(2,100);
    delay(148);
    ledcWrite(2,0);
    Serial.println("shot");
    shoot_flag=1;
  }
  else{
    shoot_flag=0;
  }
  if(ps5.Circle())
  {
    digitalWrite(shooter[1],0);
    ledcWrite(2,100);
    delay(10);
    ledcWrite(2,0);

  }
    lastTimeStamp = millis();

  }


}

void onConnect()
{
  Serial.println("Connected!.");
}

void onDisConnect()
{
  Serial.println("Disconnected!.");    
}

const int freq = 5000;
const int resolution = 8;

void setup() 
{
  Serial.begin(115200);
  ledcSetup(0, freq, resolution);
  ledcSetup(1, freq, resolution);
  ledcSetup(2, freq, resolution);
  ledcAttachPin(12, 0);
  pinMode(14,OUTPUT);
  ledcAttachPin(27, 1);
  pinMode(26,OUTPUT);
  ledcAttachPin(23, 2);
  pinMode(22,OUTPUT);
  
  ps5.attachOnConnect(onConnect);
  ps5.attachOnDisconnect(onDisConnect);
  ps5.begin("7c:66:ef:77:8a:4f");
  ps5.attach(notify);
  Serial.println("Ready.");
}

void loop() 
{

}