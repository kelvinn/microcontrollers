import time
from machine import Pin
from machine import ADC
import utime as time
import sds011

# Set init value
last_interrupt = 0


def pm():
    pm25, pm10, packet_status = sds011.read()
    return pm25, pm10, packet_status


"""

def noise():

    pin = Pin(35)
    adc = ADC(pin)
    adc.atten(ADC.ATTN_11DB)

    peak_to_peak = 0

    signal_max = 0
    signal_min = 0
    sample_window = 50

    start_millis = time.ticks_ms()
    while (time.ticks_diff(time.ticks_ms(), start_millis) < sample_window):
        sample = adc.read()
        if sample > signal_max:
            signal_max = sample
        elif sample < signal_min:
            signal_min = sample
        peak_to_peak = signal_max - signal_min

    reading = (peak_to_peak * 3.3) / 4095

    #reading = str(sum(measurements) / len(measurements))
    return reading
    
"""


# The PIR Sensor
def pir():
    pin = Pin(17, Pin.IN, Pin.PULL_UP)
    reading = str(pin.value())
    print("pir: %s" % reading)
    return reading


def callback(client, pin):
    global last_interrupt
    now = time.time()
    print(now)
    if now > last_interrupt + 5:
        print("pir: on")
        client.publish(b"home/kitchen/pir", "ON")
        last_interrupt = now


def setup_pir_callback(client):
    print("Setup PIR sensor")
    p33 = Pin(33, Pin.IN, Pin.PULL_UP)
    p33.irq(trigger=Pin.IRQ_FALLING, handler=callback(client))
