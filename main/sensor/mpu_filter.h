#ifndef MPU_FILTER_H
#define MPU_FILTER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// Define the data structure for IMU values
/*typedef struct {
    float roll;
    float pitch;
    float yaw;
} IMUData_t;

extern IMUData_t imu_data; */



void mpu6050(void *pvParameters); 


// Function to get the latest IMU data



#ifdef __cplusplus
}
#endif

#endif  // MPU_FILTER_H
