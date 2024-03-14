idf_component_register(
    SRCS 
        "sensor/mpu_filter.cpp"
        "drone_flight_control.c"
        "esc_controller.c"
        "dshot/dshot_esc_encoder.c"
        "pwm.c"
        "sensor/common.c"
        "lidar.c"
        
       
        
        
    INCLUDE_DIRS 
        "." 
        "sensor" 
        "dshot"
       
)
