#include "DigitalOut.h"
#include "InterruptIn.h"
#include "PinNameAliases.h"
#include "ThisThread.h"
#include "mbed.h"
#include "PinNames.h"
#include "mbed_events.h"
#include "EventQueue.h"
#include "TCPSocket.h"

#include "stm32l475e_iot01_gyro.h"
#include "stm32l475e_iot01_accelero.h"

#include <cstdint>
#include <cstdio>
#include <stdio.h>
#include <string.h>

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

static EventQueue event_queue(/* event count */ 16 * EVENTS_EVENT_SIZE);

#define SENT_SPACING_TIME 100
#define IP_address "192.168.1.102"
#define Port_number 3000
#define Sample_rate 2
#define DownScaler 0.001

DigitalOut _itemFrontOut(LED1);
DigitalOut _itemBackOut(LED2);
DigitalOut _driftOut(LED3);

DigitalIn _itemFrontIn(D0);
DigitalIn _itemBackIn(D1);
DigitalIn _driftIn(D2);

InterruptIn button(PC_13); // user button

static BufferedSerial serial_port(USBTX, USBRX);
FileHandle *mbed::mbed_override_console(int fd)
{
    return &serial_port;
}

const char *sec2str(nsapi_security_t sec)
{
    switch (sec) {
        case NSAPI_SECURITY_NONE:
            return "None";
        case NSAPI_SECURITY_WEP:
            return "WEP";
        case NSAPI_SECURITY_WPA:
            return "WPA";
        case NSAPI_SECURITY_WPA2:
            return "WPA2";
        case NSAPI_SECURITY_WPA_WPA2:
            return "WPA/WPA2";
        case NSAPI_SECURITY_UNKNOWN:
        default:
            return "Unknown";
    }
}

class Sensor
{
public:
    Sensor(events::EventQueue &event_queue)
        : _event_queue(event_queue)
    {
        BSP_ACCELERO_Init();    
        BSP_GYRO_Init();
        calibrate();
        // _event_queue.call_every(Sample_rate, this, &Sensor::);
    }

    void calibrate()
    {
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
            ThisThread::sleep_for(100);
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

    void calcDirection(uint8_t& left, uint8_t& right)
    {
        BSP_ACCELERO_AccGetXYZ(_pAccDataXYZ);
        if ( ((_pAccDataXYZ[0]-_AccOffset[0])*DownScaler) > 0.6)
            left = 1;
        if ( ((_pAccDataXYZ[0]-_AccOffset[0])*DownScaler) < -0.6)
            right = 1;
        // printf("left: %d, right: %d\n", left, right);
    }

    void calcJump(uint8_t& jump) 
    {
        BSP_ACCELERO_AccGetXYZ(_pAccDataXYZ);
        if ( ((_pAccDataXYZ[1]-_AccOffset[1])*DownScaler) > 0.5)
        {
            if (_canJump) jump = 1;
            _canJump = false;
        }
        if (!_canJump) _jumpCount = _jumpCount+1;
        if (_jumpCount > 10)
        {
            _jumpCount = 0;
            _canJump = true;
        }

    }

    void getData(uint8_t& left, uint8_t& right, uint8_t& jump, uint8_t& itemFront, uint8_t& itemBack, uint8_t& drift, uint8_t acc)
    {
        itemFront = (_itemFrontIn.read() && _canItemFront) ? 1 : 0;
        if (itemFront) _canItemFront = false;
        if (!_canItemFront) _itemFrontCount = _itemFrontCount +1;
        if (_itemFrontCount > 10)
        {
            _itemFrontCount = 0;
            _canItemFront = true;
        }
        _itemFrontOut = (itemFront) ? 1 : 0;
        

        itemBack = (_itemBackIn.read() && _canItemBack)? 1 : 0;
        if (itemBack) _canItemBack = false;
        if (!_canItemBack) _itemBackCount = _itemBackCount+1;
        if (_itemBackCount > 10)
        {
            _itemBackCount = 0;
            _canItemBack = true;
        }
        _itemBackOut = (itemBack)? 1 : 0;


        drift = (_driftIn.read() && _canDrift)? 1 : 0;
        if (drift) _canDrift = false;
        if (!_canDrift) _driftCount = _driftCount+1;
        if (_driftCount > 10)
        {
            _driftCount = 0;
            _canDrift = true;
        }
        _driftOut = (drift)? 1 : 0;

        acc = 1;

        calcJump(jump);
        calcDirection(left, right);
    }

private:
    events::EventQueue &_event_queue;
    int16_t _pAccDataXYZ[3] = {0};
    float _pGyroDataXYZ[3] = {0};

    int   _AccOffset[3] = {};
    float _GyroOffset[3] = {};

    int _jumpCount = 0;
    bool _canJump = true;

    int _driftCount = 0;
    bool _canDrift = true;

    int _itemFrontCount = 0;
    bool _canItemFront = 0;

    int _itemBackCount = 0;
    bool _canItemBack = true;
};

class WIFI
{
public:
    WIFI(NetworkInterface* wifi, Sensor* sensor, events::EventQueue &event_queue, TCPSocket* socket)
        : _wifi(wifi), _sensor(sensor), _event_queue(event_queue), _socket(socket)
        { 
            connect(); 
            reset_data();
        }

