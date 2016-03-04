## Read a temperature sensor on an I2C bus using RIOT-OS


[![RIOT-OS](http://www.riot-os.org/images/logo-menu.png)](http://www.riot-os.org)

See https://github.com/RIOT-OS/RIOT for more information.

### Prerequisites

This application has been tested with the following setup:
* Developpement computer running Ubuntu 15.10, Wily Werewolf, 64bits
* Tested board: [Atmel SAMR21-xpro](https://github.com/RIOT-OS/RIOT/wiki/Board%3A-SAMR21-xpro)

There are a few package that needs to be installed on the Ubuntu developpement system.

  1. Common packages

```bash
sudo apt install build-essential g++-multilib openocd gtkterm
```

  2. ARM MCU Family

```bash
sudo apt install gcc-arm-none-eabi gdb-arm-none-abi binutils-arm-linux-gnueabi
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
$ git clone git@github.com:riot-apps/i2c_temperature.git
```

## About this application

["I2C temperature"](https://github.com/aabadie/riot-apps/tree/master/i2c_temperature) shows how
to use the I2C API to read the temperature on a sensor. The application is
designed to work with a SAMR21-xpro and its Xplained extension, which provides
the temperature sensor.
  
