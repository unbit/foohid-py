# foohid-py
Python wrapper for the foohid OSX driver. Compatible with Python 2 and 3.

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

Troubleshooting
===============

Currently, there is a bug in the original foohid package where the userclient is not properly terminated.
If the code is run repeatedly, this sometimes results in a "unable to open it_unbit_foohid service" error.
If this happens, unload and remove the kernel extension:

```bash
$ sudo kextunload /Library/Extensions/foohid.kext
$ sudo rm -rf /System/Library/Extensions/foohid.kext
```

Then reinstall the kernel extension: https://github.com/unbit/foohid/releases/latest (no restart required)