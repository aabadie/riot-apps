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

There are a few package that needs to be installed on the Ubuntu developpement system.

  1. Common packages

```bash
sudo apt install build-essential g++-multilib screen
```

  2. AVR MCU Family

```bash
sudo apt install gcc-avr avr-libc binutils-avr
```

  3. ARM MCU Family

```bash
sudo apt install gcc-arm-none-eabi openocd binutils-arm-linux-gnueabi
```

  4. MSP430 MCU Family (but not tested yet)

```bash
sudo apt install gcc-msp430 msp430-libc binutils-msp430
```

More information is available on the RIOT documentation : https://github.com/RIOT-OS/RIOT/wiki/Introduction#compiling-riot


### Getting the code

Here we suppose all is code is located in `~/work`.

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

This repository contains 2 self-documented sample applications:
* The usual ["Hello World"](https://github.com/aabadie/riot-apps/tree/master/hello-world) shows how
to use the build system and display terminal
* ["Serial to serial"](https://github.com/aabadie/riot-apps/tree/master/serial_to_serial) is a
little more complex example showing how to forward messages from one UART interface to
another UART interface. This example uses IPC and IRQ concepts of RIOT

