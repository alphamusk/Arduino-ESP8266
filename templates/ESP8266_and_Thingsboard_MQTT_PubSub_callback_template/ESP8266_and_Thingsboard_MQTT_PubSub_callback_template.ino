/* 
 * config.h
 * 
 * Common settings, change as needed
 */
#include "config.h"

/* 
 * Rename default_credentials.h to credentials.h
 * 
 * Credentials can be set here or by changing the parameters in  
 * default_credentials and then renaming the file to credentials.h
 */
#include "credentials.h"  

/* Required Sketch libraries */
#include <ArduinoJson.h>

/* 
 * WiFi Settings
 *  
 * Credentials can be set here or by changing the parameters in  
 * default_credentials and then renaming the file credentials.h
 */
#include <ESP8266WiFi.h>
WiFiClient wifi_client;
int status = WL_IDLE_STATUS;
const char* wifi_ssid = MY_WIFI_SSID;
const char* wifi_password = MY_WIFI_PASSWORD;
String mac_address;
String ip_address;
int wifi_connection_attempts = 0;
int wifi_connection_attempts_max = 20;

/* 
 * Thingsboard Settings
 *  
 * Credentials can be set here or by changing the parameters in  
 * default_credentials and then renaming the file credentials.h
 */
//#include <ThingsBoard.h>
//ThingsBoard tb(wifi_client);
#include <PubSubClient.h>
PubSubClient client(wifi_client);
const char* thingsboard_server = TB_SERVER;
const uint16_t port = TB_SERVER_PORT;
const char* tb_token = MY_TB_TOKEN;
int tb_retry_connect_delay = 5000; // In ms, how long to wait before we retry connecting to tb server

/* 
 * Sensors here
 */
 
/* ESP8266 NODEMCU */
#define GPIO1 1
#define GPIO2 2
#define GPIO1_PIN 1
#define GPIO2_PIN 2

// We assume that all GPIOs are HIGH, aka off
boolean gpioState[] = {false, false};


//////////////////////
/////// SETUP ////////
//////////////////////
void setup() {
  
  Serial.begin(SERIAL_BAUD_RATE);

  // Debug setting
  Serial.println();
  Serial.print("DEBUG: ");
  if(DEBUG) {
    Serial.print("[ON]");
  } else {
    Serial.print("[OFF]");
  }
  Serial.println();
  
  // Set output mode for all GPIO pins
  pinMode(GPIO1, OUTPUT); digitalWrite(GPIO1, HIGH); // NodeMCU is backwards, Arduino is LOW
  pinMode(GPIO2, OUTPUT);  digitalWrite(GPIO2, HIGH); // NodeMCU is backwards, Arduino is LOW
  
  delay(10);
  
  // Wifi
  connectWifi();

  client.setServer(thingsboard_server, 1883);
  client.setCallback(on_message);
}


//////////////////////
/////// LOOP  ////////
//////////////////////
void loop() {

  // If we are not connected to thingsboard
  if (!client.connected()) {
    reconnect();
  }

  // Must loop to keep tb connected
  client.loop();
}


//////////////////////
///// FUNCTIONS //////
//////////////////////

/* The callback for when a PUBLISH message is received from the server. */
void on_message(const char* topic, byte* payload, unsigned int length) {

  #if DEBUG 
      Serial.print("Function: ");
      Serial.println(__FUNCTION__);
  #endif
  
  Serial.println("On message");

  char json[length + 1];
  strncpy (json, (char*)payload, length);
  json[length] = '\0';

  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  Serial.println(json);

  // Decode JSON request
  StaticJsonDocument<200> jsonDoc;
  auto error = deserializeJson(jsonDoc, (char*)json);
  
  if (error) {
      Serial.print(F("deserializeJson() failed with code "));
      Serial.println(error.c_str());
      return;
  }

  // Check request method
  String methodName = String((const char*)jsonDoc["method"]);

  // Reply with GPIO status
  if (methodName.equals("getGpioStatus")) {
    
    String responseTopic = String(topic);
    responseTopic.replace("request", "response");
    client.publish(responseTopic.c_str(), get_gpio_status().c_str());
    
  } else if (methodName.equals("setGpioStatus")) {
    
    // Update GPIO status and reply
    set_gpio_status(jsonDoc["params"]["pin"], jsonDoc["params"]["enabled"]);
    String responseTopic = String(topic);
    responseTopic.replace("request", "response");
    client.publish(responseTopic.c_str(), get_gpio_status().c_str());
    client.publish("v1/devices/me/attributes", get_gpio_status().c_str());
    
  }
}

