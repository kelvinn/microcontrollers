import time
from machine import Pin
from machine import ADC
import utime as time

# Set init value
last_interrupt = 0

# The MQ-6 Sensor
def propane():
    pin = Pin(35)
    adc = ADC(pin)
    adc.atten(ADC.ATTN_11DB)
    reading = str(adc.read())
    volt = reading / 1024 * 5.0
    print("propane: %s" % volt)
    return volt


# The MQ-4 Sensor
def methane():
    pin = Pin(33)
    adc = ADC(pin)
    adc.atten(ADC.ATTN_11DB)
    reading = str(adc.read())
    volt = reading / 1024 * 5.0
    print("methane: %s" % volt)
    return volt


def callback(pin):
    global last_interrupt
    now = time.time()
    if now > last_interrupt + 5:
        print("Interrupt has occurred")
        last_interrupt = now


def setup_push_button():
    button = Pin(18, Pin.IN, Pin.PULL_UP)
    button.irq(trigger=Pin.IRQ_FALLING, handler=callback)

