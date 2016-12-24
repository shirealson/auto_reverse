int period=5;
bool state=1;//中断
//--------------------蓝牙
String cmd="";
//float t1,t2,t3;//用于超声波测距
//------------------声控
#include<SoftwareSerial.h>
SoftwareSerial myserial(12,13);
int val;
//----------------舵机
#include <Servo.h>
Servo myservo;  
#define servo_pin SDA
float pos = 0;    // variable to store the servo position
//-------------------红外
#include<IRremote.h>
#define IR_PIN 10
decode_results results;
IRrecv irrecv(IR_PIN);
#define irhead 33464415
#define irback 33478695
#define irleft 33480735
#define irright 33460335
#define dianyuan 33441975
#define ir_auto_reverse 33441975
String break_value = "break";//避障中断
//-------------------------------电机部分
//D6，D8控制1A，1B的电机
#define OUT1A 6
#define OUT1B 8
//D5，D7控制2A，2B的电机
#define OUT2A 5
#define OUT2B 7
// 设定SR04连接的Arduino引脚（超声波）
const int TrigPin1 = 2; 
const int EchoPin1 = 3; 
float distance1; 
const int TrigPin2 = A2; 
const int EchoPin2 = A3; 
float distance2; 
const int TrigPin3 = A0; 
const int EchoPin3 = A1; 
float distance3; 


//-------------------------------------------跑马灯
#include <Adafruit_NeoPixel.h>
#define PIN A6 
#define PIN_NUM 2 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIN_NUM,PIN, NEO_GRB + NEO_KHZ800); 


//---------------------------------------------------中断
void change()
{
   period=5;
}

//-----------------------灯
void liangdeng(int r,int g,int b,int mark)
{
    if(mark==0)
     {   strip.setPixelColor(0, strip.Color(r,g,b));
          strip.show(); 
     }
     if(mark==1)
      { 
        strip.setPixelColor(1,strip.Color(r,g,b));
       strip.show(); 
        }
      if(mark==2)
        {
           strip.setPixelColor(0, strip.Color(r,g,b));
          strip.show();  
              strip.setPixelColor(1, strip.Color(r,g,b));
          strip.show();  
        }
          }  

//-----------------------------------方向控制
void left()
{
  digitalWrite(OUT1A, HIGH);
  digitalWrite(OUT1B, LOW);
  digitalWrite(OUT2A, HIGH);
  digitalWrite(OUT2B, LOW);
}
void right()
{
  digitalWrite(OUT1A, LOW);
  digitalWrite(OUT1B, HIGH);
  digitalWrite(OUT2A, LOW);
  digitalWrite(OUT2B, HIGH);
}

void back()
{
 digitalWrite(OUT1A,HIGH);
 digitalWrite(OUT1B,LOW);
 digitalWrite(OUT2A,LOW);
 digitalWrite(OUT2B,HIGH);

  }

void head()
{
 digitalWrite(OUT1A,LOW);
 digitalWrite(OUT1B,HIGH);
 digitalWrite(OUT2A,HIGH);
 digitalWrite(OUT2B,LOW);


  }

void STOP()
{
 digitalWrite(OUT1A,LOW);
 digitalWrite(OUT1B,LOW);
 digitalWrite(OUT2A,LOW);
 digitalWrite(OUT2B,LOW);


  }
  void turnleft90()
  {
  left();
  delay(600);//------------------此处需要实验 原410
  STOP(); 
    }
    void turnright90()
    {
  right();
  delay(600);//------------------此处需要实验 原410
  STOP(); 
      }
    
//-----------------------------------------测距
 float  Distance_test1()//主
{
        digitalWrite(TrigPin1, LOW); 
        delayMicroseconds(2); 
        digitalWrite(TrigPin1, HIGH); 
        delayMicroseconds(10);
        digitalWrite(TrigPin1, LOW); 
        distance1 = pulseIn(EchoPin1, HIGH) / 58.00;  
        return (distance1);
}

float  Distance_test2()//侧
{
        digitalWrite(TrigPin2, LOW); 
        delayMicroseconds(2); 
        digitalWrite(TrigPin2, HIGH); 
        delayMicroseconds(10);
        digitalWrite(TrigPin2, LOW); 
        distance2 = pulseIn(EchoPin2, HIGH) / 58.00;  
        return (distance2+5.98);//5.98为补偿值
}
float  Distance_test3()//后
{
        digitalWrite(TrigPin3, LOW); 
        delayMicroseconds(2); 
        digitalWrite(TrigPin3, HIGH); 
        delayMicroseconds(10);
        digitalWrite(TrigPin3, LOW); 
        distance3 = pulseIn(EchoPin3, HIGH) / 58.00;  
        return (distance3);
}

  void bluetooth()
{
while(Serial1.available()>0)
{
  cmd+= char(Serial1.read());
  delay(2);
}
   if(cmd=="head")
    {
      liangdeng(255,255,255,2);
      head();
      Serial.println("HEAD");
     }
    if(cmd=="back")
     {
      liangdeng(255,215,0,2);
      back();
      Serial.println("BACK");
      }
       if(cmd=="left")
      {
      liangdeng(255,192,203,0);
      left();
      delay(350);
      liangdeng(0,0,0,0);
      STOP();
      Serial.println("LEFT");
      }
      if(cmd=="right")
      {
      liangdeng(255,192,203,1);
      right();
      delay(350);
       liangdeng(0,0,0,0);
       STOP();
      Serial.println("RIGHT");
      }
      if(cmd=="stop")
      {
       STOP(); 
       Serial.println("RIGHT");
        liangdeng(0,0,0,2);
       }
      if(cmd=="daoche")
      {
      period=1;//自动倒车
      }
      if(cmd=="auto drive")
      {
      period=6;//自动驾驶
      }
      cmd="";//执行后清空
      }
      
