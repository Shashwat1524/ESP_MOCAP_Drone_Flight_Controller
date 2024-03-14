#ifndef PWM
#define PWM_H

#include <stdint.h>
#include "sensor/mpu_filter.h"
#include "esp_log.h"

// Define your desired target angles
extern float target_roll, target_pitch, target_yaw;
extern float roll1, pitch1, yaw1; //change these three
extern float throttle; //change this

// PID constants for each axis
extern float Kp_roll, Ki_roll, Kd_roll;
extern float Kp_pitch, Ki_pitch, Kd_pitch;
extern float Kp_yaw, Ki_yaw, Kd_yaw;
extern float esc_values[3];
extern float dt_ms;

// Previous errors for derivative term
extern float previous_error_roll, previous_error_pitch, previous_error_yaw;

extern float esc_values_1;
extern float esc_values_2;
extern float esc_values_3;
extern float esc_values_4;



// Function to constrain values within a range
float constrain(float value, float min, float max);

// PID function prototype
void pid_controller();

#endif // PWM_H
