#include <Servo.h>

Servo myservo;           // 创建一个舵机对象
int ledPin = 13; // 引脚13作为指示灯控制引脚
const int sensorPin = A0; // 使用A0模拟引脚
unsigned long previousMillis = 0;  // 上一次执行任务的毫秒时间
const unsigned long interval = 1000;  // 定时执行任务的时间间隔（1秒）
const unsigned long targetInterval = 1000;  // 目标定时中断的时间间隔（毫秒）
const unsigned long targetInterval2 = 100;  // 目标定时中断的时间间隔（毫秒）
volatile bool clearSerialData = false; // 标志用于清除串口数据
volatile bool timerEnabled2 = false;  // 定时器2开关状态标志
volatile bool timerEnabled0 = false;  // 定时器0开关状态标志
// 计算所需的预分频和比较寄存器值
  unsigned long timerPrescaler = 64;  // 设置预分频为64
  unsigned long timerCountsPerInterval = (F_CPU / timerPrescaler) * (targetInterval / 1000UL);
  unsigned long timerPrescaler2 = 64;  // 设置预分频为64
  unsigned long timerCountsPerInterval2 = (F_CPU / timerPrescaler2) * (targetInterval2 / 1000UL);
void setup() {
  Serial.begin(9600);  // 初始化串口通信
  myservo.attach(9);   // 将舵机连接到数字引脚9
  
  
  
  // 配置定时器2为CTC（比较匹配模式）
  TCCR2A = 0;  // 清除控制寄存器A
  TCCR2B = 0;  // 清除控制寄存器B
  TCNT2 = 0;   // 清零计数器
  OCR2A = timerCountsPerInterval - 1;  // 设置比较寄存器值
  TCCR2A |= (1 << WGM21);  // 启用CTC模式
  TCCR2B |= (1 << CS22);  // 设置预分频为64
  TIMSK2 |= (1 << OCIE2A);  // 启用比较匹配中断
  // 配置定时器0为CTC（比较匹配模式）
  TCCR0A = 0;  // 清除控制寄存器A
  TCCR0B = 0;  // 清除控制寄存器B
  TCNT0 = 0;   // 清零计数器
  OCR0A = timerCountsPerInterval2- 1;  // 设置比较寄存器值
  TCCR0A |= (1 << WGM01);  // 启用CTC模式
  TCCR0B |= (1 << CS02);  // 设置预分频为64
  TIMSK0 |= (1 << OCIE0A);  // 启用比较匹配中断
  sei();  // 启用全局中断
}

void loop() {
  unsigned long currentMillis = millis();
startTimer0Interrupt();
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
       
   
    }
    startTimer2Interrupt();
    while(timerEnabled2 == true){
if (Serial.available() > 0) { // 检查是否有可用的字符数据
    char receivedChar = Serial.read(); // 读取接收到的字符
    }
   }
    //delay(1000);
  
    }
  
  }
  myservo.write(0);
  delay(100);
  // 在这里可以添加其他需要执行的代码
}
// 定时中断函数2
ISR(TIMER2_COMPA_vect) {
  // 在定时中断函数中可以执行一些需要定时的操作
  // 例如，每隔1秒执行一次

  // 注意：不要在这里执行与Servo库相关的操作
   if (timerEnabled2) {
    stopTimer2Interrupt();
    myservo.write(0);
    TCCR2A = 0;  // 清除控制寄存器A
  TCCR2B = 0;  // 清除控制寄存器B
  TCNT2 = 0;   // 清零计数器
  OCR2A = timerCountsPerInterval - 1;  // 设置比较寄存器值
  TCCR2A |= (1 << WGM21);  // 启用CTC模式
  TCCR2B |= (1 << CS22);  // 设置预分频为64
  TIMSK2 |= (1 << OCIE2A);  // 启用比较匹配中断
  }
}
// 启动定时中断2
void startTimer2Interrupt() {
  timerEnabled2 = true;
  TIMSK2 |= (1 << OCIE2A);  // 启用比较匹配中断
}

// 停止定时中断2
void stopTimer2Interrupt() {
  timerEnabled2 = false;
  TIMSK2 &= ~(1 << OCIE2A);  // 禁用比较匹配中断
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
  // 打印传感器值到串口监视器
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);
  // 可以在这里进行进一步的处理或根据传感器值执行其他操作
  // 等待一段时间，避免过多的串口输出
   // 配置定时器0为CTC（比较匹配模式）
  TCCR0A = 0;  // 清除控制寄存器A
  TCCR0B = 0;  // 清除控制寄存器B
  TCNT0 = 0;   // 清零计数器
  OCR0A = timerCountsPerInterval2- 1;  // 设置比较寄存器值
  TCCR0A |= (1 << WGM01);  // 启用CTC模式
  TCCR0B |= (1 << CS02);  // 设置预分频为64
  TIMSK0 |= (1 << OCIE0A);  // 启用比较匹配中断
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

