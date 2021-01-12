#include <Arduino.h>
#include <string.h>
#include <ESP8266WiFi.h>
#include <MQTT.h>


MQTTClient client;
WiFiClient wificlient;
const char ssid[] = "IFD2-TR54";
const char pass[] = "90xV@FsT";
const char hostname[14] = "192.168.1.46";
const char* mqqtTopicIN = "topicc";
const char* mqqtTopicOUT = "topic";


void connect() {
  Serial.println("[DEBUG#ESP Checking wifi...]");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(".");
    delay(100);
  }
  Serial.println("[DEBUG#ESP Wifi connected, MQTT connecting...]");
  while (!client.connect("NGAUD", "try", "try")) {
    Serial.println("+");
    delay(100);
  }
  Serial.println("[DEBUG#ESP MQTT connected!]");
  client.subscribe(mqqtTopicIN);
  
}


void messageReceived(String &topic, String &payload) {
  Serial.println("[DEBUG#ESP Forwarding to MEGA Incoming message from MQTT on topic: "); 
  Serial.println(topic);
  Serial.println(" - "); 
  Serial.println(payload);
  Serial.println("]");
}

String inString;
void serialEvent() {
  while (Serial.available()) {
    
    char inChar = Serial.read();
    inString += inChar;
    if (inChar == ']') { // ] = caractère de fin de commande
      /*if (inString.indexOf("[MACOMMANDE]")>0) {
        //Je fais un truc
      }
      else if (inString.indexOf("[MACOMMANDE]")>0) {
        //Je fais un autre truc
      }
      else
      {
        Serial.println("Problème commande inconnue arrive depuis l'ESP"); // s'affiche sur le monitor de debug
      }*/
      client.publish(mqqtTopicOUT, inString);
      inString = "";//reset de la string jusqu'à la prochaine commande
    }
  }
  
}

unsigned long lastMillis = 0;
void alive() {
  // publish a message roughly every second.
  if (millis() - lastMillis > 10000) {
    lastMillis = millis();
    client.publish("/esp/alive", "coucou");
  }
}

void setup() {
  // Initialisation des ports et sorties (ESP envoie/reçoit vers ATMega avec serial, ATMega envoie/Reçoit vers ESP avec serial3)
  Serial.begin(115200);//vers ATMega
 
  Serial.println("[DEBUG#ESP Welcome]");
  WiFi.begin(ssid, pass);
  
  client.begin(hostname, 1883, wificlient);
  client.onMessage(messageReceived);
  connect();
}

void loop() {
  client.loop();
  if (!client.connected()) {
    connect();
  }
  serialEvent();
  alive();
}