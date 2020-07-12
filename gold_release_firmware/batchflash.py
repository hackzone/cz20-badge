import sys, threading, time
from os import system
from os.path import exists

def find_device(number):
    devices = ['/dev/ttyUSB%d' % number, '/dev/ttyACM%d' % number, '/dev/tty.usbmodem123456%d' % number, '/dev/tty.usbmodem%d' % number, '/dev/tty.usbmodem123456%d' % number]
    for device in devices:
        if exists(device):
            return device
    return None

def flash_daemon(number):
    print('Starting flash thread for %s' % number)
    sys.stdout.flush()
    while True:
        device = find_device(number)
        if device is not None:
            print('Flashing %s' % device)
            sys.stdout.flush()
            system('esptool.py -p %s --baud 2000000 write_flash 0x1e1000 initial_fs.zip 0xd000 ota_data_initial.bin 0x1000 bootloader.bin 0x10000 firmware.bin 0x8000 campzone2020_16MB.bin' % device)

            # Give system time to adjust
            time.sleep(1)

            # Wait for device to detach
            while exists(device):
                time.sleep(0.1)

        time.sleep(0.1)

for i in range(0,60):
    threading.Thread(target=flash_daemon, args=(i,)).start()

while True:
    time.sleep(1)
# flash_daemon(0)
