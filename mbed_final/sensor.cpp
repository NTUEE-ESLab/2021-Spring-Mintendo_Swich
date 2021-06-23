#include "sensor.h"
#include <cmath>

// #define DownScaler 0.001 //adjust units of acceleration read from STM

DigitalIn BUTTON_RIGHT(D0);  
DigitalIn BUTTON_LEFT(D1); 
DigitalIn BUTTON_TOP(D2); 


Sensor::Sensor(events::EventQueue &event_queue) : _event_queue(event_queue){
    
    BSP_ACCELERO_Init();    
    BSP_GYRO_Init();
    init_params();
    calibrate();
    
}

void Sensor::calibrate(){
    printf("Calibrating Sensors\n");
    printf("Please don't move your board\n");
    int n = 0;
    _AccOffset[0] = _AccOffset[1] = _AccOffset[2] = 0;
    _GyroOffset[0] = _GyroOffset[1] = _GyroOffset[2] = 0;
    while(n < 1000){
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
    leftButton = ((BUTTON_LEFT.read())?1:0);
    rightButton = (BUTTON_RIGHT.read()?1:0);
    topButton = (BUTTON_TOP.read()?1:0) ;
    BSP_ACCELERO_AccGetXYZ(_pAccDataXYZ);
    BSP_GYRO_GetXYZ(_pGyroDataXYZ);
}

void Sensor::judge_motion(){
    if (judge_motion_count<judge_motion_threshold){
        judge_motion_count++;
        motion_occur = true;
    }
    else{
        for (int i=0 ;i<3; i++){
            if(abs(prev_pAccDataXYZ[i]-_pAccDataXYZ[i]) > acc_motion_threshold || abs(prev_pGyroDataXYZ[i]-_pGyroDataXYZ[i])>gyro_motion_threshold){
                motion_occur = true;
                break;
            }       
        }
    }
    
}

void Sensor:: init_params(){
    leftButton = 0;
    rightButton = 0;
    topButton = 0;
    motion_occur = false;

    for(int i=0; i<3; i++){
        prev_pAccDataXYZ[i] = _pAccDataXYZ[i];
        prev_pGyroDataXYZ[i] = _pGyroDataXYZ[i];
    }

}