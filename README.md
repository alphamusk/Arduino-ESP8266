### ESP8266 GPIO Pin Sketch using ArduinoJson6
_MQTT Publishing aka callback using ```PubSubClient.h``` library_
- [View Sketch](https://github.com/alphamusk/Arduino-ESP8266/blob/master/templates/ESP8266_and_Thingsboard_MQTT_PubSub_callback_template/ESP8266_and_Thingsboard_MQTT_PubSub_callback_template.ino)



#### Basic ESP8266 Thingsboard Sketch
_Uses ```ESP8266WiFi.h``` and ```ThingsBoard.h``` libraries_
- :file: [View Sketch](https://github.com/alphamusk/Arduino-ESP8266/blob/master/templates/ESP8266_and_Thingsboard_Template/ESP8266_and_Thingsboard_Template.ino)



#### Configuring common vars
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
#define SKETCH_AUTHOR   "ALPHAMUSK" // Credit 


/* 
 *   Thingsboard Server 
 */
// #define TB_SERVER  "demo.thingsboard.io"
#define TB_SERVER  "10.0.1.254"     
#define TB_SERVER_PORT 1883
```



#### Setting credentials for authentication
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



### Markdown
```markdown
Syntax highlighted code block

# Header 1
## Header 2
### Header 3

- Bulleted
- List

1. Numbered
2. List

**Bold** and _Italic_ and `Code` text

[Link](url) and ![Image](src)
```

For more details see [GitHub Flavored Markdown](https://guides.github.com/features/mastering-markdown/).
