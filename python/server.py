import socket
import json
import numpy as np

HOST = '192.168.10.106'  # Standard loopback interface address (localhost) 192.168.50.254
PORT = 3000        # Port to listen on (non-privileged ports are > 1023)

# data format example
# {"Left": "0", "Right": "0", "Jump":"0", "Item_front":"0", "Item_back":"0", "Drift":"0", "Acc": "1"}

def main():
    size = 103
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
                print("Receive data:", data)
                # check data size!! if size is reasonable then parse json
                if (len(data) == size):
                    j_data=json.loads(data)
                    print(j_data)
                else:
                    j_data=json.loads(data[0:size])
                    print(j_data)
            s.close()
            return
    return

if __name__ == '__main__':
    main()