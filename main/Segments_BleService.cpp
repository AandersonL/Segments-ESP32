 #include "Segments_BleService.h"


/*
  ConfigCallBack implementation
  @param1 BLECharacteristic
*/
void ConfigCallback::onWrite(BLECharacteristic *pCharacteristic) {
  /*
    Ble data is splited on 20 bytes on each msg, 
    so we concat our data while json is not valid
  */  
  printf("%d\n", strlen(raw_json)); 
  
  if (strlen(raw_json) <= JSON_CONFIG_BUFF)  // 256 bytes is the limit
    strcat(raw_json, pCharacteristic->getValue().c_str());
  else {
    memset(raw_json, 0x00, JSON_CONFIG_BUFF);
    return ;
  }
  
  pCharacteristic->setValue("");
  try {
    JsonObject config_json = JSON::parseObject(raw_json);
    if (config_json.isValid()) {    
      save_config_sd(config_json);
      memset(raw_json, 0x00, JSON_CONFIG_BUFF);
    }
  } catch (const char* error) {
    printf("Error: %s\n", error);
  }
}

/*
  ConfigCallback onRead implementation
  @param1 BLECharacteristic
*/

void ConfigCallback::onRead(BLECharacteristic* pCharacteristic) {
  char* raw_sd_json = get_public_sd_info();
  std::string raw_string_json = raw_sd_json;
  JsonObject information = JSON::parseObject(raw_string_json);
  
  if (information.isValid()) {
    pCharacteristic->setValue(information.toStringUnformatted());
  }
  
  else 
    pCharacteristic->setValue("{'error':'Sem informação disponivel'}");

  JSON::deleteObject(information);
  free(raw_sd_json);

}


/*
  ResetCallback onWrite implementation
  @param1 BLECharacteristic
*/

void ResetCallback::onWrite(BLECharacteristic* pCharacteristic) {
  int option = atoi(pCharacteristic->getValue().c_str());

  /*
    1 - Reset
  */
  switch(option) {
    case 1: 
      if(reset_all())printf("rebooting...\n");break;
  }
}

 
/*
	Start bluetooth low energy
	@param1 - Segments struct reference
*/
void init_ble(Segments& segments) {
  
  // Initialize blue enviroment 
  BLEDevice::init(segments.name); 
  
  // Create a server and services
  BLEServer* ble_server = BLEDevice::createServer();
  ble_server->setCallbacks(new MyServerCallbacks);
  // Access/Config Service ( admin only, check pub key )
  start_services(ble_server);

  BLEAdvertising* ble_advertising = ble_server->getAdvertising();
  ble_advertising->start();
}


/*
  Start Bluetooth services
  @param1 - BLEServer pointer
*/

void start_services(BLEServer* ble_server) {
  // Admin config service
  BLEService *ble_admin_access = ble_server->createService(BLEUUID(ACCESS_UUID));
  BLECharacteristic* ble_admin_char =
    ble_admin_access->createCharacteristic(BLEUUID(CONFIG_INFO_UUID), 
      BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);
  
  ble_admin_char->setCallbacks(new ConfigCallback);
  ble_admin_access->start();


  // Product service

  BLEService* ble_product_upload = ble_server->createService(BLEUUID(PRODUCT_SERVICE_UUID));
  BLECharacteristic* ble_product_char = 
    ble_product_upload->createCharacteristic(BLEUUID(PRODUCT_CHAR_UUID),
      BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);

  ble_product_char->setCallbacks(new ProductAddCallback);
  ble_product_upload->start();
  
  // Pending service

  BLEService* ble_pending_service = ble_server->createService(BLEUUID(PENDING_SERVICE_UUID));
  BLECharacteristic* ble_pending_char =
    ble_pending_service->createCharacteristic(BLEUUID(PENDING_CHAR_UUID),
      BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);  
  
  ble_pending_char->setCallbacks(new PendingCallback);
  ble_pending_service->start();

  // Pending waiting confirm Service
  BLEService* ble_update_pending_update_service = ble_server->createService(BLEUUID(PENDING_UPDATE_SERVICE_UUID));
  BLECharacteristic* ble_pending_update_char =
    ble_update_pending_update_service->createCharacteristic(BLEUUID(PENDING_UPDATE_CHAR_UUID),
      BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);

  ble_pending_update_char->setCallbacks(new PendingWaitingUpdateCallback);
  ble_update_pending_update_service->start();

  // User, this looks not a good thing to do
  BLEService* ble_pending_wait_user = ble_server->createService(BLEUUID(PENDING_UPDATE_SERVICE_USER_UUID));
  BLECharacteristic* ble_pending_wait_user_char = 
    ble_pending_wait_user->createCharacteristic(BLEUUID(PENDING_UPDATE_CHAR_USER_UUID),
      BLECharacteristic::PROPERTY_READ);
  ble_pending_wait_user_char->setCallbacks(new PendingWaitUserCallback);
  ble_pending_wait_user->start();
  // Status service


  // Reset service

  BLEService* ble_reset_service = ble_server->createService(BLEUUID(RESET_SERVICE_UUID));
  BLECharacteristic* ble_reset_char = 
    ble_reset_service->createCharacteristic(BLEUUID(RESET_CHAR_UUID),
      BLECharacteristic::PROPERTY_WRITE);

  ble_reset_char->setCallbacks(new ResetCallback);
  ble_reset_service->start();
  // Confirmation service
}