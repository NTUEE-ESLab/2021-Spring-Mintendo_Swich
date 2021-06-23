import socket
import json
import numpy as np
import time

HOST = '192.168.1.102'  
PORT = 3000   

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
json_s = {'Acc_x' : 10000.25,"Acc_y":10000.25,"Acc_z":10000.25,"Gyro_x":10000.25,"Gyro_y":10000.25,"Gyro_z":10000.25,"leftButton":1,"rightButton":1,"topButton":1}
obj_s = json.dumps(json_s)
print(obj_s)
print(len(obj_s))

n = 0


while True:
    s.sendall(bytes(obj_s,encoding="utf-8"))

    # if (json_s["leftButton"] == 1):
    # 	json_s["leftButton"] = 0
    # elif (json_s["leftButton"] == 0):
    # 	json_s["leftButton"] = 1

    # if (json_s["topButton"] == 1):
    # 	json_s["topButton"] = 0
    # elif (json_s["topButton"] == 0):
    # 	json_s["topButton"] = 1
    # obj_s = json.dumps(json_s)
    time.sleep(2)
    
    # indata = s.recv(1024)
    # if len(indata) == 0: # connection closed
    #     s.close()
    #     print('server closed connection.')
    #     break
    # print('recv: ' + indata.decode())