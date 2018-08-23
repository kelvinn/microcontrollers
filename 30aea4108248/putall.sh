
killall SCREEN

echo "Uploading Config"
ampy -d 2 -p /dev/tty.SLAB_USBtoUART put config.py

echo "Uploading Boot"
ampy -d 5 -p /dev/tty.SLAB_USBtoUART put boot.py

echo "Uploading Main"
ampy -d 2 -p /dev/tty.SLAB_USBtoUART put main.py

echo "Uploading Sensors"
ampy -d 2 -p /dev/tty.SLAB_USBtoUART put sensors.py