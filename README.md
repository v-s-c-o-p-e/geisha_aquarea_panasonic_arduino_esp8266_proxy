# Geisha Aquarea Panasonic Arduino Esp8266 Wifi Controller

Trying to reverse enginieer the protocol of Panasonic Aquarea WH-MDC05f3e5 and use a microcontroller to control the heatpump over wifi.

heatpump Protocoll:<br>
960 Baud,8bits,Even Parity Bit,1 Stopbit, Leas significant Bit First, Inverse

Procoll is modbus style: <br>
4 Bits are send in a row: <br>
Remote sends 170,RegisterId,Bit,Checksum <br>
Heatpump returns 85,RegisterId,Value,Checksum

Current Code for ESP8266 using a 4 port bi-directional LogicLevel converter.

Allready coded a Proof of Concept "Proxy" using 2 SoftwareSerials (https://github.com/plerup/espsoftwareserial changed a little bit to work with protocoll) on ESP8266 to collect the data from remote and send it to heatpump.<br>
Then receive the response from heatpump and send it to the remote.<br>
Todo: Send own messages from esp to heatpump.<br>

SerialProxy<br>
https://github.com/vscope/geisha_aquarea_panasonic_arduino_esp8266_proxy/tree/master/GeishaSerialProxy

Remote Dummy for Debugging:<br>
https://github.com/vscope/geisha_aquarea_panasonic_arduino_esp8266_proxy/tree/master/GeishaSerialRemote

Heatpump Dummy for Debugging:<br>
https://github.com/vscope/geisha_aquarea_panasonic_arduino_esp8266_proxy/tree/master/GeishaSerialHeatPump

Anybody free to join
