/*
  This example connects to an encrypted WiFi network (WPA/WPA2).
  Then it prints the MAC address of the board,
  the IP address obtained, and other network details.
  Then it continuously pings given host specified by IP Address or name.

  Circuit:
  * Board with NINA module (Arduino MKR WiFi 1010, MKR VIDOR 4000 and UNO WiFi Rev.2)

  created 13 July 2010
  by dlf (Metodo2 srl)
  modified 09 June 2016
  by Petar Georgiev
*/
#include <SPI.h>
#include <WiFiNINA.h>
#include <DHT.h>
#include "ThingSpeak.h"
#define DHTPIN 7   
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
#define DHTTYPE DHT11   // DHT 11
char ssid[] = "Gateway_12";        // your network SSID (name)
char pass[] = "Kuopio1234$$";    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the WiFi radio's status
WiFiClient  client;

DHT dht(DHTPIN, DHTTYPE);
unsigned long myChannelNumber = 1607188;
const char * myWriteAPIKey="N0I9C8RKREFOO2ZH";
int number = 0;
// Specify IP address or hostname
String hostName = "www.google.com";
int pingResult;

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  Serial.println("DHT11 test!");

  dht.begin();
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 5 seconds for connection:
    delay(5000);
  }

  // you're connected now, so print out the data:
  Serial.println("You're connected to the network");
  ThingSpeak.begin(client); 
}

void loop() {
  //Humidity
  float h = dht.readHumidity();
  //Temp in Celsius
  float t = dht.readTemperature();
  //Temp in Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check for Failure
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  //Heat Index in Farenheit
  float hif = dht.computeHeatIndex(f, h);
  //Heat Index in Celsius
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%  Temperature: ");
  Serial.print(t);
  Serial.print("°C ");
  Serial.print(f);
  Serial.print("°F  Heat index: ");
  Serial.print(hic);
  Serial.print("°C ");
  Serial.print(hif);
  Serial.println("°F");
  // pieces of information in a channel.  Here, we write to field 1.
  int x = ThingSpeak.writeField(myChannelNumber, 1, t, myWriteAPIKey);
  
  if(x == 200){
    Serial.println("temperature Channel update successful.");
  }
  else{
    Serial.println("temperature Problem updating channel. HTTP error code " + String(x));
  }
  delay(20000);
  int y = ThingSpeak.writeField(myChannelNumber, 2, h, myWriteAPIKey);
  if(y == 200){
    Serial.println("Humidity Channel update successful.");
  }
  else{
    Serial.println("humidity Problem updating channel. HTTP error code " + String(x));
  }
  delay(50000);
}
