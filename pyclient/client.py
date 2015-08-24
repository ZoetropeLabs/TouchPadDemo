import serial
import matplotlib.pyplot as plt
import time
import numpy as np

sensorValues = [0 for x in range(16)]
sensorValues = [4, 5, 2, 3, 7, 6, 6, 6, 1, 0, 7, 0, 6, 1, 143, 45]
Nx = 8
Ny = 6
indx = np.arange(0, Nx*30, step=30)
indy = np.arange(0, Ny*20, step=20)
width = 10

fig, ax = plt.subplots()
rectsx = ax.bar(indx, sensorValues[:8], width, color='r')
rectsy = ax.barh(indy, sensorValues[8:14], width, color='b')

ser = serial.Serial(
        port = '/dev/ttyACM0',
        baudrate=115200
        )

plt.ion()
ax.set_ylim([-255,140])
ax.set_xlim([-255,255])
plt.show()

ser.isOpen()

while 1:
    line = ser.readline()
    sensorValues = line.strip(', \r\n').split(", ")
    #print(sensorValues)
    if len(sensorValues) == 15:
        
        sensorValues = map(int, sensorValues)
        print sensorValues
        #Draw the bg
        ax.draw_artist(ax.patch)
        #Set the new data and draw the rectangles
        for i,rect in enumerate(rectsx):
            rect.set_height(-sensorValues[i])
            ax.draw_artist(rect)
        for i,rect in enumerate(rectsy):
            #Need to reverse the slice to get it in the same orientation
            rect.set_width(-sensorValues[13:6:-1][i])
            ax.draw_artist(rect)
        #Update the graph
        fig.canvas.update()
        fig.canvas.flush_events()


