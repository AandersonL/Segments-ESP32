#ifndef _SD_H
#define _SD_H

// STD headers
#include <stdio.h>
#include <string.h>

// System headers
#include <sys/unistd.h>
#include <sys/stat.h>
#include <sys/dirent.h>

// Esp headers
#include "esp_err.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"

// Os headers
#include "freertos/task.h"

// Drivers/SD headers
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"

/*
	All IO operations in SD card will be here
*/

// Pin mapping when using SPI mode.
// With this mapping, SD card can be used both in SPI and 1-line SD mode.
// Note that a pull-up on CS line is required in SD mode.
#define PIN_NUM_MISO 19
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  18
#define PIN_NUM_CS   5
#define MAX_BUFSIZE 16384


int mount();
int unmount();

int save_new_file(const char* fileName, const char* content);
int append_to_file(const char* fileName, const char* content);
char* read_file(const char* fileName);
int file_exist(const char* fileName);

int rewrite_file(const char* fileName, const char* content);
int update_file(const char* fileName, const char* content);
int erase_file(const char* fileName);

int create_dir(const char* dirName);
int change_dir(const char* dirName);




#endif