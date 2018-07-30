#include "SD.h"

/*
	Mount sd card
*/
int mount() {
    ESP_LOGI("Segments", "Mouting SD");

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    sdspi_slot_config_t slot_config = SDSPI_SLOT_CONFIG_DEFAULT();
    slot_config.gpio_miso = (gpio_num_t) PIN_NUM_MISO;
    slot_config.gpio_mosi = (gpio_num_t) PIN_NUM_MOSI;
    slot_config.gpio_sck  = (gpio_num_t) PIN_NUM_CLK;
    slot_config.gpio_cs   = (gpio_num_t) PIN_NUM_CS;
    host.max_freq_khz = SDMMC_FREQ_PROBING;

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = true,
        .max_files = 5
    };

    sdmmc_card_t* card;
    esp_err_t ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE("Segments", "Failed to mount filesystem. "
                "If you want the card to be formatted, set format_if_mount_failed = true.");
        } else {
            ESP_LOGE("Segments", "Failed to initialize the card (%d). "
                "Make sure SD card lines have pull-up resistors in place.", ret);
        }
        return 0;
      } 

    // Card has been initialized, print its properties
    sdmmc_card_print_info(stdout, card);

    ESP_LOGI("Segments", "SD Mounted and ready for use");
	return 1;
}

/*
	Unmount sd card
*/
int unmount() {
	esp_vfs_fat_sdmmc_unmount();
	ESP_LOGI("Segments", "SD unmounted");
	return 0;
}

/*
	Create a new file with content
	
	@param1 - file name to be saved
	@param2 - bytes to write inside
*/
int save_new_file(const char* fileName, const char* content) {
	char path[64]; // 64 bytes for path
	strcpy(path,"/sdcard/");
	strcat(path, fileName);
	
	FILE* file = fopen(path, "w");
	if (file == NULL) return 0;
	fprintf(file,"%s",content);
	fclose(file);

	return 1;
}


/*
	Append data to a file
	@param1 - fileName
	@param2 - content
*/
int append_to_file(const char* fileName, const char* content) {
	char path[64];
	strcpy(path,"/sdcard/");
	strcat(path, fileName);

	FILE* file = fopen(path, "a");
	if (file == NULL) return 0;
	fprintf(file, "%s", content);
	fclose(file);
	return 1;
}
/*
	Read a file 

	@param1 - fileName
*/

char* read_file(const char* fileName) {
	unsigned int buff;
	
	char path[64];
	char *data;
	strcpy(path, "/sdcard/");
	strcat(path, fileName);

	FILE* file = fopen(path, "r");

	if (file == NULL) return "err";
	fseek(file, 0, SEEK_END);
	buff = ftell(file);
	fseek(file, 0, SEEK_SET);
	buff++;
	printf("Buffer: %d\n", buff);
	data = (char*) malloc(buff);
	fgets(data, buff, file);
	printf("Content: %s\n", data);
	fclose(file);
	return data;
}


int file_exist(const char* fileName) {
	unsigned int buff;
	
	char path[64];
	char *data;
	strcpy(path, "/sdcard/");
	strcat(path, fileName);

	FILE* file = fopen(path, "r");

	if (file == NULL) return 0;

	fclose(file);
	return 1;
}
int rewrite_file(const char* fileName, const char* content) {
	return 0;
}

int update_file(const char* fileName, const char* content) {
	return 0;
}

int erase_file(const char* fileName) {
	return 0;
}

int create_dir(const char* dirName) {
	return 0;
}

int change_dir(const char* dirName) {
	return 0;
}
