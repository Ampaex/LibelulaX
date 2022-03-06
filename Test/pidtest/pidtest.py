# Code for testing the PID implementation
import time

import serial, os
import PyGnuplot as gp

serialString = ""  # Used to hold data coming over UART
timeToSleep = 1
setpoint = 30

def connectPort(port):
    return serial.Serial(port = port, baudrate=115200, bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE, xonxoff=True)

class simulationModel:
    def input(self, inputValue):
        dt = time.time() - self.lastTimeEval
        self.lastTimeEval = time.time()
        accel = (inputValue*20-self.lastValue) / dt
        self.angle = self.angle + accel * dt
        return self.angle

    def __init__(self):
        self.angle = 0.
        self.lastTimeEval = time.time()
        self.lastValue = 0.

def main():
    serialPort = connectPort("/dev/ttyUSB0")
    sim = simulationModel()

    while (1):

        # Wait until there is data waiting in the serial buffer
        if (serialPort.in_waiting > 0):
            # Read data out of the buffer until a carraige return / new line is found
            serialString = serialPort.readline()

            # Print the contents of the serial data
            print(serialString.decode('Ascii'))
            if ("esp32>" in serialString.decode('Ascii')):
                break

    simList = []
    microList = []
    targetList = []
    nList = []
    n = 0

    time.sleep(timeToSleep)
    serialPort.write(("pidtest 0 -s "+str(setpoint)+"\n").encode("UTF-8"))
    print(serialPort.readline().decode("Ascii"))
    simList.append(0)
    microList.append(0)
    targetList.append(setpoint)
    nList.append(n)

    n =1
    gp.c('set yrange[-90:90]')
    while(1):
        # Wait until there is data waiting in the serial buffer
        if (serialPort.in_waiting > 0):

            serialString = float(serialPort.readline().decode("Ascii"))
            print(serialString)

            outp = sim.input(float(serialString))

            microList.append(serialString)
            simList.append(outp)
            targetList.append(setpoint)
            nList.append(n)
            n = n+1
            if len(simList) > 60:
                simList.pop(0)
            if len(microList) > 60:
                microList.pop(0)
            if len(targetList) > 60:
                targetList.pop(0)
            if len(nList) > 60:
                nList.pop(0)

            gp.s([nList, microList, simList, targetList])

            #Gnuplot graphics
            gp.c('plot "tmp.dat" using 1:2 with lines title "elevator", "tmp.dat" using 1:3 with lines title "inclination", "tmp.dat" using 1:4 with lines title "target"')

            time.sleep(timeToSleep)
            if outp < 0:
                serialPort.write(("pidtest -- " + str(outp) + "\n").encode("UTF-8"))
            else:
                serialPort.write(("pidtest "+str(outp)+"\n").encode("UTF-8"))
            print(serialPort.readline().decode("Ascii"))



if __name__ == "__main__" :
    main()
