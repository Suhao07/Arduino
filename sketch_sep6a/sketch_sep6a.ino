// 定义传感器引脚
const int sensorPin = A0; // 使用A0模拟引脚

void setup() {
  // 初始化串口通信
  Serial.begin(9600);
}

void loop() {
  // 读取传感器值
  int sensorValue = analogRead(sensorPin);

  // 打印传感器值到串口监视器
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);

  // 可以在这里进行进一步的处理或根据传感器值执行其他操作

  // 等待一段时间，避免过多的串口输出
  delay(1000); // 1秒延迟      
}
