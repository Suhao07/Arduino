int ledPin = 13; // 例如，将引脚13用作示例
void setup()
{
	Serial.begin(9600);    //初始化与电脑通信的串口
   // 将引脚设置为输出模式
  pinMode(ledPin, OUTPUT);
}

void loop()
{
   if (Serial.available() > 0) { // 检查是否有可用的字符数据
    char receivedChar = Serial.read(); // 读取接收到的字符

    // 判断接收到的字符并执行相应操作
    if (receivedChar == 'o' || receivedChar == 'a') {
      Serial.println("Received 'o' or 'a'");
      // 在此处执行与字符 'o' 或 'a' 相关的操作
      digitalWrite(ledPin, HIGH);
  delay(1000); // 等待1秒钟   
    }
   }
    // 将引脚设置为低电平（0V）
  digitalWrite(ledPin, LOW);
  delay(1000); // 等待1秒钟

  }
	
