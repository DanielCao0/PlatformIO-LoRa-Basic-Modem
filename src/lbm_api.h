#pragma once

#include <stdint.h>
#include <stddef.h>
#include "lbm_core.h"

extern "C" {
#include "smtc_modem_api.h"
}

// LoRaWAN region definitions for easy usage
#define REGION_EU433     SMTC_MODEM_REGION_EU_433     // 0: EU433
#define REGION_CN470     SMTC_MODEM_REGION_CN_470     // 1: CN470
#define REGION_RU864     SMTC_MODEM_REGION_RU_864     // 2: RU864
#define REGION_IN865     SMTC_MODEM_REGION_IN_865     // 3: IN865
#define REGION_EU868     SMTC_MODEM_REGION_EU_868     // 4: EU868
#define REGION_US915     SMTC_MODEM_REGION_US_915     // 5: US915
#define REGION_AU915     SMTC_MODEM_REGION_AU_915     // 6: AU915
#define REGION_KR920     SMTC_MODEM_REGION_KR_920     // 7: KR920
#define REGION_AS923_1   SMTC_MODEM_REGION_AS_923     // 8: AS923-1
#define REGION_AS923_2   SMTC_MODEM_REGION_AS_923_GRP2 // 9: AS923-2
#define REGION_AS923_3   SMTC_MODEM_REGION_AS_923_GRP3 // 10: AS923-3
#define REGION_AS923_4   SMTC_MODEM_REGION_AS_923_GRP4 // 11: AS923-4

// Forward declarations
class LBMApi;

// Global user event callback variable declaration
extern LBMEventCallback userEventCallback;

// LoRaWAN network management class
class LoRaWANClass {
    friend class LBMApi;
public:
    // Network management and credentials
    /**
     * @brief Set Device EUI (Extended Unique Identifier)
     * @param dev_eui Pointer to 8-byte DevEUI array
     * @return SMTC_MODEM_RC_OK on success
     * @note Must be called before join, cannot be changed while joined
     */
    smtc_modem_return_code_t setDevEUI(const uint8_t* dev_eui);
    
    /**
     * @brief Set Join EUI (also called App EUI in LoRaWAN 1.0.x)
     * @param join_eui Pointer to 8-byte JoinEUI array
     * @return SMTC_MODEM_RC_OK on success
     * @note Must be called before join, cannot be changed while joined
     */
    smtc_modem_return_code_t setJoinEUI(const uint8_t* join_eui);
    
    /**
     * @brief Set Application Key (LoRaWAN 1.1.x) or Gen Application Key (LoRaWAN 1.0.x)
     * @param app_key Pointer to 16-byte AppKey array
     * @return SMTC_MODEM_RC_OK on success
     * @note Must be called before join, cannot be changed while joined
     */
    smtc_modem_return_code_t setAppKey(const uint8_t* app_key);
    
    /**
     * @brief Set Network Key (LoRaWAN 1.1.x) or Application Key (LoRaWAN 1.0.x)
     * @param nwk_key Pointer to 16-byte NwkKey array
     * @return SMTC_MODEM_RC_OK on success
     * @note Must be called before join, cannot be changed while joined
     */
    smtc_modem_return_code_t setNwkKey(const uint8_t* nwk_key);
    
    /**
     * @brief Set LoRaWAN region
     * @param region Region to configure (EU868, US915, AS923, etc.)
     * @return SMTC_MODEM_RC_OK on success
     * @note Must be set before join, cannot be changed while joined
     * @note Use REGION_XXX defines for convenience
     */
    smtc_modem_return_code_t setRegion(smtc_modem_region_t region);
    
    /**
     * @brief Set LoRaWAN device class
     * @param modem_class Class to set (SMTC_MODEM_CLASS_A, SMTC_MODEM_CLASS_B, or SMTC_MODEM_CLASS_C)
     * @return SMTC_MODEM_RC_OK on success
     * @note Class A is default, Class B/C require network support
     */
    smtc_modem_return_code_t setClass(smtc_modem_class_t modem_class);
    
    /**
     * @brief Start OTAA (Over-The-Air Activation) join procedure
     * @return SMTC_MODEM_RC_OK on success
     * @note Generates SMTC_MODEM_EVENT_JOINED on success or SMTC_MODEM_EVENT_JOINFAIL on failure
     * @note Must call setDevEUI, setJoinEUI, setAppKey, setNwkKey, and setRegion first
     */
    smtc_modem_return_code_t join();
    
