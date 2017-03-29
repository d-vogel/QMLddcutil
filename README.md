# QMLddcutil
Simple app using the [ddcutil](http://www.ddcutil.com/) lib

ddcutil allows you to set the settings of monitors without using the On Screen Display.

The first prerequisite is to be able to access your monitor using the comand line tool.

## Setting up ddcutil (CLI)
* Install ddcutil: [prebuilt](http://www.ddcutil.com/#installing-ddcutil-from-prebuilt-packages), [aur](https://aur.archlinux.org/packages/ddcutil-git/) or [from source](http://www.ddcutil.com/building/)
* load i2c-dev kernel module at boot:
```
#/etc/modules-load.d/ddc.conf
    i2c_dev
```
after reboot `modprobe` should report i2c-dev.
* create a udev rule setting ownership for `/dev/i2c-*` files to members of the i2c group.
```
#/etc/udev/rules.d/10-local_i2c_group.rules
    KERNEL=="i2c-[0-9]*", GROUP="i2c"
```
* create the i2c group, and add yourself to the group.

`ddcutil detect` should now find your DDC/CI capable monitors.

## QMLddcutil
The GUI tool is pretty much a toy/demo app. Please don't suppose it to be rock solid.

Things that need to be improved:
* multi monitor support: several methods of the ddccontroller class do not make any use of the data stored for other monitors than the first one listed by ddcutil.
* loading the app is somewhat slow: it would be nice to have the GUI popup to inform that work is beeing done by the ddccontroller instance in the back.
* many other settings from the monitor can be added to the GUI.
