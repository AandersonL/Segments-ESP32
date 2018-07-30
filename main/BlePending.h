#ifndef _BLE_PENDING_H
#define _BLE_PENDING_H

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

// local
#include "Segments.h"


#define JSON_CONFIG_BUFFER 4096
#define CHUNK_LIMIT 500

#define PENDING_SERVICE_UUID "98aab6e5-4b4b-4760-91b8-46b1f3615340"
#define PENDING_CHAR_UUID "1025d294-6f7b-4710-8197-21bba467c50d"

#define PENDING_UPDATE_SERVICE_UUID "ae36d807-91ad-4015-8367-14e558b6883a"
#define PENDING_UPDATE_CHAR_UUID "78654110-bbdc-4e59-897f-71b63712843d"

#define PENDING_UPDATE_SERVICE_USER_UUID "ecc318c0-7963-45ba-a582-4dcc77faebcd"
#define PENDING_UPDATE_CHAR_USER_UUID "491c64fb-8fb4-4a06-8f97-d5f5ee1efeac"

class PendingCallback : public BLECharacteristicCallbacks {
public:
	PendingCallback();
	void onRead(BLECharacteristic* pCharacteristic);
	void onWrite(BLECharacteristic* pCharacteristic);
private:
	char raw_json[JSON_CONFIG_BUFFER] = { 0 };
	std::string json;
	std::string sender;
	char bufferToSend[CHUNK_LIMIT] = { 0 };
	int sending;
	int lastMsgLen;
};

class PendingWaitingUpdateCallback : public BLECharacteristicCallbacks  {
public:
	void onWrite(BLECharacteristic* pCharacteristic);
	void onRead(BLECharacteristic* pCharacteristic);
private:
	char raw_json[JSON_CONFIG_BUFFER] = { 0 };
	std::string json;
	std::string sender;
	char bufferToSend[CHUNK_LIMIT] = { 0 };
	int sending;
	int lastMsgLen;

};

class PendingWaitUserCallback : public BLECharacteristicCallbacks {
public:
	void onRead(BLECharacteristic* pCharacteristic);
private:
	char raw_json[JSON_CONFIG_BUFFER] = { 0 };
	std::string json;
	std::string sender;
	char bufferToSend[CHUNK_LIMIT] = { 0 };
	int sending;
	int lastMsgLen;
};

#endif