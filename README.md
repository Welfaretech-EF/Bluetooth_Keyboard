# Bluetooth Keyboard
This project presents a simple 8 port programmable bluetooth keyboard. Each port can be controlled using a 3.5mm minijack switch and is able to send standard HID-signals through Bluetooth, making it possible to connect to a vast amount of devices, such as smartphones and laptops.

# Requirements
* ESP32
* 8x Female 3.5mm socket

# Assembly
In order to assemble the circuit, the following fritzing sketch has been designed. The pin headers used are instead of the 3.5mm mono sockets, for using a standard 0/1-switch.
<img src="https://github.com/Welfaretech-EF/Bluetooth_Keyboard/blob/main/Images/Fritzing.png" width="512">

# Customization
Once the Bluetooth keyboard is powered on and connected to the receiving device, the 8 ports can control the 8 inputs. In order to customize the inputs, it is possible to connect to the local wifi-hotspot and accessing a local website on: http://192.168.1.1
# 3D Printed case
A 3d printable custom case has been designed, enclosing all the electronics. All the stl files can be found in the "3D Models" folder.