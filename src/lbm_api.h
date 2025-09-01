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
    // Network management
    bool setDevEUI(const uint8_t* dev_eui);
    bool setJoinEUI(const uint8_t* join_eui);
    bool setAppKey(const uint8_t* app_key);
    bool setNwkKey(const uint8_t* nwk_key);
    bool setRegion(smtc_modem_region_t region);
    bool join();
    bool isJoined() const;
    
    // Data transmission
    bool send(const uint8_t* data, size_t len, uint8_t port = 2, bool confirmed = false);
    
    // Data reception
    bool getDownlinkData(uint8_t* payload, uint8_t* payload_size, smtc_modem_dl_metadata_t* metadata, uint8_t* remaining);

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
    bool init();
    
    // Core modem engine function - must be called regularly
    void runEngine();
    
    // Event callback registration
    void setEventCallback(LBMEventCallback callback);

    // Sub-modules
    LoRaWANClass lorawan;
    P2PClass p2p;
};

extern LBMApi lbm;
