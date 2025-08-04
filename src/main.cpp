#include <Arduino.h>

void setup() {
    // 初始化串口，波特率115200
    Serial.begin(115200);
    
    // 等待串口连接（可选）
    while (!Serial) {
        delay(10);
    }
    
    Serial.println("RAK3112 Loop Print Started...");
}

void loop() {
    static unsigned long counter = 0;
    
    // 打印计数器和时间戳
    Serial.print("Loop Count: ");
    Serial.print(counter);
    Serial.print(" | Runtime: ");
    Serial.print(millis());
    Serial.println(" ms");
    
    counter++;
    
    // 延时1秒
    delay(1000);
}