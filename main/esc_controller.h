

#ifndef ESC_CONTROLLER_H
#define ESC_CONTROLLER_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/rmt_tx.h"
#include "dshot/dshot_esc_encoder.h"

#define DSHOT_ESC_GPIO_NUM_1 32
#define DSHOT_ESC_GPIO_NUM_2 26
#define DSHOT_ESC_GPIO_NUM_3 33
#define DSHOT_ESC_GPIO_NUM_4 25

extern rmt_transmit_config_t tx_config;
extern dshot_esc_throttle_t throttle1;
extern dshot_esc_throttle_t throttle2;
extern dshot_esc_throttle_t throttle3;
extern dshot_esc_throttle_t throttle4;



void dshot_esc_init(void);
void dshot_esc_control(void);

#endif  // ESC_CONTROLLER_H
