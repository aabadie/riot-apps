## Communication between 2 UART interfaces

This example demonstrates how 2 UART interfaces can communicate in a
bi-directionnal way.

You need at least 2 UART interfaces on your board to test it :
* UART0: connected to a PC USB port (serial debug interface).
* UART1: can be connected to any type of serial device (Bluetooth, Serial to USB, etc).

Both UARTs are initialized using a baudrate of 9600.

3 concepts of RIOT are illustrated here:
* [IPC](http://riot-os.org/api/group__core__msg.html#details)
* [UART](http://riot-os.org/api/group__drivers__periph__uart.html#details)
* [IRQ](http://riot-os.org/api/group__core__irq.html#details)


### Hardware setup

This application has been tested with a [bluetooth 42N based board from
Sparkfun](https://www.sparkfun.com/products/12577). It also works with a HC-05
bluetooth module but requires the pins to be adapted.


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
$ gtkterm -p /dev/ttyACM0 -s 9600 -b 8 -e
</pre>
5. Now you can communicate to the other serial Bluetooth interface (using `rfcomm` on Linux or
6. bluetooth terminal application on an Android phone)
7. You can also skip the bluetooth setup hell by using an USB to serial board
between the PC and the second UART interface.


