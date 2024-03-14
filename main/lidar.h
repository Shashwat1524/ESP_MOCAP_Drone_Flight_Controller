#ifndef LIDAR_H_
#define LIDAR_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"

#define HEADER 89
#define RX_BUF_SIZE 1024
#define RXD_PIN GPIO_NUM_16

extern float distance_val;
extern unsigned char check;

void rx_task(void *arg);

#endif /* LIDAR_H_ */