    /**
     * @brief Check if device has joined the network
     * @param joined Output: true if joined, false if not joined
     * @return SMTC_MODEM_RC_OK on success
     * @note Checks SMTC_MODEM_STATUS_JOINED bit in modem status
     */
    smtc_modem_return_code_t isJoined(bool* joined) const;
    
    // Data transmission
    /**
     * @brief Send uplink data
     * @param data Pointer to payload data buffer
     * @param len Length of payload (max depends on region and data rate)
     * @param port LoRaWAN FPort (1-223, default: 2)
     * @param confirmed true for confirmed uplink, false for unconfirmed (default: false)
     * @return SMTC_MODEM_RC_OK on success
     * @note Device must be joined before sending
     * @note Generates SMTC_MODEM_EVENT_TXDONE event after transmission
     */
    smtc_modem_return_code_t send(const uint8_t* data, size_t len, uint8_t port = 2, bool confirmed = false);
    
    // Data reception
    /**
     * @brief Get received downlink data
     * @param payload Output buffer for downlink payload (max 242 bytes)
     * @param payload_size Output: actual payload length received
     * @param metadata Output: downlink metadata (RSSI, SNR, window, etc.)
     * @param remaining Output: number of additional downlinks pending
     * @return SMTC_MODEM_RC_OK on success
     * @note Call this after receiving SMTC_MODEM_EVENT_DOWNDATA event
     * @note If remaining > 0, call again to retrieve next downlink
     */
    smtc_modem_return_code_t getDownlinkData(uint8_t* payload, uint8_t* payload_size, smtc_modem_dl_metadata_t* metadata, uint8_t* remaining);

    // ADR (Adaptive Data Rate) configuration
    /**
     * @brief Set custom DataRate distribution for Join procedure
     * @param dr_distribution 16-byte array where each byte represents DR0-DR15 probability weight (sum should be 100)
     * @return SMTC_MODEM_RC_OK on success
     * @note For EU868: typically use DR0-DR5, others set to 0
     * @note No effect on US915/AU915 regions
     */
    smtc_modem_return_code_t setJoinDataRateDistribution(const uint8_t dr_distribution[SMTC_MODEM_CUSTOM_ADR_DATA_LENGTH]);
    
    /**
     * @brief Set ADR profile for uplink after join
     * @param adr_profile ADR profile type (NETWORK_CONTROLLED, MOBILE_LONG_RANGE, MOBILE_LOW_POWER, CUSTOM)
     * @param dr_distribution Custom distribution (required only if adr_profile is CUSTOM), sum should be 100
     * @return SMTC_MODEM_RC_OK on success
     */
    smtc_modem_return_code_t setADRProfile(smtc_modem_adr_profile_t adr_profile, const uint8_t dr_distribution[SMTC_MODEM_CUSTOM_ADR_DATA_LENGTH] = nullptr);
    
    /**
     * @brief Set number of transmissions for unconfirmed uplink
     * @param nb_trans Number of transmissions (1-15)
     * @return SMTC_MODEM_RC_OK on success
     * @note Only works in MOBILE or CUSTOM ADR modes, not NETWORK_CONTROLLED
     */
    smtc_modem_return_code_t setNbTrans(uint8_t nb_trans);

    // LBT (Listen Before Talk) configuration
    /**
     * @brief Set LBT parameters
     * @param listening_duration_ms Listening duration in milliseconds (typically 5ms for Japan)
     * @param threshold_dbm RSSI threshold in dBm (typically -80dBm)
     * @param bw_hz Bandwidth in Hz (typically 200000 for 200kHz)
     * @return SMTC_MODEM_RC_OK on success
     * @note Required for Japan (AS923) and Korea (KR920) regions
     */
    smtc_modem_return_code_t setLBTParameters(uint32_t listening_duration_ms, int16_t threshold_dbm, uint32_t bw_hz);
    
    /**
     * @brief Get current LBT parameters
     * @param listening_duration_ms Output: listening duration in ms
     * @param threshold_dbm Output: RSSI threshold in dBm
     * @param bw_hz Output: bandwidth in Hz
     * @return SMTC_MODEM_RC_OK on success
     */
    smtc_modem_return_code_t getLBTParameters(uint32_t* listening_duration_ms, int16_t* threshold_dbm, uint32_t* bw_hz);
    
