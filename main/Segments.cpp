#include "Segments.h"

/*  Get Segments info in SD
 *  
 *  @param1 Segments struct reference.
 */
int fill_sd_info(Segments& segments) {
  mount();
  char* saved_data = read_file("config");
  std::string data_string(saved_data);

  printf("%s\n", data_string.c_str());
  JsonObject config = JSON::parseObject(data_string);

  if (config.isValid()) {
    segments.name = config.getString("name").c_str();
    segments.description = config.getString("description").c_str();
    segments.info = config.getObject("info").toStringUnformatted().c_str();
    free(saved_data);
  } else {
    segments.name = "Segments";
    segments.description = "Comece a usar o seu segments";
  }
  JSON::deleteObject(config);
  unmount();
  return 0;
}

/*
*	Save config data on SD card
*
*	@param1 JsonObject with config data
*/
int save_config_sd(JsonObject& parsedJson) {
	// TODO: implement AES256 encryption to save data
	/*
		Save new Segments Name
		Save new Segments Description
		Sava new Image b64
		Save new admin RSA pub key
	*/
  mount();

  printf("Saving data...");

  if (save_new_file("config", parsedJson.toStringUnformatted().c_str())) {
  	printf("Everything correct!\n");
  	unmount();
    xTaskCreate(restart_task,"Restart Task", 10000, NULL, 1, NULL);
    
  	return 1;
  } else {
  	printf("Not ok\n");
  }
  JSON::deleteObject(parsedJson);
  unmount();
  return 0;
}

/*
  Factory reset :)
*/
int reset_all() {
  mount();
  save_new_file("config","");
  save_new_file("products","[]");
  save_new_file("pendings","[]");
  save_new_file("waiting", "[]");
  unmount();  

  printf("Resetado!, rebooting...\n");
  xTaskCreate(restart_task,"Restart Task", 10000, NULL, 1, NULL);
  return 0; 
}
/*
  Return public info for user/admin sync
*/
char* get_public_sd_info() {
  mount();

  char* data = read_file("config");

  unmount();
  return data;
}
/*
  Get all products saved
*/
char* get_products_sd_info() {
  mount();

  char* products = read_file("products");

  unmount();
  return products;
}

/*
  Add a new product array
*/
int add_product(JsonObject& parsedJson) {
  mount();
  char* raw_array = read_file("products");
  std::string raw_string_array = raw_array;
  JsonArray json_array = JSON::createArray();

  if (raw_string_array != "err") {
    JSON::deleteArray(json_array);
    json_array = JSON::parseArray(raw_string_array);
    free(raw_array);
  } 

  json_array.addObject(parsedJson);
  save_new_file("products", json_array.toStringUnformatted().c_str());
  JSON::deleteArray(json_array);
 // printf("Saved new object on %s!", fileName);

  unmount();

  return 1;
 // add_object("products", parsedJson);
}

/*
  Get all pendings requests
*/
char* get_pendings_info() {
  mount();
  if (file_exist("pendings")) {
    char* pendings = read_file("pendings");
    unmount();
    return pendings;
  }

  if(save_new_file("pendings", "[]")) {
    //char* pendings = "[]";
    unmount();
    return "[]";
  } else {
    unmount();
    return NULL;
  }
}

/*
  Get all pendings wait requests
*/
char* get_pending_wainting_info() {
  mount();
  if (file_exist("waiting")) {
    char* waitin_list = read_file("waiting");
    unmount();
    return waitin_list;
  }
  if (save_new_file("waiting", "[]")) {
    unmount();
    return "[]";
  } else {
    unmount();
    return NULL;
  }
}

/*
  Add a new pending request to array
*/
int add_new_pending(JsonObject& parsedJson) {
  mount();
  char* raw_array = read_file("pendings");
  std::string raw_string_array = raw_array;
  JsonArray json_array = JSON::createArray();

  if (raw_string_array != "err") {
    JSON::deleteArray(json_array);
    json_array = JSON::parseArray(raw_string_array);
    free(raw_array);
  } 

  json_array.addObject(parsedJson);
  save_new_file("pendings", json_array.toStringUnformatted().c_str());
  JSON::deleteArray(json_array);
 // printf("Saved new object on %s!", fileName);

  unmount();

  return 1;
}
/*
  Add a new pending wait request to array
*/
int add_new_waiting_pending(JsonObject& parsedJson) {
  mount();
  char* raw_array = read_file("waiting");
  std::string raw_string_array = raw_array;
  JsonArray json_array = JSON::createArray();

  if (raw_string_array != "err") {
    JSON::deleteArray(json_array);
    json_array = JSON::parseArray(raw_string_array);
    free(raw_array);
  } 

  json_array.addObject(parsedJson);
  save_new_file("waiting", json_array.toStringUnformatted().c_str());
  JSON::deleteArray(json_array);
 // printf("Saved new object on %s!", fileName);

  unmount();

  return 1;
}

/*
  Restar esp
*/
void restart_task( void * parameter ) {

    printf("Task created, waiting 3 seconds before restart...\n");
    vTaskDelay(500 / portTICK_PERIOD_MS);
    printf("Restarting...\n");
    esp_restart();
}


/*
  Add a new obejct int a json array saved file

  @param1 file name to be saved
  @param2 Parsed json obeject to be added

ESP STARTED DIE WITH THAT (???), was so beatiful...

int add_object(const char* fileName, JsonObject& parsedJson) {
  mount();
  char* raw_array = read_file(fileName);
  std::string raw_string_array = raw_array;
  JsonArray json_array = JSON::createArray();

  if (raw_string_array != "err") {
    JSON::deleteArray(json_array);
    json_array = JSON::parseArray(raw_string_array);
    free(raw_array);
  } 

  json_array.addObject(parsedJson);
  save_new_file(fileName, json_array.toStringUnformatted().c_str());
  JSON::deleteArray(json_array);
 // printf("Saved new object on %s!", fileName);

  unmount();

  return 0;
}

TODO: TEST THAT FUNCTION WITH THE MEMORY MANAGEMENT FIXED
*/