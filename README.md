# [Documentation](http://arduino-esp8266.traceyflanders.com)

## Getting Started

> Clone the repo
```git clone https://github.com/traceyflanders/Arduino-ESP8266.git```


### Changing config.h
The ```config.h``` include header file
```C++
/* 
 *  Common configs header file
 */
#define DEBUG false                 // Enables Serial.print() statements for debugging purposes    
#define SERIAL_BAUD_RATE  115200    // Serial.begin(SERIAL_BAUD_RATE); 


/* 
 * Copyleft credit 
 */
#define SKETCH_VERSION  "200108A"   // YYMMDD[A-Z]
#define SKETCH_AUTHOR   "Tracey Flanders" // Credit 


/* 
 *   Thingsboard Server 
 */
// #define TB_SERVER  "demo.thingsboard.io"
#define TB_SERVER  "10.0.1.254"     
#define TB_SERVER_PORT 1883
```


### Setting credentials for authentication to Wifi and Thingsboard
The ```default_credentials.h``` include header file. Rename the file to ```credentials.h```
```C++
/*
 * Credentials header file
 */
/////////////////////////////////////////////////////////////////////////////////
/////////////////////// RENAME THIS FILE TO credentials.h ///////////////////////
/////////////////////////////////////////////////////////////////////////////////

/* 
 * WIFI Credentials 
 * Change to your wifi name and password 
 */
#ifndef MY_WIFI_SSID 
  #define MY_WIFI_SSID "cool_wifi_name"
  #define MY_WIFI_PASSWORD "super_duper_password"   
#endif


/*
 * Thingsboard API Token 
 * Change to your api token
 */
#ifndef MY_TB_TOKEN
  #define MY_TB_TOKEN "123456789"
#endif


/* MQTT credentials
 * Change to your mqtt username & password
 */
#ifndef MY_MQTT_USERNAME
  #define MY_MQTT_USERNAME "abcefg"
  #define MY_MQTT_PASSWORD "123456789"  
#endif              
```
