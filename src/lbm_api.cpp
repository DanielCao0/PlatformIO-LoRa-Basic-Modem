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

smtc_modem_return_code_t LBMApi::init() {
    DEBUG_PRINTLN("Initializing Basic Modem...");
    lbm_init();
    return SMTC_MODEM_RC_OK;
}

void LBMApi::runEngine() {
    smtc_modem_run_engine();
}

void LBMApi::setEventCallback(LBMEventCallback callback) {
    userEventCallback = callback;
    DEBUG_PRINTF("User event callback set: %s\n", callback ? "registered" : "cleared");
}

// LoRaWAN class implementations
smtc_modem_return_code_t LoRaWANClass::setDevEUI(const uint8_t* dev_eui) {
    smtc_modem_return_code_t ret = smtc_modem_set_deveui(0, dev_eui);
    DEBUG_PRINTF("Set DevEUI result: %d\n", ret);
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::setJoinEUI(const uint8_t* join_eui) {
    smtc_modem_return_code_t ret = smtc_modem_set_joineui(0, join_eui);
    DEBUG_PRINTF("Set JoinEUI result: %d\n", ret);
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::setAppKey(const uint8_t* app_key) {
    smtc_modem_return_code_t ret = smtc_modem_set_appkey(0, app_key);
    DEBUG_PRINTF("Set AppKey result: %d\n", ret);
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::setNwkKey(const uint8_t* nwk_key) {
    smtc_modem_return_code_t ret = smtc_modem_set_nwkkey(0, nwk_key);
    DEBUG_PRINTF("Set NwkKey result: %d\n", ret);
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::setRegion(smtc_modem_region_t region) {
    smtc_modem_return_code_t ret = smtc_modem_set_region(0, region);
    DEBUG_PRINTF("Set Region result: %d\n", ret);
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::setClass(smtc_modem_class_t modem_class) {
    smtc_modem_return_code_t ret = smtc_modem_set_class(0, modem_class);
    DEBUG_PRINTF("Set Class result: %d (Class %c)\n", ret, 'A' + modem_class);
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::join() {
    smtc_modem_return_code_t ret = smtc_modem_join_network(0);
    DEBUG_PRINTF("Join network result: %d\n", ret);
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::send(const uint8_t* data, size_t len, uint8_t port, bool confirmed) {
    smtc_modem_return_code_t ret = smtc_modem_request_uplink(0, port, confirmed, data, len);
    DEBUG_PRINTF("Send uplink: port=%d, len=%d, confirmed=%s, result=%d\n", 
                 port, len, confirmed ? "true" : "false", ret);
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::isJoined(bool* joined) const {
    smtc_modem_status_mask_t status_mask;
    smtc_modem_return_code_t ret = smtc_modem_get_status(0, &status_mask);
    if (ret == SMTC_MODEM_RC_OK) {
        if (joined != nullptr) {
            *joined = ((status_mask & SMTC_MODEM_STATUS_JOINED) != 0);
        }
    } else {
        DEBUG_PRINTLN("Failed to get modem status");
        if (joined != nullptr) {
            *joined = false;
        }
    }
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::getDownlinkData(uint8_t* payload, uint8_t* payload_size, smtc_modem_dl_metadata_t* metadata, uint8_t* remaining) {
    smtc_modem_return_code_t ret = smtc_modem_get_downlink_data(payload, payload_size, metadata, remaining);
    DEBUG_PRINTF("Get downlink data result: %d\n", ret);
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::setJoinDataRateDistribution(const uint8_t dr_distribution[SMTC_MODEM_CUSTOM_ADR_DATA_LENGTH]) {
    smtc_modem_return_code_t ret = smtc_modem_adr_set_join_distribution(0, dr_distribution);
    DEBUG_PRINTF("Set Join DR distribution result: %d\n", ret);
#if BASIC_MODEM_DEBUG
    // Print distribution for debugging
    DEBUG_PRINT("DR distribution: [");
    uint16_t sum = 0;
    for (int i = 0; i < SMTC_MODEM_CUSTOM_ADR_DATA_LENGTH; i++) {
        DEBUG_PRINTF("%d", dr_distribution[i]);
        sum += dr_distribution[i];
        if (i < SMTC_MODEM_CUSTOM_ADR_DATA_LENGTH - 1) DEBUG_PRINT(",");
    }
    DEBUG_PRINTF("] sum=%d\n", sum);
#endif
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::setADRProfile(smtc_modem_adr_profile_t adr_profile, const uint8_t dr_distribution[SMTC_MODEM_CUSTOM_ADR_DATA_LENGTH]) {
    smtc_modem_return_code_t ret = smtc_modem_adr_set_profile(0, adr_profile, dr_distribution);
    const char* profile_names[] = {"NETWORK_CONTROLLED", "MOBILE_LONG_RANGE", "MOBILE_LOW_POWER", "CUSTOM"};
    DEBUG_PRINTF("Set ADR profile: %s, result: %d\n", 
                 (adr_profile <= SMTC_MODEM_ADR_PROFILE_CUSTOM) ? profile_names[adr_profile] : "UNKNOWN", 
                 ret);
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::setNbTrans(uint8_t nb_trans) {
    smtc_modem_return_code_t ret = smtc_modem_set_nb_trans(0, nb_trans);
    DEBUG_PRINTF("Set NbTrans: %d, result: %d\n", nb_trans, ret);
    return ret;
}

// LBT (Listen Before Talk) implementations
smtc_modem_return_code_t LoRaWANClass::setLBTParameters(uint32_t listening_duration_ms, int16_t threshold_dbm, uint32_t bw_hz) {
    smtc_modem_return_code_t ret = smtc_modem_lbt_set_parameters(0, listening_duration_ms, threshold_dbm, bw_hz);
    DEBUG_PRINTF("Set LBT parameters: duration=%dms, threshold=%ddBm, bw=%dHz, result: %d\n", 
                 listening_duration_ms, threshold_dbm, bw_hz, ret);
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::getLBTParameters(uint32_t* listening_duration_ms, int16_t* threshold_dbm, uint32_t* bw_hz) {
    smtc_modem_return_code_t ret = smtc_modem_lbt_get_parameters(0, listening_duration_ms, threshold_dbm, bw_hz);
    if (ret == SMTC_MODEM_RC_OK) {
        DEBUG_PRINTF("Get LBT parameters: duration=%dms, threshold=%ddBm, bw=%dHz\n", 
                     *listening_duration_ms, *threshold_dbm, *bw_hz);
    } else {
        DEBUG_PRINTF("Get LBT parameters failed: result=%d\n", ret);
    }
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::setLBTState(bool enable) {
    smtc_modem_return_code_t ret = smtc_modem_lbt_set_state(0, enable);
    DEBUG_PRINTF("Set LBT state: %s, result: %d\n", enable ? "enabled" : "disabled", ret);
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::getLBTState(bool* enabled) {
    smtc_modem_return_code_t ret = smtc_modem_lbt_get_state(0, enabled);
    if (ret == SMTC_MODEM_RC_OK) {
        DEBUG_PRINTF("Get LBT state: %s\n", *enabled ? "enabled" : "disabled");
    } else {
        DEBUG_PRINTF("Get LBT state failed: result=%d\n", ret);
    }
    return ret;
}

// CSMA (Carrier Sense Multiple Access) implementations
smtc_modem_return_code_t LoRaWANClass::setCSMAState(bool enable) {
    smtc_modem_return_code_t ret = smtc_modem_csma_set_state(0, enable);
    DEBUG_PRINTF("Set CSMA state: %s, result: %d\n", enable ? "enabled" : "disabled", ret);
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::getCSMAState(bool* enabled) {
    smtc_modem_return_code_t ret = smtc_modem_csma_get_state(0, enabled);
    if (ret == SMTC_MODEM_RC_OK) {
        DEBUG_PRINTF("Get CSMA state: %s\n", *enabled ? "enabled" : "disabled");
    } else {
        DEBUG_PRINTF("Get CSMA state failed: result=%d\n", ret);
    }
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::setCSMAParameters(uint8_t max_ch_change, bool bo_enabled, uint8_t nb_bo_max) {
    smtc_modem_return_code_t ret = smtc_modem_csma_set_parameters(0, max_ch_change, bo_enabled, nb_bo_max);
    DEBUG_PRINTF("Set CSMA parameters: max_ch_change=%d, back-off=%s, nb_bo_max=%d, result: %d\n", 
                 max_ch_change, bo_enabled ? "enabled" : "disabled", nb_bo_max, ret);
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::getCSMAParameters(uint8_t* max_ch_change, bool* bo_enabled, uint8_t* nb_bo_max) {
    smtc_modem_return_code_t ret = smtc_modem_csma_get_parameters(0, max_ch_change, bo_enabled, nb_bo_max);
    if (ret == SMTC_MODEM_RC_OK) {
        DEBUG_PRINTF("Get CSMA parameters: max_ch_change=%d, back-off=%s, nb_bo_max=%d\n", 
                     *max_ch_change, *bo_enabled ? "enabled" : "disabled", *nb_bo_max);
    } else {
        DEBUG_PRINTF("Get CSMA parameters failed: result=%d\n", ret);
    }
    return ret;
}

// Network utility implementations
smtc_modem_return_code_t LoRaWANClass::getNextTxMaxPayload(uint8_t* tx_max_payload_size) {
    smtc_modem_return_code_t ret = smtc_modem_get_next_tx_max_payload(0, tx_max_payload_size);
    if (ret == SMTC_MODEM_RC_OK) {
        DEBUG_PRINTF("Get next TX max payload: %d bytes\n", *tx_max_payload_size);
    } else {
        DEBUG_PRINTF("Get next TX max payload failed: result=%d\n", ret);
    }
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::getDutyCycleStatus(int32_t* duty_cycle_status_ms) {
    smtc_modem_return_code_t ret = smtc_modem_get_duty_cycle_status(0, duty_cycle_status_ms);
    if (ret == SMTC_MODEM_RC_OK) {
        if (*duty_cycle_status_ms >= 0) {
            DEBUG_PRINTF("Duty cycle status: %dms available\n", *duty_cycle_status_ms);
        } else {
            DEBUG_PRINTF("Duty cycle status: wait %dms until available\n", -*duty_cycle_status_ms);
        }
    } else {
        DEBUG_PRINTF("Get duty cycle status failed: result=%d\n", ret);
    }
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::sendEmptyUplink(bool send_fport, uint8_t fport, bool confirmed) {
    smtc_modem_return_code_t ret = smtc_modem_request_empty_uplink(0, send_fport, fport, confirmed);
    DEBUG_PRINTF("Send empty uplink: fport=%s%d, confirmed=%s, result=%d\n",
                 send_fport ? "" : "none(", send_fport ? fport : 0, 
                 confirmed ? "true" : "false", ret);
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::leaveNetwork() {
    smtc_modem_return_code_t ret = smtc_modem_leave_network(0);
    DEBUG_PRINTF("Leave network: result=%d\n", ret);
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::getNetworkType(bool* network_type) {
    smtc_modem_return_code_t ret = smtc_modem_get_network_type(0, network_type);
    if (ret == SMTC_MODEM_RC_OK) {
        DEBUG_PRINTF("Get network type: %s\n", *network_type ? "public" : "private");
    } else {
        DEBUG_PRINTF("Get network type failed: result=%d\n", ret);
    }
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::setNetworkType(bool network_type) {
    smtc_modem_return_code_t ret = smtc_modem_set_network_type(0, network_type);
    DEBUG_PRINTF("Set network type: %s, result=%d\n", 
                 network_type ? "public" : "private", ret);
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::getEnabledDatarates(uint16_t* enabled_datarates_mask) {
    smtc_modem_return_code_t ret = smtc_modem_get_enabled_datarates(0, enabled_datarates_mask);
    if (ret == SMTC_MODEM_RC_OK) {
        DEBUG_PRINTF("Get enabled datarates: 0x%04X\n", *enabled_datarates_mask);
    } else {
        DEBUG_PRINTF("Get enabled datarates failed: result=%d\n", ret);
    }
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::setADRAckLimitDelay(uint8_t adr_ack_limit, uint8_t adr_ack_delay) {
    smtc_modem_return_code_t ret = smtc_modem_set_adr_ack_limit_delay(0, adr_ack_limit, adr_ack_delay);
    DEBUG_PRINTF("Set ADR ACK limit/delay: %d/%d, result=%d\n", 
                 adr_ack_limit, adr_ack_delay, ret);
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::getADRAckLimitDelay(uint8_t* adr_ack_limit, uint8_t* adr_ack_delay) {
    smtc_modem_return_code_t ret = smtc_modem_get_adr_ack_limit_delay(0, adr_ack_limit, adr_ack_delay);
    if (ret == SMTC_MODEM_RC_OK) {
        DEBUG_PRINTF("Get ADR ACK limit/delay: %d/%d\n", *adr_ack_limit, *adr_ack_delay);
    } else {
        DEBUG_PRINTF("Get ADR ACK limit/delay failed: result=%d\n", ret);
    }
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::suspendRadio(bool suspend) {
    smtc_modem_return_code_t ret = smtc_modem_suspend_radio_communications(suspend);
    DEBUG_PRINTF("Radio communications: %s, result=%d\n", 
                 suspend ? "suspended" : "resumed", ret);
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::getRadioSuspendStatus(bool* suspended) {
    smtc_modem_return_code_t ret = smtc_modem_get_suspend_radio_communications(0, suspended);
    if (ret == SMTC_MODEM_RC_OK) {
        DEBUG_PRINTF("Radio suspend status: %s\n", *suspended ? "suspended" : "active");
    } else {
        DEBUG_PRINTF("Get radio suspend status failed: result=%d\n", ret);
    }
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::getJoinDutyCycleBackoffBypass(bool* enabled) {
    smtc_modem_return_code_t ret = smtc_modem_get_join_duty_cycle_backoff_bypass(0, enabled);
    if (ret == SMTC_MODEM_RC_OK) {
        DEBUG_PRINTF("Join duty cycle backoff bypass: %s\n", *enabled ? "enabled" : "disabled");
    } else {
        DEBUG_PRINTF("Get join duty cycle backoff bypass failed: result=%d\n", ret);
    }
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::setJoinDutyCycleBackoffBypass(bool enable) {
    smtc_modem_return_code_t ret = smtc_modem_set_join_duty_cycle_backoff_bypass(0, enable);
    DEBUG_PRINTF("Set join duty cycle backoff bypass: %s, result=%d\n", 
                 enable ? "enabled" : "disabled", ret);
    return ret;
}

// Hardware/Board implementations
smtc_modem_return_code_t LoRaWANClass::setCrystalError(uint32_t crystal_error_ppm) {
    smtc_modem_return_code_t ret = smtc_modem_set_crystal_error_ppm(crystal_error_ppm);
    DEBUG_PRINTF("Set crystal error: %d ppm, result=%d\n", crystal_error_ppm, ret);
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::getCrystalError(uint32_t* crystal_error_ppm) {
    smtc_modem_return_code_t ret = smtc_modem_get_crystal_error_ppm(crystal_error_ppm);
    if (ret == SMTC_MODEM_RC_OK) {
        DEBUG_PRINTF("Get crystal error: %d ppm\n", *crystal_error_ppm);
    } else {
        DEBUG_PRINTF("Get crystal error failed: result=%d\n", ret);
    }
    return ret;
}

// Alarm timer implementations
smtc_modem_return_code_t LoRaWANClass::startAlarmTimer(uint32_t alarm_timer_in_s) {
    smtc_modem_return_code_t ret = smtc_modem_alarm_start_timer(alarm_timer_in_s);
    DEBUG_PRINTF("Start alarm timer: %ds, result=%d\n", alarm_timer_in_s, ret);
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::clearAlarmTimer() {
    smtc_modem_return_code_t ret = smtc_modem_alarm_clear_timer();
    DEBUG_PRINTF("Clear alarm timer: result=%d\n", ret);
    return ret;
}

smtc_modem_return_code_t LoRaWANClass::getAlarmRemainingTime(uint32_t* remaining_time_in_s) {
    smtc_modem_return_code_t ret = smtc_modem_alarm_get_remaining_time(remaining_time_in_s);
    if (ret == SMTC_MODEM_RC_OK) {
        DEBUG_PRINTF("Alarm remaining time: %ds\n", *remaining_time_in_s);
    } else {
        DEBUG_PRINTF("Get alarm remaining time failed: result=%d\n", ret);
    }
    return ret;
}

// Global instance
LBMApi lbm;
