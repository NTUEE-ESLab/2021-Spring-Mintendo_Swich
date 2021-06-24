esys_final_project
====
**Authors:** [陳妍喻](https://github.com/yenyuuuuu), 
[門玉仁](https://github.com/dennismenn) and 
[廖耕毅](https://github.com/danielliao66)


## Introduction 
This is a Mario Kart game based on open source code and implement gamepad using STM32. We intended to design stm32 as a video game controller which can easily adapt on different video games with sensitive controlling, low energy consideration and some user friendly designs.

## Motivation
Mario Kart game is interesting for most age groups, but is it possible to take a step further? In this project, we use STM32 as game controller, and we expect to have a better gaming experience than using keyboard. The game we play is an open source project, [Süper Mario Kart](https://github.com/vmbatlle/super-mario-kart).

## Dowload Steps
1.	Download repository
2.	Prerequisites:

    a. MBED OS 6.x (Mbed Studio)  
    b. Python 3.7 + keyboard package (pip install)  
    c. Stm32L4 (B-L475E-IOT01A)  
    d. Wires  
    e.	3 buttons  
    f. MinGW32 posix 8.1.0 or GCC 7.5.0 (C++ 11 compiler)  
    g. SFML 2.5.1 (graphics generation and audio playback)  
    h. OpenAL (cross-platform API for 3D audio)  
       ***please check out the [link](https://github.com/vmbatlle/super-mario-kart#21-prerequisites) for more information about compiling the game on your own***

3.	Settings

    a.	Wifi
    - MBED:

        i. Go to mbed_app.json and change wifi-ssid and wifi password to your own wifi  
        ii. Go to cmd to find your server’s IP address  
        iii. Change the IP address in wifi.cpp to your server’s wifi

    - Python:

        iv.	Go to server2.py and change the host to your server’s IP address

    b.	Stm32  

    - Equip 3 buttons on stm32 
    
        i. top button should connect to D2  
        ii. left button should connect to D1  
        iii. right button should connect to D0

4.	Start playing

    a.	launch server2.py  
    b. connect stm32  
    c. start your game   
    d. enjoy


