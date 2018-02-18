import serial
import serial.tools.list_ports
from pynput.keyboard import Key, Controller

COM_PORT_NAME = "USB-SERIAL CH340"


"""
Return port with name given as parameter
"""
def getDevicePort(name):
    ports = list(serial.tools.list_ports.comports())
    for port in ports:
        if port.description.__contains__(name):
            return port

def performAction(message):
    action_code = message[:2]
    if action_code == '1':
        print("PERFORMING ACTION 1")

def main():

    keyboard = Controller()
    keyboard.press('A')
    keyboard.release('A')

    port = getDevicePort(COM_PORT_NAME)
    if port.description.__contains__(COM_PORT_NAME):
        ser = serial.Serial()
        ser.baudrate = 115200
        ser.port = port.device
        ser.open()
        while (ser.is_open):
            performAction(str(ser.readline())[2:-5])

if __name__ == "__main__":
    main()