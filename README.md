

# Setup virtualenv and install prereqs
~~~~
python3 -m venv ~/venv3
source ~/venv3/bin/activate
pip install esptool
pip install adafruit-ampy
~~~~


# For ESP32

## Erase flash - ESP32
~~~~
esptool.py --chip esp32 -p /dev/tty.SLAB_USBtoUART erase_flash
~~~~

## Load new MicroPython firmware
~~~~
esptool.py --chip esp32 -p /dev/tty.SLAB_USBtoUART write_flash -z 0x1000 ~/Downloads/esp32-20181106-v1.9.4-683-gd94aa577a.bin
~~~~



# For ESP8266

## Erase flash - ESP8266
~~~~
esptool.py --chip esp8266 -p /dev/tty.SLAB_USBtoUART erase_flash
~~~~

## Load new MicroPython firmware
~~~~
esptool.py --chip esp8266 -p /dev/tty.SLAB_USBtoUART --baud 460800 write_flash --flash_size=detect 0 ~/Downloads/esp8266-20180511-v1.9.4.bin
esptool.py --port /dev/tty.SLAB_USBtoUART --baud 115200 write_flash --flash_size=4MB 0 ~/Downloads/esp8266-20180511-v1.9.4.bin
~~~~

# Now, put files and do some stuff

## Connect
~~~~
screen /dev/tty.SLAB_USBtoUART 115200
help()
ctrl+a+d, then kill SCREEN
~~~~

## Put some files
~~~~
ampy -p /dev/tty.SLAB_USBtoUART put config.py
ampy -p /dev/tty.SLAB_USBtoUART put boot.py
ampy -p /dev/tty.SLAB_USBtoUART put sensors.py
~~~~

# Disconnect WiFi
~~~~
import network
sta_if = network.WLAN(network.STA_IF)
sta_if.disconnect()
~~~~