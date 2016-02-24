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
Sparkfun](https://www.sparkfun.com/products/12577) and the boards mentioned in
the top-level [README](https://github.com/aabadie/riot-apps#sample-applications-using-riot-os) of this repository.

You can find more information on a RN42 module configuration and especially the commands
required
[here](https://eewiki.net/display/Wireless/Getting+Started+with+RN42+Bluetooth+Module#GettingStartedwithRN42BluetoothModule-RN42UARTCommands)
The HC-05 module has also been tested and should work as
well. [Here](http://www.linotux.ch/arduino/HC-0305_serial_module_AT_commamd_set_201104_revised.pdf)
is a document explaining the set of commands to configure it.

<img src="https://github.com/aabadie/riot-apps/blob/master/serial_to_serial/docs/uart_2_uart_arduino_due_bb.png" width="400">

### Using this application

* Ensure you followed the
[prerequisites](https://github.com/aabadie/riot-apps#prerequisites) first
* Follow the setup displayed above and plug the board to an USB port of your computer
* Build and flash:
```bash
$ cd ~/work/i2c_temperature
$ make RIOTBASE=~/work/RIOT BOARD=arduino-due flash
```
* Start a serial terminal application on your PC (assuming the board is
visible on `/dev/ttyACM0`)
```bash
gtkterm -p /dev/ttyACM0 -s 115200 -b 8
```
* In the RIOT-OS shell, type `help` to list the available commands.
* By default the RN42 works at 115200 bps and the configuration mode is launched
with the `$$$` string. Simply use the following command to switch the module in
config mode:
```bash
> start 115200 $$$
```
* Then, you can list and change some settings, for example:
```bash
# show the list of settings:
> send D
# set a new name:
> send SN,<new_name>
```
* Once done, the module needs to be rebooted (some settings require this):
```bash
> send R,1
```
* If you just want to quit the the configuration mode:
```bash
> send ---
```
