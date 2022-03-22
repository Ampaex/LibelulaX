# Code for testing the PID implementation
import time

import serial, os

def connectPort(portDirectory):
    return serial.Serial(port = portDirectory, baudrate=115200, bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE, xonxoff=True)

def readPort(port):
    # Wait until there is data waiting in the serial buffer
    if (port.in_waiting > 0):
        retString = str(port.readline().decode("Ascii"))
        print(retString)
        return retString
    else:
        return None

def writePort(port, string):
    port.write(str(string).encode("UTF-8"))
    print(port.readline().decode("Ascii"))


def main():
    serialPort = connectPort("/dev/ttyUSB0")
    with open("inFile", "w") as inputFile:
        with open("outFile", "r+") as outputFile:

            error = False
            while(error is not True):
                readLine = readPort(serialPort)
                if readLine is not None:
                    inputFile.truncate(0)
                    inputFile.write(readLine)
                    inputFile.flush()

                lines = outputFile.readlines()
                if(len(lines)>0):
                    writePort(serialPort, lines[0])
                    outputFile.truncate(0)
                time.sleep(0.02)



if __name__ == "__main__" :
    main()
