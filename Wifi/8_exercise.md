# Wireless communication and ESP8266

## Wireless communication ("Wi-Fi")

Wi-Fi is a family of wireless network protocols, based on the IEEE 802.11 family of standards, which are commonly used for local area networking of devices and Internet access, allowing nearby digital devices to exchange data by radio waves. These are the most widely used computer networks in the world, used globally in home and small office networks to link desktop and laptop computers, tablet computers, smartphones, smart TVs, printers, and smart speakers together and to a wireless router to connect them to the Internet, and in wireless access points in public places like coffee shops, hotels, libraries and airports to provide the public Internet access for mobile devices.

<p align="center">
    <img  src="./figures/WiFi_Logo.svg" width="250">
</p>

### Standardi za Wi-Fi komunikacijo 

#### 802.11

The oldest  Wi-Fi standard, which was created in 1997 by IEEE (Institute for Electronic and Electrical Engineering). Standard supports bandpass speed to 11 Mb/s at frequency of 2.4 GHz. 


#### 802.11b

Represents the advanced version of 802.11 standard, which supports up to 11 Mb/s. It  can easily cause interference on home appliances that use bandpass 2.4 GHz


#### 802.11a

Was developed from 802.11b standard. Due to increased costs, the 802.11a was not popular as 802.11b. The 802.11a standard supports up to 54 Mb/s at frequency of 5 GHZ.


#### 802.11g

Combines  802.11a and 802.11b standards and offers a speed of  54 Mb/s at a frequency of 2.4 GHz. The 802.11g standard is compatible with devices that support the  802.11b standard.


#### 802.11n

Represents expanded 802.11g standard which supports communication with  multiple antennas and speed to 300 Mb/s.


#### 802.11ac 

The newest and most popular wifi standard on modern generation devices. 802.11ac uses two-pass wifi technology, which supports duplex communication lines. It is compatible with 802.11 b/g/n standards. 


## ESP8266 as Wi-Fi device 

The ESP8266 represent the popular solution for enabling Internet-of-Things technologies and connecting different edge devices in one network. To program ESP8266, we can use various software development environments from Arduino IDE to Micropython. An alternative way for programming ESP8266 is through the use of AT commands. 

### Properties of ESP8266 in the role of  Wi-Fi transceiver

1. Supports  802.11 b/g/n protokols
2. Integrated TCP/IP 
3. Supports network protocols IPv4, TCP/UDP/HTTP/FTP
4. Supports antenna diversity
5. We can use it in Station mode (STA), access point (AP) or both at the same time (STA+AP)
6. Supports smart connection with Android and iOS devices


##  Wi-Fi and Arduino


All functions for establishing and setting the wireless communication are implemented in the ESP8266WiFi.h library.


Function:


|             Method            |                                                    Description                                                   |
|:-----------------------------:|:---------------------------------------------------------------------------------------------------------:|
| WiFi.mode()                   | Selecting the  operating mode (AP, STA).                                                            |
| WiFi.softAP()                 | Use the ESP8266 as Access Point                         |
| WiFi.begin()                  | Connect to the existing network. |
| WiFi.status()                 |Check the connection status. (Returns WL_CONNECTED if the connection is successful)                                   |
| WiFi.localIP()                | Returns the IP address ( only  in STA mode)                                                                   |
| WiFi.softAPIP()               | Return the IP address of the soft access point’s network interface. ( only  in AP mode)                                                                   |
| ESP8266WebServer server(port) | Starts the web server on port *port*                                                                    |                               |



### WiFi.mode(mode);

|             Arguments             |                                                    Description                                                   |
|:-----------------------------:|:---------------------------------------------------------------------------------------------------------:|
| WIFI_AP                       | The ESP8266 acts as Access Point.                                     | 
| WIFI_STA                      | The ESP8266  is used in Station mode   |
| WIFI_STA_AP                   |  The ESP8266 works simultaneously in both modes.                               |                                                          


### WiFi.softAP("name", "pass");

|             Arguments             |                                                    Description                                                   |
|:-----------------------------:|:---------------------------------------------------------------------------------------------------------:|
|         name                 | Name of the access point which we want to set up (mode WIFI_AP).                                        | 
|         pass                 | Access point password (mode WIFI_AP)


### WiFi.begin("name", "pass");

|             Arguments             |                                                    Description                                                   |
|:-----------------------------:|:---------------------------------------------------------------------------------------------------------:|
|         name                 | Name of the existing access point or network (mode WIFI_STA).                                                     | 
|         pass                 | Password of the existing access point or network(mode WIFI_STA).                                                           | 

### Primeri:

- Establishing an access point

```c 
    WiFi.mode(WIFI_AP);
    WiFi.softAP(name, pass);
    // Remaining code: ..
``` 

- Connect to the existing network

```c 
   WiFi.mode(WIFI_STA);
   WiFi.begin(name, pass);
   //  We need to wait until the ESP8266 connects to the network.
   while (WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.print(".");
   }
``` 
## Assignment

-  Write a program that connects WEMOS8266 to the existing network.

- Implement webserver ( the core is already available on e-classrom), which enables:
  * Turning the light on and off of the embedded LED on the WEMOS board (blue LED). User authorization is required.
  * If the user selects the wrong URL, the webserver should notify the user and return him to log in.

### Tips 

- Use #define directive to define username and password
- When writing your HTML code, you can help with: 
   * core html code,
   * https://www.w3schools.com/tags/ref_httpmethods.asp
   * https://www.w3schools.com/tags/att_form_method.asp
* Use the serial monitor to display LED status
*  Enter the device's IP address in the web browser to visit the web link created by the webserver. 

