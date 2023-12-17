/*

 【Arduino】168种传感器模块系列实验（资料代码+仿真编程+图形编程）

  项目：简单控制舵机MG966R

*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
int ledPin = 13; // 例如，将引脚13用作示例
void setup() {

  myservo.attach(9);  // (pin, min, max)
pinMode(ledPin, OUTPUT);
}

void loop() {

  myservo.write(0);  // tell servo to go to a particular angle
digitalWrite(ledPin, LOW);
  delay(5000);
digitalWrite(ledPin, HIGH);
  myservo.write(90);

  delay(5000);

  myservo.write(0);
digitalWrite(ledPin, LOW);
  delay(5000);

  myservo.write(180);
digitalWrite(ledPin, HIGH);
  delay(5000);

}



