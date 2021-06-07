#ifndef SENSOR_H
#define SENSOR_H

#include "all_headers.h"

class Sensor {
    private:
        events::EventQueue &_event_queue;
        
    public:
        Sensor(events::EventQueue &event_queue);
        void getData();
        void init_params();
        void calibrate();

        int16_t _pAccDataXYZ[3] = {0};
        float _pGyroDataXYZ[3] = {0};
        int16_t _AccOffset[3] = {0};
        float _GyroOffset[3] = {0};
        int8_t leftButton;
        int8_t rightButton;
        int8_t topButton;
};

#endif