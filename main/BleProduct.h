#ifndef _BLE_PRODUCT_H
#define _BLE_PRODUCT_H

/// BLE Headers (cpp utils snippets)

#include "BLEUtils.h"
#include "BLEServer.h"
#include "BLEDevice.h"

// std & esp headers
#include <stdio.h>
#include <string.h>
#include <string>
#include <esp_log.h>

// JSON

#include "JSON.h"
// Local
#include "Segments.h"

#define JSON_CONFIG_BUFF 4096
#define CHUNK_LIMIT 500 

#define PRODUCT_SERVICE_UUID "850b03cd-0b9c-4ed0-b894-71af216911c2"
#define PRODUCT_CHAR_UUID "4a6f301d-8b8b-4c23-a8a4-e82637cb5cdd"

class ProductAddCallback : public BLECharacteristicCallbacks {
public:
	ProductAddCallback();
	void onRead(BLECharacteristic *pCharacteristic);
	void onWrite(BLECharacteristic *pCharacteristic);
private:
	char raw_json[JSON_CONFIG_BUFF] = { 0 };
	std::string json;
	std::string sender;
	char bufferToSend[CHUNK_LIMIT] = { 0 };
	int sending;
	int lastMsgLen;
};


#endif