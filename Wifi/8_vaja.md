# Brezžična komunikacija in ESP8266

## Brezžična komunikacija ("Wi-Fi")

Brezžična komunikacija se nanaša na vse vrste povezav radijskih frekvenc med dvema ali več terminali. "Wi-Fi"  je tehnologija za lokalna brezžična omrežja (WLAN - Wireless LAN). Igralne konzole, mobilni telefoni, tablični računalniki, pametne televizije, tiskalniki, alarmni sistemi in številne druge naprave lahko uporabljajo to vrsto povezave.

<p align="center">
    <img  src="../figures/WiFi_Logo.svg" width="250">
</p>

### Standardi za Wi-Fi komunikacijo 

#### 802.11

Najstarejši Wi-Fi standard. Bil je ustvarjen leta 1997 v IEEE (Inštitut za elektrotehniko in elektroniko). Dobil je naziv po skupini, ki ga je izdelala. Standard podpira pasovno širino do 2 Mb/s pri frekvenci 2,4 GHZ. 

#### 802.11b

Predstavlja razširjeno različico standarda 802.11, ki lahko podpira do 11 Mb/s. Ker uporablja frekvenco 2,4 GHZ brez regulacije, lahko provzroči motnje na gospodinjskih aparatih in drugih napravah, ki uporabljajo pas 2,4 GHz. 

#### 802.11a

Se je razvijal hkrati z 802.11b. Zaradi večjih stroškov 802.11a ni postal tako priljubljen kot 802.11b. 802.11a podpira hitrosti do 54 Mb/s pri frekvenci 5 GHZ.

#### 802.11g

Združuje standarda 802.11a in 802.11b in ponuja hitrosti do 54 Mb/s v frekvenčnem območju 2,4 GHz. Standard 802.11g je združljiv z napravami 802.11b, ki uporabljajo brezžično omrežno kartico na dostopni točki.

#### 802.11n

Predstavlja razširjen standard 802.11g, ki podpira kominikacijo s več anteni za brezžično komunikacijo in hitrosti do 300 Mb/s. 

#### 802.11ac 

Najnovejši in najbolj priljubljen Wi-Fi standard na najnovejših generacijah naprav. 802.11ac uporablja dvopasovno brezžično tehnologijo, ki lahko podpira dve sočasni povezavi na frekvencah 2,4 GHz in 5 GHz.
Velike prednosti 802.11ac so pasovne širine do 1300 Mb/s (1,3 Gb/s) na frekvenci 5 GHz in 450 Mb/s na 2,4 GHz. Poleg tega ponuja standardno združljivost 802.11 b/g/n.


## ESP8266 in Wi-Fi komunikacija 

Med moduli za brezžično povezovanje elektronskih naprav v internet so danes nedvomno najpogostejši tisti, ki temeljijo na čipih ESP8266/ESP32 kitajskega proizvajalca Espressif. Delo z njimi si lahko olajšamo z uporabo katerega izmed obstoječih integriranih razvojnih okolij, kot je na primer Arduino in programskih jezikov, kot sta na primer MicroPython in Lua, ali z ukazi AT, vendar nam največ fleksibilnosti omogoča uporaba razvojne programske opreme SDK (Software Development Kit) samega proizvajalca čipov.

### Lastnosti ESP8266 kot Wi-Fi strežnik/odjemalec

1. Podpira 802.11 b/g/n protokole
2. Integriran protokolni sklad TCP/IP 
3. Podpira omrežne protokole IPv4, TCP/UDP/HTTP/FTP
4. Podpira antenski diverzitet ali antensko raznolikost 
5. Lahko ga uporabljamo kot odjemalca (STA), dostopno točko (AP) ali oboje hkrati (STA+AP)
6. Podpira pametno povezavo za Android in iOS naprave

##  Wi-Fi in Arduino

Vse funkcije za vzpostavljanje in nastavljanje brezžične komunikacije so implementirane v knjižnici ESP8266WiFi.h

Seznam funkcij:


