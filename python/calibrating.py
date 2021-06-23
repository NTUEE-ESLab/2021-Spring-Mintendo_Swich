def collecting_data(j_data, acc_bias, gyro_bias):
    acc_bias[0]+=j_data["Acc_x"]
    acc_bias[1]+=j_data["Acc_y"]
    acc_bias[2]+=j_data["Acc_z"]
    gyro_bias[0]+=j_data["Gyro_x"]
    gyro_bias[1]+=j_data["Gyro_y"]
    gyro_bias[2]+=j_data["Gyro_z"]

def calibrate(j_data,acc_bias, gyro_bias):
    j_data["Acc_x"]-=acc_bias[0]
    j_data["Acc_y"]-=acc_bias[1]
    j_data["Acc_z"]-=acc_bias[2]
    j_data["Gyro_x"]-=gyro_bias[0]
    j_data["Gyro_y"]-=gyro_bias[1]
    j_data["Gyro_z"]-=gyro_bias[2]
