import os
import time
import re
import tkinter as tk
from tkinter import *
from tkinter import filedialog

from image_client import ImageClient
from image import *


class ImageClientGui():
    def __init__(self):
        self.app = tk.Tk()
        self.ip_label = Label(self.app, text="Host IP:")
        self.port_label = Label(self.app, text="PORT:   ")
        self.ip_entry = Entry(self.app)
        self.port_entry = Entry(self.app)
        self.send_button = tk.Button(self.app, text="Send image", state=tk.DISABLED,
                                     command=(self.send))
        self.menu = Menu(self.app)

    def activateSendButton(self):
        self.send_button["state"] = tk.NORMAL

    def deactiveSendButton(self):
        self.send_button["state"] = tk.DISABLED

    def selectImageFile(self):
        self.app.filename = filedialog.askopenfilename(
            initialdir=os.getcwd(), title="Select file", filetypes=(("png files", "*.png"), ("all files", "*.*")))

        if self.app.filename:
            self.activateSendButton()

    def send(self):
        assert re.match(r'[0-9,. ]+$', self.ip_entry.get()
                        ), 'host_ip should only contain numbers and dots'
        assert self.port_entry.get().isdigit() == True, 'port is should be integer'

        tcp_client = ImageClient()
        host_ip = self.ip_entry.get()
        port = int(self.port_entry.get())
        file_path = self.app.filename
        image = loadImage(file_path)
        pixel_data = image.tobytes()
        n_pixels = 4
        width, height = image.size
        img_size = width * height * n_pixels

        # Send data to host
        begin = time.time()
        tcp_client.connect(host_ip, port)
        tcp_client.send_all(pixel_data)

        # Receive data from host
        data = tcp_client.recv_all(img_size)
        end = time.time()
        tcp_client.close()

        result_path = file_path[:-4] + "_from_socfpga.png"
        saveImage(result_path, data, width, height)

        print("Time to recv bytes:", (end-begin))

    def initFileMenu(self):
        self.app.config(menu=self.menu)

        filemenu = Menu(self.menu, tearoff=0)
        self.menu.add_cascade(label='File', menu=filemenu)
        filemenu.add_command(label='Select Image',
                             command=self.selectImageFile)
        filemenu.add_command(label='Exit', command=self.app.quit)

    def initUI(self):
        self.app.geometry("240x120")
        self.app.resizable(width=False, height=False)
        self.app.title("Image Client")
        self.initFileMenu()

        self.ip_label.grid(row=0, column=0, padx=4, pady=8)
        self.ip_entry.grid(row=0, column=1, padx=4, pady=8)
        self.port_label.grid(row=1, column=0, padx=4, pady=8)
        self.port_entry.grid(row=1, column=1, padx=4, pady=8)
        self.send_button.grid(row=2, column=1, padx=4, pady=8)

    def start(self):
        self.initUI()
        self.app.mainloop()


def main():
    img_gui = ImageClientGui()
    img_gui.start()


if __name__ == "__main__":
    main()
