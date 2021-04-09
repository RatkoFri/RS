# 5. WEMOS D1: osnove

Na tokratni vaji se bomo spoznali z delom v razvojnem okolju Arduino IDE in naložili program na Wemos D1 mini čip. 

Neke lastnosti WeMos D1 mini čipa:

* Mikrokrmilnik: ESP8266
* Frekvenca: 80 MHz (160 MHz)
* Flash: 4 MB
* Delovna napetost: 3,3 V
* Digitalni vhodi in izhodi: 11
* Analogni vhod: 1 (napetost <= 3,2 V)

### Povezljivost WeMos D1 mini 

| Wemos Pin  | Funkcija                            | ESP8266 Pin |
|------------|-------------------------------------|-------------|
| TX         | TXD (Pošiljanje podatkov)           | TXD         |
| RX         | RXD (Sprejem podatkov)              | RXD         |
| A0         | Analogni vhod, <= 3,2 V             | A0          |
| D0         | IO (Vhod/Izhod)                     | GPIO16      |
| D1         | IO, SCL (I2C komunikacija)          | GPIO5       |
| D2         | IO, SDA (I2C komunikacija)          | GPIO4       |
| D3         | IO, z 10k pull-up uporom            | GPIO0       |
| D4         | IO, z 10k pull-up uporom, LED dioda | GPIO2       |
| D5         | IO, SCK (SPI komunikacija)          | GPIO14      |
| D6         | IO, MISO (SPI komunikacija)         | GPIO12      |
| D7         | IO, MOSI (SPI komunikacija)         | GPIO13      |
| D8         | IO, z 10k pull-down uporom          | GPIO15      |
| G          | Ground (ozemljitev)                 | GND         |
| 5V         | 5 V                                 | -           |
| 3V3        | 3,3 V                               | 3,3 V       |
| RST        | Reset                               | RST         |
||||

## 5.1 Arduino IDE in ESP8266

