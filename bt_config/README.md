## RIOT-OS based tool for bluetooth module configuration

Bluetooth modules available on the market generally need to be configured
before one can start using them. Indeed, the public name, the mode (slave or
master), the pin code password and serial speed have to be adapted most of the time.
The goal of this sample application is to show how to use the shell module of
RIOT-OS to achieve these bluetooth configuration initial steps.

3 concepts of RIOT are illustrated here:
* [Shell](http://riot-os.org/api/group__sys__shell.html)
* [IPC](http://riot-os.org/api/group__core__msg.html#details)
* [UART](http://riot-os.org/api/group__drivers__periph__uart.html#details)
* [IRQ](http://riot-os.org/api/group__core__irq.html#details)

Note that you need at least 2 UART interfaces on your board to test it :
* UART0: connected to a PC USB port (serial debug interface) and used by the shell.
* UART1: connected to the UART of the bluetooth module.

### Hardware setup

This application has been tested with a [bluetooth 42N based module from
Sparkfun](https://www.sparkfun.com/products/12577). You can find more
information on the RN42 module configuration and especially the commands required
[here](https://eewiki.net/display/Wireless/Getting+Started+with+RN42+Bluetooth+Module#GettingStartedwithRN42BluetoothModule-RN42UARTCommands).

The
[HC-05](http://wiki.iteadstudio.com/Serial_Port_Bluetooth_Module_%28Master/Slave%29_:_HC-05)
module has also been tested and should work as well (commands to configure it
are also provided in the link).

* Arduino Due

<img src="https://github.com/aabadie/riot-apps/blob/master/serial_to_serial/docs/uart_2_uart_arduino_due_bb.png" width="400">

* Arduino Mega

<img src="https://github.com/aabadie/riot-apps/blob/master/serial_to_serial/docs/uart_2_uart_arduino_mega2560_bb.png" width="400">

* Samr21-xpro : use EXT1 pins. RX pin is PA22 and TX is PA23. (Note that RX of
BT module goes to TX pin on the board, and vice versa)
* mbed_lpc1768 : RX pin is n°9 and TX is n°10. (Note that RX of
BT module goes to TX pin on the board, and vice versa)

### Using this application

1. Ensure you followed the
[prerequisites](https://github.com/aabadie/riot-apps#prerequisites) first
2. Follow the setup displayed above and plug the board to an USB port of your computer
3. Build and flash:
<pre>
$ cd ~/work/i2c_temperature
$ make RIOTBASE=~/work/RIOT BOARD=arduino-due flash
</pre>
4. Start a serial terminal application on your PC (assuming the board is
visible on `/dev/ttyACM0`)
<pre>
gtkterm -p /dev/ttyACM0 -s 115200 -e -b 8
</pre>
5. In the RIOT-OS shell, type `help` to list the available commands.
6. By default the RN42 works at 115200 bps and the configuration mode is launched
with the `$$$` string. Simply use `init` following command to switch the module in
config mode:
<pre>
> init 115200 $$$
</pre>
7. Then, you can list and change some settings using the `send` command:
<pre>
# show the list of basic settings:
> send D
# show the list of advanced settings:
> send E
# set a new name:
> send SN,<new_name>
</pre>
8. Once done, the module needs to be rebooted (some settings change may require this):
<pre>
> send R,1
</pre>
9. If you just want to quit the configuration mode:
<pre>
> send ---
</pre>
