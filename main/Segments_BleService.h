#ifndef _SEGMENTS_BLESERVICE_H
#define _SEGMENTS_BLESERVICE_H

/// BLE Headers (cpp utils snippets)

#include "BLEUtils.h"
#include "BLEServer.h"
#include "BLEDevice.h"

// JSON header (cpp utils snippets)
#include "JSON.h"

// std & esp headers
#include <stdio.h>
#include <string.h>
#include <string>
#include <esp_log.h>


// gpio/freertos headers
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"


// local libraries

#include "Segments.h"
#include "BleProduct.h"
#include "BlePending.h"

#include "sdkconfig.h"


#define byte unsigned char
#define JSON_CONFIG_BUFF 4096

#define ACCESS_UUID "f7a036c9-603f-46bd-a832-6bc84a1d9298"
#define CONFIG_INFO_UUID "9adcafc0-a1eb-4bea-a666-059a4876eb77"

#define IMAGE_SERVICE_UUID "29df3293-5e1c-4b85-9ce4-9b01d4f4d50f"
#define IMAGE_CHARACT_UUID "7ea076b5-436d-4262-8509-5697f374f5e6"

#define INFO_UUID "403f7874-4739-48f8-a6df-a5b1f40bd583"
#define INFO_CHAR_UUID "f4f2e1ac-d3a6-42f3-bf84-9716a62d558d"

#define RESET_SERVICE_UUID "6c678185-7d56-471c-aed0-8e6ff75230f5"
#define RESET_CHAR_UUID "b55bfe49-9ef9-47f8-94c5-613dba4724e1"

class ConfigCallback : public BLECharacteristicCallbacks {
public:
	void onWrite(BLECharacteristic *pCharacteristic);
  void onRead(BLECharacteristic *pCharacteristic);

private:
	char raw_json[JSON_CONFIG_BUFF] = { 0 };
};

class InfoCallback : public BLECharacteristicCallbacks {
public:
	void onRead(BLECharacteristic *pCharacteristic);
};


class ResetCallback : public BLECharacteristicCallbacks {
public:
  void onWrite(BLECharacteristic* pCharacteristic);
};

class MyServerCallbacks: public BLEServerCallbacks {
  // TODO this doesn't take into account several clients being connected
    void onConnect(BLEServer* ble_server) {
		printf("Conectaram\n");
		BLEAdvertising* ble_advertising = ble_server->getAdvertising();
  		ble_advertising->start();
    };
    void onDisconnect(BLEServer* pServer) {
   //   bleConnected = false;
    }
};




void init_ble(Segments& segments);
void start_services(BLEServer* server);
#endif
