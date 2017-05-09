import foohid
import struct
import random
import time

device_name = "FooHID simple joypad"
device_serial = "SN 123"
vendor_id = 0x1234
product_id = 0x4321

joypad = (
    0x05, 0x01,
    0x09, 0x05,
    0xa1, 0x01,
    0xa1, 0x00,
    0x05, 0x09,
    0x19, 0x01,
    0x29, 0x10,
    0x15, 0x00,
    0x25, 0x01,
    0x95, 0x10,
    0x75, 0x01,
    0x81, 0x02,
    0x05, 0x01,
    0x09, 0x30,
    0x09, 0x31,
    0x09, 0x32,
    0x09, 0x33,
    0x15, 0x81,
    0x25, 0x7f,
    0x75, 0x08,
    0x95, 0x04,
    0x81, 0x02,
    0xc0,
    0xc0)

joypad_str = ''.join(chr(x) for x in joypad)

try:
    foohid.destroy(device_name)
except:
    pass
foohid.create(device_name, joypad_str, device_serial, vendor_id, product_id)

try:
    while True:
        x = random.randrange(0, 255)
        y = random.randrange(0, 255)
        z = random.randrange(0, 255)
        rx = random.randrange(0, 255)
        foohid.send(device_name, struct.pack('H4B', 0, x, y, z, rx))
        time.sleep(1)
except KeyboardInterrupt:
    foohid.destroy(device_name)
