#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/message_buffer.h"
#include "esp_log.h"

#include "driver/i2c.h"

#include "sensor/mpu_filter.h"
#include "esc_controller.h"

#include "sensor/common.h"
#include "pwm.h"
#include "mocap.h"
#include "lidar.h"

void start_i2c(void);
void uart_init(void);
void dshot_controller_task(void *pvParameters);

void app_main(void)
{
    start_i2c();
    uart_init();

    imu_data_init();
    esc_data_init();

    lidar_data_queue = xQueueCreate(1, sizeof(LidarData_t));

    xTaskCreate(&mpu6050, "IMU", 1024*8, NULL, 5, NULL);
    xTaskCreate(&pid_controller, "PWM", 1024 * 8, NULL, 5, NULL);
    xTaskCreate(&dshot_controller_task, "ESC CONTROLLER", 1024 * 8, NULL, 5, NULL);
    xTaskCreate(&rx_task, "uart_rx_task", 1024 * 8, NULL, 8, NULL);
}

void start_i2c(void) {
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = (gpio_num_t)21;
    conf.scl_io_num = (gpio_num_t)22;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 400000;
    conf.clk_flags = 0;
    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0));
}

void uart_init(void)
{
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, UART_PIN_NO_CHANGE, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

void dshot_controller_task(void *pvParameters) {
    dshot_esc_init();
}
