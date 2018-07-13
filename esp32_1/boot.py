import network
import esp
from time import sleep
import config


def no_debug():
    # this can be run from the REPL as well
    esp.osdebug(None)


def turn_on_debug():
    # this can be run from the REPL as well
    esp.osdebug(True)


def connect():
    wifi_name = config['wifi']['name']
    wifi_pass = config['wifi']['pass']
    sta_if = network.WLAN(network.STA_IF)
    if not sta_if.isconnected():
        turn_on_debug()
        print('connecting to network...')
        sta_if.active(True)
        sta_if.connect(wifi_name, wifi_pass)
        while not sta_if.isconnected():
            sta_if = network.WLAN(network.STA_IF)
            print('network is not connected')
            sleep(1)
    else:
        print('already connected')
    print('network config:', sta_if.ifconfig())
    no_debug()

