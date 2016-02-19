## Communicate between 2 UART interfaces

This is a little advanced example that shows a way to communicate between 2 UART interfaces.

You need at least 2 UART interfaces to make it work :
* UART0 (PC terminal).
* UART1 can be connected to any serial device (Bluetooth, Serial to USB, etc).

UARTs are initialize with a baudrate of 9600.

This example illustrates 3 concepts of RIOT:
* IPC: http://riot-os.org/api/group__core__msg.html#details
* UART: http://riot-os.org/api/group__drivers__periph__uart.html#details
* IRQ: http://riot-os.org/api/group__core__irq.html#details


### Hardware setup example

This sample application has been tested with a [bluetooth 42N based board from
Sparkfun](https://www.sparkfun.com/products/12577) and the boards mentioned in
the top-level [README](https://github.com/aabadie/riot-apps#sample-applications-using-riot-os) of this repository.

<img src="https://github.com/aabadie/riot-apps/blob/master/serial_to_serial/docs/uart_2_uart_arduino_due_bb.png" width="400">

### Testing this application

1. Ensure you followed the
[prerequisites](https://github.com/aabadie/riot-apps#prerequisites) first
2. Follow the setup displayed above and plug the usb port of the board on your computer
3. Build and flash:
```bash
$ cd ~/work/serial_to_serial
$ make RIOTBASE=~/work/RIOT BOARD=arduino-due flash
```
4. Start a serial terminal application on your PC (assuming the board UART is
visible in `/dev/ttyACM0`)
```bash
gtkterm -p /dev/ttyACM0 -s 9600 -b 8
```
5. Now you can communicate with any terminal via USB (using `rfcomm` on Linux or
a bluetooth terminal application on an Android phone)
6. You can also skip the bluetooth setup hell by using an USB to serial board
between the PC and the second UART interface.


