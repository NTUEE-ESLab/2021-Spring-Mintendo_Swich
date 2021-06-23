import json
import numpy as np
import keyboard as kb

keyDict = {"Up": "up", "Down": "down"}
DownScaler = 0.005
DownScaler2 = 0.001

x_threshold = 0.3

def action(j_data, past_j_data):
    if past_j_data == None:
        return

    # deal with button    
    if ((j_data["Acc_y"]*DownScaler) > x_threshold) :
        kb.press(keyDict["Up"])
    elif ((j_data["Acc_y"]*DownScaler) < x_threshold):
        kb.release(keyDict["Up"])

    if ((j_data["Acc_y"]*DownScaler) < -1*x_threshold):
        kb.press(keyDict["Down"])
    elif ((j_data["Acc_y"]*DownScaler) > -1*x_threshold) :
        kb.release(keyDict["Down"])
    
    return