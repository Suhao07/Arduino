#include <Servo.h>

Servo myservo;           // 创建一个舵机对象
int ledPin = 13; // 引脚13作为指示灯控制引脚
const int sensorPin = A0; // 使用A0模拟引脚
unsigned long previousMillis = 0;  // 上一次执行任务的毫秒时间
const unsigned long interval = 1000;  // 定时执行任务的时间间隔（1秒）

volatile bool clearSerialData = false; // 标志用于清除串口数据

void setup() {
  Serial.begin(9600);  // 初始化串口通信
  myservo.attach(9);   // 将舵机连接到数字引脚9

  // 设置定时器中断
  cli();  // 关闭全局中断
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 15624; // 设置比较寄存器值，用于生成1秒的定时
  TCCR1B |= (1 << WGM12); // 启用CTC模式
  TCCR1B |= (1 << CS12) | (1 << CS10); // 设置预分频为1024
  TIMSK1 |= (1 << OCIE1A); // 启用比较匹配中断
  sei(); // 启用全局中断
}

void loop() {
  unsigned long currentMillis = millis();

  // 检查定时执行任务的时间间隔是否已过去
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // 更新上一次执行任务的时间

    // 在这里执行定时任务
    if (clearSerialData) {
      clearSerialData = false;
      while (Serial.available() > 0) {
        char receivedChar = Serial.read(); // 读取接收到的字符并清除
      }
    }

    // 其余的定时任务代码
    // ...

    // 最后清除串口数据
    clearSerialData = true;
  }

  // 在这里可以添加其他需要执行的代码
}

// 定时中断函数
ISR(TIMER1_COMPA_vect) {
  // 在定时中断函数中可以执行一些需要定时的操作
  // 例如，每隔1秒执行一次
  // ...
}
