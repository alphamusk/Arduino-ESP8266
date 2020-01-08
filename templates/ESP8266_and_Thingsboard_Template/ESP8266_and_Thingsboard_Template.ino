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
#include <ThingsBoard.h>
ThingsBoard tb(wifi_client);
const char* thingsboard_server = TB_SERVER;
const uint16_t port = TB_SERVER_PORT;
const char* tb_token = MY_TB_TOKEN;
unsigned long tb_last_send; // Keeps track of last time we sent telemetry to tb using millis()
int tb_send_delay = 2000; // In ms, how long to wait to send data updates to tb
int tb_retry_connect_delay = 5000; // In ms, how long to wait before we retry connecting to tb server


/* 
 * Sensors here
 */


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

  delay(10);
  
  // Wifi
  connectWifi();

  // Thingsboard delays & timing
  tb_last_send = 0;
}


//////////////////////
/////// LOOP  ////////
//////////////////////
void loop() {

  // If we are not connected to thingsboard
  if ( !tb.connected() ) {
    reconnect();
  }

  // Check since last send
  if (millis() - tb_last_send > tb_send_delay) { // Update and send only after x seconds
    
    // Example send sensor data function
    sendTelemetryData();

    // Reset time tracking
    tb_last_send = millis(); 

  }

  // Must loop to keep tb connected
  tb.loop();
}



//////////////////////
///// FUNCTIONS //////
//////////////////////

/*
 * sendTelemetryData()
 * 
 * Send Thingsboard telemetry
 * @returns void
 */
void sendTelemetryData() {

  #if DEBUG 
      Serial.print("Function: ");
      Serial.println(__FUNCTION__);
      Serial.print("Sending telemetry to "); Serial.print(thingsboard_server);
      Serial.print(" every "); Serial.print((tb_send_delay/1000)); Serial.print(" seconds"); Serial.println();
  #endif
  
  // Get temperature data, mock up example of random number
  float temperature = random(-15, 120);
  float humidity = random(0, 100);
  
  #if DEBUG 
      Serial.print("temperature: "); Serial.print(temperature); Serial.println();
      Serial.print("humidity: "); Serial.print(humidity); Serial.println();
      Serial.print("wifi_ssid: "); Serial.print(wifi_ssid); Serial.println();
      Serial.print("wifi_signal: "); Serial.print(WiFi.RSSI()); Serial.println();
      Serial.print("ip_address: "); Serial.print(WiFi.localIP().toString()); Serial.println();
      Serial.print("mac_address: "); Serial.print(WiFi.macAddress()); Serial.println();
      Serial.println(); Serial.println();
  #endif 

  // Send data the thingsboard
  tb.sendTelemetryFloat("temperature", temperature);
  tb.sendTelemetryFloat("humidity", humidity);

  tb.sendTelemetryString("wifi_ssid", wifi_ssid);
  tb.sendTelemetryInt("wifi_signal", WiFi.RSSI()); 
  tb.sendTelemetryString("ip_address", WiFi.localIP().toString().c_str());
  tb.sendTelemetryString("mac_address", WiFi.macAddress().c_str());
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
  while (!tb.connected()) {
    if ( WiFi.status() != WL_CONNECTED) {
        connectWifi();
    }
    Serial.print("Connecting to ThingsBoard server ");
    Serial.print(thingsboard_server);
    Serial.print(" TOKEN: ");
    Serial.print(tb_token);
    
    if ( tb.connect(thingsboard_server, tb_token) ) {
      Serial.println( "...[DONE]" );
    } else {
      Serial.print( "...[FAILED]" );
      Serial.print( " retrying in "); Serial.print((tb_retry_connect_delay/1000)); Serial.println(" seconds" );
      // Wait X seconds before retrying
      delay(tb_retry_connect_delay);
    }
  }
  Serial.println(); Serial.println();
}

/* EOF */