    /**
     * @brief Enable or disable LBT
     * @param enable true to enable, false to disable
     * @return SMTC_MODEM_RC_OK on success
     * @note Must call setLBTParameters before enabling
     */
    smtc_modem_return_code_t setLBTState(bool enable);
    
    /**
     * @brief Get LBT state
     * @param enabled Output: true if enabled, false if disabled
     * @return SMTC_MODEM_RC_OK on success
     */
    smtc_modem_return_code_t getLBTState(bool* enabled);

    // CSMA (Carrier Sense Multiple Access) configuration
    /**
     * @brief Enable or disable CSMA
     * @param enable true to enable, false to disable
     * @return SMTC_MODEM_RC_OK on success
     * @note CSMA is automatically enabled in regions where LBT is not mandatory
     */
    smtc_modem_return_code_t setCSMAState(bool enable);
    
    /**
     * @brief Get CSMA state
     * @param enabled Output: true if enabled, false if disabled
     * @return SMTC_MODEM_RC_OK on success
     */
    smtc_modem_return_code_t getCSMAState(bool* enabled);
    
    /**
     * @brief Set CSMA parameters
     * @param max_ch_change Number of channel changes when noisy before sending (default: 4)
     * @param bo_enabled Enable back-off (multiple short CAD on same channel, default: false)
     * @param nb_bo_max Number of CAD checks to verify channel is free (default: 6)
     * @return SMTC_MODEM_RC_OK on success
     * @note CAD = Channel Activity Detection
     */
    smtc_modem_return_code_t setCSMAParameters(uint8_t max_ch_change, bool bo_enabled, uint8_t nb_bo_max);
    
    /**
     * @brief Get current CSMA parameters
     * @param max_ch_change Output: number of channel changes when noisy
     * @param bo_enabled Output: is back-off enabled
     * @param nb_bo_max Output: number of CAD checks
     * @return SMTC_MODEM_RC_OK on success
     */
    smtc_modem_return_code_t getCSMAParameters(uint8_t* max_ch_change, bool* bo_enabled, uint8_t* nb_bo_max);

    // Network utility functions
    /**
     * @brief Get maximum payload size for next uplink
     * @param tx_max_payload_size Output: maximum payload size in bytes
     * @return SMTC_MODEM_RC_OK on success
     * @note Value depends on current data rate and LoRaWAN regional parameters
     */
    smtc_modem_return_code_t getNextTxMaxPayload(uint8_t* tx_max_payload_size);
    
    /**
     * @brief Get current duty cycle status
     * @param duty_cycle_status_ms Output: duty cycle status in milliseconds
     * @return SMTC_MODEM_RC_OK on success
     * @note Positive value = time still available, Negative value = time to wait until band available
     */
    smtc_modem_return_code_t getDutyCycleStatus(int32_t* duty_cycle_status_ms);
    
    /**
     * @brief Request empty uplink (no payload, optional FPort)
     * @param send_fport true to include FPort, false to send without FPort
     * @param fport FPort value if send_fport is true
     * @param confirmed true for confirmed uplink, false for unconfirmed
     * @return SMTC_MODEM_RC_OK on success
     * @note Used for heartbeat or creating downlink opportunities
     */
    smtc_modem_return_code_t sendEmptyUplink(bool send_fport, uint8_t fport, bool confirmed);
    
    /**
     * @brief Leave network or cancel ongoing join
     * @return SMTC_MODEM_RC_OK on success
     */
    smtc_modem_return_code_t leaveNetwork();
    
    /**
     * @brief Get network type configuration
     * @param network_type Output: true=public network, false=private network
     * @return SMTC_MODEM_RC_OK on success
     */
    smtc_modem_return_code_t getNetworkType(bool* network_type);
    
    /**
     * @brief Set network type
     * @param network_type true=public network, false=private network
     * @return SMTC_MODEM_RC_OK on success
     */
    smtc_modem_return_code_t setNetworkType(bool network_type);
    
