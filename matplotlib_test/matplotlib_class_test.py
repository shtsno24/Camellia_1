#-*-coding:utf-8-*-
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import serial
import codecs
import numpy as np

Data_num = 4

Plot_data = np.empty((Data_num, 1))
ser = serial.Serial('COM5')
fig = plt.figure()

ax0 = fig.add_subplot(321)
ax1 = fig.add_subplot(322)
ax2 = fig.add_subplot(323)
ax3 = fig.add_subplot(324)


def update_graph():
    global Plot_data
    global Data_num

    line = ser.readline()   
    line = line.decode("utf-8")
    line = line.replace("\r\n","")
    data = line.split(",")
    for i in range(len(data)):
        data[i] = int(data[i])

    Plot_data = np.hstack((Plot_data, np.array(data).reshape((Data_num,1))))

    plt.clf()
    ax0.plot(Plot_data[0,Plot_data.shape[1] - 50:])
    ax1.plot(Plot_data[1,Plot_data.shape[1] - 50:]) 
    ax2.plot(Plot_data[2,Plot_data.shape[1] - 50:])
    ax3.plot(Plot_data[3,Plot_data.shape[1] - 50:])

    
try:
    input(">>")
    ani = animation.FuncAnimation(fig, update_graph(), interval = 100)
    plt.show()
    """
    while True:
        plt.clf()
        
        line = ser.readline()
        
        line = line.decode("utf-8")
        line = line.replace("\r\n","")
        data = line.split(",")
        print(data)
        for i in range(len(data)):
            data[i] = int(data[i])
            plot_data = np.hstack((plot_data, np.array(data).reshape((data_num,1))))

        
        ax0.plot(plot_data[0,plot_data.shape[1] - 50:])
        
        ax1.plot(plot_data[1,plot_data.shape[1] - 50:])
    
        ax2.plot(plot_data[2,plot_data.shape[1] - 50:])
        
        ax3.plot(plot_data[3,plot_data.shape[1] - 50:])

        ax4.scatter(plot_data[0,plot_data.shape[1] - 50:], plot_data[1,plot_data.shape[1] - 50:])
        #plt.plot(data)
        plt.pause(0.001)
        """
    #ani = animation.FuncAnimation(fig, update_graph(Data_num, Plot_data), interval = 100)
    #plt.show()

except KeyboardInterrupt:
    print("keyboard interrupt")
    input(">>")

finally:
    ser.close()
    plt.close()
    
