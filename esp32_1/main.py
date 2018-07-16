#!/usr/bin/env python3
from umqtt.robust import MQTTClient
import network
import utime as time
import sensors
from config import config

sta_if = network.WLAN(network.STA_IF)

mqtt_host = config['mqtt']['host']
mqtt_username = config['mqtt']['username']
mqtt_password = config['mqtt']['password']

client = MQTTClient("esp32-01", mqtt_host, user=mqtt_username, password=mqtt_password)
client.connect()


def checkwifi():
    while not sta_if.isconnected():
        time.sleep_ms(500)
        print(".")
        sta_if.connect()


def main():
    while True:
        checkwifi()
        try:
            client.publish(b"home/bedroom/light", sensors.light())
            client.publish(b"home/bedroom/onewire", sensors.temperature())
            client.publish(b"home/bedroom/aqi", sensors.aqi())
            temperature, humidity = sensors.temp_and_hum()
            client.publish(b"home/bedroom/temperature", temperature)
            client.publish(b"home/bedroom/humidity", humidity)
        except:
            print("Caught some exception...")
        time.sleep_ms(10000)


if __name__ == '__main__':
    main()