void period1()//---------------调整姿态
{
   liangdeng(255,165,0,2);
   Serial.println("auto drive");
   float t1,t2;
   myservo.write(0);  
   delay(200);
    t1=Distance_test1();
    t2=Distance_test2();
    while((fabs(t1-t2))>1)//----------跳出条件
    {
      Serial.print("t1=");Serial.print(t1);
      Serial.print("t2=");Serial.print(t2);
      Serial.println("    ");
      if (t1>t2&&(t1-t2)>10)//----------差距大用大调整
      { 
        right();
        delay(80);
        STOP();
        delay(250);
        t1=Distance_test1();
        t2=Distance_test2();
        Serial.println("right");
      }
      if (t1<t2&&fabs(t1-t2)>10)  
      {
        left();
        delay(80);
          STOP();
        delay(250);
        t1=Distance_test1();
        t2=Distance_test2();
        Serial.println("LEFT");
       }
       if (t1>t2&&fabs(t1-t2)<=10)//----------差距小用小调整
      { 
        right();
        delay(30);
        STOP();
        delay(250);
        t1=Distance_test1();
        t2=Distance_test2();
        Serial.println("right");
      }
      if (t1<t2&&fabs(t1-t2)<=10)  
      {
        left();
        delay(30);
          STOP();
        delay(250);
        t1=Distance_test1();
        t2=Distance_test2();
        Serial.println("LEFT");
      }
     //  Serial.print(t1-t2); //---------串口监视器查看 
     //   Serial.println(); 
    
    }
    t1=Distance_test1();
    t2=Distance_test2();
    
 Serial.print("t1=");Serial.print(t1);
 Serial.print("t2=");Serial.print(t2);
 Serial.println("    ");
 liangdeng(0,0,0,2);
 period=2;
}

  void period2()//前进
{
    liangdeng(128,0,128,2);
    float t1,t2;
    t1=Distance_test1();
    t2=Distance_test2();
    head();
  //delay(100);
while(t1<25.98 && t2<25.98)
{
  Serial.println("-----------------------------------------");
  Serial.print("x=");Serial.println(t1);
  Serial.print("y=");Serial.println(t2);
  Serial.println("-----------------------------------------");
    t1=Distance_test1();
    t2=Distance_test2();
    delay(2);
  }
   Serial.print("x=");Serial.println(t1);
  Serial.print("y=");Serial.println(t2);
  delay(500);//让车尾通过
  STOP();
Serial.println("Period2 COMPLETE");
 liangdeng(0,0,0,2);
 period=3;
} 


void period3()//转弯
{
   liangdeng( 255,255,0,2);
   turnleft90();
   STOP();
   delay(400);
   liangdeng(0,0,0,2);
   period=4;
   Serial.println("Period3 COMPLETE");
  }


void period4()//----倒车
{
  liangdeng(255,0,0,2);
  float t3;
  t3=Distance_test3();
   back();
  while(t3>10)
  {
    
    t3=Distance_test3();
    Serial.print("t3 ");
    Serial.println(t3);
    }
   Serial.print("t3 ");
    Serial.println(t3);
    STOP();
    
  Serial.println("COMPLETE4");
  myservo.write(90);
  liangdeng(0,0,0,2);
  period=5;
  
  }

 void period5()//遥控(语音蓝牙红外)
  {
    if(myserial.available())
  {
        val = myserial.read();
        delay(50);
   
        if (val==1)
        {
          liangdeng(255,255,255,2);
          head();
          delay(300);
          STOP();
          liangdeng(0,0,0,2);
          Serial.println(val);
          
        }
         if (val==2)
        {
          liangdeng(255,215,0,2);
          back();
          delay(300);
          STOP();
          liangdeng(0,0,0,2);
          Serial.println(val);
        } 
        if (val==15)
        {
          liangdeng(255,192,203,0);
          left();
          delay(300);
          STOP();
           liangdeng(0,0,0,0);
          Serial.println(val);
        } 
        if (val==6)
        {
          liangdeng(255,192,203,1);
          right();
          delay(300);
          STOP();
          liangdeng(0,0,0,1);
          Serial.println(val);
        } 
        if (val==10)
        {
          STOP();
          Serial.println(val);
        }
        if(val==19)
        {
          period=6;
        }
        }
  
     
      if (irrecv.decode(&results)){
    Serial.println(results.value,DEC);
  switch(results.value)
  {
  
  case irhead:
  liangdeng(255,255,255,2);
  head();
  Serial.println("HEAD");
  delay(300);
  STOP();
  liangdeng(0,0,0,2);
  break;

   case irback:
   liangdeng(255,215,0,2);
  back();
  Serial.println("BACK");
  delay(300);
  STOP();
  liangdeng(0,0,0,2);
  break;

 case irleft:
 liangdeng(255,192,203,0);
  left();
  Serial.println("LEFT");
  delay(300);
  STOP();
  liangdeng(0,0,0,0);
  break;

   case irright:
   liangdeng(255,192,203,1);
  right();
  Serial.println("RIGHT");
  delay(300);
  STOP();
  liangdeng(0,0,0,1);
  break;

  case dianyuan:
  period=1;
  break;

  }
     irrecv.resume(); 
  
  }
  else if(Serial1.available())
   {
     bluetooth();
   }
    }

