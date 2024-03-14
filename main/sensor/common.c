#include "common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/message_buffer.h"
#include "esp_log.h"
#include "esp_err.h"

// Define the global variables
QueueHandle_t imu_data_queue;
QueueHandle_t esc_data_queue;
QueueHandle_t lidar_data_queue;
static const char *TAG = "FAILED";

// Implement functions declared in common.h

void imu_data_init() {
    imu_data_queue = xQueueCreate(5, sizeof(IMUData_t));  // Adjust the queue size as needed
}

void send_imu_data_to_queue(float roll, float pitch, float yaw) {
    IMUData_t imu_data;
    imu_data.roll = roll;
    imu_data.pitch = pitch;
    imu_data.yaw = yaw;

    if (xQueueSend(imu_data_queue, &imu_data, portMAX_DELAY) != pdPASS) {
        ESP_LOGI(TAG, "Data send Failed");
    }
}

void esc_data_init() {
    esc_data_queue = xQueueCreate(5, sizeof(ESCData_t));  // Adjust the queue size as needed
}

void send_esc_data_to_queue(float esc1, float esc2, float esc3, float esc4) {
    ESCData_t esc_values;
    esc_values.esc_values_1 = esc1;
    esc_values.esc_values_2 = esc2;
    esc_values.esc_values_3 = esc3;
    esc_values.esc_values_4 = esc4;

    if (xQueueSend(esc_data_queue, &esc_values, portMAX_DELAY) != pdPASS) {
        ESP_LOGI(TAG, "ESC Data send Failed");
    }
}

