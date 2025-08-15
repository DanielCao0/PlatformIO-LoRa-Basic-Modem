# RAK3112 LoRaWAN Basic Modem

A complete LoRaWAN communication solution for RAK3112 module based on ESP32-S3 and SX1262, implementing Semtech LBM (LoRa Basic Modem) protocol stack.

## 📊 Status Monitoring

### Serial Output Example

```
RAK3112 LoRaWAN Started...
LoRaWAN Task Created
LoRaWAN Task Started...
INFO: Modem Initialization
Attaching IRQ handler for pin 47
INFO: Use soft secure element for cryptographic functionalities
stack_id 0
 DevNonce = 148
 JoinNonce = 0x67 00 00, NetID = 0x00 00 00
 Region = EU868
LoRaWAN Certification is disabled on stack 0
Modem event callback
INFO: Event received: RESET
INFO: smtc_modem_join_network
 Start a new join sequence in 2 seconds on stack 0
DevEUI - (8 bytes):
 00 00 00 00 00 00 00 01
JoinEUI - (8 bytes):
 00 00 00 00 00 00 00 01
DevNonce 0x95, stack_id 0

  *************************************
  * Send Payload  for stack_id = 0
  *************************************
  Tx  LoRa at 5554 ms: freq:868300000, SF7, BW125, len 23 bytes 14 dBm, fcnt_up 0, toa = 62

  *************************************
  *  TX DONE
  *************************************

  Open RX1 for Hook Id = 1  RX1 LoRa at 10620 ms: freq:868300000, SF7, BW125, sync word = 0x34
  Timer will expire in 4964 ms

  *************************************
  * Receive a Valid downlink RX1 for stack_id = 0
  *************************************
 update join procedure
MacTxFrequency [0] = 867100000
MacDataRateChannel [0] =  1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
MacChannelIndexEnabled [0] = 1
MacTxFrequency [1] = 867300000
MacDataRateChannel [1] =  1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
MacChannelIndexEnabled [1] = 1
MacTxFrequency [2] = 867500000
MacDataRateChannel [2] =  1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
MacChannelIndexEnabled [2] = 1
MacTxFrequency [3] = 867700000
MacDataRateChannel [3] =  1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
MacChannelIndexEnabled [3] = 1
MacTxFrequency [4] = 867900000
MacDataRateChannel [4] =  1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
MacChannelIndexEnabled [4] = 1
 DevAddr= db94e7
 MacRx1DataRateOffset= 0
 MacRx2DataRate= 0
 MacRx1Delay= 3
Modem event callback
INFO: Event received: JOINED
INFO: Modem is now joined
INFO: add send task
 User LoRaWAN tx on FPort 101

  *************************************
  * Send Payload  for stack_id = 0
  *************************************
  Tx  LoRa at 14980 ms: freq:868500000, SF7, BW125, len 17 bytes 14 dBm, fcnt_up 1, toa = 52

  *************************************
  *  TX DONE
  *************************************

  Open RX1 for Hook Id = 1  RX1 LoRa at 18036 ms: freq:868500000, SF7, BW125, sync word = 0x34
  Timer will expire in 2991 ms

  *************************************
  * Receive a Valid downlink RX1 for stack_id = 0
  *************************************
1/1 - Cmd link_adr_parser = 52 ff 00 01
1 - LINK ADR REQ , channel mask = 0xff , ChMaskCntl = 0x0
MacTxDataRateAdr = 5
MacTxPower = 10
MacNbTrans = 1
Receive a dev status req
 .
  *************************************
  * Send Payload  for stack_id = 0
  *************************************
  Tx  LoRa at 19576 ms: freq:867900000, SF7, BW125, len 18 bytes 10 dBm, fcnt_up 2, toa = 52

  *************************************
  *  TX DONE
  *************************************

  Open RX1 for Hook Id = 1  RX1 LoRa at 22633 ms: freq:867900000, SF7, BW125, sync word = 0x34
  Timer will expire in 2990 ms

  *************************************
  * Receive a Valid downlink RX1 for stack_id = 0
  *************************************
1/1 - Cmd link_adr_parser = 55 ff 00 01
1 - LINK ADR REQ , channel mask = 0xff , ChMaskCntl = 0x0
MacTxDataRateAdr = 5
MacTxPower = 4
MacNbTrans = 1
 .
  *************************************
  * Send Payload  for stack_id = 0
  *************************************
  Tx  LoRa at 25056 ms: freq:867500000, SF7, BW125, len 15 bytes 4 dBm, fcnt_up 3, toa = 47

  *************************************
  *  TX DONE
  *************************************

  Open RX1 for Hook Id = 1  RX1 LoRa at 28102 ms: freq:867500000, SF7, BW125, sync word = 0x34
  Timer will expire in 2985 ms

  *************************************
  * Receive a Valid downlink RX1 for stack_id = 0
  *************************************
1/1 - Cmd link_adr_parser = 57 ff 00 01
1 - LINK ADR REQ , channel mask = 0xff , ChMaskCntl = 0x0
MacTxDataRateAdr = 5
MacTxPower = 0
MacNbTrans = 1
 .
  *************************************
  * Send Payload  for stack_id = 0
  *************************************
  Tx  LoRa at 31092 ms: freq:867100000, SF7, BW125, len 15 bytes 0 dBm, fcnt_up 4, toa = 47

  *************************************
  *  TX DONE
  *************************************

  Open RX1 for Hook Id = 1  RX1 LoRa at 34138 ms: freq:867100000, SF7, BW125, sync word = 0x34
  Timer will expire in 2986 ms

  *************************************
  * RX1 Timeout for stack_id = 0
  *************************************

  Open RX2 for Hook Id = 1  RX2 LoRa at 35174 ms: freq:869525000, SF12, BW125, sync word = 0x34
  Timer will expire in 1005 ms

  *************************************
  * RX2 Timeout for stack_id = 0
  *************************************
Modem event callback
INFO: Event received: TXDONE
INFO: Transmission done
```

