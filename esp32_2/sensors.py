import time
from machine import Pin
from machine import ADC
import utime as time
from dht import DHT22

# Set init value
last_interrupt = 0

def temp_and_hum():
    pin = Pin(2), Pin.IN, Pin.PULL_UP)
    data = DHT22(pin)
    data.measure()
    temp = str(data.temperature())
    hum = str(data.humidity())
    print("dht22: %s, %s" % (temp, hum))
    return temp, hum


"""


# The PIR Sensor
def pir():
    pin = Pin(17, Pin.IN, Pin.PULL_UP)
    reading = str(pin.value())
    print("pir: %s" % reading)
    return reading


def callback(pin):
    global last_interrupt
    now = time.time()
    print(now)
    if now > last_interrupt + 5:
        print("Interrupt has occurred")
        last_interrupt = now


def setup_pir_callback():
    button = Pin(19, Pin.IN)
    button.irq(trigger=Pin.IRQ_RISING, handler=callback)
    
"""

