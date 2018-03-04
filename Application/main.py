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

import time

do_simulate = None
controller = None
control = None
root = None
app = None

last_value = 0
calk = 0
pos = 0
start = True

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
    def calibrate():
        print("Calibrate Start")
        mouse = MController()
        h = root.winfo_screenheight()
        for i in range(5):
            mouse.position = mouse.position[0], h
            time.sleep(0.05)
            mouse.position = mouse.position[0], h
            time.sleep(0.05)

        for i in range(180):
            Controller.move_by_one_deg(1)
            print(i)
        print("Calibrate End")

    factor = 83

    @staticmethod
    def move_by_one_deg(sign,_factor = factor):
        mouse = MController()
        h = root.winfo_screenheight()
        mouse.position = mouse.position[0], round(h / 2 - sign * h / 2 / _factor)  # h/4)
        time.sleep(0.03)
        #print(i)

    @staticmethod
    def move_by_one_deg_X(X, _factor=factor):
        mouse = MController()
        h = root.winfo_screenheight()
        mouse.position = mouse.position[0], round(h / 2 + X * h / 2 / _factor)  # h/4)
        time.sleep(0.03)
        # print(i)
    @staticmethod
    def move_by_X_deg(X):
        sign = 1
        if X < 0:
            sign = -1
            X = abs(X)
        for i in range(X):
            Controller.move_by_one_deg(sign)


    @staticmethod
    def perform_action(message, value):
        global configuration
        message = int(message)
        value = float(value)
        if do_simulate:
            keyboard = KController()
            mouse = MController()
            h = root.winfo_screenheight()
            if 0<= message <40:
                key = configuration.buttons[int(message)].value
                if not str(key).__contains__("Button"):
                    print(value)
                    keyboard.press(key)
                    keyboard.release(key)
                    if value == 1 and key == 'd':
                        print('Calibrate')
                        Controller.calibrate()
                    elif value == 1 and key != 'd':
                        #Controller.move_by_X_deg(-10)
                        Controller.move_by_one_deg_X(-10)
                elif str(key) == "Button.left":
                    mouse.click(Button.left)
                elif str(key) == "Button.right":
                    mouse.click(Button.right)
            elif message == 40:
                global start
                global calk
                if (start):
                    calk = value
                    start = False
                #print(value,end=' ')
                #print(calk,end=' ')
                #print(round(value - calk))
                #Controller.move_by_X_deg(round(value - calk))
                if abs(round(value - calk)) > 1:
                    Controller.move_by_one_deg_X(value - calk)
                    calk = calk + value - calk
                #mouse.position = mouse.position[0] , round(value / 180 * h)
                """
                global last_value
                global calk
                global pos

                h = root.winfo_screenheight()

                if calk > h/2:
                    calk-=1
                elif calk<h/2:
                    calk+=1
                offset = ((value) / 180) * h - last_value
                print(offset)
                print(mouse.position)
                last_value = ((value) / 180) * h
                calk+=offset
                if value/180*h == h/2:
                    calk = h/2
                mouse.position = (mouse.position[0],round(calk))
                """
                """
               # print(value/180)
                #print(last_value - value / 180)

                #if (last_value - (value)/180) <= 0.01 and (last_value - value/180) >= -0.01 :
                #    last_value = value / 180
                #else
                distance = abs(last_value - (value - 90) / 180)
                #if distance <= 0.002:
                #    distance = 0
                offset = (1.0 if value-90 >=0 else -1.0) * distance
                if offset <= 0.02 and offset >= -0.02:
                    offset = 0
                print(offset)
                print(round(offset * configuration.controller_sensitivity))
                mouse.move(0,round(offset*configuration.controller_sensitivity))
                #mouse.move(0,round((value - 90) / 180*configuration.controller_sensitivity))
                print("a")
                last_value =(value- 90) / 180

                #mouse.move(0,round((value-90)/180*configuration.controller_sensitivity))
            #elif message == 41:
                #mouse.move( round((180*-value)/180*configuration.controller_sensitivity),0)
                """
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
                        #mouse = MController()
                        #print(mouse.position)
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
        self.label.grid(row=3, column=2, pady=10)
        self.entry.grid(row=4, column=2, pady=10)

    def createElements(self):
        global configuration
        for button,row in zip(configuration.buttons,range(len(configuration.buttons))):
            ButtonView(self, button, row, 0)

        self.save = tk.Button(self)
        self.save["text"] = "Save"
        self.save["command"] = self.save_config

        self.back = tk.Button(self)
        self.back["text"] = "Back"
        self.back["command"] = self.configure_gun

        self.label = tk.Label(self)
        self.label["text"] = "Controller sensitivity"
        self.label.grid(row=3, column=1, pady=10, padx=0)
        self.label.focus()

        self.entry = tk.Entry(self)
        self.entry.configure(justify=tk.CENTER)
        self.entry.delete(0, tk.END)
        self.entry.insert(0, configuration.controller_sensitivity)

    def save_config(self):
        global configuration
        try:
            configuration.controller_sensitivity = float(self.entry.get())
        except:
            messagebox.showerror("Error", "Bad controller sensitivity")
            configuration.controller_sensitivity = configuration.DEFAULT_CONTROLLER_SENSITIVITY
            self.entry.delete(0, tk.END)
            self.entry.insert(0, configuration.controller_sensitivity)
        configuration.save()

    def configure_gun(self):
        self.label.focus()
        global configuration
        try:
            configuration.controller_sensitivity = float(self.entry.get())
        except:
            messagebox.showerror("Error", "Bad controller sensitivity")
            configuration.controller_sensitivity = configuration.DEFAULT_CONTROLLER_SENSITIVITY
            self.entry.delete(0, tk.END)
            self.entry.insert(0, configuration.controller_sensitivity)
        print(configuration.controller_sensitivity)
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
        h = root.winfo_screenheight() * 0.5
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
    global calk
    calk = root.winfo_screenheight() / 2
    app = StartView(master=root)
    app.master.title("Gun Controller")

    control = ConfigureView(master=root)

    app.raise_frame()
    root.mainloop()


if __name__ == "__main__":
    main()
