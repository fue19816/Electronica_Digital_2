# Código de ejemplo AdafruitIO
# Universidad del Valle de Guatemala
# IE3027 - Electrónica Digital 2
# Editado por Oscar Fuentes

# Adafruit IO
# https://io.adafruit.com/

# if Module not Found. Open Terminal/CMD and execute:
# pip3 install Adafruit_IO

from Adafruit_IO import Client, RequestError, Feed
import serial
import time

ser = serial.Serial('COM2', baudrate = 9600)

ADAFRUIT_IO_KEY = "aio_waOm37ypx0VILIlaZjY2DBjDixrS"
ADAFRUIT_IO_USERNAME = "fue19816"
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

while(1):
    ser.close()
    ser.open()
    dataser = str(ser.read(9))
    print(dataser)
    dataser1 = dataser.split(',')
    d1 = dataser1[1]

    print(d1)
    
    #Sensor1 Feed
    sensor1_feed = aio.feeds('sensor1')
    aio.send_data(sensor1_feed.key, d1)
    sensor1_data = aio.receive(sensor1_feed.key)
    print(f'sensor1 signal: {sensor1_data.value}')

    #Sensor2 Feed
    sensor2_feed = aio.feeds('sensor2')
    aio.send_data(sensor2_feed.key, 80)
    sensor2_data = aio.receive(sensor2_feed.key)
    print(f'Sensor2 signal: {sensor2_data.value}')

    #Recibir
    envio_feed = aio.feeds('envio')
    envio_data = aio.receive(envio_feed.key)
    print(f'valor variable: {envio_data.value}')
    dato = int(envio_data.value)
    data = dato.to_bytes(3, 'big')
    ser.write(data)
    time.sleep(5)
