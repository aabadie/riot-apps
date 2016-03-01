## Sample applications using RIOT-OS


[![RIOT-OS](http://www.riot-os.org/images/logo-menu.png)](http://www.riot-os.org)

See https://github.com/RIOT-OS/RIOT for more information.

### Prerequisites

All samples applications here have been tested with the following setup:
* Developpement computer running Ubuntu 15.10, Wily Werewolf, 64bits
* Tested boards:
  * AVR MCU Family:
    * [Arduino Mega2560](https://github.com/RIOT-OS/RIOT/wiki/Board%3A-Arduino-Mega2560)
  * ARM MCU Family:
    * [Arduino due](https://github.com/RIOT-OS/RIOT/wiki/Board%3A-Arduino-Due)
    * [Atmel SAMR21-xpro](https://github.com/RIOT-OS/RIOT/wiki/Board%3A-SAMR21-xpro)
    * [mbed LPC1768](https://github.com/RIOT-OS/RIOT/wiki/Board%3A-mbed_lpc1768)

There are a few package that needs to be installed on the Ubuntu developpement system.

  1. Common packages

```bash
sudo apt install build-essential g++-multilib openocd gtkterm
```

  2. AVR MCU Family

```bash
sudo apt install gcc-avr gdb-avr avr-libc binutils-avr
```

  3. ARM MCU Family

```bash
sudo apt install gcc-arm-none-eabi gdb-arm-none-abi binutils-arm-linux-gnueabi
```

  4. MSP430 MCU Family (but not tested yet)

```bash
sudo apt install gcc-msp430 mspdebug msp430-libc binutils-msp430
```

More information is available on the RIOT documentation : https://github.com/RIOT-OS/RIOT/wiki/Introduction#compiling-riot


### Getting the code

We suppose all the code is located in `~/work`.

1. Clone RIOT code (or your fork)

```bash
$ cd ~/work
$ git clone git@github.com:RIOT-OS/RIOT.git
```

2. Clone this repository (or your fork)

```bash
$ cd ~/work
$ git clone git@github.com:aabadie/riot-apps.git
```

### Samples

This repository contains 5 self-documented sample applications:
* The usual ["Hello World"](https://github.com/aabadie/riot-apps/tree/master/hello-world) shows how
to use the build system and display terminal
* ["Serial to serial"](https://github.com/aabadie/riot-apps/tree/master/serial_to_serial) is a
little more complex example showing how messages can be forwared from one UART interface to
another UART interface. This example uses IPC and IRQ concepts of RIOT.
* ["Bluetooth module configuration tool"](https://github.com/aabadie/riot-apps/tree/master/bt_config) is a
tool showing how to play with threads and the shell os RIOT-OS in order to
configure a bluetooth module connected to one of the UART of a board. 
* ["GPIO to serial"](https://github.com/aabadie/riot-apps/tree/master/gpio_to_serial) shows how
to use the GPIO API. This application has only been tested on a SAMR21-xpro
board and will require some adjustement to make it work on other boards (use
different ports and pins).
* ["I2C temperature"](https://github.com/aabadie/riot-apps/tree/master/i2c_temperature) shows how
to use the I2C API to read the temperature on a sensor. The application is
designed to work with a SAMR21-xpro and its Xplained extension, which provides
the temperature sensor.
* ["I2C temperature 2"](https://github.com/aabadie/riot-apps/tree/master/i2c_temperature_2) which 
is a variant of the previous one allowing to control on demand the read of the temperature
sensor from the uart interface.
* ["Point to point network"](https://github.com/aabadie/riot-apps/tree/master/point_to_point_network)
  which is an advanced sample application showing how to transfert data via udp
  other a wireless connection. It reuses parts of the
  [serial to serial](https://github.com/aabadie/riot-apps/tree/master/serial_to_serial)
  and [I2C temperature](https://github.com/aabadie/riot-apps/tree/master/i2c_temperature)
  examples.
  
