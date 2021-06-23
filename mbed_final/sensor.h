#ifndef SENSOR_H
#define SENSOR_H

#include "all_headers.h"

class Sensor {
    private:
        events::EventQueue &_event_queue;
        
    public:
        Sensor(events::EventQueue &event_queue);
        void getData();
        void judge_motion();
        void init_params();
        void calibrate();

        int16_t _pAccDataXYZ[3] = {0};
        float _pGyroDataXYZ[3] = {0};
        int16_t prev_pAccDataXYZ[3] = {0};
        float prev_pGyroDataXYZ[3] = {0};
        
        int16_t _AccOffset[3] = {0};
        float _GyroOffset[3] = {0};
        
        float  acc_motion_threshold = 50;
        float  gyro_motion_threshold = 200;
        int8_t judge_motion_count = 0;
        int8_t judge_motion_threshold = 10;
        
        int8_t leftButton;
        int8_t rightButton;
        int8_t topButton;

        bool motion_occur = false;
};

#endif