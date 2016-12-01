import serial

serialPort = serial.Serial( , 9600, timeout = 0.5)

while True:
    command = serialport.read()
    print str(command)
