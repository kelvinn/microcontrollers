
esptool.py -p /dev/tty.wchusbserial1430 erase_flash
esptool.py -p /dev/tty.wchusbserial1430 --baud 115200 write_flash -fm dio --flash_size=detect 0 ~/Downloads/esp8266-20180820-v1.9.4-479-g828f771e3.bin
