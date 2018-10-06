#!/usr/bin/env python3
from umqtt.robust import MQTTClient
import network
import utime as time
import ubinascii
from machine import Pin, unique_id, RTC, DEEPSLEEP, reset_cause, DEEPSLEEP_RESET
import sensors
from config import config

sta_if = network.WLAN(network.STA_IF)

mqtt_host = config['mqtt']['host']
mqtt_username = config['mqtt']['username']
mqtt_password = config['mqtt']['password']

mcu_id = unique_id()
mcu_name = ubinascii.hexlify(mcu_id).decode('utf-8')

client = MQTTClient(mcu_name, mqtt_host, user=mqtt_username, password=mqtt_password)

# Input pin D2 on ESP32
INPUTPIN = 26

# Check if the device woke from a deep sleep
if reset_cause() == DEEPSLEEP_RESET:
    print('woke from a deep sleep')


def is_wifi_connected():
    if sta_if.isconnected():
        return True
    else:
        print("Could not connect to WiFi... trying again.")
        sta_if.connect()
        return False


def is_mqtt_connected():
    try:
        client.ping()
    except (OSError, AttributeError):
        print("Could not connect to MQTT... trying again.")
        client.reconnect()
        return False
    else:
        return True


def callback(pin):
    is_wifi_connected()
    is_mqtt_connected()
    client.publish(b"home/kitchen/pir", "ON")


p33 = Pin(33, Pin.IN, Pin.PULL_UP)
p33.irq(trigger=Pin.IRQ_FALLING, handler=callback)


def main():
    # Try to ensure MQTT is up before looping

    is_mqtt_connected()
    while True:

        try:

            # probably need to change to ms
            now = time.time()

            # Do the 10 second stuff, which for me includes publishing mqtt messages
            if now % 10 == 0:

                # Check if WiFi and MQTT work
                if is_wifi_connected and is_mqtt_connected():

                    pm25, pm10, packet_status = sensors.pm()
                    client.publish(b"home/kitchen/pm25", str(pm25))
                    client.publish(b"home/kitchen/pm10", str(pm10))
                    client.publish(b"home/kitchen/packet_status", str(packet_status))

                    # Now publish stats
                    client.publish(b"home/kitchen/light", sensors.light())
                    client.publish(b"home/kitchen/onewire", sensors.temperature())
                    client.publish(b"home/kitchen/aqi", sensors.aqi())
                    #temperature, humidity = sensors.temp_and_hum()
                    #client.publish(b"home/kitchen/temperature", temperature)
                    #client.publish(b"home/kitchen/humidity", humidity)

        except BaseException as error:
            print('An exception occurred: {}'.format(error))


if __name__ == '__main__':
    main()
