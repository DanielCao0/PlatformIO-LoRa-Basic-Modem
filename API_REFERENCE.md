# LoRa Basic Modem API Reference

This document provides detailed documentation for the RAK3112 LoRa Basic Modem C++ API interface.

## Table of Contents

- [Initialization](#initialization)
  - [lbm.init()](#lbminit)
  - [lbm.runEngine()](#lbmrunengine)
  - [lbm.setEventCallback()](#lbmseteventcallback)
- [Network Management](#network-management)
  - [lbm.lorawan.setDevEUI()](#lbmlorawansetdeveui)
  - [lbm.lorawan.setJoinEUI()](#lbmlorawansetjoineui)
  - [lbm.lorawan.setAppKey()](#lbmlorawansetappkey)
  - [lbm.lorawan.setNwkKey()](#lbmlorawansetnwkkey)
  - [lbm.lorawan.setRegion()](#lbmlorawansetregion)
  - [lbm.lorawan.setClass()](#lbmlorawansetclass)
  - [lbm.lorawan.join()](#lbmlorawanjoin)
  - [lbm.lorawan.isJoined()](#lbmlorawanisjoined)
  - [lbm.lorawan.leaveNetwork()](#lbmlorawanleavenetwork)
  - [lbm.lorawan.getNetworkType()](#lbmlorawangetnetworktype)
  - [lbm.lorawan.setNetworkType()](#lbmlorawansetnetworktype)
- [Data Transmission](#data-transmission)
  - [lbm.lorawan.send()](#lbmlorawansend)
  - [lbm.lorawan.sendEmptyUplink()](#lbmlorawansendemptyuplink)
  - [lbm.lorawan.getDownlinkData()](#lbmlorawangetdownlinkdata)
  - [lbm.lorawan.getNextTxMaxPayload()](#lbmlorawangetnexttxmaxpayload)
  - [lbm.lorawan.getDutyCycleStatus()](#lbmlorawangetdutycyclestatus)
- [ADR Configuration](#adr-configuration)
  - [lbm.lorawan.setJoinDataRateDistribution()](#lbmlorawansetjoindataratedistribution)
  - [lbm.lorawan.setADRProfile()](#lbmlorawansetadrprofile)
  - [lbm.lorawan.setNbTrans()](#lbmlorawansetnbtrans)
  - [lbm.lorawan.getEnabledDatarates()](#lbmlorawangetenableddatarates)
  - [lbm.lorawan.setADRAckLimitDelay()](#lbmlorawansetadracklimitdelay)
  - [lbm.lorawan.getADRAckLimitDelay()](#lbmlorawangetadracklimitdelay)
- [Channel Access Control](#channel-access-control)
  - [LBT Functions](#lbt-listen-before-talk)
    - [lbm.lorawan.setLBTParameters()](#lbmlorawansetlbtparameters)
    - [lbm.lorawan.getLBTParameters()](#lbmlorawangetlbtparameters)
    - [lbm.lorawan.setLBTState()](#lbmlorawansetlbtstate)
    - [lbm.lorawan.getLBTState()](#lbmlorawangetlbtstate)
  - [CSMA Functions](#csma-carrier-sense-multiple-access)
    - [lbm.lorawan.setCSMAState()](#lbmlorawansetcsmastate)
    - [lbm.lorawan.getCSMAState()](#lbmlorawangetcsmastate)
    - [lbm.lorawan.setCSMAParameters()](#lbmlorawansetcsmaparameters)
    - [lbm.lorawan.getCSMAParameters()](#lbmlorawangetcsmaparameters)
- [Network Utilities](#network-utilities)
  - [lbm.lorawan.suspendRadio()](#lbmlorawansuspendradio)
  - [lbm.lorawan.getRadioSuspendStatus()](#lbmlorawangetradiosuspendstatus)
  - [lbm.lorawan.getJoinDutyCycleBackoffBypass()](#lbmlorawangetjoindutycyclebackoffbypass)
  - [lbm.lorawan.setJoinDutyCycleBackoffBypass()](#lbmlorawansetjoindutycyclebackoffbypass)
- [Hardware Configuration](#hardware-configuration)
  - [lbm.lorawan.setCrystalError()](#lbmlorawansetcrystalerror)
  - [lbm.lorawan.getCrystalError()](#lbmlorawangetcrystalerror)
- [Timers](#timers)
  - [lbm.lorawan.startAlarmTimer()](#lbmlorawanstartAlarmtimer)
  - [lbm.lorawan.clearAlarmTimer()](#lbmlorawanclearalarmtimer)
  - [lbm.lorawan.getAlarmRemainingTime()](#lbmlorawangetalarmremainingtime)
- [Event Handling](#event-handling)
- [Complete Usage Example](#complete-usage-example)
- [Return Codes](#return-codes)
- [Important Notes](#important-notes)
- [Version Information](#version-information)

---

## Initialization

### `lbm.init()`

Initialize the LoRa Basic Modem.

**Returns:** `smtc_modem_return_code_t`
- `SMTC_MODEM_RC_OK`: Success

**Example:**
```cpp
void setup() {
    Serial.begin(115200);
    lbm.init();
}
```

### `lbm.runEngine()`

Run the modem engine, must be called periodically in the main loop.

**Example:**
```cpp
void loop() {
    lbm.runEngine();
    delay(10);
}
```

### `lbm.setEventCallback(callback)`

Register an event callback function.

**Parameters:**
- `callback`: Callback function of type `void (*)(void)`

**Example:**
```cpp
void onModemEvent() {
    // Handle modem events
}

void setup() {
    lbm.setEventCallback(onModemEvent);
}
```

---

## Network Management

### `lbm.lorawan.setDevEUI(dev_eui)`

Set the device unique identifier (DevEUI).

**Parameters:**
- `dev_eui`: Pointer to 8-byte DevEUI array

**Returns:** `smtc_modem_return_code_t`

**Note:** 
- Must be called before join
- Cannot be modified when already joined

**Example:**
```cpp
uint8_t dev_eui[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
lbm.lorawan.setDevEUI(dev_eui);
```

### `lbm.lorawan.setJoinEUI(join_eui)`

Set Join EUI (also known as App EUI).

**Parameters:**
- `join_eui`: Pointer to 8-byte JoinEUI array

**Returns:** `smtc_modem_return_code_t`

**Example:**
```cpp
uint8_t join_eui[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
lbm.lorawan.setJoinEUI(join_eui);
```

### `lbm.lorawan.setAppKey(app_key)`

Set the application key (AppKey).

**Parameters:**
- `app_key`: Pointer to 16-byte AppKey array

**Returns:** `smtc_modem_return_code_t`

**Example:**
```cpp
uint8_t app_key[16] = {0x00, 0x01, 0x02, ..., 0x0F};
lbm.lorawan.setAppKey(app_key);
```

### `lbm.lorawan.setNwkKey(nwk_key)`

Set the network key (NwkKey).

**Parameters:**
- `nwk_key`: Pointer to 16-byte NwkKey array

**Returns:** `smtc_modem_return_code_t`

### `lbm.lorawan.setRegion(region)`

Set the LoRaWAN region.

**Parameters:**
- `region`: Region code, predefined macros:
  - `REGION_EU868`: Europe 868MHz
  - `REGION_US915`: USA 915MHz
  - `REGION_AS923_1`: Asia 923MHz Group 1
  - `REGION_CN470`: China 470MHz
  - `REGION_AU915`: Australia 915MHz
  - `REGION_KR920`: Korea 920MHz
  - etc.

**Returns:** `smtc_modem_return_code_t`

**Example:**
```cpp
lbm.lorawan.setRegion(REGION_EU868);
```

### `lbm.lorawan.setClass(modem_class)`

Set the LoRaWAN device class.

**Parameters:**
- `modem_class`: Device class
  - `SMTC_MODEM_CLASS_A`: Default class
  - `SMTC_MODEM_CLASS_B`: Requires network support
  - `SMTC_MODEM_CLASS_C`: Requires network support

**Returns:** `smtc_modem_return_code_t`

**Example:**
```cpp
lbm.lorawan.setClass(SMTC_MODEM_CLASS_A);
```

### `lbm.lorawan.join()`

Start the OTAA join procedure.

**Returns:** `smtc_modem_return_code_t`

**Note:**
- Must set DevEUI, JoinEUI, AppKey, NwkKey and Region first
- Generates `SMTC_MODEM_EVENT_JOINED` event on success
- Generates `SMTC_MODEM_EVENT_JOINFAIL` event on failure

**Example:**
```cpp
lbm.lorawan.join();
```

### `lbm.lorawan.isJoined(joined)`

Check if the device is joined to the network.

**Parameters:**
- `joined`: Output parameter, `true` if joined, `false` otherwise

**Returns:** `smtc_modem_return_code_t`

**Example:**
```cpp
bool joined;
lbm.lorawan.isJoined(&joined);
if (joined) {
    Serial.println("Device is joined");
}
```

### `lbm.lorawan.leaveNetwork()`

Leave the network or cancel an ongoing join.

**Returns:** `smtc_modem_return_code_t`

**Example:**
```cpp
lbm.lorawan.leaveNetwork();
```

### `lbm.lorawan.getNetworkType(network_type)`

Get the network type configuration.

**Parameters:**
- `network_type`: Output parameter, `true` for public network, `false` for private network

**Returns:** `smtc_modem_return_code_t`

### `lbm.lorawan.setNetworkType(network_type)`

Set the network type.

**Parameters:**
- `network_type`: `true` for public network, `false` for private network

**Returns:** `smtc_modem_return_code_t`

**Example:**
```cpp
lbm.lorawan.setNetworkType(true); // Public network
```

---

## Data Transmission

### `lbm.lorawan.send(data, len, port, confirmed)`

Send uplink data.

**Parameters:**
- `data`: Pointer to payload data
- `len`: Payload length (maximum depends on region and data rate)
- `port`: LoRaWAN FPort (1-223, default 2)
- `confirmed`: `true` for confirmed uplink, `false` for unconfirmed (default)

**Returns:** `smtc_modem_return_code_t`

**Note:**
- Device must be joined
- Generates `SMTC_MODEM_EVENT_TXDONE` event after transmission

**Example:**
```cpp
uint8_t payload[] = {0x01, 0x02, 0x03};
lbm.lorawan.send(payload, 3, 2, false);
```

### `lbm.lorawan.sendEmptyUplink(send_fport, fport, confirmed)`

Send an empty uplink (keepalive packet).

**Parameters:**
- `send_fport`: `true` to include FPort, `false` otherwise
- `fport`: FPort value (if send_fport is true)
- `confirmed`: `true` for confirmed uplink, `false` for unconfirmed

**Returns:** `smtc_modem_return_code_t`

**Purpose:** Create a downlink opportunity or keepalive without sending data to the application server

**Example:**
```cpp
// Send keepalive packet
lbm.lorawan.sendEmptyUplink(false, 0, false);
```

### `lbm.lorawan.getDownlinkData(payload, payload_size, metadata, remaining)`

Get received downlink data.

**Parameters:**
- `payload`: Output buffer (maximum 242 bytes)
- `payload_size`: Output actual received payload length
- `metadata`: Output downlink metadata (RSSI, SNR, window, etc.)
- `remaining`: Output number of remaining downlinks to read

**Returns:** `smtc_modem_return_code_t`

**Note:**
- Call after receiving `SMTC_MODEM_EVENT_DOWNDATA` event
- If remaining > 0, call again to get next downlink

**Example:**
```cpp
void onModemEvent() {
    uint8_t payload[242];
    uint8_t payload_size;
    smtc_modem_dl_metadata_t metadata;
    uint8_t remaining;
    
    if (lbm.lorawan.getDownlinkData(payload, &payload_size, &metadata, &remaining) == SMTC_MODEM_RC_OK) {
        Serial.printf("Received %d bytes on port %d\n", payload_size, metadata.fport);
        Serial.printf("RSSI: %d dBm, SNR: %d dB\n", metadata.rssi - 64, metadata.snr / 4);
    }
}
```

### `lbm.lorawan.getNextTxMaxPayload(tx_max_payload_size)`

Get the maximum payload size for the next uplink.

**Parameters:**
- `tx_max_payload_size`: Output maximum payload size in bytes

**Returns:** `smtc_modem_return_code_t`

**Note:** Value depends on current data rate and regional parameters

**Example:**
```cpp
uint8_t max_size;
lbm.lorawan.getNextTxMaxPayload(&max_size);
Serial.printf("Max payload: %d bytes\n", max_size);
```

### `lbm.lorawan.getDutyCycleStatus(duty_cycle_status_ms)`

Get the current duty cycle status.

**Parameters:**
- `duty_cycle_status_ms`: Output duty cycle status (milliseconds)
  - Positive value: Available time remaining
  - Negative value: Wait time required

**Returns:** `smtc_modem_return_code_t`

**Example:**
```cpp
int32_t duty_cycle_ms;
lbm.lorawan.getDutyCycleStatus(&duty_cycle_ms);
if (duty_cycle_ms >= 0) {
    Serial.printf("Can transmit, %dms available\n", duty_cycle_ms);
} else {
    Serial.printf("Must wait %dms\n", -duty_cycle_ms);
}
```

---

## ADR Configuration

### `lbm.lorawan.setJoinDataRateDistribution(dr_distribution)`

Set custom data rate distribution during join phase.

**Parameters:**
- `dr_distribution`: 16-byte array, each byte represents the probability weight for DR0-DR15 (sum should be 100)

**Returns:** `smtc_modem_return_code_t`

**Note:**
- EU868 typically uses DR0-DR5, others set to 0
- Invalid for US915/AU915 regions

**Example:**
```cpp
// EU868: DR0=20%, DR1=20%, DR2=20%, DR3=20%, DR4=10%, DR5=10%
uint8_t dr_dist[16] = {20, 20, 20, 20, 10, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
lbm.lorawan.setJoinDataRateDistribution(dr_dist);
```

### `lbm.lorawan.setADRProfile(adr_profile, dr_distribution)`

Set the ADR profile after joining the network.

**Parameters:**
- `adr_profile`: ADR configuration type
  - `SMTC_MODEM_ADR_PROFILE_NETWORK_CONTROLLED`: Network controlled (static device)
  - `SMTC_MODEM_ADR_PROFILE_MOBILE_LONG_RANGE`: Mobile device long range
  - `SMTC_MODEM_ADR_PROFILE_MOBILE_LOW_POWER`: Mobile device low power
  - `SMTC_MODEM_ADR_PROFILE_CUSTOM`: Custom distribution
- `dr_distribution`: Custom distribution array (only needed for CUSTOM mode)

**Returns:** `smtc_modem_return_code_t`

**Example:**
```cpp
// Use mobile device long range mode
lbm.lorawan.setADRProfile(SMTC_MODEM_ADR_PROFILE_MOBILE_LONG_RANGE, nullptr);

// Use custom distribution
uint8_t custom_dr[16] = {10, 20, 30, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
lbm.lorawan.setADRProfile(SMTC_MODEM_ADR_PROFILE_CUSTOM, custom_dr);
```

### `lbm.lorawan.setNbTrans(nb_trans)`

Set the number of transmissions for unconfirmed uplinks.

**Parameters:**
- `nb_trans`: Number of transmissions (1-15)

**Returns:** `smtc_modem_return_code_t`

**Note:** Only effective in MOBILE or CUSTOM ADR modes

**Example:**
```cpp
lbm.lorawan.setNbTrans(3); // Retransmit 3 times
```

### `lbm.lorawan.getEnabledDatarates(enabled_datarates_mask)`

Get the currently enabled data rates mask.

**Parameters:**
- `enabled_datarates_mask`: Output bitmask

**Returns:** `smtc_modem_return_code_t`

**Note:** Depends on uplink channel mask and dwell time settings

**Example:**
```cpp
uint16_t dr_mask;
lbm.lorawan.getEnabledDatarates(&dr_mask);
Serial.printf("Enabled DRs: 0x%04X\n", dr_mask);
```

### `lbm.lorawan.setADRAckLimitDelay(adr_ack_limit, adr_ack_delay)`

Set ADR ACK limit and delay.

**Parameters:**
- `adr_ack_limit`: ADR ACK limit (2-127)
- `adr_ack_delay`: ADR ACK delay (2-127)

**Returns:** `smtc_modem_return_code_t`

**Purpose:** Control ADR fallback behavior when no downlinks are received

**Example:**
```cpp
lbm.lorawan.setADRAckLimitDelay(64, 32);
```

### `lbm.lorawan.getADRAckLimitDelay(adr_ack_limit, adr_ack_delay)`

Get ADR ACK limit and delay configuration.

**Parameters:**
- `adr_ack_limit`: Output ADR ACK limit
- `adr_ack_delay`: Output ADR ACK delay

**Returns:** `smtc_modem_return_code_t`

---

## Channel Access Control

### LBT (Listen Before Talk)

Used for regulatory requirements in regions like Japan (AS923) and Korea (KR920).

#### `lbm.lorawan.setLBTParameters(listening_duration_ms, threshold_dbm, bw_hz)`

Set LBT parameters.

**Parameters:**
- `listening_duration_ms`: Listening duration (typically 5ms for Japan)
- `threshold_dbm`: RSSI threshold (typically -80dBm)
- `bw_hz`: Listening bandwidth (typically 200000 for 200kHz)

**Returns:** `smtc_modem_return_code_t`

**Example:**
```cpp
// Typical configuration for Japan AS923
lbm.lorawan.setLBTParameters(5, -80, 200000);
```

#### `lbm.lorawan.getLBTParameters(listening_duration_ms, threshold_dbm, bw_hz)`

Get current LBT parameters.

#### `lbm.lorawan.setLBTState(enable)`

Enable or disable LBT.

**Parameters:**
- `enable`: `true` to enable, `false` to disable

**Note:** Must call `setLBTParameters` first

#### `lbm.lorawan.getLBTState(enabled)`

Get LBT state.

### CSMA (Carrier Sense Multiple Access)

Channel access mechanism automatically enabled in regions where LBT is not mandatory.

#### `lbm.lorawan.setCSMAState(enable)`

Enable or disable CSMA.

#### `lbm.lorawan.getCSMAState(enabled)`

Get CSMA state.

#### `lbm.lorawan.setCSMAParameters(max_ch_change, bo_enabled, nb_bo_max)`

Set CSMA parameters.

**Parameters:**
- `max_ch_change`: Number of channel changes when channel is busy (default 4)
- `bo_enabled`: Enable backoff (multiple CAD detections on same channel, default false)
- `nb_bo_max`: Number of CAD detections (default 6)

**Note:** CAD = Channel Activity Detection

**Example:**
```cpp
lbm.lorawan.setCSMAParameters(4, false, 6);
```

#### `lbm.lorawan.getCSMAParameters(max_ch_change, bo_enabled, nb_bo_max)`

Get current CSMA parameters.

---

## Network Utilities

### `lbm.lorawan.suspendRadio(suspend)`

Suspend or resume radio communications.

**Parameters:**
- `suspend`: `true` to suspend, `false` to resume

**Returns:** `smtc_modem_return_code_t`

### `lbm.lorawan.getRadioSuspendStatus(suspended)`

Get radio suspension status.

### `lbm.lorawan.getJoinDutyCycleBackoffBypass(enabled)`

Get join duty cycle backoff bypass status.

**Purpose:** LoRaWAN certification testing

### `lbm.lorawan.setJoinDutyCycleBackoffBypass(enable)`

Set join duty cycle backoff bypass.

**Parameters:**
- `enable`: `true` to bypass duty cycle limits, `false` for normal mode

**Note:** Only for certification testing, should be disabled in production

**Example:**
```cpp
// Only for testing
lbm.lorawan.setJoinDutyCycleBackoffBypass(true);
```

---

## Hardware Configuration

### `lbm.lorawan.setCrystalError(crystal_error_ppm)`

Set crystal error (ppm).

**Parameters:**
- `crystal_error_ppm`: Crystal error value

**Returns:** `smtc_modem_return_code_t`

**Example:**
```cpp
lbm.lorawan.setCrystalError(10); // 10ppm error
```

### `lbm.lorawan.getCrystalError(crystal_error_ppm)`

Get crystal error configuration.

---

## Timers

### `lbm.lorawan.startAlarmTimer(alarm_timer_in_s)`

Start an alarm timer.

**Parameters:**
- `alarm_timer_in_s`: Timer duration (seconds, maximum 864000 seconds = 10 days)

**Returns:** `smtc_modem_return_code_t`

**Note:** Generates `SMTC_MODEM_EVENT_ALARM` event when expired

**Example:**
```cpp
// Set 60-second timer
lbm.lorawan.startAlarmTimer(60);
```

### `lbm.lorawan.clearAlarmTimer()`

Stop and clear the alarm timer.

**Returns:** `smtc_modem_return_code_t`

### `lbm.lorawan.getAlarmRemainingTime(remaining_time_in_s)`

Get alarm remaining time.

**Parameters:**
- `remaining_time_in_s`: Output remaining seconds

**Returns:** `smtc_modem_return_code_t`

---

## Event Handling

The modem notifies the application of state changes through an event system. Main events include:

### Event Types

- `SMTC_MODEM_EVENT_RESET`: Modem has reset
- `SMTC_MODEM_EVENT_ALARM`: Alarm timer expired
- `SMTC_MODEM_EVENT_JOINED`: Successfully joined network
- `SMTC_MODEM_EVENT_TXDONE`: Transmission completed
- `SMTC_MODEM_EVENT_DOWNDATA`: Downlink data received
- `SMTC_MODEM_EVENT_JOINFAIL`: Join failed

### Event Handling Example

```cpp
void onModemEvent() {
    smtc_modem_event_t event;
    uint8_t event_pending_count;
    
    while (smtc_modem_get_event(&event, &event_pending_count) == SMTC_MODEM_RC_OK) {
        switch (event.event_type) {
            case SMTC_MODEM_EVENT_RESET:
                Serial.println("Modem reset");
                break;
                
            case SMTC_MODEM_EVENT_JOINED:
                Serial.println("Network joined!");
                break;
                
            case SMTC_MODEM_EVENT_TXDONE:
                Serial.printf("TX done, status: %d\n", event.event_data.txdone.status);
                break;
                
            case SMTC_MODEM_EVENT_DOWNDATA:
                Serial.println("Downlink received");
                // Call getDownlinkData() to retrieve data
                break;
                
            case SMTC_MODEM_EVENT_JOINFAIL:
                Serial.println("Join failed");
                break;
                
            case SMTC_MODEM_EVENT_ALARM:
                Serial.println("Alarm triggered");
                break;
        }
    }
}

void setup() {
    lbm.setEventCallback(onModemEvent);
}
```

---

## Complete Usage Example

```cpp
#include <Arduino.h>
#include "lbm_api.h"

// LoRaWAN credentials
uint8_t dev_eui[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
uint8_t join_eui[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
uint8_t app_key[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                       0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
uint8_t nwk_key[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                       0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

bool joined = false;

void onModemEvent() {
    smtc_modem_event_t event;
    uint8_t event_pending_count;
    
    while (smtc_modem_get_event(&event, &event_pending_count) == SMTC_MODEM_RC_OK) {
        switch (event.event_type) {
            case SMTC_MODEM_EVENT_JOINED:
                Serial.println("✓ Network joined successfully!");
                joined = true;
                break;
                
            case SMTC_MODEM_EVENT_TXDONE:
                Serial.println("✓ Transmission completed");
                break;
                
            case SMTC_MODEM_EVENT_DOWNDATA:
                handleDownlink();
                break;
                
            case SMTC_MODEM_EVENT_JOINFAIL:
                Serial.println("✗ Join failed, retrying...");
                break;
        }
    }
}

void handleDownlink() {
    uint8_t payload[242];
    uint8_t payload_size;
    smtc_modem_dl_metadata_t metadata;
    uint8_t remaining;
    
    if (lbm.lorawan.getDownlinkData(payload, &payload_size, &metadata, &remaining) == SMTC_MODEM_RC_OK) {
        Serial.printf("Downlink: port=%d, size=%d, RSSI=%ddBm, SNR=%ddB\n",
                     metadata.fport, payload_size, 
                     metadata.rssi - 64, metadata.snr / 4);
        
        // Process payload data
        for (int i = 0; i < payload_size; i++) {
            Serial.printf("%02X ", payload[i]);
        }
        Serial.println();
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("RAK3112 LoRaWAN Starting...");
    
    // Initialize modem
    lbm.init();
    lbm.setEventCallback(onModemEvent);
    
    // Configure LoRaWAN parameters
    lbm.lorawan.setDevEUI(dev_eui);
    lbm.lorawan.setJoinEUI(join_eui);
    lbm.lorawan.setAppKey(app_key);
    lbm.lorawan.setNwkKey(nwk_key);
    lbm.lorawan.setRegion(REGION_EU868);
    lbm.lorawan.setClass(SMTC_MODEM_CLASS_A);
    
    // Configure ADR (optional)
    uint8_t dr_dist[16] = {20, 20, 20, 20, 10, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    lbm.lorawan.setJoinDataRateDistribution(dr_dist);
    
    // Start join
    lbm.lorawan.join();
    Serial.println("Join request sent...");
}

void loop() {
    // Must be called periodically
    lbm.runEngine();
    
    // Send data every 30 seconds after joining
    static uint32_t last_send = 0;
    if (joined && (millis() - last_send > 30000)) {
        last_send = millis();
        
        // Prepare payload
        uint8_t payload[] = {0x01, 0x02, 0x03, 0x04};
        
        // Check maximum payload
        uint8_t max_size;
        if (lbm.lorawan.getNextTxMaxPayload(&max_size) == SMTC_MODEM_RC_OK) {
            if (sizeof(payload) <= max_size) {
                // Send data
                lbm.lorawan.send(payload, sizeof(payload), 2, false);
                Serial.println("Uplink sent");
            }
        }
    }
    
    delay(10);
}
```

---

## Return Codes

All API functions return `smtc_modem_return_code_t` type:

- `SMTC_MODEM_RC_OK` (0x00): Command executed successfully
- `SMTC_MODEM_RC_NOT_INIT`: Command not initialized
- `SMTC_MODEM_RC_INVALID`: Invalid parameter
- `SMTC_MODEM_RC_BUSY`: Command cannot be executed at this time
- `SMTC_MODEM_RC_FAIL`: Command execution failed
- `SMTC_MODEM_RC_NO_TIME`: No time available
- `SMTC_MODEM_RC_INVALID_STACK_ID`: Invalid stack_id
- `SMTC_MODEM_RC_NO_EVENT`: No event available

---

## Important Notes

1. **Initialization Order**: Must call `lbm.init()` before configuring other parameters
2. **Periodic Calling**: `lbm.runEngine()` must be called periodically in the main loop
3. **Event Handling**: Register an event callback to handle asynchronous events
4. **Pre-Join Configuration**: DevEUI, JoinEUI, AppKey, NwkKey, Region must be set before join
5. **Duty Cycle**: Regions like Europe have strict duty cycle limits, use `getDutyCycleStatus()` to check
6. **Maximum Payload**: Use `getNextTxMaxPayload()` to check maximum payload at current DR
7. **Debug Output**: Enable `BASIC_MODEM_DEBUG` macro to view detailed debug information

---

## Version Information

- API Version: 1.0
- Based on: Semtech LoRa Basic Modem SWL2001
- Hardware Platform: RAK3112 (ESP32-S3 + SX1262)
- Last Updated: 2025-11-22
