#!/usr/bin/env python3
from umqtt.robust import MQTTClient
import network
import utime as time
import ubinascii
from machine import Pin, unique_id
import sensors
from config import config

sta_if = network.WLAN(network.STA_IF)

mqtt_host = config['mqtt']['host']
mqtt_username = config['mqtt']['username']
mqtt_password = config['mqtt']['password']

mcu_id = unique_id()
mcu_name = ubinascii.hexlify(mcu_id).decode('utf-8')

client = MQTTClient(mcu_name, mqtt_host, user=mqtt_username, password=mqtt_password)
client.connect()


def checkwifi():
    while not sta_if.isconnected():
        time.sleep_ms(500)
        print(".")
        sta_if.connect()


def is_mqtt_connected():
    try:
        client.ping()
    except OSError:
        client.connect()
        time.sleep(5)


def main():
    sensors.setup_push_button()
    while True:
        checkwifi()
        try:
            is_mqtt_connected()
            #client.publish(b"home/bedroom/propane", sensors.propane())
            #client.publish(b"home/bedroom/methane", sensors.methane())
        except:
            print("Caught some exception...")
        time.sleep_ms(10000)


if __name__ == '__main__':
    main()
