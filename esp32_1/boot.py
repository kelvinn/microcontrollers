
import network
import upip
from config import config
import utime as time

wifi_name = config['wifi']['name']
wifi_pass = config['wifi']['pass']
sta_if = network.WLAN(network.STA_IF)
sta_if.active(True)
sta_if.connect(wifi_name, wifi_pass)

# Wait until network is connected
while not sta_if.isconnected():
    time.sleep_ms(500)
    print(".")

upip.install('micropython-umqtt.simple')
upip.install('micropython-umqtt.robust')