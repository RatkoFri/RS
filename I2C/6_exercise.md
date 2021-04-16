# I2C protocol and its usage on ESP8266

## I2C 

In this exercise, we will get to know the basics of the Inter-Integrated Circuit (I2C) protocol. The I2C protocol represents one of the most widespread protocols for serial communication on short distances, at least if we are talking about microcontrollers and embedded systems. The protocol is frequently used for communication with different sensors and actuators. 

In the rest of the exercise, we will first get to know the basics of the I2C protocol and its API. In the end, we will get to know with PCF8574AN component, which connects external LEDs and buttons with the microcontroller.

## Protokol I2C 

All integrated circuits in the I2C network share a common bus, consisting of two lines (SCL and SDA) and common ground. SDA (Serial Data) serves for data and command transmission. SCL (Serial clock) represents the clock signal, which synchronises transmission and determines the transmission speed. Every device connected to the I2C network has an address and can transmit or receive data. 

The control unit (master) manages the I2C  communication, generates a clock,  configures the slave devices and sends them various data and commands. The slave device can also send data to the master unit, but only when the master unit requires it. The bits are transmitted in serial fashion, and after the 8th bit, the receiving side sets the SDA line on zero to acknowledge (ACK) the transmission.

On the sending side, the device needs to set the next bit on the SDA line as long as the SCL is in state 0. The protocol I2C recognises two special states of bus: START and STOP, which occur when the SCL line is set to  1, while the SDA line changes state:

``` 
SCL = 1, SDA = 1 ->0: START
``` 

``` 
SCL = 1, SDA = 0 ->1: STOP
``` 

Every slave unit needs to have its address, which is unique compared to the rest of the connected devices.  For some slave devices, the address cannot be changed, while for some, we can change the address by setting the device pins. Most I2C devices have the 7-bit address, and 8 bit represents command ( value 0 -> writing, value 1 -> reading). 

I2C communication starts when the master unit generates a START signal and sends the slave address. The START signal activates all devices on the bus. The slave device, which recognises its address, sends the acknowledgement confirmation. The remaining devices wait for the next START signal. The Master unit transmits the data to the selected device and terminates with a STOP signal.

