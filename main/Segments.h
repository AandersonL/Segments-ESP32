#ifndef _SEGMENTS_H
#define _SEGMENTS_H


// Local headers
#include "SD.h"

// std & stl headers
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
// JSON header (cpp utils snippets)
#include "JSON.h"

// Esp headers
#include "freertos/FreeRTOS.h"

#define byte unsigned char

struct Segments {
  const char* name;  
  const char* description;
  const char* info;
  unsigned status;
  unsigned clients;
  unsigned transactions;
};



int fill_sd_info(Segments& segments);

int save_config_sd(JsonObject& parsedJson);

int reset_all();

int add_product(JsonObject& parsedJson);
int add_new_pending(JsonObject& parsedJson);
int add_new_waiting_pending(JsonObject& parsedJson);
//int add_object(const char* fileName, JsonObject& parsedJson);

char* get_public_sd_info();
char* get_products_sd_info();
char* get_pendings_info();
char* get_pending_wainting_info();

void restart_task( void * parameter );


#endif
