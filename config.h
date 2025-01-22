#ifndef CONFIG_H
#define CONFIG_H

// Настройки Wi-Fi
const char* ssid = "Pixel_4938";      
const char* password = "sosiska123";  

// Настройки MQTT
const char* mqtt_broker = "broker.emqx.io"; 
const int mqtt_port = 1883;                 
const char* mqtt_user = "emqx";            
const char* mqtt_pass = "public";          
const char* mqtt_topic = "emqx/esp8266";

#endif