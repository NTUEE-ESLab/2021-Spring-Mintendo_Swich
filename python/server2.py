import socket
import json
import numpy as np
import keyboard as kb
import time

HOST = '192.168.10.100'  # Standard loopback interface address (localhost) 192.168.50.254
PORT = 3000        # Port to listen on (non-privileged ports are > 1023)

#_mod_
keyDict = {"Left": "left", "Right": "right", "Jump":"c", "Item_front":"up", "Item_back":"down", "Drift":"x", "Acc": "c"}
#keyDict = {"Left": "left", "Right": "right", "Jump":"up", "Drift":"space"}

# use ggplot style for more sophisticated visuals

# data format example
# {"Left": "0", "Right": "0", "Jump":"0", "Item_front":"0", "Item_back":"0", "Drift":"0", "Acc": "1"}

def main():
    size = 76
    past_j_data = None
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, PORT))
        print('listen...')
        s.listen()
        conn, addr = s.accept()
        with conn:
            print('Connected by', addr)

            while True:
                data = conn.recv(76).decode('utf-8')
                if not data:
                    print("no data!!!")
                    break
                #print("Receive data:", data)
                # check data size!! if size is reasonable then parse json
                if len(data) == size:
                    if past_j_data == None:
                        past_j_data = {"Left": "0", "Right": "0", "Jump":"0", "Item_front":"0", "Item_back":"0", "Drift":"0", "Acc": "0"}
                    
                    j_data=json.loads(data)
                    for control in j_data:
                        if j_data[control] == 1 and past_j_data[control] ==0 :
                            kb.press(keyDict[control])
                        elif j_data[control] == 0 and past_j_data[control] == 1:
                            kb.release(keyDict[control])
                    past_j_data = j_data


                    print("c: ", j_data)
                    
                    past_j_data = j_data

                else:
                    print("error in transmission length")
                    #print(data)
                    
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