
killall SCREEN

echo "Uploading Config"

ampy -d 2 -p /dev/tty.wchusbserial1430 put config.py

echo "Uploading Boot"
ampy -d 2 -p /dev/tty.wchusbserial1430 put boot.py

echo "Uploading Main"
ampy -d 10  -p /dev/tty.wchusbserial1430 put main.py

echo "Uploading Sensors"
ampy -d 10  -p /dev/tty.wchusbserial1430 put sensors.py