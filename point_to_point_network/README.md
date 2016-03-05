## 

This example demonstrates how to use the I2C low-level API of RIOT by reading a
temperature sensor and how to send this value by udp through a wireless
interface to a server node. The received packet is then forwarded by the server
node to one of its UART interface making this node a router from 6LowPan to
UART.
Two implementations of the server node are provided by this example:
* [Server to terminal](https://github.com/aabadie/riot-apps/tree/master/point_to_point_server/server_to_terminal):
  messages received by the server node are forwarded to the debug console (UART 0)
* [Server to bluetooth](https://github.com/aabadie/riot-apps/tree/master/point_to_point_server/server_to_bluetooth):
  messages received by the server node are forwarded to UART 1 which is
  connected to a Bluetooth module (it can also be a TTL to USB converter).

4 concepts of RIOT are illustrated here:
* [I2C](http://doc.riot-os.org/group__drivers__periph__i2c.html#details)
* [Timers](http://doc.riot-os.org/group__sys__xtimer.html )
* [UART](http://riot-os.org/api/group__drivers__periph__uart.html#details)
* [Application connection API](http://riot-os.org/api/group__net__conn.html#details)

This example also shows how to flash 2 boards plugged on the same computer.

### Hardware setup

This application has only been tested with the
[Atmel SAMR21-XPRO](https://github.com/RIOT-OS/RIOT/wiki/Board%3A-SAMR21-xpro)
board. You also need the
[Io1-xplained-pro](http://www.atmel.com/images/atmel-42078-io1-xplained-pro_user-guide.pdf)
extension from atmel.

If you want to try the
['Server to bluetooth'](https://github.com/aabadie/riot-apps/tree/master/point_to_point_server/server_to_bluetooth)
example, you also need a Bluetooth module using UART and plugged on the second UART of the board
that will be used as the udp server. For example you can use a bluetooth device
like the one tried in [serial to serial](https://github.com/aabadie/riot-apps/tree/master/serial_to_serial)
sample application.
* Plug __RX__ pin of the serial device to the TX pin of the board: __PA22__
* Plug __TX__ pin of the serial device to the RX pin of the board: __PA23__

### Using this application

The principle is the following:

1. The Io1-xplained extention is plugged on the board used as a client. This board
   periodically reads the I2C temperature sensor and sends this value to an udp server.
2. The second board runs an udp server and forwards on its second UART
   interface all packets received.

For simplicity, the ipv6 address of server is hard coded in the
client code, so we'll have to determine it for the server board before flashing
the server code of this example on this board. It's explained in the following steps.

* Ensure you followed the
[prerequisites](https://github.com/aabadie/riot-apps#prerequisites) first
* Plug the Io1-xplaned-pro extension on the client board
* Plug the client board to an USB port of your computer
* Get the information of the boardin the list of connected ttys. We use this
here to determine the serial identifier and the tty used for the client board:
```bash
$ ~/work/RIOT/dist/tools/usb-serial/list-ttys.sh
/sys/bus/usb/devices/2-1.2.1: Atmel Corp. EDBG CMSIS-DAP serial: 'ATML2127031800004653', tty(s): ttyACM0
```
* Plug the server board to another USB port of your computer
* Get the new list of ttys connected to the boards:
```bash
$ ~/work/RIOT/dist/tools/usb-serial/list-ttys.sh
/sys/bus/usb/devices/2-1.2.1: Atmel Corp. EDBG CMSIS-DAP serial: 'ATML2127031800004653', tty(s): ttyACM0
/sys/bus/usb/devices/2-1.2.2: Atmel Corp. EDBG CMSIS-DAP serial:
'ATML2127031800004658', tty(s): ttyACM1
```
* Now we can identify both boards:
 * client board has serial identifier __'ATML2127031800004653'__ and is connected
 to tty __/dev/ttyACM0__
 * server board has serial identifier __'ATML2127031800004658'__ and is connected
 to tty __/dev/ttyACM1__
* We now need to retrieve the server IP (ipv6). To achieve this, let's use the
__gnrc_networking__ example:
```bash
$ cp -R ~/work/RIOT/examples/gnrc_networking ~/work/riot-apps/.
$ cd ~/work/riot-apps/gnrc_networking
```
* and flash the server board. Note that we use SERIAL and PORT options
corresponding to the server board:
```bash
$ make RIOTBASE=~/work/RIOT BOARD=samr21-xpro SERIAL=ATML2127031800004658
PORT=/dev/ttyACM1 flash term
```
* let's now get the ip address (from the riot-os shell on the board):
```
> ifconfig
2016-03-01 10:46:36,415 - INFO # Iface  7   HWaddr: 12:9a  Channel: 26  Page: 0  NID: 0x23
2016-03-01 10:46:36,419 - INFO #            Long HWaddr: 5a:47:3c:7c:49:50:12:9a 
2016-03-01 10:46:36,426 - INFO #            TX-Power: 0dBm  State: IDLE  max. Retrans.: 3  CSMA Retries: 4 
2016-03-01 10:46:36,431 - INFO #            AUTOACK  CSMA  MTU:1280  HL:64  6LO  RTR  IPHC  
2016-03-01 10:46:36,435 - INFO #            Source address length: 8
2016-03-01 10:46:36,437 - INFO #            Link type: wireless
2016-03-01 10:46:36,443 - INFO #            inet6 addr: ff02::1/128  scope: local [multicast]
2016-03-01 10:46:36,448 - INFO #            inet6 addr: fe80::5847:3c7c:4950:129a/64  scope: local
2016-03-01 10:46:36,455 - INFO #            inet6 addr: ff02::1:ff50:129a/128
scope: local [multicast]
```
* The server ip (v6) is __fe80::5847:3c7c:4950:129a__  (it's the line ending
  with scope: local, you should have a different value). In
  *~/work/riot-apps/point_to_point_network/client_with_sensor/main.c*, replace the value
  __SERVER_PORT__ with the on you just got with *ifconfig*.
* Let's now flash the server board with our server application
(server_to_terminal or server_to_bluetooth):
```bash
$ cd ~/work/riot-apps/point_to_point_network/server_to_bluetooth
$ make RIOTBASE=~/work/RIOT BOARD=samr21-xpro SERIAL=ATML2127031800004658 flash
```
* Let's now flash the client board with our sensor client application:
```bash
$ cd ~/work/riot-apps/point_to_point_/client_with_sensor
$ make RIOTBASE=~/work/RIOT BOARD=samr21-xpro SERIAL=ATML2127031800004653 flash
```
* Now all is in place. Simply connect to the UART device plugged on the server
  board (via bluetooth if it's a bluetooth device or using a terminal on the
  right tty if it's a tty to USB device. Note that this UART device should be
  initialized at 115200 bps (value UART_BAUDRATE in the code).
  You should now see the temperature messages displayed !! You are on the way of the IoT, Woohoo :)
  
  
