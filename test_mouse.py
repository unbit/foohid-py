import foohid
import struct
import random
import time
    
mouse = (
    0x05, 0x01,
    0x09, 0x02,                    
    0xa1, 0x01,                    
    0x09, 0x01,                    
    0xa1, 0x00,                    
    0x05, 0x09,                    
    0x19, 0x01,                    
    0x29, 0x03,                    
    0x15, 0x00,                    
    0x25, 0x01,                    
    0x95, 0x03,                    
    0x75, 0x01,                    
    0x81, 0x02,                    
    0x95, 0x01,                    
    0x75, 0x05,                    
    0x81, 0x03,                    
    0x05, 0x01,                    
    0x09, 0x30,                    
    0x09, 0x31,                    
    0x15, 0x81,                    
    0x25, 0x7f,                    
    0x75, 0x08,                    
    0x95, 0x02,                    
    0x81, 0x06,                    
    0xc0,                          
    0xc0)

try:
    foohid.destroy("FooHID simple mouse")
except:
    pass
foohid.create("FooHID simple mouse", struct.pack('{0}B'.format(len(mouse)), *mouse), "SN 123456", 2, 3)

while True:
    x = random.randrange(0,255)
    y = random.randrange(0,255)
    foohid.send("FooHID simple mouse", struct.pack('3B', 0, x, y))
    time.sleep(1)
