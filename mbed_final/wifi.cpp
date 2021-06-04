#include "wifi.h"

#define WIFI_IDW0XX1    2

#if (defined(TARGET_DISCO_L475VG_IOT01A) || defined(TARGET_DISCO_F413ZH))
#include "ISM43362Interface.h"
extern ISM43362Interface wifi;

#else // External WiFi modules

#if MBED_CONF_APP_WIFI_SHIELD == WIFI_IDW0XX1
#include "SpwfSAInterface.h"
extern SpwfSAInterface wifi;
#endif // MBED_CONF_APP_WIFI_SHIELD == WIFI_IDW0XX1

#endif

#define SENT_SPACING_TIME 100
#define IP_address "192.168.10.100"
#define Port_number 3000
#define Sample_rate 2


WIFI::WIFI(NetworkInterface* wifi, events::EventQueue &event_queue, TCPSocket* socket)
        : _wifi(wifi), _event_queue(event_queue), _socket(socket)
{ 
    
}

WIFI::~WIFI(){
    _socket->close(); 
    _wifi->disconnect();
}

void WIFI::connect(){
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

}


void WIFI:: send_data(Sensor* sensor)
{
    // printf("send Data\n");
    char sbuffer[105] = "";
    nsapi_error_t response;
    nsapi_size_t size = strlen(sbuffer);


    int len = sprintf(sbuffer,"{\"Left\":%d,\"Right\":%d,\"Jump\":%d,\"Item_front\":%d,\"Item_back\":%d,\"Drift\":%d,\"Acc\":%d}",sensor->left,sensor->right,sensor->jump,sensor->itemFront,sensor->itemBack,sensor->drift,sensor->acc);
    // printf("{\"Left\":%d,\"Right\":%d,\"Jump\":%d,\"Item_front\":%d,\"Item_back\":%d,\"Drift\":%d,\"Acc\":%d}\n",_left,_right,_jump,_itemFront,_itemBack,_drift,_acc);
    if (sensor->jump) printf("jump\n");
    if (sensor->left) printf("left\n");
    if (sensor->right) printf("right\n");
    if (sensor->itemFront) printf("throw item front\n");
    if (sensor->itemBack) printf("throw item back\n");
    if (sensor->drift) printf("drift\n");


    response = _socket->send(sbuffer,len);
    if (0 >= response){
        printf("Error sending: %d\n", response);
    }
}

/*
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
*/
