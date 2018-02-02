#-*-coding:utf-8-*-k
import serial


ser = serial.Serial('COM5')
try:
    while True:
        print("---------")
        line = ser.readline()
        print(line)


finally:
    ser.close()
