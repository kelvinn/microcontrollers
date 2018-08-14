from machine import Pin
from dht import DHT22


def temp_and_hum():
    data = DHT22(Pin(2))
    data.measure()
    temp = str(data.temperature())
    hum = str(data.humidity())
    print("dht22: %s, %s" % (temp, hum))
    return temp, hum