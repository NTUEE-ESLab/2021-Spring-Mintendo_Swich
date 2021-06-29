ESLAB_2021_spring Mintendo Swich
====
**Authors:** [門玉仁](https://github.com/dennismenn), [陳妍喻](https://github.com/danielliao66) and [廖耕毅](https://github.com/yenyuuuuu) (according to Chinese alphabetic indexes)

Note: for details please refer to report.pdf
## Introduction 
This is a project based on open source codes for games, which implements gamepad to control keyboard using STM32. We intended to design stm32 as a video game controller which can easily adapt on different keyboard-controlled video games with sensitive controlling, low energy consideration and some user friendly designs. Specifically, we will demonstrate our project on Mario kart game and pong.

## Motivation
Mario Kart as well as other keyboard games are interesting for most age groups, but is it possible to take a step further? In this project, we use STM32 as game controller, and we expect to have a better gaming experience than using keyboard. The first game we play is an open source game project, [S羹per Mario Kart](https://github.com/vmbatlle/super-mario-kart), and the second game we demonstrate is pong.

## Dowload Steps
1.	Download repository
2.	Prerequisites:

    **controller**  
    a. MBED OS 6.x (Mbed Studio)  
    b. Python 3.7 + keyboard package (pip install)  
    c. Stm32L4 (B-L475E-IOT01A)  
    d. Wires  
    e.	3 buttons  
    **mario kart game**  
    f. MinGW32 posix 8.1.0 or GCC 7.5.0 (C++ 11 compiler)  
    g. SFML 2.5.1 (graphics generation and audio playback)  
    h. OpenAL (cross-platform API for 3D audio)  
       ***please check out the [link](https://github.com/vmbatlle/super-mario-kart#21-prerequisites) for more information about compiling the game on your own***

3.	Settings

    a.	Wifi
    - MBED:

        i. Go to ./mbed_final/mbed_app.json and change wifi-ssid and wifi password to your own wifi:  
        ```bash
        "wifi-ssid": {
            "help": "WiFi SSID",
            "value": "\"thessid\""
        },
        "wifi-password": {
            "help": "WiFi Password",
            "value": "\"thepassword\""
        ```  
        ii. Go to cmd to find your server? IP address:  
        
        In cmd, type in  
        ```bash
        ipconfig /all
        ````
        then you will see in one row that says IPv4 Address, and your server's IP address comes right after.  
        
        iii. Change the IP address in ./mbed_final/wifi.cpp to your server? IP address

    - Python:

        iv.	Go to ./python/server.py and change the host to your server? IP address and make sure the wifi is the same as stm32's, i.e. the one mentioned in iii.

    b.	Stm32  

    - Equip 3 buttons on stm32 
    
        i. top button should connect to D2  
        ii. left button should connect to D1  
        iii. right button should connect to D0  
        ***please check out the [manual](https://www.st.com/resource/en/user_manual/dm00347848-discovery-kit-for-iot-node-multichannel-communication-with-stm32l4-stmicroelectronics.pdf) (p.10) for more information about the ports on stm32***

4.	Start playing

    a.	launch server.py  
    b. connect stm32  
    c. start your game   
    d. enjoy

