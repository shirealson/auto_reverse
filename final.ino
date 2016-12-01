  
int period=5;
//float t1,t2,t3;


//--------------------红外
#include<IRremote.h>
#define IR_PIN 10
decode_results results;

IRrecv irrecv(IR_PIN);
#define irhead 33464415
#define irback 33478695
#define irleft 33480735
#define irright 33460335


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
const int TrigPin3 = 12; 
const int EchoPin3 = 13; 
float distance3; 
//速度0-255
int fadeValue=150;
int i;

//---------------------------------------------------------------
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
//-----------------------------------------测距
 float  Distance_test1()
{
  // 产生一个10us的高脉冲去触发TrigPin 
        digitalWrite(TrigPin1, LOW); 
        delayMicroseconds(2); 
        digitalWrite(TrigPin1, HIGH); 
        delayMicroseconds(10);
        digitalWrite(TrigPin1, LOW); 
    // 检测脉冲宽度，并计算出距离
        distance1 = pulseIn(EchoPin1, HIGH) / 58.00;  
        return (distance1);
}
float  Distance_test2()
{
        digitalWrite(TrigPin2, LOW); 
        delayMicroseconds(2); 
        digitalWrite(TrigPin2, HIGH); 
        delayMicroseconds(10);
        digitalWrite(TrigPin2, LOW); 
        distance2 = pulseIn(EchoPin2, HIGH) / 58.00;  
        return (distance2);
}
float  Distance_test3()
{
        digitalWrite(TrigPin3, LOW); 
        delayMicroseconds(2); 
        digitalWrite(TrigPin3, HIGH); 
        delayMicroseconds(10);
        digitalWrite(TrigPin3, LOW); 
        distance3 = pulseIn(EchoPin3, HIGH) / 58.00;  
        return (distance3);
}

 

  
  
  
  
  
  
  
  
 void period1()//---------------调整姿态
{
 Serial.println("auto drive");
  float t2,t3;
    t2=Distance_test2();
    t3=Distance_test3();
    while((fabs(t2-t3))>0.5)//----------跳出条件
    {
      Serial.print("t2=");Serial.print(t2);
      Serial.print("t3=");Serial.print(t3);
      Serial.println("    ");
      if (t2>t3&&(t2-t3)>10)//----------差距大用大调整
      { 
        right();
        delay(50);
        STOP();
        delay(250);
        t2=Distance_test2();
        t3=Distance_test3();
        Serial.println("right");
      }
      if (t2<t3&&fabs(t2-t3)>10)  
      {
        left();
        delay(50);
          STOP();
        delay(250);
        t2=Distance_test2();
    t3=Distance_test3();
    Serial.println("LEFT");
      }
       if (t2>t3&&fabs(t2-t3)<=10)//----------差距小用小调整
      { 
        right();
        delay(30);
        STOP();
        delay(250);
        t2=Distance_test2();
    t3=Distance_test3();
    Serial.println("right");
      }
      if (t2<t3&&fabs(t2-t3)<=10)  
      {
        left();
        delay(30);
          STOP();
        delay(250);
        t2=Distance_test2();
    t3=Distance_test3();
    Serial.println("LEFT");
      }
     //  Serial.print(t2-t3); //---------串口监视器查看
      //  Serial.print("cm"); 
     //   Serial.println(); 
    
    }
    t2=Distance_test2();
    t3=Distance_test3();
    
 Serial.print("t2=");Serial.print(t2);
 Serial.print("t3=");Serial.print(t3);
 Serial.println("    ");
 period=2;
}

  
  
  void period2()//前进
{
 float t2,t3;
    t2=Distance_test2();
    t3=Distance_test3();
 
  head();
  //delay(100);
while(t2<20 && t3<20)
{
  Serial.println("-----------------------------------------");
  Serial.print("x=");Serial.println(t2);
  Serial.print("y=");Serial.println(t3);
  Serial.println("-----------------------------------------");
    t2=Distance_test2();
    t3=Distance_test3();
  
  }
  delay(300);//让车尾通过
  STOP();
  
  
Serial.println("Period2 COMPLETE");
 //STOP();
 period=3;
} 


void period3()//转弯
{
  digitalWrite(OUT1A, HIGH);
  digitalWrite(OUT1B, LOW);
  digitalWrite(OUT2A, HIGH);
  digitalWrite(OUT2B, LOW);
  delay(600);//------------------此处需要实验 原410
  STOP();
  period=4;
  Serial.println("Period3 COMPLETE");
  }


void period4()//----倒车
{
  float t1;
  t1=Distance_test1();
   back();
  while(t1>5)
  {
    
    t1=Distance_test1();
    }
   
    //delay(50);
    STOP();
    
  Serial.println("COMPLETE");
  period=5;
  
  }

  void period5()//红外遥控
  {
      
      if (irrecv.decode(&results)){
    Serial.println(results.value,DEC);
  switch(results.value)
  {
  
  case irhead:
  head();
  Serial.println("HEAD");
  delay(300);
  STOP();
  break;

   case irback:
  back();
  Serial.println("BACK");
  delay(300);
  STOP();
  break;

 case irleft:
  left();
  Serial.println("LEFT");
  delay(300);
  STOP();
  break;

   case irright:
  right();
  Serial.println("RIGHT");
  delay(300);
  STOP();
  break;

  case 33441975:
  period=1;
break;
  }
  
  irrecv.resume();
  }
    }

    

void setup()
{
  pinMode(OUT1A, OUTPUT);//驱动
  pinMode(OUT1B, OUTPUT);
  pinMode(OUT2A, OUTPUT);
  pinMode(OUT2B, OUTPUT);
  
   Serial.begin(9600);  // 初始化串口通信及连接SR04的引脚
        pinMode(TrigPin1, OUTPUT);  // 要检测引脚上输入的脉冲宽度，需要先设置为输入状态
        pinMode(EchoPin1, INPUT); 
        pinMode(TrigPin2, OUTPUT); 
        pinMode(EchoPin2, INPUT); 
        pinMode(TrigPin3, OUTPUT); 
        pinMode(EchoPin3, INPUT); 
        irrecv.enableIRIn();//启动遥控器
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
  period2();
  break;

  case 3:
  period3();
  break;

  case 4:
  period4();
  break;

  case 5:
  period5();
  break;
  }
  
  
  
 
  
  
  
  }



  
 


