#include "BleProduct.h"



ProductAddCallback::ProductAddCallback() {
  sending = lastMsgLen = 0;
}

/*
  Receive a json object with new product infomartion
  @param1 - pCharacteristic
*/
void ProductAddCallback::onWrite(BLECharacteristic* pCharacteristic) {
  
  printf("%d\n", strlen(raw_json));
  if (strlen(raw_json) <= JSON_CONFIG_BUFF)  // 256 bytes is the limit
    strcat(raw_json, pCharacteristic->getValue().c_str());
  else {
    memset(raw_json, 0x00, JSON_CONFIG_BUFF);
    return ;
  }

  try {
    JsonObject product = JSON::parseObject(raw_json);
    if (product.isValid()) {
      if (add_product(product))
        printf("New product saved with success!\n");

      memset(raw_json, 0x00, JSON_CONFIG_BUFF);
      printf("Passed\n");
    }

  } catch(const char* msg) {
    printf("Error: %s\n", msg);
  }

 // printf("%s\n", pCharacteristic->getValue().c_str());
 // pCharacteristic->setValue("");
}

/*
  Return all products in raw json format
*/

void ProductAddCallback::onRead(BLECharacteristic* pCharacteristic) {
  if (sending) {
    unsigned i, j;
    memset(bufferToSend,0, CHUNK_LIMIT);
    for (i = 0, j = lastMsgLen; (i < CHUNK_LIMIT && j <= json.length() ); i++,j++) {
      bufferToSend[i] = json[j]; 
    }
    sender = bufferToSend;
    lastMsgLen = j;
    
    if (lastMsgLen >= json.length())  {
      sending = 0;
    }
  //  printf("Enviando -> %s\n", sender.c_str());
    pCharacteristic->setValue(sender.c_str());
  } else {
    char* raw_json_info = get_products_sd_info();
    std::string raw_json_string = raw_json_info;
   
    JsonArray json_array = JSON::parseArray(raw_json_string);
    
    if (json_array.toStringUnformatted().length() > CHUNK_LIMIT) {// break the bytes
      sending = 1;
      json = json_array.toStringUnformatted();
      
      for ( unsigned i = 0; i < CHUNK_LIMIT; i++ ) {
        bufferToSend[i] = json[i];
      }
      lastMsgLen = CHUNK_LIMIT;
      sender = bufferToSend;
      printf("Enviando -> %s\n", sender.c_str());
      pCharacteristic->setValue(sender.c_str());
      
    } else {
      pCharacteristic->setValue(json_array.toStringUnformatted());
    }
     JSON::deleteArray(json_array);
     free(raw_json_info);
  }
}
