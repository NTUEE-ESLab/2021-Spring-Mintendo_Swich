import json
import numpy as np
import keyboard as kb

keyDict = {"Left": "left", "Right": "right", "rightButton":"up", "leftButton":"down", "Drift":"c", "topButton": "x"}
DownScaler = 0.005
DownScaler2 = 0.001

x_threshold = 0.8

def action(j_data, past_j_data):
    if past_j_data == None:
        return

    # deal with button
    if j_data["leftButton"] == 1 and past_j_data["leftButton"] ==0:
        kb.press(keyDict["leftButton"])
    elif j_data["leftButton"] == 0 and past_j_data["leftButton"] == 1:
        kb.release(keyDict["leftButton"])
    
    if j_data["rightButton"] == 1 and past_j_data["rightButton"] ==0:
        kb.press(keyDict["rightButton"])
    elif j_data["rightButton"] == 0 and past_j_data["rightButton"] == 1:
        kb.release(keyDict["rightButton"])

    if j_data["topButton"] == 1 and past_j_data["topButton"] ==0:
        kb.press(keyDict["topButton"])
    elif j_data["topButton"] == 0 and past_j_data["topButton"] == 1:
        kb.release(keyDict["topButton"])

    # deal with acc and gyro data

    
    if ((j_data["Acc_x"]*DownScaler) > x_threshold) :
        kb.press(keyDict["Left"])
    elif ((j_data["Acc_x"]*DownScaler) < x_threshold):
        kb.release(keyDict["Left"])

    if ((j_data["Acc_x"]*DownScaler) < -1*x_threshold):
        kb.press(keyDict["Right"])
    elif ((j_data["Acc_x"]*DownScaler) > -1*x_threshold) :
        kb.release(keyDict["Right"])
    
    if ((j_data["Acc_y"]*DownScaler2) > 0.5) and ((past_j_data["Acc_y"]*DownScaler2) < 0.5):
        kb.press(keyDict["Drift"])
    elif ((j_data["Acc_y"]*DownScaler2) < 0.5) and ((past_j_data["Acc_y"]*DownScaler2) > 0.5):
        kb.release(keyDict["Drift"])
    
    return