#!/usr/bin/env python3
from umqtt.robust import MQTTClient
import network
import utime as time
import ubinascii
from machine import Pin, unique_id, RTC, DEEPSLEEP, reset_cause, DEEPSLEEP_RESET

import sensors
from config import config

# Set init value
last_interrupt = 0

sta_if = network.WLAN(network.STA_IF)

mqtt_host = config['mqtt']['host']
mqtt_username = config['mqtt']['username']
mqtt_password = config['mqtt']['password']

mcu_id = unique_id()
mcu_name = ubinascii.hexlify(mcu_id).decode('utf-8')

client = MQTTClient(mcu_name, mqtt_host, user=mqtt_username, password=mqtt_password)
client.connect()


# check if the device woke from a deep sleep
if reset_cause() == DEEPSLEEP_RESET:
    print('woke from a deep sleep')


def callback(pin):
    global last_interrupt
    now = time.time()
    print(now)
    if now > last_interrupt + 5:
        print("pir: on")
        client.publish(b"home/kitchen/pir", "ON")
        last_interrupt = now


p16 = Pin(16, Pin.IN, Pin.PULL_UP)
p16.irq(trigger=Pin.IRQ_RISING, handler=callback)


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
    except OSError:
        print("Couldn't contact MQTT... trying again.")
        client.reconnect()


def main():
    #sensors.setup_pir_callback(client)
    noise_measurements = []
    while True:

        try:

            now = time.time()

            # Do the every second stuff
            #sensors.pir()
            #noise = sensors.noise()
            #noise_measurements.append(noise)

            # Do the 10 second stuff, which for me includes publishing mqtt messages
            if now % 10 == 0:

                #noise_avg = sum(noise_measurements) / len(noise_measurements)

                # First check if wifi and MQTT is up
                is_wifi_connected()
                is_mqtt_connected()

                #pm25, pm10, packet_status = sensors.pm()
                #client.publish(b"home/kitchen/pm25", str(pm25))
                #client.publish(b"home/kitchen/pm10", str(pm10))
                #client.publish(b"home/kitchen/packet_status", str(packet_status))

                # Now publish stats
                #client.publish(b"home/kitchen/noise", str(noise_avg))

                # Reset
                noise_measurements = []


        except BaseException as error:
            print('An exception occurred: {}'.format(error))


if __name__ == '__main__':
    main()
