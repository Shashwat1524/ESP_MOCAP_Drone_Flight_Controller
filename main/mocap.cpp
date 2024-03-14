#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include <eigen3/Eigen/Geometry>
#include <math.h>
#include "mocap.h"

#include <uros_network_interfaces.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <phasespace_msgs/msg/rigids.h>
#include <phasespace_msgs/msg/rigid.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#ifdef CONFIG_MICRO_ROS_ESP_XRCE_DDS_MIDDLEWARE
#include <rmw_microros/rmw_microros.h>
#endif

#define RCCHECK(fn)                  
    {                                
        rcl_ret_t temp_rc = fn;      
        if ((temp_rc != RCL_RET_OK)) 
        {                            
            printf("Failed status on line %d: %d. Aborting.\n", __LINE__, (int)temp_rc); 
            vTaskDelete(NULL);       
        }                            
    }

#define RCSOFTCHECK(fn)                  
    {                                   
        rcl_ret_t temp_rc = fn;         
        if ((temp_rc != RCL_RET_OK))    
        {                               
            printf("Failed status on line %d: %d. Continuing.\n", __LINE__, (int)temp_rc); 
        }                               
    }

rcl_subscription_t subscriber;
phasespace_msgs__msg__Rigids *recv_msg;
phasespace_msgs__msg__Rigids msg;
bool quaternionReceived = false;
float qw_value, qx_value, qy_value, qz_value;
void printQuaternions();

void subscription_callback(const void *msg)
{
    if (!msg)
    {
        printf("Received NULL message in subscription callback\n");
        return;
    }
    const struct phasespace_msgs__msg__Rigids *rigids_msg = (const struct phasespace_msgs__msg__Rigids *)msg;

    for (size_t i = 0; i < rigids_msg->rigids.size; i++)
    {
        const struct phasespace_msgs__msg__Rigid *rigid_msg = &rigids_msg->rigids.data[i];

        if (rigid_msg->id == 1)
        {
            qw_value = rigid_msg->qw;
            qx_value = rigid_msg->qx;
            qy_value = rigid_msg->qy;
            qz_value = rigid_msg->qz;
            printf("Orientation (quaternion): (w: %f, x: %f, y: %f, z: %f)\n", rigid_msg->qw, rigid_msg->qx, rigid_msg->qy, rigid_msg->qz);
            quaternionReceived = true;
        }
        printQuaternions();
    }
}

void printEulerAngles(float roll, float pitch, float yaw)
{
    //printf("Roll (rad): %f\nPitch (rad): %f\nYaw (rad): %f\n", roll, pitch, yaw);

   
    float roll_deg = roll * 180.0 / M_PI;
    float pitch_deg = pitch * 180.0 / M_PI;
    float yaw_deg = yaw * 180.0 / M_PI;

    //printf("Roll (deg): %f\nPitch (deg): %f\nYaw (deg): %f\n", roll_deg, pitch_deg, yaw_deg);
}
void printQuaternions()
{
    if (quaternionReceived)
    {
        Eigen::Quaternionf q(qw_value, qx_value, qy_value, qz_value);

        q = q * Eigen::AngleAxisf(M_PI / 2.0, Eigen::Vector3f::UnitX());

        Eigen::Matrix3f rotationMatrix = q.toRotationMatrix(); // Convert to rotation matrix
        Eigen::Vector3f eulerAngles = rotationMatrix.eulerAngles(0, 1, 2); // Extract ZYX Euler angles

        printEulerAngles(eulerAngles(0), eulerAngles(1), eulerAngles(2)); // Print Euler angles
    }
    
    else
    {
        printf("Quaternions not received yet.\n");
    }
}

void micro_ros_task(void *arg)
{
    rcl_allocator_t allocator = rcl_get_default_allocator();
    rclc_support_t support;

    recv_msg = (phasespace_msgs__msg__Rigids *)malloc(sizeof(phasespace_msgs__msg__Rigids));

    if (!recv_msg)
    {
        printf("Memory allocation error for recv_msg\n");
        vTaskDelete(NULL);
    }

    recv_msg->rigids.capacity = 10;
    recv_msg->rigids.size = 0;
    recv_msg->rigids.data = (phasespace_msgs__msg__Rigid *)malloc(recv_msg->rigids.capacity * sizeof(phasespace_msgs__msg__Rigid));

    for (size_t i = 0; i < recv_msg->rigids.capacity; i++)
    {
        recv_msg->rigids.data[i].id = 0;
        recv_msg->rigids.data[i].flags = 0;
        recv_msg->rigids.data[i].time = 0;
        recv_msg->rigids.data[i].x = 0.0;
        recv_msg->rigids.data[i].y = 0.0;
        recv_msg->rigids.data[i].z = 0.0;
        recv_msg->rigids.data[i].qw = 0.0;
        recv_msg->rigids.data[i].qx = 0.0;
        recv_msg->rigids.data[i].qy = 0.0;
        recv_msg->rigids.data[i].qz = 0.0;
        recv_msg->rigids.data[i].cond = 0.0;
    }

    // Create init_options.
    rcl_init_options_t init_options = rcl_get_zero_initialized_init_options();
    RCCHECK(rcl_init_options_init(&init_options, allocator));

#ifdef CONFIG_MICRO_ROS_ESP_XRCE_DDS_MIDDLEWARE
    rmw_init_options_t *rmw_options = rcl_init_options_get_rmw_init_options(&init_options);

    // Static Agent IP and port can be used instead of autodiscovery.
    RCCHECK(rmw_uros_options_set_udp_address(CONFIG_MICRO_ROS_AGENT_IP, CONFIG_MICRO_ROS_AGENT_PORT, rmw_options));
    //RCCHECK(rmw_uros_discover_agent(rmw_options));
#endif

    // Setup support structure.
    RCCHECK(rclc_support_init_with_options(&support, 0, NULL, &init_options, &allocator));

    // Create node.
    rcl_node_t node = rcl_get_zero_initialized_node();
    RCCHECK(rclc_node_init_default(&node, "esp", "", &support));

    // Create subscriber.
    RCCHECK(rclc_subscription_init_default(
        &subscriber,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(phasespace_msgs, msg, Rigids),
        "phasespace/rigids"));

    printf("Subscriber initialized successfully.\n");

    // Create executor.
    rclc_executor_t executor = rclc_executor_get_zero_initialized_executor();
    RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
    unsigned int rcl_wait_timeout = 1000; // in ms
    RCCHECK(rclc_executor_set_timeout(&executor, RCL_MS_TO_NS(rcl_wait_timeout)));

    // Add subscriber to executor.
    RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, recv_msg, &subscription_callback, ON_NEW_DATA));

    while (1)
    {
        rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
        usleep(100000);
    }

    // Free resources.
    RCCHECK(rcl_subscription_fini(&subscriber, &node));
    RCCHECK(rcl_node_fini(&node));

    vTaskDelete(NULL);
}
