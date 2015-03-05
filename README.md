# foohid-py
Python wrapper for the foohid OSX driver

Exposed functions
=================

```py
import foohid

foohid.create('your new device', report_descriptor)
foohid.destroy('your new device')
foohid.send('your new device', hid_message)
foohid.list()
```
