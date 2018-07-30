#include "Segments.h"
#include "SD.h"
#include "Segments_BleService.h"
#include "JSON.h"

extern "C" {
	void app_main(void);
}



void app_main(void) {

  Segments segments;

  //segments.name = "hue";
  fill_sd_info(segments);
  init_ble(segments);

  printf("Bluetooth online\nName: %s\n", segments.name);
}// app_main
