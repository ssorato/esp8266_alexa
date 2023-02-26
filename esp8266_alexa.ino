#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"

// prototypes
boolean connectWifi();

//on/off callbacks 
bool espLightOn();
bool espLightOff();

// Change this before you flash
const char* ssid = "****";
const char* password = ""****";";

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *esp = NULL;
bool isEspLightOn = false;


void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());
   
  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  if(wifiConnected){
    upnpBroadcastResponder.beginUdpMulticast();
    
    // Define your switches here. Max 10
    // Format: Alexa invocation name, local port no, on callback, off callback
    esp = new Switch("led", 80, espLightOn, espLightOff);

    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*esp);
  }
}
 
void loop()
{
	 if(wifiConnected){
      upnpBroadcastResponder.serverLoop();
      esp->serverLoop();
	 }
}

bool espLightOn() {
    Serial.println("Turn the LED on ...");
    digitalWrite(LED_BUILTIN, LOW);
    isEspLightOn = true;    
    return isEspLightOn;
}

bool espLightOff() {
    Serial.println("Turn the LED off ...");
    digitalWrite(LED_BUILTIN, HIGH);
    isEspLightOn = false;
    return isEspLightOn;
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10){
      state = false;
      break;
    }
    i++;
  }
  
  if (state){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  
  return state;
}