### Prenos zadnje verzije:
  
  - *[Link za prenos](https://www.arduino.cc/en/Main/Software)*
    - Windows:
      - Na prenosnik si namestite »Windows app«


### Nastavitve serijskega porta

- Linux (npr. Ubuntu):
  -  Shell ukaz:  `sudo adduser $USER dialout`
  -  Nato morate ponovno zagnati računalnik.

### Arduino IDE

<p align="center">
    <img  src="./figures/arduino_slo.png" width="300">
</p>

### Namestitev dodatne knjižnice:

  - Odprete *Arduino IDE -> File -> Preferences ->
Settings -> "Additional Boards Manager URLs:"*
(dodate link:)
  **http://arduino.esp8266.com/stable/package_esp8266com_index.json**
- *Arduino IDE -> Tools -> Board: "\*" -> Board
Manager ->* (polje za iskanje:) esp8266 -> klik
na rezultat: "*esp8266 by ESP8266 Comunity"
-> Install* (samodejno izbere najnovejšo verzijo,
počakate da se namesti) -> *Close*

### Namestitev dodatne knjižnice:

- *Arduino IDE -> Tools -> Board: "\*" -> Wemos D1 R2 & mini*
- Vse, kar je označeno z \* so privzete vrednosti in jih ni
potrebno spreminjati
- Arduino IDE -> Tools:
  - \* Flash Size: "4M(1M SPIFFS)"
  - \* Debug port: "Disabled"
  - \* Debug Level: "None"
  - \* lwIP Variant: "V2 Prebuilt (MSS=536)"
  - \* CPU Frequency: "80 MHz"
  - \* Upload speed: "921600"


## 5.2 Arduino Hello_world

```c
void setup() {
  // Del kode, ki se izvaja samo enkrat

  // primer serijske komunikacije
  Serial.begin(115200); 		// Nastavljanje serijske komunikacije
  Serial.println("Hello from setup()"); // Izpis na serijski terminal
}


void loop() {
  // Del kode ki se izvaja ves čas (v neskončnost)

  Serial.println("Hello from loop()"); // Izpis na serijski terminal
}

```

V vsakem Arduino programu razlikujemo dva dela:

1. Funkcija ``` setup()``` se izvaja samo enkrat ob zagonu programa. V večini programa uporabljamo funckijo ``` setup()``` za nastavljanje pinov, serijske komunikacije in zunanjih naprav. Lahko nastavljamo tudi prekinitve, hardverske ali softverske. V zgornjem programu smo nastavili hitrost serijske komunikacije na 115200 baudov na sekundo. Poleg tega smo na serijskem terminalu izpisali "Hello from setup()". 
2. Funkcija ``` loop()``` se izvaja v neskončnost. V funkciji ``` loop()``` pišemo kodo, ki obravnava nalogo, npr. nastavljanje in branje pinov, pošiljanje in sprejmanje podatkov preko I2C naprave, zagon servo-motorja, ipd. V našem primeru izpisujemo "Hello from loop()" na serijski terminal. 

### Tipi spremenljivk

| Tip                      | Opis                       | Dolžina (v bitih) |
|--------------------------|----------------------------|-------------------|
| uint8_t, unsigned char   | Nepredznačen bajt          | 8                 |
| int8_t, char             | Predznačen bajt            | 8                 |
| uint16_t, unsigned short | Nepredznačena dva bajta    | 16                |
| int16_t, short           | Predznačena dva bajta      | 16                |
| uint32_t, unsigned int   | Nepredznačeni štirje bajti | 32                |
| int32_t, int             | Predznačeni štirje bajti   | 32                |
| uint64_t, unsigned long  | Nepredznačenih osem bajtov | 64                |
| Int64_t, long            | Predznačenih osem bajtov   | 64                |
| float                    | Zapis v plavajoči vejici   | 32                |
||||


### Dodatni materiali:

1. Funkcija setup ([Povezava](https://www.arduino.cc/reference/en/language/structure/sketch/setup/))
2. Funkcija loop ([Povezava](https://www.arduino.cc/reference/en/language/structure/sketch/loop/))
3. Serijska komunikacija ([Povezava1](https://www.arduino.cc/reference/en/language/functions/communication/serial/begin)) ([Povezava2](https://www.arduino.cc/reference/en/language/functions/communication/serial/println)

## 5.3 Splošnonamenski vhod/izhod

Pri uporabi vhodno-izhodnih (I/O) pinov moramo najprej določiti vloge pinov. Na tokratni vaji bomo pine uporabljali kot digitalni vhod ali izhod. 

### Vhod 
Za nastavitev načina delovanja (mode) pina uporabljamo naslednji ukaz:

```c
pinMode(pin, mode)
```
kjer *pin* predstavlja številko pina, ki ga želimo uporabiti, *mode* pa določa način delovanja pina:

* ```INPUT```  	-> nastavimo izbrani pin kot vhod
* ```INPUT_PULLUP```  	-> nastavimo izbrani pin kot vhod s pull-up (dvižnim) uporom
* ```OUTPUT```  	-> nastavimo izbrani pin kot izhod

Stanje na vhodnih pinih lahko beremo s pomočjo funkcije  ```digitalRead()```.
Če želimo prebrati stanje na vhodnem pinu *pinIn*, lahko uporabimo naslednjo kodo:

```c
...
pinMode(pinIn, INPUT);
...
int pinState;
pinState = digitalRead(pinIn);
...
```

Po ukazu se stanje na vhodu pina *pinIn* shrani v spremenljivko *pinState*.


S pomočjo funkcije ```digitalWrite()``` nastavimo podani pin na podano vrednost. Če želimo nastaviti visoko stanje (1) na pin *pinOut*, lahko uporabimo naslednjo kodo:

```c
...
pinMode(pinOut, OUTPUT);
...
int outputValue = 1;
digitalWrite(pinOut,outputValue);
...

```

### Enostaven primer

Naslednji program ilustrira uporabo funkcij ```pinMode(pin, mode)```, ```digitalRead()```  in ```digitalWrite()```. Stanje vhodnega pina *inPin* se prenese na izhodni pin.


```c
int outPin = 13;  
int inPin = 7;    
int val = 0;     

void setup() {              // Nastavljanje pinov se izvaja v funkciji setup
  pinMode(outPin, OUTPUT);  // Postavimo outPin kot izhod
  pinMode(inPin, INPUT);    // Postavimo inPin kot vhod
}

void loop() {
  val = digitalRead(inPin);   // Beremo vhodni pin
  digitalWrite(outPin, val);  // Nastavimo izhodni pin
  delay(500); 		     // Nadaljevanje zakasni za 500 ms
}
```

Dodatni materiali:
1. Nastavitev pinov ([Povezava](https://www.arduino.cc/reference/en/language/functions/digital-io/pinmode/))
2. Branje vhodnih pinov ([Povezava](https://www.arduino.cc/reference/en/language/functions/digital-io/digitalread/))
3. Nastavljanje izhodnih pinov ([Povezava](https://www.arduino.cc/reference/en/language/functions/digital-io/digitalwrite/)) 
4. Zakasnitev  ([Povezava](https://www.arduino.cc/reference/en/language/functions/time/delay/)) 


## 5.4 Periodični klic funkcij

Objekti *Ticker* omogočajo periodično klicanje poljubne funkcije. Naslednji primer ilustrira uporabo Ticker objekta.

```c
#include <Ticker.h>  // Ticker knjižnica
 
Ticker blinker;
 
#define OUT 2  

int val;

//=======================================================================
void spremeni_stanje()
{
  digitalWrite(OUT, 1);  
  delay(1000);           	// Počakamo sekundo
  digitalWrite(OUT, 0);   
  delay(1000);                	// Počakamo sekundo 
}
//=======================================================================
void setup()
{
    Serial.begin(115200);
    Serial.println("");
 
    pinMode(OUT,OUTPUT);  	// nastavimo OUT pin kot izhod
 
    blinker.attach(0.5, spremeni_stanje);  // Funkcija spremeni_stanje
                                           // se kliče vsakih 0.5 s
}

//=======================================================================
void loop()
{
}
```

V prejšnjem programu si je najpomembneje zapomniti dve stvari:

1. Deklarirati objekt *Ticker*
   ```c
   Ticker blinker; 
   ``` 
2. Nastaviti objekt *ticker*, določiti funkcijo, ki se bo periodično klicala 
    ```c
    blinker.attach(0.5, spremeni_stanje);   // Funkcija spremeni_stanje
                                            // se kliče vsakih 0.5 s

   ```
    Funkcija blink_led1() se kliče vsakih 0.5 sekunde.



## 5.5 Domača naloga 

* Vgrajena LED lučka naj pri podani frekvenci petkrat
utripne, nato pa frekvenco povečate za 1 Hz.
Začetna frekvenca je 1 Hz, povečujete pa vse do 5
Hz, nato pa vse ponovite. Stanje izpisujte preko
serijskega vmesnika. Ustvarite dva samostojna
programa (projekta), efekt pa implementirajte z
uporabo:

  *  zakasnitve (delay()),
  *  z uporabo programske prekinitve (Ticker).


