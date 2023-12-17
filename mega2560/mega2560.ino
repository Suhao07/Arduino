#include <Servo.h>
#include <avr/io.h>
#include <avr/interrupt.h>

Servo myservo;           // 创建一个舵机对象
int ledPin = 13; // 引脚13作为指示灯控制引脚
const int sensorPin = A0; // 使用A0模拟引脚
unsigned long previousMillis = 0;  // 上一次执行任务的毫秒时间
unsigned long last_prev=0;//上一次舵机转动时间
const unsigned long interval = 1000;  // 定时执行任务的时间间隔（1秒）
const unsigned long last_interval=100;//每次舵机转动定时十秒
const unsigned long targetInterval2 = 100;  // 目标定时中断的时间间隔（毫秒）
volatile bool timerEnabled0 = false;  // 定时器0开关状态标志
// 计算所需的预分频和比较寄存器值
  unsigned long timerPrescaler2 = 64;  // 设置预分频为64
  unsigned long timerCountsPerInterval2 = (F_CPU / timerPrescaler2) * (targetInterval2 / 1000UL);

void setup() {
  Serial.begin(9600);  // 初始化串口通信
  myservo.attach(9);   // 将舵机连接到数字引脚9
  
  // 设置定时器0为CTC模式（Clear Timer on Compare Match）
  TCCR0A = (1 << WGM01);

  // 配置预分频因子为1024
  TCCR0B = (1 << CS02) | (1 << CS00);
  // 初始化计数器值，使得1秒钟内定时器溢出
  // 1秒 = (计数值 / (时钟频率 / 预分频因子))
  // 计数值 = 1秒 * (时钟频率 / 预分频因子)
  // 在Arduino Mega 2560上，时钟频率为16 MHz
  // 所以计数值 = 1秒 * (16000000 / 1024) = 15625
  OCR0A = 15625;
  // 启用定时器0的输出比较匹配中断
  TIMSK0 = (1 << OCIE0A);
  // 启用全局中断
  sei();
}
void loop() {
  unsigned long currentMillis = millis();
  unsigned long last_current;
   myservo.write(180);
 delay(1000);
 myservo.write(0);
 delay(1000);
//startTimer0Interrupt();
stopTimer0Interrupt();
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
  
  }
  // 在这里可以添加其他需要执行的代码
}
}
// 定时中断函数0
ISR(TIMER0_COMPA_vect) {
  // 在定时中断函数中可以执行一些需要定时的操作
  // 例如，每隔1秒执行一次

  // 注意：不要在这里执行与Servo库相关的操作
   if (timerEnabled0) {
    // 读取传感器值
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
}

void startTimer0Interrupt() {
  timerEnabled0 = true;
  TIMSK0 |= (1 << OCIE0A);  // 启用比较匹配中断
}
// 停止定时中断2
void stopTimer0Interrupt() {
  timerEnabled0 = false;
  TIMSK0 &= ~(1 << OCIE0A);  // 禁用比较匹配中断
}

