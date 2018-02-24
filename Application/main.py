import tkinter as tk
from tkinter import messagebox
from configuration import Configuration
from configuration import Button
from configuration import ButtonView
import threading as th
import serial
import serial.tools.list_ports
from pynput.keyboard import Controller as KController
from pynput.mouse import Button, Controller as MController

do_simulate = None
controller = None
control = None
root = None
app = None

class Controller:
    COM_PORT_NAME = "USB-SERIAL CH340"

    """
    Return port with name given as parameter
    """
    @staticmethod
    def get_device_port(name):
        ports = list(serial.tools.list_ports.comports())
        for port in ports:
            if port.description.__contains__(name):
                return port

    @staticmethod
    def perform_action(message, value):
        global configuration
        if do_simulate:
            keyboard = KController()
            mouse = MController()
            if int(message) >= 0:
                key = configuration.buttons[int(message)].value
                if not str(key).__contains__("Button"):
                    keyboard.press(key)
                    keyboard.release(key)
                elif str(key) == "Button.left":
                    mouse.click(Button.left)
                elif str(key) == "Button.right":
                    mouse.click(Button.right)
            elif int(message) == -1:
                mouse.move(int(value), 0)
            elif int(message) == -2:
                mouse.move(0, int(value))

    @staticmethod
    def process(arg):
        port = Controller.get_device_port(Controller.COM_PORT_NAME)
        global do_simulate
        if port:
            if port.description.__contains__(Controller.COM_PORT_NAME):
                ser = serial.Serial()
                ser.baudrate = 115200
                ser.port = port.device
                ser.open()
                while ser.is_open:
                    try:
                        received = str(ser.readline())
                        Controller.perform_action(received.split(';')[0][2:], received.split(';')[1][:-5])
                    except:
                        pass
        else:
            messagebox.showerror("Error", "Device not found")


class StartView(tk.Frame):
    def __init__(self, master=None):
        tk.Frame.__init__(self, master)

        self.resize()
        self.createElements()

    def raise_frame(self):
        self.tkraise()

    def createElements(self):
        self.simulate = tk.Button(self)
        self.simulate_text()
        self.simulate["command"] = self.simulate_gun
        self.configure = tk.Button(self)
        self.configure["text"] = "Configure"
        self.configure["command"] = self.configure_gun
        self.Exit = tk.Button(self, text="Exit", command=root.destroy)
        self.placeElements()

    def placeElements(self):
        self.pack()
        self.simulate.grid(row=0, pady=10)
        self.configure.grid(row=1,pady=10)
        self.Exit.grid(row=2,pady=10)

    def configure_gun(self):
        global do_simulate
        do_simulate = False
        self.pack_forget()
        global control
        control.raise_frame()
        control.redraw()

    def simulate_gun(self):
        global do_simulate
        do_simulate = not do_simulate
        self.simulate_text()

    def simulate_text(self):
        global do_simulate
        if do_simulate:
            self.simulate["text"] = "Stop Simulation"
        else:
            self.simulate["text"] = "Simulate"

    def redraw(self):
        self.resize()
        self.placeElements()
        self.simulate_text()

    def resize(self):
        w = root.winfo_screenwidth() * 0.2
        h = root.winfo_screenheight() * 0.14
        x = (root.winfo_screenwidth() / 2) - (w / 2)
        y = (root.winfo_screenheight() / 2) - (h / 2)
        self.master.geometry('%dx%d+%d+%d' % (w, h, x, y))


class ConfigureView(tk.Frame):
    def __init__(self, master=None):
        tk.Frame.__init__(self, master)
        self.Dosimulate = True
        self.createElements()

    def raise_frame(self):
        self.tkraise()

    def placeElements(self):
        self.pack()
        self.save.grid(row=1,column=2, pady=10)
        self.back.grid(row=2, column=2, pady=10)

    def createElements(self):
        global configuration
        for button,row in zip(configuration.buttons,range(len(configuration.buttons))):
            ButtonView(self,button,row,0)

        self.save = tk.Button(self)
        self.save["text"] = "Save"
        self.save["command"] = configuration.save
        self.back = tk.Button(self)
        self.back["text"] = "Back"
        self.back["command"] = self.configure_gun



    def configure_gun(self):
        global do_simulate
        do_simulate = True
        self.pack_forget()
        global app
        app.redraw()
        app.raise_frame()

    def simulate_gun(self):
        self.Dosimulate = not self.Dosimulate
        if self.Dosimulate:
            self.simulate["text"] = "Stop Simulation"
        else:
            self.simulate["text"] = "Simulate"

    def redraw(self):
        self.resize()
        self.placeElements()

    def resize(self):
        w = root.winfo_screenwidth() * 0.2
        h = root.winfo_screenheight() * 0.4
        x = (root.winfo_screenwidth() / 2) - (w / 2)
        y = (root.winfo_screenheight() / 2) - (h / 2)
        self.master.geometry('%dx%d+%d+%d' % (w, h, x, y))
        self.focus_set()


def main():
    global do_simulate, configuration, control, root,app
    do_simulate = True
    controller_thread = th.Thread( target=Controller.process, args=("Thread-1", ))
    controller_thread.setDaemon(True)
    controller_thread.start()

    configuration = Configuration()

    root = tk.Tk()
    root.resizable(False, False)
    app = StartView(master=root)
    app.master.title("Gun Controller")

    control = ConfigureView(master=root)

    app.raise_frame()
    root.mainloop()


if __name__ == "__main__":
    main()
