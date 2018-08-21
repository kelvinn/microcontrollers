from machine import Pin
from dht import DHT22


def temp_and_hum():
    pin = Pin(14, Pin.IN, Pin.PULL_UP)
    data = DHT22(pin)
    data.measure()
    temp = str(data.temperature())
    hum = str(data.humidity())
    print("dht22: %s, %s" % (temp, hum))
    return temp, hum