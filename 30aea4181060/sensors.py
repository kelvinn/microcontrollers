import time
from machine import Pin
from machine import ADC
from dht import DHT11
import onewire
import ds18x20
import sds011


def temp_and_hum():
    data = DHT11(Pin(16))
    data.measure()
    temp = str(data.temperature())
    hum = str(data.humidity())
    print("dht11: %s, %s" % (temp, hum))
    return temp, hum


def temperature():
    dat = Pin(15)
    ds = ds18x20.DS18X20(onewire.OneWire(dat))
    sensors = ds.scan()
    ds.convert_temp()
    time.sleep_ms(750)
    reading = str(ds.read_temp(sensors[0]))
    print("onewire: %s" % reading)
    return reading


def light():
    pin = Pin(33)
    adc = ADC(pin)
    adc.atten(ADC.ATTN_11DB)
    reading = str(adc.read())
    print("light: %s" % reading)
    return reading


def pm():
    pm25, pm10, packet_status = sds011.read()
    return pm25, pm10, packet_status


def aqi():
    pin = Pin(34)
    adc = ADC(pin)
    adc.atten(ADC.ATTN_11DB)
    reading = str(adc.read())
    print("aqi: %s" % reading)
    return reading
