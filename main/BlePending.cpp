#include "BlePending.h"

PendingCallback::PendingCallback() {
	lastMsgLen = sending = 0;
}

/*
	Receive a pending request, and returns all the pendings
	@param1 pCharacteristic
*/

void PendingCallback::onRead(BLECharacteristic* pCharacteristic) {
	
  if (sending) {
    unsigned i, j;
    memset(bufferToSend,0, CHUNK_LIMIT);
    for (i = 0, j = lastMsgLen; (i < CHUNK_LIMIT && j <= json.length() ); i++,j++) {
      bufferToSend[i] = json[j]; 
    }
    sender = bufferToSend;
    lastMsgLen = j;
    
    if (lastMsgLen >= json.length())  {
      bufferToSend[json.length()] = '\0';
      sending = 0;
    }
    pCharacteristic->setValue(sender);
  } else {
    char* raw_json_info = get_pendings_info();
    std::string raw_json_string = raw_json_info;
    free(raw_json_info);
    JsonArray json_array = JSON::parseArray(raw_json_string);
    //if (json)
    if (json_array.toStringUnformatted().length() > CHUNK_LIMIT) {// break the bytes
      sending = 1;
      json = json_array.toStringUnformatted();
      
      for ( unsigned i = 0; i < CHUNK_LIMIT; i++ ) {
        bufferToSend[i] = json[i];
      }
      lastMsgLen = CHUNK_LIMIT;
      sender = bufferToSend;
      pCharacteristic->setValue(sender);
      
    } else {
      pCharacteristic->setValue(json_array.toStringUnformatted());
    }
    JSON::deleteArray(json_array);
  }
}

/*
	Receive a pending request, and save the new pending
	@param1 pCharacteristic
*/

void PendingCallback::onWrite(BLECharacteristic* pCharacteristic) {
	printf("%d\n", strlen(raw_json));

	if (strlen(raw_json) <= JSON_CONFIG_BUFFER) {
		strcat(raw_json, pCharacteristic->getValue().c_str());
	} else {
		memset(raw_json, 0, JSON_CONFIG_BUFFER);
	}

	try {
		JsonObject new_pending = JSON::parseObject(raw_json);
		if (new_pending.isValid()) {
      if (add_new_pending(new_pending)) 
        printf("Saved new pending!");
      memset(raw_json, 0x00, JSON_CONFIG_BUFFER);
		}
		
	} catch(const char* msg) {
		printf("Error: %s\n", msg);
	}
}



/*
	Update new pending list 
*/

void PendingWaitingUpdateCallback::onWrite(BLECharacteristic* pCharacteristic) {
	printf("%d\n", strlen(raw_json));

	if (strlen(raw_json) <= JSON_CONFIG_BUFFER) {
		strcat(raw_json, pCharacteristic->getValue().c_str());
	} else {
		memset(raw_json, 0, JSON_CONFIG_BUFFER);
	}

  try {
    JsonObject new_pending_wait = JSON::parseObject(raw_json);
    if (new_pending_wait.isValid()) {
      if (add_new_waiting_pending(new_pending_wait)) 
        printf("Saved new pending! wait");
      memset(raw_json, 0x00, JSON_CONFIG_BUFFER);
    }
    
  } catch(const char* msg) {
    printf("Error: %s\n", msg);
  }
}

/*
	Update wait list 
*/
void PendingWaitingUpdateCallback::onRead(BLECharacteristic* pCharacteristic) {
  if (sending) {
    unsigned i, j;
    memset(bufferToSend,0, CHUNK_LIMIT);
    for (i = 0, j = lastMsgLen; (i < CHUNK_LIMIT && j <= json.length() ); i++,j++) {
      bufferToSend[i] = json[j]; 
    }
    sender = bufferToSend;
    lastMsgLen = j;
    
    if (lastMsgLen >= json.length())  {
      bufferToSend[json.length()] = '\0';
      sending = 0;
    }
    pCharacteristic->setValue(sender);
  } else {
    char* raw_json_info = get_pending_wainting_info();
    std::string raw_json_string = raw_json_info;
    free(raw_json_info);
    JsonArray json_array = JSON::parseArray(raw_json_string);
    
    if (json_array.toStringUnformatted().length() > CHUNK_LIMIT) {// break the bytes
      sending = 1;
      json = json_array.toStringUnformatted();
      
      for ( unsigned i = 0; i < CHUNK_LIMIT; i++ ) {
        bufferToSend[i] = json[i];
      }
      lastMsgLen = CHUNK_LIMIT;
      sender = bufferToSend;
      pCharacteristic->setValue(sender);
      
    } else {
      pCharacteristic->setValue(json_array.toStringUnformatted());
    }
    JSON::deleteArray(json_array);
  }
}


/*
  Same thing as above, but only for user
*/

void PendingWaitUserCallback::onRead(BLECharacteristic* pCharacteristic) {
   if (sending) {
    unsigned i, j;
    memset(bufferToSend,0, CHUNK_LIMIT);
    for (i = 0, j = lastMsgLen; (i < CHUNK_LIMIT && j <= json.length() ); i++,j++) {
      bufferToSend[i] = json[j]; 
    }
    sender = bufferToSend;
    lastMsgLen = j;
    
    if (lastMsgLen >= json.length())  {
      bufferToSend[json.length()] = '\0';
      sending = 0;
    }
    pCharacteristic->setValue(sender);
  } else {
    char* raw_json_info = get_pending_wainting_info();
    std::string raw_json_string = raw_json_info;
    free(raw_json_info);
    JsonArray json_array = JSON::parseArray(raw_json_string);
    
    if (json_array.toStringUnformatted().length() > CHUNK_LIMIT) {// break the bytes
      sending = 1;
      json = json_array.toStringUnformatted();
      
      for ( unsigned i = 0; i < CHUNK_LIMIT; i++ ) {
        bufferToSend[i] = json[i];
      }
      lastMsgLen = CHUNK_LIMIT;
      sender = bufferToSend;
      pCharacteristic->setValue(sender);
      
    } else {
      pCharacteristic->setValue(json_array.toStringUnformatted());
    }
    JSON::deleteArray(json_array);
  } 
}



/*

  YOU CAN REFACTORE THIS, WHEN YOU HAVE TIME

*/