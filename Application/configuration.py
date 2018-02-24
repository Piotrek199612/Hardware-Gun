import os
import tkinter as tk
from pynput import keyboard, mouse
from pynput.keyboard import Key
from pynput.mouse import Button

class Configuration:
    """Configuration file name"""
    file_name = "config.gun"

    """List of buttons"""
    buttons = []

    """Reads or creates a configuration file"""
    def __init__(self):
        if not os.path.isfile(self.file_name):
            self.create_default_file()
        with open(self.file_name, 'r') as config:
            for line in config:
                if line != '':
                    code = line.split(';')[0]
                    value = self.get_key(line.split(';')[1].replace('\n',''))
                    self.buttons.append(Button(code, value))


    """Return Key object from string"""
    @staticmethod
    def get_key(key_string):
        if not key_string.__contains__('Key.'):
            return key_string
        elif key_string == 'Key.alt ':
            return Key.alt
        elif key_string == 'Key.backspace':
            return Key.backspace
        elif key_string == 'Key.caps_lock':
            return Key.caps_lock
        elif key_string == 'Key.cmd':
            return Key.cmd
        elif key_string == 'Key.ctrl':
            return Key.ctrl
        elif key_string == 'Key.delete':
            return Key.delete
        elif key_string == 'Key.down':
            return Key.down
        elif key_string == 'Key.end':
            return Key.end
        elif key_string == 'Key.enter':
            return Key.enter
        elif key_string == 'Key.esc':
            return Key.esc
        elif key_string == 'Key.f1':
            return Key.f1
        elif key_string == 'Key.f2':
            return Key.f2
        elif key_string == 'Key.f3':
            return Key.f3
        elif key_string == 'Key.f4':
            return Key.f4
        elif key_string == 'Key.f5':
            return Key.f5
        elif key_string == 'Key.f6':
            return Key.f6
        elif key_string == 'Key.f7':
            return Key.f7
        elif key_string == 'Key.f8':
            return Key.f8
        elif key_string == 'Key.f9':
            return Key.f9
        elif key_string == 'Key.f10':
            return Key.f10
        elif key_string == 'Key.f11':
            return Key.f11
        elif key_string == 'Key.f12':
            return Key.f12
        elif key_string == 'Key.home':
            return Key.home
        elif key_string == 'Key.insert':
            return Key.insert
        elif key_string == 'Key.left':
            return Key.left
        elif key_string == 'Key.menu':
            return Key.menu
        elif key_string == 'Key.num_lock ':
            return Key.num_lock
        elif key_string == 'Key.page_down':
            return Key.page_down
        elif key_string == 'Key.page_up':
            return Key.page_up
        elif key_string == 'Key.pause':
            return Key.pause
        elif key_string == 'Key.print_screen ':
            return Key.print_screen
        elif key_string == 'Key.right':
            return Key.page_down
        elif key_string == 'Key.scroll_lock':
            return Key.scroll_lock
        elif key_string == 'Key.shift':
            return Key.shift
        elif key_string == 'Key.space':
            return Key.space
        elif key_string == 'Key.tab':
            return Key.tab
        elif key_string == 'Key.up':
            return Key.up

    """Saves configuration file"""
    def save(self):
        with open(self.file_name, 'w') as config:
            for button in self.buttons:
                button.save(config)

    """Creates file with default values"""
    def create_default_file(self):
        with open(self.file_name, 'w') as file:
            default = self.default_config()
            file.writelines(default)

    """Default values"""
    @staticmethod
    def default_config():
        default = []
        default.append("01;w\n")
        default.append("02;s\n")
        default.append("03;a\n")
        default.append("04;d\n")
        default.append("05;l\n")
        default.append("06;r\n")
        default.append("07;e")
        return default

class Button:
    def __init__(self, code, value):
        self.code = code
        self.value = value

    def save(self, opened_file):
        opened_file.write(self.code)
        opened_file.write(';')
        opened_file.write(str(self.value).replace('\n','').replace('\'', ''))
        opened_file.write('\n')

    def get_text_value(self):
        return str(self.value).replace('\'','')

class ButtonView:
    def __init__(self,frame, button, row, column):
        self.name = self.get_name(button.code)
        self.button = button
        self.row = row
        self.column = column
        self.frame = frame

        self.label = tk.Label(frame)
        self.label["text"] = self.name
        self.label.grid(row=self.row,column=self.column, pady=10, padx=0)

        self.btn = tk.Button(frame)
        self.btn["text"] = self.button.get_text_value()
        self.btn["command"] = self.get_input
        self.btn.grid(row=self.row, column=self.column + 1, pady=10, padx=50)

    @staticmethod
    def get_name(code):
        name = {
            '01': 'Front grip 1',
            '02': 'Front grip 2',
            '03': 'Front grip 3',
            '04': 'Front grip 4',
            '05': 'Front grip 5',
            '06': 'Aim',
            '07': 'Trigger',
        }
        return name[str(code)]

    def get_input(self):
        self.btn["text"] = ""

        self.mouse_listener = mouse.Listener(
                on_click=self.on_click
                )
        self.mouse_listener.start()

        self.keyboard_listener = keyboard.Listener(
                on_press=self.on_press
        )
        self.keyboard_listener.start()
        self.btn.configure(bg="blue")

    def on_press(self,key):
        self.button.value = key
        self.btn["text"] = self.button.get_text_value()
        self.btn.configure(bg="SystemButtonFace")
        self.mouse_listener.stop()
        self.keyboard_listener.stop()
        return False

    def on_click(self,x, y, button, pressed):
        self.button.value = button
        self.btn["text"] = self.button.get_text_value()
        self.btn.configure(bg="SystemButtonFace")
        self.mouse_listener.stop()
        self.keyboard_listener.stop()
        return False
