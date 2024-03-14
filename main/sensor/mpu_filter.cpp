#include "math.h"
#include "mpu_filter.h"

#include <eigen3/Eigen/Dense>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/message_buffer.h"
#include "esp_log.h"
#include "esp_err.h"
#include "driver/i2c.h"
static const char *TAG = "IMU";

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

// Source: https://github.com/arduino-libraries/MadgwickAHRS
#include "MadgwickAHRS.h"
#include "common.h"

#define RAD_TO_DEG (180.0/M_PI)
#define DEG_TO_RAD 0.0174533

MPU6050 mpu;
Madgwick madgwick;

// Accel & Gyro scale factor
float accel_sensitivity;
float gyro_sensitivity;
//IMUData_t imu_data; 

//void rotateAxes(float& roll, float& pitch, float& yaw);





// Get scaled value
void _getMotion6(float *_ax, float *_ay, float *_az, float *_gx, float *_gy, float *_gz) {
	int16_t ax,ay,az;
	int16_t gx,gy,gz;
	// read raw accel/gyro measurements from device
	// The accelerometer output is a 16-bit signed integer relative value.
	// The gyroscope output is a relative value in degrees per second (dps).
	mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

	// Convert relative to absolute
#if 1
	*_ax = (float)ax / accel_sensitivity;
	*_ay = (float)ay / accel_sensitivity;
	*_az = (float)az / accel_sensitivity;
#else
	*_ax = (float)ax;
	*_ay = (float)ay;
	*_az = (float)az;
#endif

	// Convert relative degree per second to absolute radian per second
#if 1
	*_gx = (float)gx / gyro_sensitivity;
	*_gy = (float)gy / gyro_sensitivity;
	*_gz = (float)gz / gyro_sensitivity;
#else
	*_gx = (float)gx;
	*_gy = (float)gy;
	*_gz = (float)gz;
#endif
}

// Get time in seconds since boot
// Compatible with ROS's time.toSec() function
double TimeToSec() {
	int64_t _time = esp_timer_get_time(); // Get time in microseconds since boot
	double __time = (double)_time / 1000000;
	return __time;
}

void mpu6050(void *pvParameters){
	// Initialize mpu6050
	mpu.initialize();

	// Get DeviceID
	uint8_t devid = mpu.getDeviceID();
	ESP_LOGI(TAG, "devid=0x%x", devid);

	// Get the sample rate
	ESP_LOGI(TAG, "getRate()=%d", mpu.getRate());
	// Set the sample rate to 8kHz
	if (mpu.getRate() != 0) mpu.setRate(0);

	// Get FSYNC configuration value
	ESP_LOGI(TAG, "getExternalFrameSync()=%d", mpu.getExternalFrameSync());
	// Disable FSYNC and set 260 Hz Acc filtering, 256 Hz Gyro filtering
	if (mpu.getExternalFrameSync() != 0) mpu.setExternalFrameSync(0);

	// Set Digital Low Pass Filter
	ESP_LOGI(TAG, "getDLPFMode()=%d", mpu.getDLPFMode());
	if (mpu.getDLPFMode() != 6) mpu.setDLPFMode(6);

	// Get Accelerometer Scale Range
	ESP_LOGI(TAG, "getFullScaleAccelRange()=%d", mpu.getFullScaleAccelRange());
	// Set Accelerometer Full Scale Range to ±2g
	if (mpu.getFullScaleAccelRange() != 0) mpu.setFullScaleAccelRange(0); // -2 --> +2[g]
	accel_sensitivity = 16384.0; // g

	// Get Gyro Scale Range
	ESP_LOGI(TAG, "getFullScaleGyroRange()=%d", mpu.getFullScaleGyroRange());
	// Set Gyro Full Scale Range to ±250deg/s
	if (mpu.getFullScaleGyroRange() != 0) mpu.setFullScaleGyroRange(0); // -250 --> +250[Deg/Sec]
	gyro_sensitivity = 131.0; // Deg/Sec

	double last_time_ = TimeToSec();
	

	mpu.setXAccelOffset(-1660);
	mpu.setYAccelOffset(-771);
	mpu.setZAccelOffset(1335);
	mpu.setXGyroOffset(96);
	mpu.setYGyroOffset(-18);
	mpu.setZGyroOffset(15);

	/*mpu.setXAccelOffset(-5594);
	mpu.setYAccelOffset(1978);
	mpu.setZAccelOffset(1388);
	mpu.setXGyroOffset(68);
	mpu.setYGyroOffset(-164);
	mpu.setZGyroOffset(-36);*/




	while(1){
		// Get scaled value
		float ax, ay, az;
		float gx, gy, gz;
		_getMotion6(&ax, &ay, &az, &gx, &gy, &gz);


		// Get the elapsed time from the previous
		float dt = (TimeToSec() - last_time_);
		//ESP_LOGD(TAG, "dt=%f",dt);
		last_time_ = TimeToSec();
		//switchAxes(ax, ay, az, gx, gy, gz);

		// Get Euler
		madgwick.updateIMU(gx, gy, gz, ax, ay, az, dt);
		float roll = madgwick.getRoll();
		float pitch = madgwick.getPitch();
		float yaw = madgwick.getYaw();

		IMUData_t imu_data;
		imu_data.roll = -pitch;
		imu_data.pitch = -roll;
		imu_data.yaw =(yaw-180);

		send_imu_data_to_queue(imu_data.roll, imu_data.pitch, imu_data.yaw);
		ESP_LOGI(TAG, "roll=%f pitch=%f yaw=%f", imu_data.roll, imu_data.pitch, imu_data.yaw);

		vTaskDelay(10);
	} // end while

	// Never reach here
	vTaskDelete(NULL);
}