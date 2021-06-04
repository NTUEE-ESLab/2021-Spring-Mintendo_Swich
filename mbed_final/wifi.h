#ifndef  WIFI_H
#define WIFI_H

#include "all_headers.h"
#include "sensor.h"

class WIFI{
    private:
        NetworkInterface* _wifi;
        events::EventQueue& _event_queue;
        TCPSocket* _socket;

    public:
        WIFI(NetworkInterface* wifi, events::EventQueue &event_queue, TCPSocket* socket);
        ~WIFI();
        void connect();
        void send_data(Sensor* sensor);

};

#endif