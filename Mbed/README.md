# mbed-os-example-wifi #

Wi-Fi example for Mbed OS

## Getting started with the Wi-Fi API ##

This is an example of a Wi-Fi application using the Wi-Fi and network socket APIs that [Mbed OS](https://github.com/ARMmbed/mbed-os) provides.

The program brings up the Wi-Fi and the underlying network interface and uses it to scan available networks, connects to a network, prints interface and connection details and performs an HTTP operation.

For more information about Wi-Fi APIs, please visit the [Mbed OS Wi-Fi](https://os.mbed.com/docs/latest/reference/wi-fi.html) documentation.

### Supported hardware ###

* [NUCLEO-F401RE](https://os.mbed.com/platforms/ST-Nucleo-F401RE/) with [X-NUCLEO-IDW04A1](http://www.st.com/content/st_com/en/products/ecosystems/stm32-open-development-environment/stm32-nucleo-expansion-boards/stm32-ode-connect-hw/x-nucleo-idw04a1.html) Wi-Fi expansion board using pins D8 and D2 _(of the Arduino connector)_.
* [NUCLEO-F401RE](https://os.mbed.com/platforms/ST-Nucleo-F401RE/) with [X-NUCLEO-IDW01M1](https://os.mbed.com/components/X-NUCLEO-IDW01M1/) Wi-Fi expansion board using pins PA_9 and PA_10 _(of the Morpho connector)_.
* [DISCO_L475VG_IOT01A](https://os.mbed.com/platforms/ST-Discovery-L475E-IOT01A/) with ISM43362 built-in module
* [DISCO_F413ZH](https://os.mbed.com/platforms/ST-Discovery-F413H/) with ISM43362 built-in module
* Other Mbed targets with  [X-NUCLEO-IDW04A1](http://www.st.com/content/st_com/en/products/ecosystems/stm32-open-development-environment/stm32-nucleo-expansion-boards/stm32-ode-connect-hw/x-nucleo-idw04a1.html) or [X-NUCLEO-IDW01M1](https://os.mbed.com/components/X-NUCLEO-IDW01M1/) expansion board.
  *(The Mbed target board the Wi-Fi shield connects to shouldn't have any other network interface, for example Ethernet.)*

#### Connecting the X-NUCLEO-IDW0XX1 ####

To connect the [X-NUCLEO-IDW04A1](http://www.st.com/content/st_com/en/products/ecosystems/stm32-open-development-environment/stm32-nucleo-expansion-boards/stm32-ode-connect-hw/x-nucleo-idw04a1.html) or [X-NUCLEO-IDW01M1](https://developer.mbed.org/components/X-NUCLEO-IDW01M1/) expansion board to your NUCLEO development board, plug the expansion board on top of the NUCLEO board using the Arduino or Morpho connector.

##  Getting started ##

1. Import the example.

   ```
   mbed import mbed-os-example-wifi
   cd mbed-os-example-wifi
   ```
   
2. Configure the Wi-Fi shield to use.

   Edit ```mbed_app.json``` to include the correct Wi-Fi shield, SSID and password:

   ```
       "config": {
 	  "wifi-shield": {
               "help": "Options are WIFI_IDW0XX1",
               "value": "WIFI_IDW0XX1"
        	  },
           "wifi-ssid": {
               "help": "WiFi SSID",
               "value": "\"SSID\""
           },
           "wifi-password": {
               "help": "WiFi Password",
               "value": "\"Password\""
           }
       },
   ```

   Sample ```mbed_app.json``` files are provided for X-NUCLEO-IDW04A1 (```mbed_app_idw04a1.json```),  X-NUCLEO-IDW01M1 (```mbed_app_idw01m1```), DISCO_L475VG_IOT01A/ISM43362 (```mbed_app_ism43362.json```) and DISCO_F413ZH/ISM43362 (```mbed_app_ism43362.json```)
   
   For built-in Wi-Fi, ignore the value of `wifi-shield`.

3. Compile and generate binary.

   For example, for `GCC`:

   ```
   mbed compile -t GCC_ARM -m UBLOX_EVK_ODIN_W2
   ```
   
 4. Open a serial console session with the target platform using the following parameters:
 
    * **Baud rate:** 9600
    * **Data bits:** 8
    * **Stop bits:** 1
    * **Parity:** None
 
 5. Copy or drag the application `mbed-os-example-wifi.bin` in the folder `mbed-os-example-wifi/BUILD/<TARGET NAME>/<PLATFORM NAME>` onto the target board.
 
 6. The serial console should display a similar output to below, indicating a successful Wi-Fi connection:
 
 ```
 WiFi example

Scan:
Network: Dave Hot Spot secured: Unknown BSSID: 00:01:02:03:04:05 RSSI: -58 Ch: 1
1 network available.

Connecting...
Success

MAC: 00:01:02:03:04:05
IP: 192.168.0.5
Netmask: 255.255.255.0
Gateway: 192.168.0.1
RSSI: -27

Sending HTTP request to www.arm.com...
sent 38 [GET / HTTP/1.1]
recv 64 [HTTP/1.1 301 Moved Permanently]

Done
```

## Troubleshooting

If you have problems, you can review the [documentation](https://os.mbed.com/docs/latest/tutorials/debugging.html) for suggestions on what could be wrong and how to fix it.
