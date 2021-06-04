#include "sensor.h"

#define DownScaler 0.001 //adjust units of acceleration read from STM

DigitalIn BUTTON_RIGHT(D0);  
DigitalIn BUTTON_LEFT(D1); 
DigitalIn BUTTON_TOP(D2); 


Sensor::Sensor(events::EventQueue &event_queue) : _event_queue(event_queue){
    
    BSP_ACCELERO_Init();    
    BSP_GYRO_Init();
    calibrate();
    init_params();
    
}

void Sensor::calibrate(){
    printf("Calibrating Sensors\n");
    printf("Please don't move your board\n");
    int n = 0;
    _AccOffset[0] = _AccOffset[1] = _AccOffset[2] = 0;
    _GyroOffset[0] = _GyroOffset[1] = _GyroOffset[2] = 0;
    while(n < 50){
        BSP_ACCELERO_AccGetXYZ(_pAccDataXYZ);
        BSP_GYRO_GetXYZ(_pGyroDataXYZ);
        for(int i = 0; i < 3; ++i){
            _AccOffset[i] += _pAccDataXYZ[i];
            _GyroOffset[i] += _pGyroDataXYZ[i];
        }

        n++;
    }
    for(int i = 0; i < 3; ++i){
        _AccOffset[i] /= n;
        _GyroOffset[i] /= n;
    }

    printf("Calibration done\n");
    printf("Acc offset: %d , %d, %d\n ", _AccOffset[0], _AccOffset[1], _AccOffset[2]);
    printf("Gyro offset: %f, %f, %f\n ", _GyroOffset[0], _GyroOffset[1], _GyroOffset[2]);
}

void Sensor::getData()
{
    itemFront = BUTTON_RIGHT.read();
    itemBack = BUTTON_LEFT.read();
    drift = BUTTON_TOP.read();
    acc = 1;
    calcJump();
    calcDirection();
}

void Sensor:: calcDirection()
{
        BSP_ACCELERO_AccGetXYZ(_pAccDataXYZ);
        if ( ((_pAccDataXYZ[0]-_AccOffset[0])*DownScaler) > 0.6)
            left = 1;

        if ( ((_pAccDataXYZ[0]-_AccOffset[0])*DownScaler) < -0.6)
            right = 1;
        // printf("left: %d, right: %d\n", left, right);
}

void Sensor:: calcJump() 
    {
        BSP_ACCELERO_AccGetXYZ(_pAccDataXYZ);
        if ( ((_pAccDataXYZ[1]-_AccOffset[1])*DownScaler) > 0.5){
            jump = 1;
        }

    }

void Sensor:: init_params(){
    
    left = 0;
    right = 0;
    jump = 0;
    itemFront = 0;
    itemBack = 0;
    drift = 0;
    acc = 0;

}