    /**
     * @brief Get enabled datarates mask
     * @param enabled_datarates_mask Output: bitfield of enabled datarates
     * @return SMTC_MODEM_RC_OK on success
     * @note Depends on uplink channel mask and dwell time settings
     */
    smtc_modem_return_code_t getEnabledDatarates(uint16_t* enabled_datarates_mask);
    
    /**
     * @brief Set ADR ACK limit and delay
     * @param adr_ack_limit ADR ACK limit (2-127)
     * @param adr_ack_delay ADR ACK delay (2-127)
     * @return SMTC_MODEM_RC_OK on success
     * @note Controls ADR fallback behavior when no downlinks received
     */
    smtc_modem_return_code_t setADRAckLimitDelay(uint8_t adr_ack_limit, uint8_t adr_ack_delay);
    
    /**
     * @brief Get ADR ACK limit and delay
     * @param adr_ack_limit Output: ADR ACK limit
     * @param adr_ack_delay Output: ADR ACK delay
     * @return SMTC_MODEM_RC_OK on success
     */
    smtc_modem_return_code_t getADRAckLimitDelay(uint8_t* adr_ack_limit, uint8_t* adr_ack_delay);
    
    /**
     * @brief Suspend or resume radio communications
     * @param suspend true to suspend, false to resume
     * @return SMTC_MODEM_RC_OK on success
     */
    smtc_modem_return_code_t suspendRadio(bool suspend);
    
    /**
     * @brief Get radio suspend status
     * @param suspended Output: true if suspended, false if active
     * @return SMTC_MODEM_RC_OK on success
     */
    smtc_modem_return_code_t getRadioSuspendStatus(bool* suspended);
    
    /**
     * @brief Get join duty cycle backoff bypass status
     * @param enabled Output: true if bypass enabled, false if disabled
     * @return SMTC_MODEM_RC_OK on success
     * @note Used for LoRaWAN certification testing
     */
    smtc_modem_return_code_t getJoinDutyCycleBackoffBypass(bool* enabled);
    
    /**
     * @brief Set join duty cycle backoff bypass
     * @param enable true to bypass duty cycle backoff during join, false to enforce it
     * @return SMTC_MODEM_RC_OK on success
     * @note Used for LoRaWAN certification testing, normally should be disabled
     */
    smtc_modem_return_code_t setJoinDutyCycleBackoffBypass(bool enable);

    // Hardware/Board configuration
    /**
     * @brief Set crystal error in ppm
     * @param crystal_error_ppm Crystal error value
     * @return SMTC_MODEM_RC_OK on success
     */
    smtc_modem_return_code_t setCrystalError(uint32_t crystal_error_ppm);
    
    /**
     * @brief Get crystal error in ppm
     * @param crystal_error_ppm Output: crystal error value
     * @return SMTC_MODEM_RC_OK on success
     */
    smtc_modem_return_code_t getCrystalError(uint32_t* crystal_error_ppm);
    
    // Alarm timer
    /**
     * @brief Start alarm timer
     * @param alarm_timer_in_s Alarm duration in seconds (max 864000s = 10 days)
     * @return SMTC_MODEM_RC_OK on success
     * @note Generates SMTC_MODEM_EVENT_ALARM event when expires
     */
    smtc_modem_return_code_t startAlarmTimer(uint32_t alarm_timer_in_s);
    
    /**
     * @brief Stop and clear alarm timer
     * @return SMTC_MODEM_RC_OK on success
     */
    smtc_modem_return_code_t clearAlarmTimer();
    
    /**
     * @brief Get remaining alarm time
     * @param remaining_time_in_s Output: remaining time in seconds
     * @return SMTC_MODEM_RC_OK on success
     */
    smtc_modem_return_code_t getAlarmRemainingTime(uint32_t* remaining_time_in_s);

private:
    LoRaWANClass() {} // Only LBMApi can create
};

// P2P class (reserved for future)
class P2PClass {
    friend class LBMApi;
public:
    // P2P methods will be added here in future
    
private:
    P2PClass() {} // Only LBMApi can create
};

class LBMApi {
public:
    LBMApi();
    ~LBMApi();

    // Arduino style initialization
    smtc_modem_return_code_t init();
    
    // Core modem engine function - must be called regularly
    void runEngine();
    
    // Event callback registration
    void setEventCallback(LBMEventCallback callback);

    // Sub-modules
    LoRaWANClass lorawan;
    P2PClass p2p;
};

extern LBMApi lbm;
