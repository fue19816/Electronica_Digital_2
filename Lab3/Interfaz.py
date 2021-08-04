from tkinter import *
import time
import serial

ser = serial.Serial()

class Window(Frame):
    def __init__(self, master = None):
        Frame.__init__(self, master)

        self.master = master
        self.init_window()

    def init_window(self):

        self.master.title("GUI")

        self.pack(fill=BOTH, expand=1)

        conectar = Button(self, text="Conectar", command=self.connection_pic)

        quitButton = Button(self, text="Quit", command=self.client_exit)

        enviar = Button(self, text="Enviar dato", command = self.send_value)

        cajatexto = Entry(self)

        label1 = Label(self, text="Coloque el valor a desplegar")

        valorpot1 = Label(self, text=ser.read())

        conectar.place(x=50,y=0)

        enviar.place(x=175,y=150)

        label1.place(x=0,y=125)

        cajatexto.place(x=160,y=125)

        quitButton.place(x=0,y=0)

        valorpot1.place(x=20,y=250)

        valorpot2.place(x=210,y=250)

    def client_exit(self):
        exit()

    def connection_pic(self):
        try:
            return
            ser = serial.Serial(port='COM3', baudrate=9600, parity=serial.PARITY_NONE,stopbits=serial.STOPBITS_ONE,bytesize=serial.EIGHTBITS)
            ser.open()
            ser.isOpen()
        except:
            return None

    def send_value(self):
        input = cajatexto.get()
        ser.write(input)


root = Tk()
root.geometry("400x300")
app = Window(root)

root.mainloop()
