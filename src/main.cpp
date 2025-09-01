
#include <Arduino.h>
#include "lbm_api.h"
#include "lbm_config.h"  // Include for LoRaWAN credential macros

// Define credential arrays using macros from lbm_config.h
static const uint8_t dev_eui[8]  = USER_LORAWAN_DEVICE_EUI;
static const uint8_t join_eui[8] = USER_LORAWAN_JOIN_EUI;
static const uint8_t app_key[16] = USER_LORAWAN_APP_KEY;
static const uint8_t nwk_key[16] = USER_LORAWAN_APP_KEY;  // Use APP_KEY as NWK_KEY for LoRaWAN 1.1

// Timer variables
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 10000; // 10 seconds in milliseconds
bool networkJoined = false;
uint32_t packetCounter = 0;

// User event callback function
void myEventCallback(smtc_modem_event_t* event) {
    Serial.printf("User callback - Event type: %d\n", event->event_type);
    
    switch(event->event_type) {
        case SMTC_MODEM_EVENT_RESET:
            Serial.println("Event: RESET");
            break;
        case SMTC_MODEM_EVENT_JOINED:
            Serial.println("Event: JOINED - Network joined successfully!");
            networkJoined = true;
            lastSendTime = millis(); // Start timer after join
            break;
        case SMTC_MODEM_EVENT_TXDONE:
            Serial.println("Event: TXDONE - Transmission completed");
            break;
        case SMTC_MODEM_EVENT_DOWNDATA:
            Serial.println("Event: DOWNDATA - Downlink data received");
            
            // Get downlink data details using API
            {
                uint8_t rx_payload[255] = {0};
                uint8_t rx_payload_size = 0;
                smtc_modem_dl_metadata_t rx_metadata = {0};
                uint8_t rx_remaining = 0;
                
                // Get the downlink data using new API
                if (lbm.lorawan.getDownlinkData(rx_payload, &rx_payload_size, &rx_metadata, &rx_remaining)) {
                    Serial.printf("RSSI: %d dBm, SNR: %d dB\n", rx_metadata.rssi, rx_metadata.snr);
                    
                    // Print payload in hex format
                    Serial.print("HEX: ");
                    for (int i = 0; i < rx_payload_size; i++) {
                        Serial.printf("%02X ", rx_payload[i]);
                    }
                    Serial.println();
                } else {
                    Serial.println("Failed to get downlink data");
                }
            }
            break;
        case SMTC_MODEM_EVENT_JOINFAIL:
            Serial.println("Event: JOINFAIL - Join failed");
            networkJoined = false;
            break;
        default:
            Serial.printf("Event: Unknown event type %d\n", event->event_type);
            break;
    }
}

// Function to send periodic data
void sendPeriodicData() {
    if (!networkJoined) {
        Serial.println("Network not joined, skipping data send");
        return;
    }
    
    // Create payload with packet counter
    uint8_t payload[8];
    payload[0] = 'R';
    payload[1] = 'A';
    payload[2] = 'K';
    payload[3] = ' ';
    payload[4] = (packetCounter >> 24) & 0xFF;
    payload[5] = (packetCounter >> 16) & 0xFF;
    payload[6] = (packetCounter >> 8) & 0xFF;
    payload[7] = packetCounter & 0xFF;
    
    // Send data on port 2
    if (lbm.lorawan.send(payload, sizeof(payload), 2, false)) {
        Serial.printf("Sent packet #%d: Hi! %d\n", packetCounter, packetCounter);
        packetCounter++;
    } else {
        Serial.println("Failed to send packet");
    }
}

void setup() {
    // Initialize serial port with 115200 baud rate
    Serial.begin(115200);
    while (!Serial) {
        delay(10);
    }
    Serial.println("RAK3112 LoRaWAN Started...");

    // Register user event callback
    lbm.setEventCallback(myEventCallback);
    
    lbm.init();
    lbm.lorawan.setRegion(REGION_EU868); // Set to EU868 region
    
    // Use credential arrays defined from lbm_config.h macros
    lbm.lorawan.setDevEUI(dev_eui);
    lbm.lorawan.setJoinEUI(join_eui);
    lbm.lorawan.setAppKey(app_key);
    lbm.lorawan.setNwkKey(nwk_key);  // Use APP_KEY as NWK_KEY for LoRaWAN 1.1
    
    lbm.lorawan.join();
}

void loop() {
    // Run the LoRaWAN modem engine - this must be called regularly
    lbm.runEngine();
    
    // Check if it's time to send data (10 second timer)
    if (networkJoined && (millis() - lastSendTime >= sendInterval)) {
        sendPeriodicData();
        lastSendTime = millis(); // Reset timer
    }

    // Small delay to prevent overwhelming the system
    delay(10);
}