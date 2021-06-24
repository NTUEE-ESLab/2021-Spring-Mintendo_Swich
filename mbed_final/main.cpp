#include "all_headers.h"
#include "sensor.h"
#include "wifi.h"


#define WIFI_IDW0XX1    2

#if (defined(TARGET_DISCO_L475VG_IOT01A) || defined(TARGET_DISCO_F413ZH))
#include "ISM43362Interface.h"
ISM43362Interface wifi(false);

#else // External WiFi modules

#if MBED_CONF_APP_WIFI_SHIELD == WIFI_IDW0XX1
#include "SpwfSAInterface.h"
SpwfSAInterface wifi(MBED_CONF_APP_WIFI_TX, MBED_CONF_APP_WIFI_RX);
#endif // MBED_CONF_APP_WIFI_SHIELD == WIFI_IDW0XX1

#endif


static EventQueue event_queue( 16 * EVENTS_EVENT_SIZE);

static BufferedSerial serial_port(USBTX, USBRX);
FileHandle *mbed::mbed_override_console(int fd)
{
    return &serial_port; 
}


TCPSocket socket;
Sensor sensor(event_queue);
WIFI   _wifi(&wifi, event_queue);
bool is_connect = true;



///ISR
Timeout top_disable_irq;
Timeout right_disable_irq;
Timeout left_disable_irq;

InterruptIn TOP(D2);
//InterruptIn RIGHT(D0);
//InterruptIn LEFT(D1);

void top_enable_IRQ();
void top_rise_handler();
void top_fall_handler();
/*
void right_enable_IRQ();
void right_rise_handler();
void right_fall_handler();

void left_enable_IRQ();
void left_rise_handler();
void left_fall_handler();
*/

int main()
{   
    
    //_wifi.connect(&socket);
    

    printf("------------------------\n");
    printf("Mario Kart game Start ><\n");
    printf("------------------------\n");

    TOP.rise(top_rise_handler);
    TOP.fall(top_fall_handler);
   /*
    RIGHT.rise(right_rise_handler);
    RIGHT.fall(right_fall_handler);
    LEFT.rise(left_rise_handler);
    LEFT.fall(left_fall_handler);
    */


    event_queue.call_every(50ms,&sensor, &Sensor::getData);
    event_queue.call_every(50ms,&sensor, &Sensor::judge_motion);
    event_queue.call_every(50ms,&_wifi, &WIFI::send_data,&sensor, &is_connect);
    event_queue.call_every(50ms,&sensor, &Sensor::init_params);


    event_queue.dispatch_forever();

    while(is_connect) {};
    if (!is_connect) return 0;
}


//ISR

void top_enable_IRQ(){
    TOP.enable_irq();
};


void top_rise_handler(){
    TOP.disable_irq();
    event_queue.call(&sensor, &Sensor::getData);
    sensor.topButton=1;
    event_queue.call(&_wifi, &WIFI::send_data, &sensor, &is_connect);
    event_queue.call(&sensor, &Sensor::init_params);
    event_queue.call(printf, "ISR event triggered\n");
    top_disable_irq.attach(&top_enable_IRQ,20ms); 
};


void top_fall_handler(){
    TOP.disable_irq();
    event_queue.call(&sensor, &Sensor::getData);
    sensor.topButton=0;
    event_queue.call(&_wifi, &WIFI::send_data, &sensor, &is_connect);
    event_queue.call(&sensor, &Sensor::init_params);
    event_queue.call(printf, "ISR event triggered\n");
    top_disable_irq.attach(&top_enable_IRQ,20ms);
    
};

/*
//right
void right_enable_IRQ(){
    RIGHT.enable_irq();
};


void right_rise_handler(){
    RIGHT.disable_irq();
    event_queue.call(&sensor, &Sensor::getData);
    sensor.rightButton=1;
    event_queue.call(&_wifi, &WIFI::send_data, &sensor, &is_connect);
    event_queue.call(&sensor, &Sensor::init_params);
    event_queue.call(printf, "ISR event triggered\n");
    right_disable_irq.attach(&right_enable_IRQ,20ms); 
};


void right_fall_handler(){
    RIGHT.disable_irq();
    event_queue.call(&sensor, &Sensor::getData);
    sensor.rightButton=0;
    event_queue.call(&_wifi, &WIFI::send_data, &sensor, &is_connect);
    event_queue.call(&sensor, &Sensor::init_params);
    event_queue.call(printf, "ISR event triggered\n");
    right_disable_irq.attach(&right_enable_IRQ,20ms);
    
};

//left
void left_enable_IRQ(){
    LEFT.enable_irq();
};


void left_rise_handler(){
    LEFT.disable_irq();
    event_queue.call(&sensor, &Sensor::getData);
    sensor.leftButton=1;
    event_queue.call(&_wifi, &WIFI::send_data, &sensor, &is_connect);
    event_queue.call(&sensor, &Sensor::init_params);
    event_queue.call(printf, "ISR event triggered\n");
    left_disable_irq.attach(&left_enable_IRQ,20ms); 
};


void left_fall_handler(){
    LEFT.disable_irq();
    event_queue.call(&sensor, &Sensor::getData);
    sensor.leftButton=0;
    event_queue.call(&_wifi, &WIFI::send_data, &sensor, &is_connect);
    event_queue.call(&sensor, &Sensor::init_params);
    event_queue.call(printf, "ISR event triggered\n");
    left_disable_irq.attach(&left_enable_IRQ,20ms);
    
};

///
*/