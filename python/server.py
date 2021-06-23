import socket
import json
import numpy as np
import keyboard as kb
import time
import calibrating as clb
import mariokart as mk
import pong

HOST = '192.168.10.100'  # Standard loopback interface address (localhost)
PORT = 3000        # Port to listen on (non-privileged ports are > 1023)
CALIB_TIME = 10

# data format example
# "{"Acc_x":%.2f,"Acc_y":%.2f,"Acc_z":%.2f,"Gyro_x":%.2f,"Gyro_y":%.2f,"Gyro_z":%.2f,"leftButton":%d,"ightButton":%d,"top":%d}"
# {'Acc_x':10000.25,"Acc_y":10000.25,"Acc_z":10000.25,"Gyro_x":10000.25,"Gyro_y":10000.25,"Gyro_z":10000.25,"leftButton":1,"rightButton":1,"topButton":1}

def main():
    size = 250 
    past_j_data = None
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, PORT))
        print('listen...')
        s.listen()
        conn, addr = s.accept()
        with conn:
            print('Connected by', addr)
            print('calibrating')
            count = 0 #for calibrating
            acc_bias = [0,0,0]
            gyro_bias = [0,0,0]

            while True:

                data = conn.recv(250).decode('utf-8')
                if not data:
                    print("no data!!!")
                    break
                conn.send('ok'.encode('utf-8'))
                #check data size!! if size is reasonable then parse json
                if len(data) < size:
                    j_data=json.loads(data)
                    if (count<CALIB_TIME):
                        count+=1
                        clb.collecting_data(j_data, acc_bias,gyro_bias)
                    elif(count==CALIB_TIME):
                        count+=1
                        print("calibaration done")
                        acc_bias[:] = [x/CALIB_TIME for x in acc_bias]
                        gyro_bias[:] = [x/CALIB_TIME for x in gyro_bias]
                    
                   # print("Receive data: "+ data)
                    else:
                        clb.calibrate(j_data,acc_bias,gyro_bias)
                        print( j_data['Acc_x'])
                        mk.action(j_data, past_j_data)
                        #pong.action(j_data,past_j_data)
                        past_j_data = j_data
                else:
                    print("error in transmission length: {:d}".format(len(data)))  
            s.close()
            return
    return
"""
def strikeKey(key):
    kb.press(key)
    time.sleep(0.1)
    kb.release(key)
    return
"""
if __name__ == '__main__':
    main()