    ~WIFI() 
    { 
        _socket->close(); 
        _wifi->disconnect(); 
    }

    void connect()
    {
        printf("\nConnecting to %s...\n", MBED_CONF_APP_WIFI_SSID);
        int ret = wifi.connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);
        if (ret != 0) {
            printf("\nConnection error\n");
            return;
        }

        printf("Success\n\n");
        printf("MAC: %s\n", wifi.get_mac_address());
        printf("IP: %s\n", wifi.get_ip_address());
        printf("Netmask: %s\n", wifi.get_netmask());
        printf("Gateway: %s\n", wifi.get_gateway());
        printf("RSSI: %d\n\n", wifi.get_rssi());

        nsapi_error_t response;

        SocketAddress addr;
        _wifi->get_ip_address(&addr);
        printf("IP address: %s\n", addr.get_ip_address() ? addr.get_ip_address() : "None");
        printf("Sending request to %s...\n", IP_address);
        _socket->open(_wifi);
        _wifi->gethostbyname(IP_address, &addr);
        addr.set_port(Port_number);
        response = _socket->connect(addr);

        if (0 != response){
            printf("Error connecting: %d\n", response);
            // socket.close();
            // return; 
        }

        _event_queue.call_every(SENT_SPACING_TIME, this, &WIFI::send_data);
    }

    void send_data()
    {
        // printf("send Data\n");
        char sbuffer[105] = "";
        nsapi_error_t response;
        nsapi_size_t size = strlen(sbuffer);

        reset_data();
        _sensor->getData(_left, _right, _jump, _itemFront, _itemBack, _drift, _acc);

        int len = sprintf(sbuffer,"{\"Left\":%d,\"Right\":%d,\"Jump\":%d,\"Item_front\":%d,\"Item_back\":%d,\"Drift\":%d,\"Acc\":%d}",_left,_right,_jump,_itemFront,_itemBack,_drift,_acc);
        // printf("{\"Left\":%d,\"Right\":%d,\"Jump\":%d,\"Item_front\":%d,\"Item_back\":%d,\"Drift\":%d,\"Acc\":%d}\n",_left,_right,_jump,_itemFront,_itemBack,_drift,_acc);
        if (_jump) printf("jump\n");
        if (_left) printf("left\n");
        if (_right) printf("right\n");
        if (_itemFront) printf("throw item front\n");
        if (_itemBack) printf("throw item back\n");
        if (_drift) printf("drift\n");




        response = _socket->send(sbuffer,len);
        if (0 >= response){
            printf("Error sending: %d\n", response);
        }
    }
    
    void reset_data()
    {
        _right = 0;
        _left = 0;
        _jump = 0;
        _itemFront = 0;
        _itemBack = 0;
        _drift = 0;
        _acc = 0;
    }

private:
    NetworkInterface* _wifi;
    Sensor* _sensor;
    events::EventQueue& _event_queue;
    TCPSocket* _socket;

    uint8_t _right;
    uint8_t _left; 
    uint8_t _jump;
    uint8_t _itemFront;
    uint8_t _itemBack;
    uint8_t _drift;
    uint8_t _acc;
};

TCPSocket socket;
Sensor sensor(event_queue);
WIFI   _wifi(&wifi, &sensor, event_queue, &socket);

void reset()
{
    event_queue.call(callback(&sensor, &Sensor::calibrate));
    event_queue.call(callback(&_wifi  , &WIFI::connect));
}

int main()
{
    printf("------------------------\n");
    printf("Mario Kart game Start ><\n");
    printf("------------------------\n");

    button.fall(&reset);

    event_queue.dispatch_forever();
}
