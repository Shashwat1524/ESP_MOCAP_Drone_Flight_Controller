#include "esc_controller.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/rmt_tx.h"
#include "dshot/dshot_esc_encoder.h"
#include "esc_controller.h"
#include "sensor/common.h"
#include "lidar.h"


#if CONFIG_IDF_TARGET_ESP32H2
#define DSHOT_ESC_RESOLUTION_HZ 32000000 // 32MHz resolution, DSHot protocol needs a relative high resolution
#else
#define DSHOT_ESC_RESOLUTION_HZ 40000000 // 40MHz resolution, DSHot protocol needs a relative high resolution
#endif

#define DSHOT_ESC_GPIO_NUM_1 32
#define DSHOT_ESC_GPIO_NUM_2 26
#define DSHOT_ESC_GPIO_NUM_3 33
#define DSHOT_ESC_GPIO_NUM_4 25

static const char *TAG = "ESC_controller";
float stored_esc_values[4];
float count=0;
float initial=0;
float lidar_distance=0;

rmt_channel_handle_t esc_chan1 = NULL;
rmt_channel_handle_t esc_chan2 = NULL;
rmt_channel_handle_t esc_chan3 = NULL;
rmt_channel_handle_t esc_chan4 = NULL;

rmt_encoder_handle_t dshot_encoder1 = NULL;
rmt_encoder_handle_t dshot_encoder2 = NULL;
rmt_encoder_handle_t dshot_encoder3 = NULL;
rmt_encoder_handle_t dshot_encoder4 = NULL;

rmt_transmit_config_t tx_config = {
    .loop_count = -1,  // infinite loop   
    };

    dshot_esc_throttle_t throttle1 = {
        .throttle = 0,
        .telemetry_req = false,  // telemetry is not supported in this example
    };

    dshot_esc_throttle_t throttle2 = {
        .throttle = 0,
        .telemetry_req = false,  // telemetry is not supported in this example
    };

    dshot_esc_throttle_t throttle3 = {
        .throttle = 0,
        .telemetry_req = false,  // telemetry is not supported in this example
    };

    dshot_esc_throttle_t throttle4 = {
        .throttle = 0,
        .telemetry_req = false,  // telemetry is not supported in this example
    };


void dshot_esc_init(void) {
    
    ESP_LOGI(TAG, "Create RMT TX channel");
    rmt_channel_handle_t esc_chan1 = NULL;
    rmt_channel_handle_t esc_chan2 = NULL;
    rmt_channel_handle_t esc_chan3 = NULL;
    rmt_channel_handle_t esc_chan4 = NULL;
    rmt_tx_channel_config_t tx_chan_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,  // select a clock that can provide needed resolution
        .gpio_num = DSHOT_ESC_GPIO_NUM_1,
        .mem_block_symbols = 64,
        .resolution_hz = DSHOT_ESC_RESOLUTION_HZ,
        .trans_queue_depth = 10,  // set the number of transactions that can be pending in the background
    };
    printf("Created RMT TX Channels");

    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &esc_chan1));
    tx_chan_config.gpio_num = DSHOT_ESC_GPIO_NUM_2;
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &esc_chan2));
    tx_chan_config.gpio_num = DSHOT_ESC_GPIO_NUM_3;
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &esc_chan3));
    tx_chan_config.gpio_num = DSHOT_ESC_GPIO_NUM_4;
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &esc_chan4));

    ESP_LOGI(TAG, "Install Dshot ESC encoder");
    rmt_encoder_handle_t dshot_encoder1 = NULL;
    rmt_encoder_handle_t dshot_encoder2 = NULL;
    rmt_encoder_handle_t dshot_encoder3 = NULL;
    rmt_encoder_handle_t dshot_encoder4 = NULL;
    dshot_esc_encoder_config_t encoder_config = {
        .resolution = DSHOT_ESC_RESOLUTION_HZ,
        .baud_rate = 300000,  // DSHOT300 protocol
        .post_delay_us = 50,  // extra delay between each frame
    };

    ESP_ERROR_CHECK(rmt_new_dshot_esc_encoder(&encoder_config, &dshot_encoder1));
    ESP_ERROR_CHECK(rmt_new_dshot_esc_encoder(&encoder_config, &dshot_encoder2));
    ESP_ERROR_CHECK(rmt_new_dshot_esc_encoder(&encoder_config, &dshot_encoder3));
    ESP_ERROR_CHECK(rmt_new_dshot_esc_encoder(&encoder_config, &dshot_encoder4));

    ESP_LOGI(TAG, "Enable RMT TX channel");

    ESP_ERROR_CHECK(rmt_enable(esc_chan1));
    ESP_ERROR_CHECK(rmt_enable(esc_chan2));
    ESP_ERROR_CHECK(rmt_enable(esc_chan3));
    ESP_ERROR_CHECK(rmt_enable(esc_chan4));  

