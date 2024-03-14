#ifdef __cplusplus
extern "C" {
#endif

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

typedef struct {
    float roll;
    float pitch;
    float yaw;
} IMUData_t;

typedef struct {
    float esc_values_1;
    float esc_values_2;
    float esc_values_3;
    float esc_values_4;
} ESCData_t;

typedef struct {
    float distance_val;
} LidarData_t;


extern QueueHandle_t imu_data_queue;
extern QueueHandle_t esc_data_queue;
extern QueueHandle_t lidar_data_queue;


void imu_data_init();
void send_imu_data_to_queue(float roll, float pitch, float yaw);
void esc_data_init();
void send_esc_data_to_queue(float esc1,float esc2,float esc3,float esc4);

#ifdef __cplusplus
}
#endif