String get_gpio_status() {
    StaticJsonDocument<200> jsonDoc;
    jsonDoc[String(GPIO1_PIN)] = gpioState[1] ? true : false; // NodeMCU is backwards, default should be HIGH : LOW for arduino
    jsonDoc[String(GPIO2_PIN)] = gpioState[2] ? true : false; // NodeMCU is backwards, default should be HIGH : LOW for arduino
    char payload[256];
    serializeJson(jsonDoc, payload);
    String strPayload = String(payload);
    Serial.print("Get gpio status: ");
    Serial.println(strPayload);
    return strPayload;
}

void set_gpio_status(int pin, boolean enabled) {
  
  if (pin == GPIO1_PIN) {
    
    // Output GPIOs state
    digitalWrite(GPIO1, enabled ? LOW : HIGH); // NodeMCU is backwards, Arduino is HIGH : LOW
    // Update GPIOs state
    gpioState[1] = enabled;
    
  }
  if (pin == GPIO2_PIN) {
    
    // Output GPIOs state
    digitalWrite(GPIO2, enabled ? LOW : HIGH); // NodeMCU is backwards, Arduino is HIGH : LOW
    // Update GPIOs state
    gpioState[2] = enabled;
    
  }
}


/*
 * Connect to wifi network
 * @returns void
 */
void connectWifi() {

  #if DEBUG
    Serial.print("Function: ");
    Serial.println(__FUNCTION__);
  #endif
  
  Serial.print("Connecting to WiFi ");
  Serial.print(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    
    wifi_connection_attempts++;
    Serial.print(wifi_connection_attempts);

    if(wifi_connection_attempts >= wifi_connection_attempts_max) {
      Serial.println("...Rebooting");
      delay(2000);
      ESP.restart(); // Reboot ESP
    }
    delay(500);
  } 

  Serial.println("...[DONE]");
  Serial.print("Connection attempts ");
  Serial.print(wifi_connection_attempts);
  Serial.print(" of ");
  Serial.println(wifi_connection_attempts_max);

  ip_address = WiFi.localIP().toString();
  mac_address = WiFi.macAddress();
  
  Serial.print("IP Addr: ");
  Serial.println(ip_address);
  Serial.print("MAC Addr: ");
  Serial.println(mac_address);
  Serial.println(); Serial.println();
}


/*
 * Reconnect if we are not connected
 * @returns void
 */
void reconnect() {

  #if DEBUG
    Serial.print("Function: ");
    Serial.println(__FUNCTION__);
  #endif

  // Loop until we're reconnected
  while (!client.connected()) {
    
    if (WiFi.status() != WL_CONNECTED) {
        connectWifi();
    }
    
    Serial.print("Connecting to ThingsBoard server ");
    Serial.print(thingsboard_server);
    Serial.print(" TOKEN: ");
    Serial.print(tb_token);
    
    // Attempt to connect (clientId, username, password)
    if (client.connect("ESP8266 Device", tb_token, NULL)) {
      
      Serial.println( "[DONE]" );
      
      // Subscribing to receive RPC requests
      client.subscribe("v1/devices/me/rpc/request/+");
      
      // Sending current GPIO status
      Serial.println("Sending current GPIO status ...");
      client.publish("v1/devices/me/attributes", get_gpio_status().c_str());
      
    } else {
      
      Serial.print( "[FAILED] [ rc = " );
      
      Serial.print(client.state());
      Serial.print( " retrying in "); Serial.print((tb_retry_connect_delay/1000)); Serial.println(" seconds" );
      
      Serial.println( " : retrying in 5 seconds]" );
      // Wait 5 seconds before retrying
      delay(tb_retry_connect_delay);
    }
    Serial.println(); Serial.println();
  }
}

/* EOF */
