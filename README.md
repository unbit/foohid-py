# foohid-py
Python wrapper for the foohid OSX driver

https://github.com/unbit/foohid

Exposed functions
=================

```py
import foohid

foohid.create('your new device', report_descriptor, serial_number, vendor_id, device_id)
foohid.destroy('your new device')
foohid.send('your new device', hid_message)
foohid.list()
```

Included tests/examples
=======================

test_mouse.py -> creates a virtual mouse and randomly moves it (your cursor will move too ;)

test_joypad.py -> creates a virtual joypad and randomly moves left and right axis (run a game with joypad support to check it)

test_keyboard.py -> creates a virtual keyboard and presses the "a" key

test_list.py -> tests listing feature
