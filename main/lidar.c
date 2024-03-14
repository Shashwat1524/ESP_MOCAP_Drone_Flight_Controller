#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"
#include "sensor/common.h"

#define RX_BUF_SIZE 1024
#define HEADER 89

float distance_val; /* actual distance measurements of LiDAR */
unsigned char check; /* save check value */
float sample=0;

#define RXD_PIN GPIO_NUM_16

//static const char *TAG = "RX_TASK";

void rx_task(void *arg)
{
    LidarData_t lidar_data;
    while (1) {
        // Get the buffered data length
        size_t bytes_available = 0;
        uart_get_buffered_data_len(UART_NUM_1, &bytes_available);

        if (bytes_available >= sizeof(uint8_t) * 9) {
            // Read the available data
            uint8_t data[9];
            const int rxBytes = uart_read_bytes(UART_NUM_1, data, sizeof(data), 100 / portTICK_PERIOD_MS);
            
            // Check if enough bytes are read
            if (rxBytes == sizeof(data)) {
                // Check the first byte of the received data
                if (data[0] == HEADER) {
                    // Calculate the check value
                    check = 0;
                    for (int i = 0; i < 8; i++) {
                        check += data[i];
                    }
                    // Verify the check value
                    if (data[8] == check) {
                        // Extract the distance value
                        distance_val = data[2] + data[3] * 256;
                        lidar_data.distance_val = data[2] + data[3] * 256;

                        // Print the distance value
                        //ESP_LOGI(TAG, "dist = %.2f", distance_val);
                        xQueueSend(lidar_data_queue, &lidar_data, portMAX_DELAY);
                    }
                }
            }
        }
        // Flush the UART buffer to clear any remaining data
        uart_flush_input(UART_NUM_1);
        

        // Reset watchdog timer and yield to other tasks
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

