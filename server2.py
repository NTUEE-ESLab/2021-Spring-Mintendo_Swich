import socket
import json
import numpy as np
import keyboard as kb
import time

HOST = '127.0.0.1'  # Standard loopback interface address (localhost) 192.168.50.254
PORT = 3000        # Port to listen on (non-privileged ports are > 1023)

keyDict = {"Left": "left", "Right": "right", "Jump":"c", "Item_front":"up", "Item_back":"down", "Drift":"c", "Acc": "x"}  
# use ggplot style for more sophisticated visuals

# data format example
# {"Left": "0", "Right": "0", "Jump":"0", "Item_front":"0", "Item_back":"0", "Drift":"0", "Acc": "1"}

def main():
    size = 103
    past_j_data = None
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, PORT))
        print('listen...')
        s.listen()
        conn, addr = s.accept()
        with conn:
            print('Connected by', addr)
            while True:
                data = conn.recv(1024).decode('utf-8')
                if not data:
                    print("no data!!!")
                    break
                #print("Receive data:", data)
                # check data size!! if size is reasonable then parse json
                if len(data) == size:
                    if past_j_data == None:
                        past_j_data = {"Left": "0", "Right": "0", "Jump":"0", "Item_front":"0", "Item_back":"0", "Drift":"0", "Acc": "1"}
                    j_data=json.loads(data)
                    for control in j_data:
                        if past_j_data[control] == "0" and j_data[control] == "1":
                            kb.press(keyDict[control])
                        elif past_j_data[control] == "1" and j_data[control] == "0":
                            kb.release(keyDict[control])   
                    past_j_data = j_data
                    #print(j_data)
                else:
                    j_data=json.loads(data[0:size])
                    print(j_data)
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