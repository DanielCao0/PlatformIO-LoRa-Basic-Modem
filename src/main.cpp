#include <Arduino.h>
#include "periodical_uplink.h"

// FreeRTOS task handle
TaskHandle_t loraTaskHandle = NULL;

// LoRaWAN task function
void loraWANTask(void *parameter) {
    Serial.println("LoRaWAN Task Started...");
    
    // Call LoRaWAN main function
    main_periodical_uplink();
    
    // Delete task if function returns
    Serial.println("LoRaWAN Task Finished.");
    vTaskDelete(NULL);
}

void setup() {
    // Initialize serial port with 115200 baud rate
    Serial.begin(115200);
    
    // Wait for serial connection (optional)
    while (!Serial) {
        delay(10);
    }
    
    Serial.println("RAK3112 LoRaWAN Started...");

    // Configure LED pins
    pinMode(PIN_LED1, OUTPUT);
    pinMode(PIN_LED2, OUTPUT);
    digitalWrite(PIN_LED1, HIGH);
    digitalWrite(PIN_LED2, HIGH);

    //Create LoRaWAN task on CPU 1 to avoid watchdog issues
    xTaskCreatePinnedToCore(
        loraWANTask,          // Task function
        "LoRaWAN Task",       // Task name
        64 * 1024,            // Stack size
        NULL,                 // Task parameters
        1,                    // Task priority (same as loop task)
        &loraTaskHandle,      // Task handle
        1                     // CPU core (1 = CPU 1, 0 = CPU 0)
    );
    
    Serial.println("LoRaWAN Task Created");
}

void loop() { 
    delay(100);
}