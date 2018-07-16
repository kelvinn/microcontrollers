

# Setup virtualenv and install prereqs
~~~~
python3 -m venv ~/venv3
source ~/venv3/bin/activate
pip install esptool
pip install adafruit-ampy
~~~~

# Erase flash
~~~~
esptool.py --chip esp32 -p /dev/tty.SLAB_USBtoUART erase_flash
~~~~

# Load new MicroPython firmware
~~~~
esptool.py --chip esp32 -p /dev/tty.SLAB_USBtoUART write_flash -z 0x1000 ~/Downloads/esp32-20180711-v1.9.4-261-ge2e22e3d.bin
~~~~

# Connect
~~~~
screen /dev/tty.SLAB_USBtoUART 115200
help()
ctrl+a+d, then kill SCREEN
~~~~

# Put some files
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