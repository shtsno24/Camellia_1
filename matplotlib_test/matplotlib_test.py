#-*-coding:utf-8-*-
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import serial
import codecs
import numpy as np

data_num = 4

plot_data = np.empty((data_num, 1))
ser = serial.Serial('COM5')
fig = plt.figure()

plt.ylim(-300,1023)
plt.yticks([0,1000])

try:
    input(">>")
    while True:
        plt.clf()
        
        line = ser.readline()
        
        line = line.decode("utf-8")
        line = line.replace("\r\n","")
        data = line.split(",")
        print(data)
        for i in range(len(data)):
            data[i] = int(data[i])
            #plt.bar(1,data[1])
            #print(np.array(data).reshape((2,1)).shape)
            plot_data = np.hstack((plot_data, np.array(data).reshape((data_num,1))))
        #print(plot_data)
        #print(plot_data.shape[1])
        plt.subplot(2,2,1)
        plt.plot(plot_data[0,plot_data.shape[1]-50:] )
        plt.subplot(2,2,2)
        plt.plot(plot_data[1,plot_data.shape[1]-50:])
        plt.subplot(2,2,3)
        plt.plot(plot_data[2,plot_data.shape[1]-50:] )
        plt.subplot(2,2,4)
        plt.plot(plot_data[3,plot_data.shape[1]-50:])
        #plt.plot(data)
        plt.pause(0.001)
        
except KeyboardInterrupt:
    print("keyboard interrupt")
    input(">>")

finally:
    ser.close()
    plt.close()
    
