#include <stdint.h>
#include "sensor/mpu_filter.h"
#include "pwm.h"
#include "esp_log.h"
#include "sensor/common.h"
#include "lidar.h"

// Define your desired target angles
float target_roll=0, target_pitch=0, target_yaw=0;
float roll1=0, pitch1=0, yaw1=0;
float distance_value;

// PID constants for each axis
float Kp_roll = 3, Ki_roll = 0.003, Kd_roll = 0;
float Kp_pitch =3, Ki_pitch = 0.003, Kd_pitch = 0;
float Kp_yaw = 0, Ki_yaw = 0.00, Kd_yaw =0;

// Previous errors for derivative term
float previous_error_roll=0, previous_error_pitch=0, previous_error_yaw=0;
float sum_error_roll=0, sum_error_pitch=0, sum_error_yaw=0;
float esc_values_1,esc_values_2,esc_values_3,esc_values_4;
static const char *TAG = "PWM";

// Function to constrain values within a range
float constrain(float value, float min, float max) {
      if (value < min) {
        return min;
      } else if (value > max) {
        return max;
      } else {
        return value;
      }
    }

float constrain_0(float value, float min, float max) {
      if (value < min) {
        return 0;
      } else if (value > max) {
        return 0;
      } else {
        return value;
      }
    }

void pid_controller(){

    IMUData_t received_data;
    LidarData_t received_lidar_data;

    // Define initial throttle value
    float throttle = 500;

    while(1){
        if (xQueueReceive(imu_data_queue, &received_data, portMAX_DELAY) == pdPASS) {
            roll1 = received_data.roll;
            pitch1 = received_data.pitch;
            yaw1 = received_data.yaw;
        }

        if (xQueueReceive(lidar_data_queue, &received_lidar_data, portMAX_DELAY) == pdTRUE) {
            distance_value= received_lidar_data.distance_val;
        }

        float error_roll = target_roll - roll1;
        float error_pitch = target_pitch - pitch1;
        float error_yaw = target_yaw - yaw1;

        sum_error_roll+=error_roll;
        sum_error_pitch+=error_pitch;
        sum_error_yaw+=error_yaw;

        // Constrain sum errors
        sum_error_roll=constrain(sum_error_roll,-400/Ki_roll,400/Ki_roll);
        sum_error_pitch=constrain(sum_error_pitch,-400/Ki_pitch,400/Ki_pitch);
        sum_error_yaw=constrain(sum_error_yaw,-400/Ki_yaw,400/Ki_yaw);

        // PID calculations for each axis
        float P_roll = Kp_roll * error_roll;
        float I_roll = Ki_roll * sum_error_roll; 
        float D_roll = Kd_roll * (error_roll - previous_error_roll);
        previous_error_roll = error_roll; 

        float P_pitch = Kp_pitch * error_pitch;
        float I_pitch = Ki_pitch * sum_error_pitch; 
        float D_pitch = Kd_pitch * (error_pitch - previous_error_pitch);
        previous_error_pitch = error_pitch; 

        float P_yaw = Kp_yaw * error_yaw;
        float I_yaw = Ki_yaw * sum_error_yaw; 
        float D_yaw = Kd_yaw * (error_yaw - previous_error_yaw);
        previous_error_yaw = error_yaw; 

        // Calculate PID output for each axis
        float output_roll = constrain(P_roll + I_roll + D_roll,-400,400);
        float output_pitch = constrain(P_pitch + I_pitch + D_pitch,-400,400);
        float output_yaw = constrain(P_yaw + I_yaw + D_yaw,-400,400);

        // Update ESC values based on PID output
        esc_values_1 = constrain(throttle - output_roll - output_pitch + output_yaw,100,1000);
        esc_values_2 = constrain((throttle + output_roll - output_pitch - output_yaw),100,1000);
        esc_values_3 = constrain((throttle - output_roll + output_pitch - output_yaw),100,1000);
        esc_values_4 = constrain((throttle + output_roll + output_pitch + output_yaw),100,1000);

        // Print lidar distance
        ESP_LOGI(TAG, "LIDAR_DISTANCE=%f",distance_value);
        
        // Send ESC data to queue
        send_esc_data_to_queue(esc_values_1,esc_values_2,esc_values_3,esc_values_4);

        // Increase throttle until distance_value reaches 35
        if (distance_value < 30) {
            throttle += 2; // Increase throttle by a small amount
            throttle = constrain(throttle, 500, 800); // Ensure throttle stays within range
        } else if (distance_value >= 30) {
            // Cut off throttle if distance_value exceeds 40
            throttle -=2;
        }
        // Delay before next iteration
        vTaskDelay(pdMS_TO_TICKS(1));
    } 
}

