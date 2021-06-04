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


int main()
{   
    TCPSocket socket;
    Sensor sensor(event_queue);
    WIFI   _wifi(&wifi, event_queue, &socket);

    _wifi.connect();

    printf("------------------------\n");
    printf("Mario Kart game Start ><\n");
    printf("------------------------\n");


    event_queue.call_every(100ms,&sensor, &Sensor::getData);
    event_queue.call_every(100ms,&_wifi, &WIFI::send_data,&sensor);
    event_queue.call_every(100ms,&sensor, &Sensor::init_params);
    event_queue.dispatch_forever();
}
