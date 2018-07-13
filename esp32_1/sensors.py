import time
from machine import Pin, ADC
from dht import DHT22
import onewire, ds18x20

def get_dht11_temperature():
    data = DHT22(Pin(16))
    data.measure()
    temp = data.temperature()
    hum = data.humidity()
    print('Temp: {}oC'.format(temp))
    print('Hum:  {}%'.format(hum))

def get_onewire_temperature():
    dat = Pin(25)
    ds = ds18x20.DS18X20(onewire.OneWire(dat))
    sensors = ds.scan()
    print('found devices:', sensors)
    ds.convert_temp()
    time.sleep_ms(750)
    print(ds.read_temp(sensors[0]))

def get_light():
    pin = Pin(34)
    light = ADC(pin).read()
    print(light)