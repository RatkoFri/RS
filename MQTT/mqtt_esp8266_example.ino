/*
 ESP8266 MQTT example
 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.
 
 Prerequisites:
 
 PubSubClient -> Install the library through : Arduino -> Tools -> Manage Libraries -> type PubSubClient -> install the package created by Nick O'Leary (newest version)
 
 Git: https://github.com/knolleary/pubsubclient
 API: https://pubsubclient.knolleary.net/
  
ESP8266 first reads acceleration data, then it connects to an MQTT server "broker.mqtt-dashboard.com", and finally publishes acceleration measurements to the topic "RatkoACCData" 
    
 More info:
 "broker.mqtt-dashboard.com" is a publicly available MQTT broker. You can set your own MQTT broker on ESP8266 or your laptop
 MQTT introduction: https://www.hivemq.com/mqtt-essentials/
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include<Wire.h>
#include<Ticker.h>

#define INTERVAL 100
#define PIN_LED 2
#define I2C_ADD_MPU 104
#define I2C_ADD_BMP 118
#define TABLE_SIZE_MPU 7
#define GYRO_OBCUT 131
#define REG_GYRO_CONFIG 0x1B // 27
#define RATE 10
#define I2C_ADD_IO1 32
#define ACC_X_OUT 59

// Update these with values suitable for your network.

const char* ssid = "esp8266";
const char* password = "adminadmin";
const char* mqtt_server = "broker.mqtt-dashboard.com";
const char* topic = "RatkoACCData"; 


WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

Ticker tick;
int32_t table[TABLE_SIZE_MPU];
float delilnik = 131.0f;
float acc_x_calib = 0.0;

// funkcije:
void beriPodatke();
void acc_config();


void beriPodatke() {
  static uint32_t count = 0;
  digitalWrite(PIN_LED, 0);
  static float acc_x = 0.0f;
  int32_t table;
  
  //**** MPU-9250
  //**** Naslov registra 
  // "zapiši", od katerega naslova registra dalje želimo brati
  Wire.beginTransmission(I2C_ADD_MPU);
  Wire.write(ACC_X_OUT);
  Wire.endTransmission();
  
  //** Branje: pospešek v x_osi
  //** Zdaj mikrokrmilnik bere od naslova ACC_X_OUT
  //** Bere dva bajta prvi bajt: 
  Wire.requestFrom(I2C_ADD_MPU, 2);
  table = (int8_t)Wire.read();
  table = table << 8;
  table += (uint8_t)Wire.read();

  
  acc_x += ((table / delilnik)-acc_x_calib)/RATE;


  if (count % RATE == 0)
  {
    // Izpišemo
    Serial.print("ACC_X: X= ");
    Serial.print(acc_x);
    Serial.println("");

    // MQTT
    // sends acceleration data as string stored in msg variable
    client.loop();
    snprintf (msg, MSG_BUFFER_SIZE, "%4.2f", acc_x);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(topic, msg);
    // resetiramo vrednost
    acc_x = 0;
  }

  
  // števec 
  count = count+1;
  digitalWrite(PIN_LED, 1);
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);

  // Inicializiramo I2C na podanih pinih
  Wire.begin(12,14);
  // nastavimo frekvenco vodila na 100 kHz
  Wire.setClock(100000);

  tick.attach_ms(INTERVAL, beriPodatke);

  if (!client.connected()) {
    reconnect();
  }
}

void loop() {

}