|             Metoda            |                                                    Opis                                                   |
|:-----------------------------:|:---------------------------------------------------------------------------------------------------------:|
| WiFi.mode()                   | Omogoča nastavitev načina delovanja (AP, STA).                                                            |
| WiFi.softAP()                 | Uporabimo, če želimo ustvariti dostopno točko. Podamo ime ter geslo za povezavo.                          |
| WiFi.begin()                  | Uporabimo, če se želimo povezati na obstoječe omrežje. Podamo ime in geslo za dostop do želenega omrežja. |
| WiFi.status()                 | Če je WiFi povezan s stacionarno (način STA) točko, vrne WL_CONNECTED.                                    |
| WiFi.localIP()                | Vrne lastni IP naslov (le v načinu STA)                                                                   |
| WiFi.softAPIP()               | Vrne IP naslov vmesnika dostopne točke                                                                    |
| ESP8266WebServer server(port) | Zagon Web strežnika na portu *port* točke                                                                    |                               |

### WiFi.mode(način);

|             Način             |                                                    Opis                                                   |
|:-----------------------------:|:---------------------------------------------------------------------------------------------------------:|
| WIFI_AP                       | WiFi modul deluje v načinu dostopne točke (ang. access point oz. AP).                                     | 
| WIFI_STA                      | WiFi modul se poveže na obstoječe omrežje, deluje v klasičnem načinu (ang. station oz. STA).              |
| WIFI_STA_AP                   | WiFi modul deluje v obeh načinih sočasno (hitro preklaplja).                |                                                          


### WiFi.softAP("naziv", "geslo");

|             Način             |                                                    Opis                                                   |
|:-----------------------------:|:---------------------------------------------------------------------------------------------------------:|
|         naziv                 | Naziv (ime) dostopne točke, ki jo želimo postaviti (mode WIFI_AP).                                        | 
|         geslo                 | Geslo dostopne točke, ki jo želimo postaviti (mode WIFI_AP), geslo lahko tudi izpustimo.                  | 

### WiFi.begin("naziv", "geslo");

|             Način             |                                                    Opis                                                   |
|:-----------------------------:|:---------------------------------------------------------------------------------------------------------:|
|         naziv                 | Naziv (ime) obstoječe dostopne točke (mode WIFI_STA).                                                     | 
|         geslo                 | Geslo obstoječe dostopne točke (mode WIFI_STA).                                                           | 

### Primeri:

- Ustvarjanje stacinonarne točke

```c 
    WiFi.mode(WIFI_AP);
    WiFi.softAP(naziv, geslo);
    // preostala koda: ..
``` 

- Povezava z obstoječim omrežjem

```c 
   WiFi.mode(WIFI_STA);
   WiFi.begin(naziv, geslo);
   // pred nadaljevanjem moramo čakati, dokler WiFi.status() ne vrne WL_CONNECTED
   while (WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.print(".");
   }
``` 
## Naloga

- Napišite program, ki WEMOS poveže na obstoječe brezžično omrežje, program pa naj omogoča tudi postavitve dostopne točke (pred nalaganjem kode izberete ustrezen način delovanja)
- Implementirajte strežnik (ogrodje se nahaja na spletni učilnici), ki omogoča:
  * Prižiganje in ugašanje na WEMOS vgrajene LED lučke, stanje LEDice pa je razvidno s spletne strani. Za krmiljenje je potrebna predhodna avtentikacija uporabnika.
  * V primeru vnosa napačnega URLja strežnik to sporoči uporabniku in mu omogoči povezavo na glavno stran (login strana).
  
### Namig 

- Za uporabnikovo ime in geslo uporabite globalno spremenljivko ali #define.
- Pri pisanju HTML kode si lahko pomagate:
   * s kodo v podanem ogrodju,
   * https://www.w3schools.com/tags/ref_httpmethods.asp
   * https://www.w3schools.com/tags/att_form_method.asp
* Uporabite serijski monitor za izpis stanja LED lučke.
* Do strežnika dostopate tako, da v spletni brskalnik vnesete naslov IP, ki ga izpišete preko serijskega terminala.
