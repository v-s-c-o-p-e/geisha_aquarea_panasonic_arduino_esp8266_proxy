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

Datenpaket von Fernbedienung zu Wärmepumpe besteht aus 4 Byte
Byte 1=dez 170  = Startbyte immer gleich
Byte 2=Datensatznummer
Byte 3=0  =0 bei Abfrage   =x bei Speicherung eines neuen Wertes
Byte 4=CRC  die 8Bit Summe aus den Bytes 1 bis 3

Datenpaket von Wärmepumpe zur Fernbedienung besteht aus 4 Byte und ist die Antwort auf das Paket der Fernbedienung
Byte 1=dez 85  = Startbyte immer gleich
Byte 2=Datensatznummer
Byte 3=Inhalt des Datensatzes
Byte 4=CRC  die 8Bit Summe aus den Bytes 1 bis 3

Datensatznummer
16=0         ???
17=0         =64 beim abtauen
18=8         Außentemperatur=8 Grad
19=37         Vorlauftemperatur
20=0         aktueller Fehler, geht wieder auf 0 wenn fehler behoben dez 38=Fehler H72(Speichertemperaturfühler)
                                156=Fehler H76(Kommunikationsfehler der Fernbedienung)
21=33         Rücklauftemperatur
22=39         Speicher(Puffer)temperatur
23=35                 Kompressorfrequenz
24=0         last Error
25=0         ???
26=1         ???
27=3         Bit 0     gesetzt=ein
         Bit 1   gesetzt=heizen
         Bit 2     gesetzt=kühlen
         Bit 3     
         Bit 4   gesetzt=Speicher(Puffer)
         Bit 5
         Bit 6   gesetzt=Quiet
         Bit 7
28=0         ???
29=246         Lowbyte heizen 3*256+246=1014 W aktueller Stromverbrauch
30=3           Highbyte heizen 3*256+246=1014 W
31=0         Lowbyte kühlen
32=0         Highbyte kühlen
33=0         Lowbyte bei Speicherbetrieb
34=0         Highbyte bei Speicherbetrieb
35=64         Pumpe Geschwindigkeitsstufe dez 16=Stufe 1,
         32=Stufe 2
         48=Stufe 3
         64=Stufe 4
36=0         ???

--------------------------------------------------------------------------------------------------------------------------

Register 129 bis ... werden nur übertragen, wenn etwas geändert wird oder beim starten ( Strom zuschalten )
129 = Reset Error(Taste an der FB)
130      Einstellen der niedrigen Außentemperatur 
131      Einstellen der hohen Außentemperatur 
132      Einstellen der Wasseraustrittstemperatur bei niedriger Außentemperatur   
133      Einstellen der Wasseraustrittstemperatur bei hoher   
134      Einstellen der Außentemperatur, bei der der Heizbetrieb in der Heizbetriebsart    abgeschaltet wird  170 134 25 73   85 134 25 244   25=25 Grad
135      Einsteller der Außentemperatur, bei der die Elektrozusatzheizung eingeschaltet wird
136   Cool Set
137      Heißwasserspeichertemperatur  170 137 50 101   85 137 50 16 
   Heißwasser(Tanktemperatur) auf 50 eingestellt
138   =1/2/3/4/5/0  Datensatz 170 138 2 54   85 138 2 225    das ist die    Parallelverschiebung der Heizkurve ,in dem Fall 2
141   ???
142   ???
143   ???
144   ???
145   ???
146   ???
147   ???
177   Cool enable
192   Bit 0=0 ext_Raumthermostat nein    =1 ext_Raumthermostat ja
   Bit 1=0 Tank_vorhanden nein      =1 Tank_vorhanden ja
   Bit 2=0 Solar_Vorrang nein      =1 Solar_Vorrang ja
   Bit 3 ???
   Bit 4 = Entkeimung nein         =1 Entkeimung ja
   Bit 5 = Zusatzheizung B         =1 Zusatzheizung A
   Bit 6 = Quiet ???
   Bit 7 = Wasserschutzfunktion nein   =1 Wasserschutzfunktion ja
193   Aufheizdauer Cool/Heat ist die Zeit in 30 min Schritten  2=60min, 1=30 min, 20=10h
194   Aufheizdauer_WW_Tank_int 5=5min, 30=30min, 95=95min
195   ???
196   Entkeimungstemperatur
197   Entkeimungsdauer
198   Bit 2 = Booster_Fun  nein      =1 Booster_Fun ja
   Bit 3 = Zusatzgehaeseheizung nein   =1 Zusatzgehaeseheizung ja
199   ???
200   ???
