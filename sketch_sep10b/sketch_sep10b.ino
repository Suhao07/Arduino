//以下程序实现检测压力并判断亮灯，接收上位机串口数据，并控制舵机
#include <Servo.h>
#include <avr/io.h>
#include <avr/interrupt.h>
// 定义传感器引脚
const int sensorPin = A0; // 使用A0模拟引脚
int ledPin = 13; // 引脚13作为指示灯控制引脚
int pos = 0;    // 舵机旋转角度
Servo myservo;  // create servo object to control a servo

unsigned long previousMillis = 0;  // 上一次执行任务的毫秒时间
unsigned long last_prev=0;//上一次舵机转动时间
const unsigned long interval = 1000;  // 定时执行任务的时间间隔（1秒）
const unsigned long last_interval=10000;//每次舵机转动定时十秒
const unsigned long targetInterval2 = 100;  // 目标定时中断的时间间隔（毫秒）
void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 引脚9作为舵机控制引脚
  // 初始化串口通信
  Serial.begin(9600);
}

void loop() {
 unsigned long currentMillis = millis();
  unsigned long last_current;
  /* myservo.write(180);
 delay(1000);
 myservo.write(0);
 delay(1000);*/
//startTimer0Interrupt();
//stopTimer0Interrupt();
  // 检查定时执行任务的时间间隔是否已过去

  if (currentMillis - previousMillis >= interval) {
   previousMillis = currentMillis;  // 更新上一次执行任务的时间

    // 在这里执行定时任务
    if (Serial.available() > 0) { // 检查是否有可用的字符数据
    char receivedChar = Serial.read(); // 读取接收到的字符
 
    // 判断接收到的字符并执行相应操作 
    if (receivedChar == 'o' || receivedChar == 'a') {
      Serial.println("Received 'o' or 'a'");
      // 在此处执行与字符 'o' 或 'a' 相关的操作
       myservo.write(180);
       delay(1000);
       last_current=millis();
       last_prev=millis();
       while(last_current-last_prev<=last_interval){
        //清除得到的串口数据
        last_current=millis();
        /*if (Serial.available() > 0) { // 检查是否有可用的字符数据
    char receivedChar = Serial.read(); // 读取接收到的字符
       }*/
   Serial.flush();    //清空串口缓存
    }
    myservo.write(0);
    delay(1000);
  
    }
    int  sensorValue= analogRead(sensorPin);
  if(sensorValue>=650){
 digitalWrite(ledPin, HIGH);
  delay(100);  
  }
  else{
   digitalWrite(ledPin, LOW);
    delay(100);  
  }
  
  }
  // 在这里可以添加其他需要执行的代码
}
  
}

