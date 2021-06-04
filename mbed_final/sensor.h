#ifndef SENSOR_H
#define SENSOR_H

#include "all_headers.h"

class Sensor {
    private:
        events::EventQueue &_event_queue;
        int   _AccOffset[3] = {};
        float _GyroOffset[3] = {};
        
    public:
        Sensor(events::EventQueue &event_queue);
        void calibrate();
        void calcDirection();
        void calcJump();
        void getData();
        void init_params();

        int16_t _pAccDataXYZ[3] = {0};
        float _pGyroDataXYZ[3] = {0};
        int8_t left;
        int8_t right;
        int8_t jump;
        int8_t itemFront;
        int8_t itemBack;
        int8_t drift;
        int8_t acc;

};

#endif