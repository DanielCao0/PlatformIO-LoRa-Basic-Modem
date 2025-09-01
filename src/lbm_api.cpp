#include "lbm_api.h"
#include "lbm_core.h"
#include <Arduino.h>

// Include necessary modem headers
extern "C" {
#include "smtc_modem_api.h"
#include "smtc_modem_utilities.h"
#include "smtc_hal_dbg_trace.h"
}

// Debug print switch
#define BASIC_MODEM_DEBUG 1

#if BASIC_MODEM_DEBUG
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINTF(fmt, ...) Serial.printf(fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINTF(fmt, ...)
#endif

// Global user event callback
LBMEventCallback userEventCallback = nullptr;

// Task handle for LoRaWAN task
TaskHandle_t loraTaskHandle = NULL;

// LoRaWAN task function
void loraWANTask(void *parameter) {
    DEBUG_PRINTLN("LoRaWAN Task Started...");
    
    // Call LoRaWAN main function
    main_periodical_uplink();
    
    // Delete task if function returns
    DEBUG_PRINTLN("LoRaWAN Task Finished.");
    vTaskDelete(NULL);
}

LBMApi::LBMApi() {}
LBMApi::~LBMApi() {}

bool LBMApi::init() {
    DEBUG_PRINTLN("Initializing Basic Modem...");
    lbm_init();
    return true;
}

void LBMApi::runEngine() {
    smtc_modem_run_engine();
}

void LBMApi::setEventCallback(LBMEventCallback callback) {
    userEventCallback = callback;
    DEBUG_PRINTF("User event callback set: %s\n", callback ? "registered" : "cleared");
}

// LoRaWAN class implementations
bool LoRaWANClass::setDevEUI(const uint8_t* dev_eui) {
    smtc_modem_return_code_t ret = smtc_modem_set_deveui(0, dev_eui);
    DEBUG_PRINTF("Set DevEUI result: %d\n", ret);
    return (ret == SMTC_MODEM_RC_OK);
}

bool LoRaWANClass::setJoinEUI(const uint8_t* join_eui) {
    smtc_modem_return_code_t ret = smtc_modem_set_joineui(0, join_eui);
    DEBUG_PRINTF("Set JoinEUI result: %d\n", ret);
    return (ret == SMTC_MODEM_RC_OK);
}

bool LoRaWANClass::setAppKey(const uint8_t* app_key) {
    smtc_modem_return_code_t ret = smtc_modem_set_appkey(0, app_key);
    DEBUG_PRINTF("Set AppKey result: %d\n", ret);
    return (ret == SMTC_MODEM_RC_OK);
}

bool LoRaWANClass::setNwkKey(const uint8_t* nwk_key) {
    smtc_modem_return_code_t ret = smtc_modem_set_nwkkey(0, nwk_key);
    DEBUG_PRINTF("Set NwkKey result: %d\n", ret);
    return (ret == SMTC_MODEM_RC_OK);
}

bool LoRaWANClass::setRegion(smtc_modem_region_t region) {
    smtc_modem_return_code_t ret = smtc_modem_set_region(0, region);
    DEBUG_PRINTF("Set Region result: %d\n", ret);
    return (ret == SMTC_MODEM_RC_OK);
}

bool LoRaWANClass::join() {
    smtc_modem_return_code_t ret = smtc_modem_join_network(0);
    DEBUG_PRINTF("Join network result: %d\n", ret);
    return (ret == SMTC_MODEM_RC_OK);
}

bool LoRaWANClass::send(const uint8_t* data, size_t len, uint8_t port, bool confirmed) {
    smtc_modem_return_code_t ret = smtc_modem_request_uplink(0, port, confirmed, data, len);
    DEBUG_PRINTF("Send uplink: port=%d, len=%d, confirmed=%s, result=%d\n", 
                 port, len, confirmed ? "true" : "false", ret);
    return (ret == SMTC_MODEM_RC_OK);
}

bool LoRaWANClass::isJoined() const {
    smtc_modem_status_mask_t status_mask;
    if (smtc_modem_get_status(0, &status_mask) == SMTC_MODEM_RC_OK) {
        // Check if JOINED bit is set in status mask
        return ((status_mask & SMTC_MODEM_STATUS_JOINED) != 0);
    }
    DEBUG_PRINTLN("Failed to get modem status");
    return false;
}

bool LoRaWANClass::getDownlinkData(uint8_t* payload, uint8_t* payload_size, smtc_modem_dl_metadata_t* metadata, uint8_t* remaining) {
    smtc_modem_return_code_t ret = smtc_modem_get_downlink_data(payload, payload_size, metadata, remaining);
    DEBUG_PRINTF("Get downlink data result: %d\n", ret);
    return (ret == SMTC_MODEM_RC_OK);
}

// Global instance
LBMApi lbm;
