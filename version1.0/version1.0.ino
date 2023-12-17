//以下程序实现一款宠物自动投喂机，能检测所剩事务重量并自动投喂，辨别猫/狗，远程监控等，可以通过看门狗硬件定时实现
#include <Servo.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h> // 包含看门狗定时器库
// 定义传感器引脚
const int sensorPin1 = A0; // 使用A0模拟引脚
const int sensorPin2 = A1; // 使用A0模拟引脚
const int irSensorPin = 5; // 红外传感器连接的数字引脚
int irSensorValue = 0;    // 红外传感器的值
//指示灯引脚设置
int yellowPin = 2; // 引脚作为黄灯控制引脚
int redPin = 3;//红灯引脚
int greenPin = 4;//绿灯引脚
int pos = 0;    // 舵机旋转角度
Servo catservo;  // create servo object to control a servo
Servo dogservo;
unsigned long previousMillis = 0;  // 上一次执行任务的毫秒时间
unsigned long last_prev=0;//上一次舵机转动时间
const unsigned long interval = 1000;  // 定时执行任务的时间间隔（1秒）
const unsigned long last_interval=10000;//每次舵机转动定时十秒
const unsigned long targetInterval2 = 100;  // 目标定时中断的时间间隔（毫秒）
volatile bool weightflag = false;  // 食槽的重量
const unsigned long timing_interval = 6UL * 60UL * 60UL * 1000UL; // 6小时的间隔，以毫秒为单位
unsigned long timing_previousMillis = 0;
// 定义两个模式的标志
bool mode1 = true;
bool mode2 = false;



void setup() {
  catservo.attach(9);  // attaches the servo on pin 9 to the servo object 引脚9作为舵机控制引脚
  dogservo.attach(10);//引脚10作为舵机控制引脚
  // 初始化串口通信
  Serial.begin(9600);
   // 设置红外传感器引脚为输入
  pinMode(irSensorPin, INPUT);
}



void loop() {
  // 读取红外传感器的值
  irSensorValue = digitalRead(irSensorPin);

  // 如果检测到红外信号（根据传感器的输出值）
  if (irSensorValue == HIGH) {
    toggleMode(); // 切换模式
  }
  //储物仓重量指示灯
   int  sensorValue1= analogRead(sensorPin1);
  if(sensorValue1>=650){
 digitalWrite(redPin, HIGH);
 digitalWrite(yellowPin, LOW);
 digitalWrite(greenPin, LOW);
  delay(100);  
  }
  else if(sensorValue1>=300&&sensorValue1<=650){
   digitalWrite(redPin, LOW);
   digitalWrite(yellowPin, HIGH);
   digitalWrite(greenPin, LOW);
    delay(100);  
  }
  else {
     digitalWrite(redPin, LOW);
   digitalWrite(yellowPin, LOW);
   digitalWrite(greenPin, HIGH);
  }
  if (mode1) {
  //检测食槽重量判断是否要添加
  int  sensorValue2= analogRead(sensorPin2);
  if(sensorValue2>=650){
 weightflag = true;
  delay(100);  
  }
  else{
   weightflag = false;
   delay(100);
  }
 unsigned long currentMillis = millis();
  unsigned long last_current;


  // 检查定时执行任务的时间间隔是否已过去
  if (currentMillis - previousMillis >= interval) {
   previousMillis = currentMillis;  // 更新上一次执行任务的时间

    // 在这里执行定时任务
    if (Serial.available() > 0) { // 检查是否有可用的字符数据
    char receivedChar = Serial.read(); // 读取接收到的字符
 
    // 判断接收到的字符并执行相应操作 
    if ( receivedChar == 'a'&&weightflag) {
      // 在此处执行与字符 'o' 或 'a' 相关的操作
       catservo.write(180);
       delay(1000);
       last_current=millis();
       last_prev=millis();
       while(last_current-last_prev<=last_interval){
        //清除得到的串口数据
        last_current=millis();
        
   Serial.flush();    //清空串口缓存
    }
    catservo.write(0);
    delay(1000);
  
    }
    else if(receivedChar == 'o' &&weightflag){
      // 在此处执行与字符 'o' 或 'a' 相关的操作
       dogservo.write(180);
       delay(1000);
       last_current=millis();
       last_prev=millis();
       while(last_current-last_prev<=last_interval){
        //清除得到的串口数据
        last_current=millis();
        
   Serial.flush();    //清空串口缓存
    }
    dogservo.write(0);
    delay(1000);
  
    }
   
  
  }
  }
  else if (mode2) {
    // 模式2下的任务
   unsigned long timing_currentMillis = millis();
   if (timing_currentMillis - timing_previousMillis >= timing_interval) {
    // 当时间间隔达到6小时时执行以下代码
    Serial.println("6小时定时任务执行中...");
    dogservo.write(180);
    catservo.write(180);
       delay(10000);
    dogservo.write(0);
    catservo.write(0);
    // 在这里执行您的任务
    
    // 更新上一次执行的时间
    timing_previousMillis = timing_currentMillis;
  }
  }
  // 在这里可以添加其他需要执行的代码
}
  
}
void toggleMode() {
  // 切换模式
  mode1 = !mode1;
  mode2 = !mode2;
  Serial.print("切换到模式 ");
  if (mode1) {
    Serial.println("1");
  } else {
    Serial.println("2");
  }

  // 延迟一段时间，以防止重复触发
  delay(1000); // 根据需要调整延迟时间
}