void period6()
{
  float t1,t2,t3;
  String break_cmd;
  myservo.write(90);//初始化
 //delay(500); 
 t1= Distance_test1();
     
    Serial.print("middleDistance=");
    Serial.println(t1);
   // ---------------------满足条件一直前进
   head();
     while(t1>10)//一直到出现障碍物为止
     {
      t1 = Distance_test1();
      delay(2);
      if (Serial1.available())//蓝牙中断机制
      {
       while(Serial1.available()>0)
         {
          break_cmd+=char(Serial1.read());
          delay(2);
         }
        if (break_cmd==break_value)
        {
          period=5;
          break; 
        }
      }
     }
      STOP();
      //t1 = Distance_test1();
      //------------------------
    if(t1<=10)//前方出现障碍物
    {     
      STOP();
      delay(500);                         
      myservo.write(0);          
      delay(1000);      
      t2 = Distance_test1();
      Serial.print("rightDistance=");
      Serial.println(t2);
      delay(500);
       myservo.write(90);              
      delay(1000);                                                  
      myservo.write(180);              
      delay(1000); 
      t3 = Distance_test1();//检查左右情况
      Serial.print("leftDistance=");
      Serial.println(t3);
      
      delay(500);
      myservo.write(90);              
      delay(1000);
      if(t2>t3&&t2>20)  //右边有路
      {
        turnright90();
        delay(400);//暂停防止打滑
        
       myservo.write(180);
       delay(1000);
       head();
       while(t3 <20)
       {
        t3 = Distance_test1();
        delay(2);
        }
        delay(500);//让车尾通过
        STOP();
        delay(500);
        turnleft90();//进入右侧道路
        delay(500);//防止打滑
        myservo.write(90);
        delay(500);
        head();
        
       }
       else if(t2<t3&&t3>20)//左边有路
       {
         turnleft90();
        delay(400);
       myservo.write(0);
       delay(1000);
       head();
       while(t2 <20)
       {
        t2 = Distance_test1();
        delay(2);
        }
        delay(500);//让车尾通过
        STOP();
        delay(500);
        turnright90();//进入左侧道路
        delay(500);//防止打滑
        myservo.write(90);
        delay(500);
        head();
       }
       else
       {
        back();
        delay(300);//没路
        STOP();
        period=5;//没路切换回手动控制
       }
    }  
// period=5;
  
  }


    

void setup()
{
  pinMode(OUT1A, OUTPUT);//驱动
  pinMode(OUT1B, OUTPUT);
  pinMode(OUT2A, OUTPUT);
  pinMode(OUT2B, OUTPUT);
   Serial1.begin(9600);  // 初始化串口通信及连接SR04的引脚
        pinMode(TrigPin1, OUTPUT);  // 要检测引脚上输入的脉冲宽度，需要先设置为输入状态
        pinMode(EchoPin1, INPUT); 
        pinMode(TrigPin2, OUTPUT); 
        pinMode(EchoPin2, INPUT); 
     //   pinMode(TrigPin3, OUTPUT); 
     //   pinMode(EchoPin3, INPUT); 
        irrecv.enableIRIn();//启动遥控器

         myservo.attach(servo_pin); //舵机

 // attachInterrupt(0,change,RISING);//中断

  myserial.begin(9600);//语音
  myserial.listen();
  myservo.write(90);

  strip.begin();//跑马灯
}

void loop()
{
//-------------------------------停止自动驾驶

  //---------------------------------
switch (period)
{
  case 1:
  period1();
  break;
  
  case 2:
  {
  period2();
  break;
  }

  case 3:
  {
  period3();
  break;
  }

  case 4:
  period4();
  break;

  case 5:
  period5();
  break;

  case 6:
  period6();
  break;
  }
  }
