## Hello World!

This is a basic example how to use RIOT in your embedded application.
It prints out the famous text `Hello World!`.

This example should foremost give you an overview how to use the Makefile system:

* First you must give your application a name, which is commonly the same as the name of the directory it resides in.
  Then you can define a default BOARD for which the application was written.
  By using e.g. `make BOARD=msba2` you can override the default board.
  With `make buildtest` the application gets compiled for all supported boards.

* The variable `RIOTBASE` contains an absolute or relative path to the directory where you have checked out RIOT.
  If your code resides in a subdirectory of RIOT, then you can use `$(CURDIR)` as it's done in here.

* The variable `QUIET`, which is either `1` or `0`, defines whether to print verbose compile information, or hide them, respectively.

* The last line of your Makefile must be `include $(RIOTBASE)/Makefile.include`.

The code itself may look like your usual *C* beginners hello-world example.

### Building this example

We assume you followed the prerequites explained [here](https://github.com/aabadie/riot-apps#prerequisites).

1. Plug your board on one of your USB.
2. Compile and flash:
  1. Let's do it in three steps first: build, flash and then start the serial terminal
<pre>
$ cd ~/work/hello-world
$ make RIOTBASE=~/work/RIOT BOARD=arduino-mega2560
$ make RIOTBASE=~/work/RIOT BOARD=arduino-mega2560 flash
$ make RIOTBASE=~/work/RIOT BOARD=arduino-meag2560 term
</pre>
  2. But all can be done with one command using multiple make targets:
<pre>
$ cd ~/work/hello-world
$ make RIOTBASE=~/work/RIOT BOARD=arduino-mega2560 flash term
</pre>
3. You should see in your terminal:
<pre>
Building application "hello-world" for "arduino-mega2560" with MCU "atmega2560".
[...]
2016-02-19 16:49:35,607 - INFO # Hello World!
2016-02-19 16:49:35,609 - INFO # You are running RIOT on a(n) arduino-mega2560 board.
2016-02-19 16:49:35,610 - INFO # This board features a(n) atmega2560 MCU.
</pre>
