
import network
import upip
from config import config
import utime as time
import esp

esp.osdebug(None)

wifi_name = config['wifi']['name']
wifi_pass = config['wifi']['pass']
sta_if = network.WLAN(network.STA_IF)
sta_if.active(True)
sta_if.connect(wifi_name, wifi_pass)

# MIGHT AS WELL SLEEP 5 SECONDS

time.sleep_ms(5000)

# Wait until network is connected
while not sta_if.isconnected():
    time.sleep_ms(500)
    print(".")

print("Set DNS if not existing")
network_config = sta_if.ifconfig()

if len(network_config) == 3:
    network_config += "8.8.8.8"
    sta_if.ifconfig(network_config)


upip.install('micropython-umqtt.simple')