## Read a temperature sensor using I2C

This example demonstrates how to use the I2C low-level API of RIOT by reading a
temperature sensor.

2 concepts of RIOT are illustrated here:
* I2C : http://doc.riot-os.org/group__drivers__periph__i2c.html#details
* Timers: http://doc.riot-os.org/group__sys__xtimer.html 

### Hardware setup

This application has only been tested with the
[Atmel SAMR21-XPRO](https://github.com/RIOT-OS/RIOT/wiki/Board%3A-SAMR21-xpro)
board. You also need the
[Io1-xplained-pro](http://www.atmel.com/images/atmel-42078-io1-xplained-pro_user-guide.pdf)
extension from atmel.

### Using this application

1. Ensure you followed the
[prerequisites](https://github.com/aabadie/riot-apps#prerequisites) first
2. Plug the board to an USB port of your computer
3. Build and flash:
```bash
$ cd ~/work/gpio_to_serial
$ make RIOTBASE=~/work/RIOT flash
```
4. Start a serial terminal application on your PC (assuming the board is
visible on `/dev/ttyACM0`)
```bash
gtkterm -p /dev/ttyACM0 -s 115200 -b 8
```
5. The temperature measured by the sensor is displayed every 1.5 second.