ESP_LOGI(TAG, "Start ESCs by sending zero throttle for a while...");

// Transmit zero throttle to all motors simultaneously
ESP_ERROR_CHECK(rmt_transmit(esc_chan1, dshot_encoder1, &throttle1, sizeof(throttle1), &tx_config));
ESP_ERROR_CHECK(rmt_transmit(esc_chan2, dshot_encoder2, &throttle2, sizeof(throttle2), &tx_config));
ESP_ERROR_CHECK(rmt_transmit(esc_chan3, dshot_encoder3, &throttle3, sizeof(throttle3), &tx_config));
ESP_ERROR_CHECK(rmt_transmit(esc_chan4, dshot_encoder4, &throttle4, sizeof(throttle4), &tx_config));

// Delay to allow ESCs to initialize
vTaskDelay(pdMS_TO_TICKS(10000));
ESCData_t received_data_ESC;
LidarData_t received_lidar_data;

    
    while(1)
    {
    if (xQueueReceive(esc_data_queue, &received_data_ESC, portMAX_DELAY) == pdPASS) {
            stored_esc_values[0] = received_data_ESC.esc_values_1;
            stored_esc_values[1] = received_data_ESC.esc_values_2;
            stored_esc_values[2] = received_data_ESC.esc_values_3;
            stored_esc_values[3] = received_data_ESC.esc_values_4;
        }

    if (xQueueReceive(lidar_data_queue, &received_lidar_data, portMAX_DELAY) == pdTRUE) {
            lidar_distance = received_lidar_data.distance_val;
        }

    initial++;
    if ((count <= 150 && initial > 150) && lidar_distance < 35)
    {
    
    /*throttle1.throttle = stored_esc_values[0];
    throttle2.throttle = stored_esc_values[2];
    throttle3.throttle = stored_esc_values[3];
    throttle4.throttle = stored_esc_values[1];*/

    throttle1.throttle = stored_esc_values[3];
    throttle2.throttle = stored_esc_values[1];
    throttle3.throttle = stored_esc_values[0];
    throttle4.throttle = stored_esc_values[2];

    //ESP_LOGI(TAG, "ESC Values: %.2f, %.2f, %.2f, %.2f", stored_esc_values[0], stored_esc_values[1], stored_esc_values[2], stored_esc_values[3]);

    /*throttle1.throttle = 000; //top left -A --------------- esc
    throttle2.throttle = 000; //bottom left -C---------------  esc
    throttle3.throttle = 000; //bottom right -D---------------- esc
    throttle4.throttle = 100; //top right ---------------------esc       */
   
    count++;
    }
    else{
        throttle1.throttle = 00;
        throttle2.throttle = 00;
        throttle3.throttle = 000;
        throttle4.throttle = 00;
    }

    ESP_LOGI(TAG, "Transmitting throttle values: %.2lf, %.2lf, %.2lf, %.2lf", (double)throttle1.throttle, (double)throttle2.throttle, (double)throttle3.throttle, (double)throttle4.throttle);


    ESP_ERROR_CHECK(rmt_transmit(esc_chan1, dshot_encoder1, &throttle1, sizeof(throttle1), &tx_config));
    ESP_ERROR_CHECK(rmt_transmit(esc_chan2, dshot_encoder2, &throttle2, sizeof(throttle2), &tx_config));
    ESP_ERROR_CHECK(rmt_transmit(esc_chan3, dshot_encoder3, &throttle3, sizeof(throttle3), &tx_config));
    ESP_ERROR_CHECK(rmt_transmit(esc_chan4, dshot_encoder4, &throttle4, sizeof(throttle4), &tx_config));


    // Disable and re-enable channels for each update (as done in the original code)
    ESP_ERROR_CHECK(rmt_disable(esc_chan1));
    ESP_ERROR_CHECK(rmt_enable(esc_chan1));
    ESP_ERROR_CHECK(rmt_disable(esc_chan2));
    ESP_ERROR_CHECK(rmt_enable(esc_chan2));
    ESP_ERROR_CHECK(rmt_disable(esc_chan3));
    ESP_ERROR_CHECK(rmt_enable(esc_chan3));
    ESP_ERROR_CHECK(rmt_disable(esc_chan4));
    ESP_ERROR_CHECK(rmt_enable(esc_chan4));

    }

    vTaskDelay(pdMS_TO_TICKS(1));
}