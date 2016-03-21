## Control and follow the status of a GPIO through an UART interface

This example demonstrates how to use the GPIO API of RIOT. A LED is controlled
by both a button and the UART interface.

The UART is initialized using a baudrate of 9600.

4 concepts of RIOT are illustrated here:
* [IPC](http://riot-os.org/api/group__core__msg.html#details)
* [UART](http://riot-os.org/api/group__drivers__periph__uart.html#details)
* [IRQ](http://riot-os.org/api/group__core__irq.html#details)
* [GPIO](http://doc.riot-os.org/group__drivers__periph__gpio.html#details)

### Hardware setup

This application has only been tested with the
[Atmel SAMR21-XPRO](https://github.com/RIOT-OS/RIOT/wiki/Board%3A-SAMR21-xpro)
board. It uses the on-board LED and Button for setup simplicity.

### Using this application

1. Ensure you followed the
[prerequisites](https://github.com/aabadie/riot-apps#prerequisites) first
2. Plug the board to an USB port of your computer
3. Build and flash:
<pre>
$ cd ~/work/gpio_to_serial
$ make RIOTBASE=~/work/RIOT flash
</pre>
4. Start a serial terminal application on your PC (assuming the board is
visible on `/dev/ttyACM0`)
<pre>
$ gtkterm -p /dev/ttyACM0 -s 9600 -e -b 8
</pre>
5. Press the on-board button : this toggles the LED status.
6. Type some character in the serial terminal: each typed character toggles the
   LED status