([Link](https://svet-el.si/revija/samogradnje-revija/i2c-monitor/))


## I2C in ESP8266 with Arduino library

On ESP8266, the SDA and SCL pins are connected with pins 14 and 12 of ESP8266. Functions from the library **Wire.h** implement the I2C protocol.
([Povezava](https://www.arduino.cc/en/Reference/Wire))

### Setting up the I2C communication


Before we start to use I2C, we need to initialize the I2C device:
- Assign I2C pins (SDA in SCL), 
- Set the I2C speed. 

To assign the I2C pins, we use the following function:

```c 
Wire.begin(pin_SCL, pin_SDA);
```
Next, we need to set the I2C speed in Hz:

```c 
Wire.setClock(clock);
```
where **clock** represent frequency in Hz.

Primer nastavljanja I2C komunikacije za ESP8266: 
Spodnji del kode boste vedno uporabili v funkciji **setup( )**.

```c 
  // Inicializiramo I2C na podanih pinih
  // SDA: pin 12
  // SDC: pin 14
  Wire.begin(12,14);
  // nastavimo frekvenco vodila na 100 kHz
  Wire.setClock(100000);
``` 

### I2C Read 

Reading I2C slave is done in two stages:

1. Microcontroller requests data transmission from the device 
   
  ```c 
  Wire.requestFrom(address, length);
  ```    
where *address* represents device address, *length* represents the number of bytes which device sends to the microcontroller

2. Slave returns requested data 

```c 
uint8_t val = Wire.read();
```  

Example: How to read one byte from the slave with address 56 (0x38)?

```c 

Wire.requestFrom(56, 1);
uint8_t val = Wire.read();
``` 

###  I2C pisanje

Writing to device is done in three stages:

1. The microcontroller begins the data transmission with the device at the specified address. 


```c 
Wire.beginTransmission(address);
```  

2. The microcontroller sends one byte or several bytes in the form of an array.
```c 
Wire.write(val);
//ali
Wire.write(uint8_t buf[], dolžina);
```  

3. The microcontroller terminates transmission
```c 
Wire.endTransmission();
```  

Example: Writting to the I2C device at address 56 (0x38)

```c 
// Na napravo z naslovom 56 zapišemo vredost val
Wire.beginTransmission(56);
Wire.write(val);
Wire.endTransmition();
``` 

## Peripheral device:  I/O expander PCF8574AN


We will try to communicate with I/O expander PCF8574AN using the I2C protocol. The PCF8574AN allows the controlling of external LEDs and buttons. 
SDA in SDC pins of PCF8574AN is connected to pins 12 and 14, respectively.


<p align="center">
    <img  src="./figures/plosca.png" width="300">
</p>

On I2C bus, PCF8574AN has address 0x20 (hexadecimal) or 32 (decimal). Lower four pins are connected with LEDs, while buttons are connected on the upper four pins, as presented in Figure:

<p align="center">
    <img  src="./figures/pcf.png" width="300">
</p>

PCF8574AN consists of one 8-bit register, which allows controlling of LEDs and buttons. The seventh bit of register corresponds to the pin P7, the sixth bit of register corresponds to P6, etc. If we want to turn off/turn on LEDs, we write appropriate values on the lower four bits or register. On the other hand, if we want to know button states, we read the register and only consider only the upper 4 bits. 

<p align="center">
    <img  src="./figures/reg.svg" width="400">
</p>


Napotki za krmiljenje:

* Writing value one to P0-P3 pins turns of LEDs. Contrary, writing value zero to P0-P3 pins turns of LEDs.
  *  If we want to turn LED1, we need to send 0xFE or 0x0E
  *  If we want to turn LED2, we need to send 0xFD or 0x0D
  *  If we want to turn LED3, we need to send 0xFB or 0x0B
  *  If we want to turn LED4, we need to send 0xF7 or 0x07
*  When we want to read button states, we need to set all the upper 4 bits of the register. If the button is pressed, the corresponding bit will be set to 1.
   *  If button S1 is pressed, 4 bit will be set to zero
   *  If button S2 is pressed, 5 bit will be set to zero
   *  If button S3 is pressed, 6 bit will be set to zero
   *  If button S4 is pressed, 7 bit will be set to zero

## Program for PCF8574AN control with the help of functions from Ticker.h and Wire.h


```c 
#include <Wire.h>
#include <Ticker.h>

#define I2C_ADD_IO1 32

Ticker tickButton, tickLED;

void readButtons();
void LedBlink();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // Initialize I2C
  // SDA: pin 12
  // SCL: pin 14
  Wire.begin(12,14);
  // Set speed of I2C
  Wire.setClock(100000);

  tickLED.attach_ms(500,LedBlink); // Ticker za LED blinking
                                   
                                
  tickButton.attach_ms(500,readButtons); // Ticker for checking buttons
}

/*
 * Function for reading buttons  
 */
void readButtons(){

  Wire.requestFrom(I2C_ADD_IO1, 1);
  uint8_t val = Wire.read() | 0xF0;

  Wire.beginTransmission(I2C_ADD_IO1);
  Wire.write(val);
  Wire.endTransmission();

  Wire.requestFrom(I2C_ADD_IO1, 1);
  val = (~(Wire.read()) & 0xF0)>>4;


  Serial.println("Button state: ");
  Serial.print(val);
  Serial.println(" ");

}

/*
 * Function for LED blinking
 */
void LedBlink(){
  static uint8_t LED_stanje = 0;
  LED_stanje = (LED_stanje + 1) % 2;
  Wire.beginTransmission (I2C_ADD_IO1);
  Wire.write(~LED_stanje);
  Wire.endTransmission();
}

void loop() {
  // put your main code here, to run repeatedly:

}
``` 

## Exercise

* Implement the effect of running light on external LEDs 
* With the button S3 increase the frequency of running light
(Start period: 250 ms, End period: 2000 ms, Increment: 250 ms ).
* (*Optional*):  With button S1, lower the number of turned-on LEDs while button S2 increases the number of turned-on LEDs

### Tips:
* Use one ticker to read buttons (read buttons every second). The second ticker updates the speed of the running light. Example:
  ```c 
   // Lower period if button S1 is pressed. variable val carries the value of button states
  if (val & 1){
    if (interval_LED_ms > INTERVAL){
      interval_LED_ms -= INTERVAL; // Calculate interval
      Serial.print("New interval: ");
      Serial.println(interval_LED_ms);
      tickLED.detach(); // Deactivate Ticker
      tickLED.attach_ms(interval_LED_ms, posodobiLED); // Activate ticker with new updated period
    }
  }  
  ```
* 
* 
Implement function for reading buttons. The function returns values that correspond to the button state. Before reading, you need to set the upper bits of the device register to 1. Don't alter the LED states. 
* Implement function for running light.
* Logical operation in C/C++: 
    * http://en.cppreference.com/w/cpp/language/operator_logical
    * http://www.cplusplus.com/doc/tutorial/operators/
    * https://en.wikipedia.org/wiki/Operators_in_C_and_C%2B%2B
