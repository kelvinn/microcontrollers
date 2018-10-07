#!/usr/bin/env python3
from umqtt.simple import MQTTClient
import network
import utime as time
import ubinascii
from machine import Pin, unique_id, RTC, DEEPSLEEP, reset_cause, DEEPSLEEP_RESET
import sensors
from config import config
import hassnode

sta_if = network.WLAN(network.STA_IF)

mqtt_host = config['mqtt']['host']
mqtt_username = config['mqtt']['username']
mqtt_password = config['mqtt']['password']

MCU_ID = unique_id()
MCU_NAME = ubinascii.hexlify(MCU_ID).decode('utf-8')


mqtt = MQTTClient(MCU_NAME, mqtt_host, user=mqtt_username, password=mqtt_password)

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
        mqtt.ping()
    except (OSError, AttributeError):
        print("Could not connect to MQTT... trying again.")
        mqtt.connect()
        return False
    else:
        return True


def callback():
    is_wifi_connected()
    is_mqtt_connected()
    mqtt.publish(b"home/kitchen/pir", "ON")


def publish_to_mqtt(topic, value):
    if is_wifi_connected and is_mqtt_connected():
        mqtt.publish(b"%s" % topic, value)


def main():
    # Try to ensure MQTT is up before progressing
    is_mqtt_connected()

    # Give auto discover a try
    aqi_sensor = hassnode.Sensor(mqtt, "Kitchen Air Quality", "V", "aqi")
    temperature_sensor = hassnode.Sensor(mqtt, "Kitchen Temperature", "°C", "onewire")
    light_sensor = hassnode.Sensor(mqtt, "Kitchen Light", "V - Light", "light")
    pm25_sensor = hassnode.Sensor(mqtt, "Kitchen PM 2.5", "µg/m3", "pm25")
    pm10_sensor = hassnode.Sensor(mqtt, "Kitchen PM 10", "µg/m3", "pm10")
    pm_status_sensor = hassnode.BinarySensor(mqtt, "Kitchen PM Status", "problem",  "packet_status")

    while True:

        try:

            # probably need to change to ms
            now = time.time()

            # Do the 10 second stuff, which for me includes publishing mqtt messages
            if now % 10 == 0:

                # Check if WiFi and MQTT work
                if is_wifi_connected and is_mqtt_connected():

                    # Publish Particulate Matter stats
                    pm25, pm10, packet_status = sensors.pm()
                    pm25_sensor.setState(pm25)
                    pm10_sensor.setState(pm10)

                    if packet_status is "OK":
                        pm_status_sensor.off()
                    else:
                        pm_status_sensor.on()

                    # Try do use hassnode
                    aqi_sensor.setState(sensors.aqi())
                    light_sensor.setState(sensors.light())
                    temperature_sensor.setState(sensors.temperature())

        except BaseException as error:
            print('An exception occurred: {}'.format(error))


if __name__ == '__main__':
    main()
