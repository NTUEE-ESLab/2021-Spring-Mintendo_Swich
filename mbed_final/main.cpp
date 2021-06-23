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

Timeout disable_irq;

InterruptIn ISR_BUT_USER(D2);

void enable_BUT_IRQ(){
    ISR_BUT_USER.enable_irq();
};


void rise_handler(){
    ISR_BUT_USER.disable_irq();
    event_queue.call(&sensor, &Sensor::getData);
    sensor.topButton=1;
    event_queue.call(&_wifi, &WIFI::send_data, &sensor, &is_connect);
    event_queue.call(&sensor, &Sensor::init_params);
    event_queue.call(printf, "ISR event triggered\n");
    disable_irq.attach(&enable_BUT_IRQ,20ms); 
};


void fall_handler(){
    ISR_BUT_USER.disable_irq();
    event_queue.call(&sensor, &Sensor::getData);
    sensor.topButton=0;
    event_queue.call(&_wifi, &WIFI::send_data, &sensor, &is_connect);
    event_queue.call(&sensor, &Sensor::init_params);
    event_queue.call(printf, "ISR event triggered\n");
    disable_irq.attach(&enable_BUT_IRQ,20ms);
    
};



int main()
{   
    

    _wifi.connect(&socket);

    printf("------------------------\n");
    printf("Mario Kart game Start ><\n");
    printf("------------------------\n");

    ISR_BUT_USER.rise(rise_handler);
    ISR_BUT_USER.fall(fall_handler);


    event_queue.call_every(50ms,&sensor, &Sensor::getData);
    event_queue.call_every(50ms,&sensor, &Sensor::judge_motion);
    event_queue.call_every(50ms,&_wifi, &WIFI::send_data,&sensor, &is_connect);
    event_queue.call_every(50ms,&sensor, &Sensor::init_params);


    event_queue.dispatch_forever();

    while(is_connect) {};
    if (!is_connect) return 0;
}
