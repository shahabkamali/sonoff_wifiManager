/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <DNSServer.h> 

#include <PubSubClient.h>
#include <string.h>

void callback(char* topic, byte* payload, unsigned int length);


// web server
ESP8266WebServer server(80);

#include "config.h"

#define DeviceID "0200000001"

int gpio13Led = 13;
int gpio12Relay = 12;



WiFiClient espClient;
PubSubClient client(espClient);


void setup(void){
  WiFiManager wifiManager;
  wifiManager.autoConnect("wifi-vivvi");
  Serial.println("connected... :)");
  // preparing GPIOs
  pinMode(gpio13Led, OUTPUT);
  digitalWrite(gpio13Led, HIGH);
  
  pinMode(gpio12Relay, OUTPUT);
  digitalWrite(gpio12Relay, HIGH);
 
  Serial.begin(9600); 
  delay(2000);
  
  client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);
    while (!client.connected()) {
      Serial.println("Connecting to MQTT...");
   
      if (client.connect("ESP8266Client")) {
   
        Serial.println("connected");  
   
      } else {
   
        Serial.print("failed with state ");
        Serial.print(client.state());
        delay(2000);
   
      }
  }
  client.subscribe("iot/switch/0200000001"); 
}
 
void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  if(payload[0] == '1'){
    digitalWrite(13,HIGH);
    digitalWrite(gpio12Relay, HIGH);
    Serial.println("on");
  }
  else{
    digitalWrite(13,LOW);
    digitalWrite(gpio12Relay, LOW);
    Serial.println("off");
  }
  Serial.println();
  Serial.println("-----------------------");
}


void loop() {
    client.loop();
}