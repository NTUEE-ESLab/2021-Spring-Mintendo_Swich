# esys_final_project
**Authors:** [陳妍喻](https://github.com/yenyuuuuu), 
[門玉仁](https://github.com/dennismenn) and 
[廖耕毅](https://github.com/danielliao66)

Mario Kart based on open source code and implement gamepad using STM32
## Motivation
Mario Kart game is interesting for most age groups, but is it possible to take a step further? In this project, we use STM32 as game controller, and we expect to have a better gaming experience than using keyboard. The game we play is an open source project, [Süper Mario Kart](https://github.com/vmbatlle/super-mario-kart).
## Implementation
### STM32
- Three buttons
    - Left
    - Right
    - Top
- Sensor
    - Gyroscope
    - Accelerometer

### Mbed program
### Python server
- In python/server2.py
- TCP socket
- Receive json data that contains information: 
    - Button pressed or not
    - Acceleration data (x, y, z)
    - Gyroscope data (x, y, z)
- Parse json data and send to next function for further actions
```bash
{'Acc_x':10000.25,"Acc_y":10000.25,"Acc_z":10000.25,"Gyro_x":10000.25,"Gyro_y":10000.25,"Gyro_z":10000.25,"leftButton":1,"rightButton":1,"topButton":1}
```

### Python keyboard control api
- In python/mariokart.py
- Function  “action”
    - Input: a dictionary after json.loads
    - Output: press/release keyboard
- In mario kart game
    - Left : check if accelerometer’s x-value > given threshold
    - Right: check if accelerometer’s x-value < given threshold
    - Jump: check if accelerometer’s y-value > given threshold
    - Throw item front/back and drift: button transition

## Dowload Steps
1.	Download repository
2.	Prerequisites:

a. MBED OS 6.x (Mbed Studio)  
b. Python 3.7 + keyboard package (pip install)  
c. Stm32L4 (B-L475E-IOT01A)  
d. Wires  
e.	3 buttons
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


