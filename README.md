# Geisha Aquarea Panasonic Arduino Esp8266 Wifi Controller

Trying to reverse enginieer the protocol of Panasonic Aquarea WH-MDC05f3e5 and use a microcontroller to control the heatpump over wifi.

heatpump Protocoll: (thx Heiko ;))<br>
960 Baud,8bits,Even Parity Bit,1 Stopbit, Leas significant Bit First, Inverse

Protocoll is modbus style: <br>
4 Bytes are send in a row: <br>
Remote sends 170,RegisterId,Byte,Checksum <br>
Heatpump returns 85,RegisterId,Value,Checksum

Current Code for ESP8266 using a 4 port bi-directional LogicLevel converter.

Allready coded a Proof of Concept "Proxy" using 2 SoftwareSerials (https://github.com/plerup/espsoftwareserial changed a little bit to work with protocoll) on ESP8266 to collect the data from remote and send it to heatpump.<br>
Then receive the response from heatpump and send it to the remote.<br>
Todo: Figure out how to send own messages from esp to heatpump.<br>

SerialProxy<br>
https://github.com/vscope/geisha_aquarea_panasonic_arduino_esp8266_proxy/tree/master/GeishaSerialProxy

Remote Dummy for Debugging:<br>
https://github.com/vscope/geisha_aquarea_panasonic_arduino_esp8266_proxy/tree/master/GeishaSerialRemote

Heatpump Dummy for Debugging:<br>
https://github.com/vscope/geisha_aquarea_panasonic_arduino_esp8266_proxy/tree/master/GeishaSerialHeatPump

Feel free to join.

Current known registers: (thx Heiko again ;))

Datenpaket von Fernbedienung zu Wärmepumpe besteht aus 4 Byte<br>
Byte 1=dez 170  = Startbyte immer gleich<br>
Byte 2=Datensatznummer<br>
Byte 3=0  =0 bei Abfrage   =x bei Speicherung eines neuen Wertes<br>
Byte 4=CRC  die 8Bit Summe aus den Bytes 1 bis 3<br>
<br>
Datenpaket von Wärmepumpe zur Fernbedienung besteht aus 4 Byte und ist die Antwort auf das Paket der Fernbedienung<br>
Byte 1=dez 85  = Startbyte immer gleich<br>
Byte 2=Datensatznummer<br>
Byte 3=Inhalt des Datensatzes<br>
Byte 4=CRC  die 8Bit Summe aus den Bytes 1 bis 3<br>
<br>
Datensatznummer<br>
16=0         ???<br>
17=0         =64 beim abtauen<br>
18=8         Außentemperatur=8 Grad<br>
19=37         Vorlauftemperatur<br>
20=0         aktueller Fehler, geht wieder auf 0 wenn fehler behoben dez 38=Fehler H72(Speichertemperaturfühler)<br>
                                156=Fehler H76(Kommunikationsfehler der Fernbedienung)<br>
21=33         Rücklauftemperatur<br>
22=39         Speicher(Puffer)temperatur<br>
23=35                 Kompressorfrequenz<br>
24=0         last Error<br>
25=0         ???<br>
26=1         ???<br>
27=3         Bit 0     gesetzt=ein<br>
         Bit 1   gesetzt=heizen<br>
         Bit 2     gesetzt=kühlen<br>
         Bit 3     <br>
         Bit 4   gesetzt=Speicher(Puffer)<br>
         Bit 5<br>
         Bit 6   gesetzt=Quiet<br>
         Bit 7<br>
28=0         ???<br>
29=246         Lowbyte heizen 3*256+246=1014 W aktueller Stromverbrauch<br>
30=3           Highbyte heizen 3*256+246=1014 W<br>
31=0         Lowbyte kühlen<br>
32=0         Highbyte kühlen<br>
33=0         Lowbyte bei Speicherbetrieb<br>
34=0         Highbyte bei Speicherbetrieb<br>
35=64         Pumpe Geschwindigkeitsstufe dez 16=Stufe 1<br>
         32=Stufe 2<br>
         48=Stufe 3<br>
         64=Stufe 4<br>
36=0         ???<br>
<br>
--------------------------------------------------------------------------------------------------------------------------<br>
<br>
Register 129 bis ... werden nur übertragen, wenn etwas geändert wird oder beim starten ( Strom zuschalten )<br>
129 = Reset Error(Taste an der FB)<br>
130      Einstellen der niedrigen Außentemperatur <br>
131      Einstellen der hohen Außentemperatur <br>
132      Einstellen der Wasseraustrittstemperatur bei niedriger Außentemperatur  <br> 
133      Einstellen der Wasseraustrittstemperatur bei hoher   <br>
134      Einstellen der Außentemperatur, bei der der Heizbetrieb in der Heizbetriebsart    abgeschaltet wird  170 134 25 73   85 134 25 244   25=25 Grad<br>
135      Einsteller der Außentemperatur, bei der die Elektrozusatzheizung eingeschaltet wird<br>
136   Cool Set<br>
137      Heißwasserspeichertemperatur  170 137 50 101   85 137 50 16 <br>
   Heißwasser(Tanktemperatur) auf 50 eingestellt<br>
138   =1/2/3/4/5/0  Datensatz 170 138 2 54   85 138 2 225    das ist die    Parallelverschiebung der Heizkurve ,in dem Fall 2<br>
141   ???<br>
142   ???<br>
143   ???<br>
144   ???<br>
145   ???<br>
146   ???<br>
147   ???<br>
177   Cool enable<br>
192   Bit 0=0 ext_Raumthermostat nein    =1 ext_Raumthermostat ja<br>
   Bit 1=0 Tank_vorhanden nein      =1 Tank_vorhanden ja<br>
   Bit 2=0 Solar_Vorrang nein      =1 Solar_Vorrang ja<br>
   Bit 3 ???<br>
   Bit 4 = Entkeimung nein         =1 Entkeimung ja<br>
   Bit 5 = Zusatzheizung B         =1 Zusatzheizung A<br>
   Bit 6 = Quiet ???<br>
   Bit 7 = Wasserschutzfunktion nein   =1 Wasserschutzfunktion ja<br>
193   Aufheizdauer Cool/Heat ist die Zeit in 30 min Schritten  2=60min, 1=30 min, 20=10h<br>
194   Aufheizdauer_WW_Tank_int 5=5min, 30=30min, 95=95min<br>
195   ???<br>
196   Entkeimungstemperatur<br>
197   Entkeimungsdauer<br>
198   Bit 2 = Booster_Fun  nein      =1 Booster_Fun ja<br>
   Bit 3 = Zusatzgehaeseheizung nein   =1 Zusatzgehaeseheizung ja<br>
199   ???<br>
200   ???<br>
